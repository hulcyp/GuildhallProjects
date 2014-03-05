#include "VariableXMLNodeGenerator.h"
#include "Node.h"
#include "CoreEngine/StringUtils.h"

int VariableXMLNodeGenerator::sm_nextVariableID = 0;

VariableXMLNodeGenerator::VariableXMLNodeGenerator( const std::string& xmlNodeName )
    :   XMLNodeGenerator( xmlNodeName )
{}
//--------------------------------------------------------------------
Monky::XMLNode* VariableXMLNodeGenerator::GetVariableDeclarationXMLNodeFromNode( const Node* node, Monky::XMLDocument& doc ) const
{
    const QList< std::pair< QString, QString > >& attributes = node->Attributes();

    std::string variableName = "variable" + Monky::toString( sm_nextVariableID++ );
    if( attributes.size() > 0 && attributes[0].first.compare( "Name" ) == 0 && !attributes[0].second.isEmpty() )
    {
        variableName = attributes[0].second.toLocal8Bit().constData();
    }
    Monky::XMLNode* xmlNode = doc.NewElement( "Constant" );
    xmlNode->SetAttribute( "name", variableName.c_str() );
    xmlNode->SetAttribute( "type", node->GetName().toLocal8Bit().constData() );
    xmlNode->InsertEndChild( doc.NewText( GetVariableValueFromAttributes( node ).c_str() ) );
    node->m_variableName = QString( variableName.c_str() );
    return xmlNode;
}
//--------------------------------------------------------------------
void VariableXMLNodeGenerator::AddExtraInfoToXMLNode(Monky::XMLNode* xmlNode, const  Node* node, Monky::XMLDocument& doc , int outputChannelIdx ) const
{
    Q_UNUSED( xmlNode );
    Q_UNUSED( node );
    Q_UNUSED( doc );
    std::string variableUsage = "$";
    variableUsage += node->GetVariableName().toLocal8Bit().constData();
    xmlNode->InsertEndChild( doc.NewText( variableUsage.c_str() ) );
}
//--------------------------------------------------------------------
std::string VariableXMLNodeGenerator::GetVariableValueFromAttributes( const Node* node ) const
{
    static std::map< QString, int > numRealNumbers;
    if( numRealNumbers.empty() )
    {
        numRealNumbers[ "Real" ] = 1;
        numRealNumbers[ "Vector2" ] = 2;
        numRealNumbers[ "Vector3" ] = 3;
        numRealNumbers[ "Vector4" ] = 4;
        numRealNumbers[ "Color" ] = 4;
    }

    std::string value = "";

    QString nodeName = node->GetName();
    auto iter  = numRealNumbers.find( nodeName );
    if( iter != numRealNumbers.end() )
    {
        const QList< std::pair< QString, QString > >& attributes = node->Attributes();
        int count = iter->second;
        for( int i = 1; i < count + 1; ++i )
        {
            if( i < attributes.size() && !attributes[i].second.isEmpty() )
            {
                value += attributes[i].second.toLocal8Bit().constData();
            }
            else
                value += "0";
            if( i != count )
                value += ",";
        }
    }

    return value;
}
