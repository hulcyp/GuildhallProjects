#ifndef TEXTRUESAMPLEXMLNODEGENERATOR_H
#define TEXTRUESAMPLEXMLNODEGENERATOR_H

#include "XMLNodeGenerator.h"

class TextureSampleXMLNodeGenerator : public XMLNodeGenerator
{
public:
    TextureSampleXMLNodeGenerator( const std::string& xmlNodeName );
    virtual ~TextureSampleXMLNodeGenerator(){}

    virtual Monky::XMLNode* GetVariableDeclarationXMLNodeFromNode( const Node* node, Monky::XMLDocument& doc ) const;

protected:
    virtual void AddExtraInfoToXMLNode( Monky::XMLNode* xmlNode, const Node* node, Monky::XMLDocument& doc, int outputChannelIdx ) const;


    static int sm_nextTextureSampleID;
};

#endif // TEXTRUESAMPLEXMLNODEGENERATOR_H
