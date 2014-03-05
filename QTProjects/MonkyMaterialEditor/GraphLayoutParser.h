#ifndef GRAPHLAYOUTPARSER_H
#define GRAPHLAYOUTPARSER_H
#include <vector>
#include <map>

class MaterialGraphWidget;
class Node;


class GraphLayoutParser
{
public:

    void ParseGraphLayout( const char* layoutBuffer, int size, MaterialGraphWidget* graph );

protected:
    struct NodeData
    {
        NodeData( Node* node )
            :   node(node)
        {}

        Node* node;
        std::vector<int> inputConnectorIDs;
        std::vector<int> outputConnectorIDs;
    };

    Node* ConstructNode( char*& currPosInLayoutBuffer, MaterialGraphWidget* graph );
    void ReconnectNode( NodeData* nodeData, std::map< int, NodeData* >& nodeMap, MaterialGraphWidget* graph );

};

#endif // GRAPHLAYOUTPARSER_H
