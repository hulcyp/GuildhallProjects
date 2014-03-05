#ifndef INPUTXMLNODEGENERATOR_H
#define INPUTXMLNODEGENERATOR_H

#include "XMLNodeGenerator.h"

class InputXMLNodeGenerator : public XMLNodeGenerator
{
public:
    InputXMLNodeGenerator( const std::string& nodeName, const std::string& variableName );
    ~InputXMLNodeGenerator(){}

protected:
    virtual void AddExtraInfoToXMLNode( Monky::XMLNode* xmlNode, const Node* node, Monky::XMLDocument& doc, int outputChannelIdx ) const;

private:
    const std::string m_variableName;
};

#endif // INPUTXMLNODEGENERATOR_H
