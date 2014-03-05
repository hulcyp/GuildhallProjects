#ifndef DELETECOMMAND_H
#define DELETECOMMAND_H

#include <QUndoCommand>

class MaterialGraphWidget;
class Node;
class BreakAllLinksCommand;

class DeleteCommand : public QUndoCommand
{
public:
    DeleteCommand(Node* node, MaterialGraphWidget* widget , QUndoCommand* parent = 0);

    void redo();
    void undo();

private:
    MaterialGraphWidget* m_graph;
    Node* m_nodeToDelete;
    QList< BreakAllLinksCommand* > m_breakLinksCommands;
};

#endif // DELETECOMMAND_H
