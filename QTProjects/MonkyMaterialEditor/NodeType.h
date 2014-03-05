#ifndef NODETYPE_H
#define NODETYPE_H

#include <QString>
#include <QList>
#include <string>
#include <QMap>

class Node;
class MaterialGraphWidget;
class XMLNodeGenerator;

struct NodeType
{

    enum NodeTypeID
    {
        NT_ADD,
        NT_SUBTRACT,
        NT_MULTIPLY,
        NT_DIVIDE,
        NT_SINE,
        NT_COSINE,
        NT_ABS,
        NT_CEIL,
        NT_FLOOR,
        NT_FRAC,
        NT_NORMALIZE,
        NT_SQUARE_ROOT,
        NT_DOT_PROD,
        NT_CROSS_PROD,
        NT_FMOD,
        NT_POWER,
        NT_REFLECT,
        NT_LINEAR_INTERP,
        NT_CLAMP,
        NT_COLOR,
        NT_REAL,
        NT_VECTOR_2,
        NT_VECTOR_3,
        NT_TIME,
        NT_TEX_COORDS,
        NT_SAMPLE_TEXTURE,
        NT_OUTPUT_CHANNELS,
        NT_COUNT
    };

    NodeType( const std::string& name,
              NodeTypeID typeId,
              const std::string& commaSeparatedInputs,
              const std::string& commaSeparatedOutputs,
              const std::string& commaSeparatedAttributes,
              XMLNodeGenerator* xmlNodeGen,
              bool shouldShowInMenu = true );

    QString name;
    QList<QString> inputs;
    QList<QString> outputs;
    QList<QString> attributes;
    XMLNodeGenerator* xmlNodeGenerator;
    NodeTypeID nodeTypeID;
    bool shouldShowInMenu;

    static void InitNodeTypes();
    static void CleanupNodeTypes();
    static Node* ConstructNodeFromType( const QString& name, MaterialGraphWidget* graph );
    static const QMap< QString, NodeType >& GetNodeTypes();
    static const QString& GetStringTypeFromID( NodeTypeID id );


private:
    static QMap< QString, NodeType > sm_nodeTypes;
};


#endif // NODETYPE_H
