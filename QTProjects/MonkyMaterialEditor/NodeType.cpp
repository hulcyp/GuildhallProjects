#include "NodeType.h"
#include "./CoreEngine/StringUtils.h"
#include "Node.h"
#include "XMLNodeGenerator.h"
#include "VariableXMLNodeGenerator.h"
#include "InputXMLNodeGenerator.h"
#include "TextureSampleXMLNodeGenerator.h"
#include "MaterialGraphWidget.h"

QMap< QString, NodeType > NodeType::sm_nodeTypes;

NodeType::NodeType(const std::string& name, NodeTypeID typeId, const std::string& commaSeparatedInputs, const std::string& commaSeparatedOutputs, const std::string& commaSeparatedAttributes, XMLNodeGenerator *xmlNodeGen, bool shouldShowInMenu )
    :   name( name.c_str() )
    ,   xmlNodeGenerator( xmlNodeGen )
    ,   nodeTypeID( typeId )
    ,   shouldShowInMenu( shouldShowInMenu )
{
    std::vector< std::string > inputsList;
    std::vector< std::string > outputsList;
    std::vector< std::string > attribsList;
    if( !commaSeparatedInputs.empty() )
        Monky::stringTokenizer( commaSeparatedInputs, inputsList, "," );
    if( !commaSeparatedOutputs.empty() )
        Monky::stringTokenizer( commaSeparatedOutputs, outputsList, "," );
    if( !commaSeparatedAttributes.empty() )
        Monky::stringTokenizer( commaSeparatedAttributes, attribsList, "," );

    for( unsigned int i = 0; i < inputsList.size(); ++i )
    {
        inputs.push_front( inputsList[i].c_str() );
    }

    for( unsigned int i = 0; i < outputsList.size(); ++i )
    {
        outputs.push_front( outputsList[i].c_str() );
    }
    for( unsigned int i = 0; i < attribsList.size(); ++i )
    {
        attributes.push_back( attribsList[i].c_str() );
    }
}
//---------------------------------------------------------------------------
void NodeType::InitNodeTypes()
{
    sm_nodeTypes.insert( "Add", NodeType( "Add", NT_ADD, "A,B", "Output", "", new XMLNodeGenerator( "Add" ) ) );
    sm_nodeTypes.insert( "Subtract", NodeType( "Subtract", NT_SUBTRACT, "A,B", "Output", "", new XMLNodeGenerator( "Subtract" ) ) );
    sm_nodeTypes.insert( "Multiply", NodeType( "Multiply", NT_MULTIPLY, "A,B", "Output", "", new XMLNodeGenerator( "Multiply" ) ) );
    sm_nodeTypes.insert( "Divide", NodeType( "Divide", NT_DIVIDE, "A,B", "Output", "", new XMLNodeGenerator( "Divide" ) ) );

    sm_nodeTypes.insert( "Sine", NodeType( "Sine", NT_SINE, "A", "Output", "", new XMLNodeGenerator( "Sine" ) ) );
    sm_nodeTypes.insert( "Cosine", NodeType( "Cosine", NT_COSINE, "A", "Output", "", new XMLNodeGenerator( "Cosine" ) ) );
    sm_nodeTypes.insert( "Abs", NodeType( "Abs", NT_ABS, "A", "Output", "", new XMLNodeGenerator( "Abs" ) ) );
    sm_nodeTypes.insert( "Ceil", NodeType( "Ceil", NT_CEIL, "A", "Output", "", new XMLNodeGenerator( "Ceil" ) ) );
    sm_nodeTypes.insert( "Floor", NodeType( "Floor", NT_FLOOR, "A", "Output", "", new XMLNodeGenerator( "Floor" ) ) );
    sm_nodeTypes.insert( "Frac", NodeType( "Frac", NT_FRAC, "A", "Output", "", new XMLNodeGenerator( "Frac" ) ) );
    sm_nodeTypes.insert( "Normalize", NodeType( "Normalize", NT_NORMALIZE, "A", "Output", "", new XMLNodeGenerator( "Normalize" ) ) );
    sm_nodeTypes.insert( "SqareRoot", NodeType( "SqareRoot", NT_SQUARE_ROOT, "A", "Output", "", new XMLNodeGenerator( "SqareRoot" ) ) );

    sm_nodeTypes.insert( "DotProd", NodeType( "DotProd", NT_DOT_PROD, "A,B", "Output", "", new XMLNodeGenerator( "DotProd" ) ) );
    sm_nodeTypes.insert( "CrossProd", NodeType( "CrossProd", NT_CROSS_PROD, "A,B", "Output", "", new XMLNodeGenerator( "CrossProd" ) ) );
    sm_nodeTypes.insert( "Fmod", NodeType( "Fmod", NT_FMOD, "A,B", "Output", "", new XMLNodeGenerator( "Fmod" ) ) );
    sm_nodeTypes.insert( "Power", NodeType( "Power", NT_POWER, "A,B", "Output", "", new XMLNodeGenerator( "Power" ) ) );
    sm_nodeTypes.insert( "Reflect", NodeType( "Reflect", NT_REFLECT, "A,B", "Output", "", new XMLNodeGenerator( "Reflect" ) ) );

    sm_nodeTypes.insert( "LinearInterpolate", NodeType( "LinearInterpolate", NT_LINEAR_INTERP, "A,B,Alpha", "Output", "", new XMLNodeGenerator( "LinearInterpolate" ) ) );
    sm_nodeTypes.insert( "Clamp", NodeType( "Clamp", NT_CLAMP, "A,Min,Max", "Output", "", new XMLNodeGenerator( "Clamp" ) ) );


    sm_nodeTypes.insert( "Color", NodeType( "Color", NT_COLOR, "", "Output", "Name,R,G,B,A", new VariableXMLNodeGenerator( "Input" ) ) );
    sm_nodeTypes.insert( "Real", NodeType( "Real", NT_REAL, "", "Output", "Name,R", new VariableXMLNodeGenerator( "Input" ) ) );
    sm_nodeTypes.insert( "Vector2", NodeType( "Vector2", NT_VECTOR_2, "", "Output", "Name,X,Y", new VariableXMLNodeGenerator( "Input" ) ) );
    sm_nodeTypes.insert( "Vector3", NodeType( "Vector3", NT_VECTOR_3, "", "Output", "Name,X,Y,Z", new VariableXMLNodeGenerator( "Input" ) ) );

    sm_nodeTypes.insert( "Time", NodeType( "Time", NT_TIME, "", "Output", "", new InputXMLNodeGenerator( "Input", "time" ) ) );
    sm_nodeTypes.insert( "TexCoords", NodeType( "TexCoords", NT_TEX_COORDS, "", "UVs", "", new InputXMLNodeGenerator( "Input", "vTexCoord" ) ) );

    sm_nodeTypes.insert( "SampleTexture", NodeType( "SampleTexture", NT_SAMPLE_TEXTURE, "TexCoords", "Color,R,G,B,A", "Name,textureSampleName", new TextureSampleXMLNodeGenerator( "SampleTexture" ) ) );

    sm_nodeTypes.insert( "OutputChannels", NodeType( "OutputChannels", NT_OUTPUT_CHANNELS, "Diffuse,Normal,Emissive,Specular,Opacity", "", "", nullptr, false ) );

}
//---------------------------------------------------------------------------
void NodeType::CleanupNodeTypes()
{
    for( auto iter = sm_nodeTypes.begin(); iter != sm_nodeTypes.end(); ++iter )
    {
        if( iter.value().xmlNodeGenerator != nullptr )
        {
            delete iter.value().xmlNodeGenerator;
            iter.value().xmlNodeGenerator = nullptr;
        }
    }
}
//---------------------------------------------------------------------------
Node* NodeType::ConstructNodeFromType( const QString& name, MaterialGraphWidget* graph )
{
    Node* newNode = nullptr;
    if( !name.isEmpty() )
    {
        auto iter = sm_nodeTypes.find( name );
        if( iter != sm_nodeTypes.end() )
        {
            newNode = new Node( graph, name, iter.value().nodeTypeID, iter.value().inputs, iter.value().outputs, iter.value().attributes );
            newNode->SetXMLNodeGenerator( iter.value().xmlNodeGenerator );
            newNode->SetNodeID( graph->GetNextNodeID() );
        }
    }
    return newNode;
}
//---------------------------------------------------------------------------
const QMap< QString, NodeType >& NodeType::GetNodeTypes()
{
    return sm_nodeTypes;
}
//---------------------------------------------------------------------------
const QString& NodeType::GetStringTypeFromID( NodeTypeID id )
{
    static QList< QString > nodeIDToStr;
    if( nodeIDToStr.size() == 0  )
    {
        nodeIDToStr.push_back( "Add" );
        nodeIDToStr.push_back( "Subtract" );
        nodeIDToStr.push_back( "Multiply" );
        nodeIDToStr.push_back( "Divide" );
        nodeIDToStr.push_back( "Sine" );
        nodeIDToStr.push_back( "Cosine" );
        nodeIDToStr.push_back( "Abs" );
        nodeIDToStr.push_back( "Ceil" );
        nodeIDToStr.push_back( "Floor" );
        nodeIDToStr.push_back( "Frac" );
        nodeIDToStr.push_back( "Normalize" );
        nodeIDToStr.push_back( "SquareRoot" );
        nodeIDToStr.push_back( "DotProd" );
        nodeIDToStr.push_back( "CrossProd" );
        nodeIDToStr.push_back( "Fmod" );
        nodeIDToStr.push_back( "Power" );
        nodeIDToStr.push_back( "Reflect" );
        nodeIDToStr.push_back( "LinearInterpolate" );
        nodeIDToStr.push_back( "Clamp" );
        nodeIDToStr.push_back( "Color" );
        nodeIDToStr.push_back( "Real" );
        nodeIDToStr.push_back( "Vector2" );
        nodeIDToStr.push_back( "Vector3" );
        nodeIDToStr.push_back( "Time" );
        nodeIDToStr.push_back( "TexCoords" );
        nodeIDToStr.push_back( "SampleTexture" );
        nodeIDToStr.push_back( "OutputChannels" );
    }

    if( id >= 0 && id < nodeIDToStr.size() )
        return nodeIDToStr[id];
    else
        return "";
}
