#ifndef MOVECOMMAND_H
#define MOVECOMMAND_H

#include <QUndoCommand>
#include <QPointF>

class Node;

class MoveCommand : public QUndoCommand
{
public:
    enum { ID = 1234 };

    explicit MoveCommand( Node* node, const QPointF& oldPos, QUndoCommand *parent = 0);

    void undo();
    void redo();
    bool mergeWith(const QUndoCommand *command);

    int id() const { return ID; }

private:
    Node* m_nodeToMove;
    QPointF m_oldPos;
    QPointF m_newPos;

};

#endif // MOVECOMMAND_H
