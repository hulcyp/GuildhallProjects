#include "GraphLayoutParser.h"
#include "Node.h"
#include "MaterialGraphWidget.h"
#include "NodeType.h"
#include "Link.h"


void GraphLayoutParser::ParseGraphLayout( const char* layoutBuffer, int size, MaterialGraphWidget* graph )
{
    char* currPos = const_cast<char*>( layoutBuffer );
    if( size > 0 )
    {
        char* bufferEnd = currPos + size;

        int versionNumber = *reinterpret_cast< int* >( currPos );
        currPos += sizeof( int );
        if( versionNumber == MATERIAL_VERSION_NUMBER )
        {
            int numNodes = *reinterpret_cast< int* >( currPos );
            currPos += sizeof( int );
            std::map< int, NodeData* > nodeMap;
            for( int i = 0; i < numNodes && currPos < bufferEnd; ++i )
            {
                Node* node = ConstructNode( currPos, graph );
                if( node != nullptr )
                {
                    graph->scene()->addItem( node );
                    NodeData* nodeData = new NodeData( node );
                    nodeMap[ node->GetNodeID() ] = nodeData;
                    unsigned int numConnectors = node->InputConnectors().size();
                    for( unsigned int c = 0; c < numConnectors; ++c )
                    {
                        nodeData->inputConnectorIDs.push_back( *reinterpret_cast< const int* >( currPos ) );
                        currPos += sizeof( int );
                        nodeData->outputConnectorIDs.push_back( *reinterpret_cast< const int* >( currPos ) );
                        currPos += sizeof( int );
                    }
                }
            }

            for( auto iter = nodeMap.begin(); iter != nodeMap.end(); ++iter )
            {
                ReconnectNode( iter->second, nodeMap, graph );
            }
            graph->scene()->update();
        }
    }

}
//---------------------------------------------------------------------------
Node* GraphLayoutParser::ConstructNode( char*& currPosInLayoutBuffer, MaterialGraphWidget* graph )
{
    int nodeID = *reinterpret_cast< int* >( currPosInLayoutBuffer );
    currPosInLayoutBuffer += sizeof( int );
    NodeType::NodeTypeID typeID = (NodeType::NodeTypeID)(*reinterpret_cast< int* >( currPosInLayoutBuffer ));
    currPosInLayoutBuffer += sizeof( int );
    float xPos = *reinterpret_cast< float* >( currPosInLayoutBuffer );
    currPosInLayoutBuffer += sizeof( float );
    float yPos = *reinterpret_cast< float* >( currPosInLayoutBuffer );
    currPosInLayoutBuffer += sizeof( float );

    Node* newNode = NodeType::ConstructNodeFromType( NodeType::GetStringTypeFromID( typeID ), graph );
    if( newNode != nullptr )
    {
        newNode->setPos( (qreal)xPos, (qreal)yPos );
        newNode->SetNodeID( nodeID );
        if( typeID == NodeType::NT_OUTPUT_CHANNELS )
            graph->m_outputChannels = newNode;

        QList< std::pair< QString, QString > >& attributes = newNode->Attributes();
        unsigned int numAttribs = attributes.size();
        for( unsigned int i = 0; i < numAttribs; ++i )
        {
            int attribLen = *reinterpret_cast< int* >( currPosInLayoutBuffer );
            currPosInLayoutBuffer += sizeof( int );
            if( attribLen > 0 )
            {
                char* attrib = new char[ attribLen + 1 ];
                std::memcpy( attrib, currPosInLayoutBuffer, attribLen );
                currPosInLayoutBuffer += attribLen;
                attrib[ attribLen ] = '\0';
                attributes[i].second = attrib;
                delete attrib;
            }
        }
        newNode->AttributesUpdated();
    }

    return newNode;
}
//---------------------------------------------------------------------------
void GraphLayoutParser::ReconnectNode( NodeData* nodeData, std::map< int, NodeData* >& nodeMap, MaterialGraphWidget* graph )
{
    for( unsigned int i = 0; i < nodeData->inputConnectorIDs.size(); ++i )
    {
        if( nodeData->inputConnectorIDs[i] != -1 && nodeData->outputConnectorIDs[i] != -1 )
        {
            NodeData* otherNode = nodeMap[ nodeData->inputConnectorIDs[i] ];
            Link* newLink = new Link( otherNode->node->OuputConnectors()[ nodeData->outputConnectorIDs[i] ], nodeData->node->InputConnectors()[i] );
            if( newLink != nullptr )
              graph->scene()->addItem( newLink );
        }
    }
}
