#include "GraphLayoutGenerator.h"
#include "Node.h"
#include "NodeType.h"
#include "Connector.h"
#include "Link.h"
#include "MaterialGraphWidget.h"

char* GraphLayoutGenerator::GenerateGraphLayoutBuffer(const std::vector<Node*>& nodes , int &bufferSize) const
{
    int size = sizeof(int)*2;

    foreach( Node* node, nodes )
    {
        size += GetNodeSizeForBuffer( node );
    }

    char* buffer = new char[size];
    char* currPos = buffer;
    *reinterpret_cast< int* >( currPos ) = (int)MATERIAL_VERSION_NUMBER;
    currPos += sizeof( int );
    *reinterpret_cast< int* >( currPos ) = static_cast< int >( nodes.size() );
    currPos += sizeof( int );
    foreach( Node* node, nodes )
    {
        AddDataToBufferForNode( node, currPos );
    }
    bufferSize = size;
    return buffer;
}
//-----------------------------------------------------------------------------------
int GraphLayoutGenerator::GetNodeSizeForBuffer( const Node* node ) const
{
    int size = sizeof(int);
    size += sizeof(NodeType::NodeTypeID);
    size += sizeof(float)*2;
    const QList< std::pair< QString, QString > >& attribs = node->Attributes();
    for( int i = 0; i < attribs.size(); ++i )
    {
        size += sizeof(int);
        size += attribs[i].second.size();
    }

    const QList<Connector*>& inputConnectors = node->InputConnectors();
    size += inputConnectors.count()*sizeof(int)*2;

    return size;
}
//-----------------------------------------------------------------------------------
void GraphLayoutGenerator::AddDataToBufferForNode( Node* node, char*& currBufferPos ) const
{
    *reinterpret_cast< int* >( currBufferPos ) = node->GetNodeID();
    currBufferPos += sizeof( int );
    *reinterpret_cast< int* >( currBufferPos ) = static_cast< int >( node->GetNodeTypeID() );
    currBufferPos += sizeof( int );
    *reinterpret_cast< float* >( currBufferPos ) = node->pos().x();
    currBufferPos += sizeof( float );
    *reinterpret_cast< float* >( currBufferPos ) = node->pos().y();
    currBufferPos += sizeof( float );

    const QList< std::pair< QString, QString > >& attribs = node->Attributes();
    for( int i = 0; i < attribs.size(); ++i )
    {
        int sizeofAttrib = static_cast<int>( attribs[i].second.size() );
        *reinterpret_cast< int* >( currBufferPos ) = sizeofAttrib;
        currBufferPos += sizeof( int );
        std::memcpy( currBufferPos, attribs[i].second.toLocal8Bit().constData(), sizeofAttrib );
        currBufferPos += sizeofAttrib;
    }

    const QList<Connector*> inputConnectors = node->InputConnectors();
    foreach( Connector* connector, inputConnectors )
    {
        QList<Link*> links = connector->GetLinks();
        if( links.size() > 0 )
        {
            *reinterpret_cast< int* >( currBufferPos ) = links[0]->SourceConnector()->GetParent()->GetNodeID();
            currBufferPos += sizeof( int );
            *reinterpret_cast< int* >( currBufferPos ) = links[0]->SourceConnector()->GetIdx();
            currBufferPos += sizeof( int );
        }
        else
        {
            *reinterpret_cast< int* >( currBufferPos ) = -1;
            currBufferPos += sizeof( int );
            *reinterpret_cast< int* >( currBufferPos ) = -1;
            currBufferPos += sizeof( int );
        }
    }

}
