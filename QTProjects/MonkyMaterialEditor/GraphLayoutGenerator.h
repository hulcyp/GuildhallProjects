#ifndef GRAPHLAYOUTGENERATOR_H
#define GRAPHLAYOUTGENERATOR_H

/*
 * Graph Layout Format
 *      versionNum : int
 *      numNodes : int
 *      for each node
 *          nodeID : int
 *          nodeType : NodeTypeID
 *          nodePos : float,float
 *          for each attrib
 *              attrib len : int
 *              attrib data : const char* without \0
 *          for each connection
 *              nodeID : int
 *              connectorID : int
 */
#include <vector>

class Node;


class GraphLayoutGenerator
{
public:
    char* GenerateGraphLayoutBuffer( const std::vector<Node*>& nodes, int& bufferSize ) const;

protected:
    int GetNodeSizeForBuffer( const Node* node ) const;
    void AddDataToBufferForNode( Node* node, char*& currBufferPos ) const;
};

#endif // GRAPHLAYOUTGENERATOR_H
