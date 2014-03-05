#ifndef VARIABLEXMLNODEGENERATOR_H
#define VARIABLEXMLNODEGENERATOR_H

#include "XMLNodeGenerator.h"

class VariableXMLNodeGenerator : public XMLNodeGenerator
{
public:
    VariableXMLNodeGenerator( const std::string& xmlNodeName );
    ~VariableXMLNodeGenerator(){}

    virtual Monky::XMLNode* GetVariableDeclarationXMLNodeFromNode( const Node* node, Monky::XMLDocument& doc ) const;

protected:
    virtual void AddExtraInfoToXMLNode( Monky::XMLNode* xmlNode, const Node* node, Monky::XMLDocument& doc, int outputChannelIdx ) const;

    std::string GetVariableValueFromAttributes( const Node* node ) const;

    static int sm_nextVariableID;
};

#endif // VARIABLEXMLNODEGENERATOR_H
