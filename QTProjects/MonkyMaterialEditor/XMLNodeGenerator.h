#ifndef XMLNODEGENERATOR_H
#define XMLNODEGENERATOR_H

#include "./CoreEngine/XMLParser.h"


class Node;

class XMLNodeGenerator
{
public:
    XMLNodeGenerator( const std::string& xmlNodeName );
    virtual ~XMLNodeGenerator(){}

    Monky::XMLNode* GetXMLNode( const Node* node, Monky::XMLDocument& doc, int connectorIdx ) const;
    //Should return nullptr if no variable declaration is required for this node
    virtual Monky::XMLNode* GetVariableDeclarationXMLNodeFromNode( const Node* node, Monky::XMLDocument& doc ) const;

protected:
    void GetListOfChildXMLNodes( const Node* node, Monky::XMLDocument& doc, std::vector< Monky::XMLNode* >& childNodes ) const;
    virtual void AddExtraInfoToXMLNode( Monky::XMLNode* xmlNode, const Node* node, Monky::XMLDocument& doc, int outputChannelIdx ) const;

protected:
    const std::string m_xmlNodeName;

};

#endif // XMLNODEGENERATOR_H
