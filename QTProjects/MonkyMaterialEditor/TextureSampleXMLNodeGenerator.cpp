#include "TextureSampleXMLNodeGenerator.h"
#include <QList>
#include "Node.h"
#include <QString>

int TextureSampleXMLNodeGenerator::sm_nextTextureSampleID = 0;

TextureSampleXMLNodeGenerator::TextureSampleXMLNodeGenerator( const std::string& xmlNodeName )
    :   XMLNodeGenerator( xmlNodeName )
{}


Monky::XMLNode* TextureSampleXMLNodeGenerator::GetVariableDeclarationXMLNodeFromNode( const Node* node, Monky::XMLDocument& doc ) const
{
    const QList< std::pair< QString, QString > >& attributes = node->Attributes();
    Monky::XMLNode* xmlNode = nullptr;

    if( attributes.size() > 0 && attributes[1].first.compare( "textureSampleName" ) == 0 && !attributes[1].second.isEmpty() )
    {
        std::string variableName = "variable" + Monky::toString( sm_nextTextureSampleID++ );
        if( attributes[0].first.compare( "Name" ) == 0 && !attributes[0].second.isEmpty() )
        {
            variableName = attributes[0].second.toLocal8Bit().constData();
        }
        xmlNode = doc.NewElement( "TextureSample" );
        xmlNode->SetAttribute( "name", variableName.c_str() );
        xmlNode->SetAttribute( "textureSampleName", attributes[1].second.toLocal8Bit().constData() );
        node->m_variableName = QString( variableName.c_str() );
    }
    return xmlNode;
}

void TextureSampleXMLNodeGenerator::AddExtraInfoToXMLNode(Monky::XMLNode* xmlNode, const Node* node, Monky::XMLDocument& doc , int outputChannelIdx ) const
{
    Q_UNUSED( xmlNode );
    Q_UNUSED( node );
    Q_UNUSED( doc );
    xmlNode->SetAttribute( "textureSampleName", node->GetVariableName().toLocal8Bit().constData() );

    //Connecters are drawn in the reverse order of their index on screen
    switch( outputChannelIdx )
    {
        case 3:
            xmlNode->SetAttribute( "comp", "r" );
            break;
        case 2:
            xmlNode->SetAttribute( "comp", "g" );
            break;
        case 1:
            xmlNode->SetAttribute( "comp", "b" );
            break;
        case 0:
            xmlNode->SetAttribute( "comp", "a" );
            break;
    }
}
