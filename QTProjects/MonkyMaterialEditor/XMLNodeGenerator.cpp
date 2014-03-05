#include "XMLNodeGenerator.h"
#include "Node.h"
#include "Connector.h"
#include "Link.h"
#include "./CoreEngine/StringUtils.h"

XMLNodeGenerator::XMLNodeGenerator( const std::string& xmlNodeName )
    :   m_xmlNodeName( xmlNodeName )
{}
//------------------------------------------------------------------------------------------
Monky::XMLNode* XMLNodeGenerator::GetXMLNode(const  Node* node , Monky::XMLDocument& doc , int connectorIdx ) const
{
    Monky::XMLNode* xmlNode = doc.NewElement( m_xmlNodeName.c_str() );
    QPointF pos = node->pos();
    std::string posStr = Monky::toString( pos.x() ) + "," + Monky::toString( pos.y() );
    //xmlNode->SetAttribute( "pos", posStr.c_str() );
    //xmlNode->SetAttribute( "id", node->GetNodeID() );
    std::vector<Monky::XMLNode*> childXMLNodes;
    GetListOfChildXMLNodes( node, doc, childXMLNodes );
    //Connecters are drawn in the reverse order of their index on screen
    for( int i = childXMLNodes.size() - 1; i >= 0; --i )
    {
        xmlNode->InsertEndChild( childXMLNodes[i] );
    }
    AddExtraInfoToXMLNode( xmlNode, node, doc, connectorIdx );
    return xmlNode;
}
//------------------------------------------------------------------------------------------
Monky::XMLNode* XMLNodeGenerator::GetVariableDeclarationXMLNodeFromNode( const Node* node, Monky::XMLDocument& doc ) const
{
    Q_UNUSED( node );
    Q_UNUSED( doc );
    return nullptr;
}
//------------------------------------------------------------------------------------------
void XMLNodeGenerator::GetListOfChildXMLNodes( const Node* node, Monky::XMLDocument &doc, std::vector<Monky::XMLNode *> &childNodes ) const
{
    QList<Connector*> inputList = node->InputConnectors();
    for( int i = 0; i < inputList.size() ; ++i )
    {
        if( inputList[i] != nullptr )
        {
            QList<Link*> links = inputList[i]->GetLinks();
            if( !links.isEmpty() )
            {
                foreach( Link* link, links )
                {
                    if( link != nullptr )
                    {
                        Node* childNode = link->SourceConnector()->GetParent();
                        if( childNode )
                        {
                            Monky::XMLNode* childXMLNode = childNode->GetXMLNode( doc, link->SourceConnector()->GetIdx() );
                            if( childXMLNode )
                                childNodes.push_back( childXMLNode );
                        }
                    }
                }
            }
        }
    }
}
//------------------------------------------------------------------------------------------
void XMLNodeGenerator::AddExtraInfoToXMLNode(Monky::XMLNode* xmlNode, const Node* node, Monky::XMLDocument& doc , int outputChannelIdx ) const
{

}
