#include "InputXMLNodeGenerator.h"


InputXMLNodeGenerator::InputXMLNodeGenerator( const std::string& nodeName, const std::string& variableName )
    : XMLNodeGenerator( nodeName )
    , m_variableName( variableName )
{}
//--------------------------------------------------------------------------------------
void InputXMLNodeGenerator::AddExtraInfoToXMLNode( Monky::XMLNode* xmlNode, const Node* node, Monky::XMLDocument& doc, int outputChannelIdx ) const
{
    node;
    xmlNode->InsertEndChild( doc.NewText( std::string( "$" + m_variableName ).c_str() ) );
}
