#ifndef ADDCOMMAND_H
#define ADDCOMMAND_H

#include <QUndoCommand>
#include <QPoint>
#include <QList>

class MaterialGraphWidget;
class Node;

class AddNodeCommand : public QUndoCommand
{
public:
    AddNodeCommand( const QString& nodeName, MaterialGraphWidget* graph, const QPointF& scenePos, QUndoCommand* parent = 0 );
    ~AddNodeCommand();

    void undo();
    void redo();


private:
    MaterialGraphWidget* m_graph;
    Node* m_nodeToAdd;
    QPointF m_initialScenePos;
};

#endif // ADDCOMMAND_H
