#ifndef ADDLINKCOMMAND_H
#define ADDLINKCOMMAND_H

#include <QUndoCommand>

class MaterialGraphWidget;
class Connector;
class Link;

class AddLinkCommand : public QUndoCommand
{
public:
    AddLinkCommand(  Connector* src, Connector* dest, MaterialGraphWidget* graph );
    ~AddLinkCommand();

    void undo();
    void redo();

private:
    MaterialGraphWidget* m_graph;
    Link* m_linkToAdd;
    Connector* m_src;
    Connector* m_dest;
};

#endif // ADDLINKCOMMAND_H
