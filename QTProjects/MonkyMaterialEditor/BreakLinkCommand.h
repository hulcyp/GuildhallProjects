#ifndef BREAKLINKSCOMMAND_H
#define BREAKLINKSCOMMAND_H

#include <QUndoCommand>

class Connector;
class MaterialGraphWidget;
class Link;

class BreakLinkCommand : public QUndoCommand
{
public:
    BreakLinkCommand( Link* link, MaterialGraphWidget* widget );
    void undo();
    void redo();


private:
    MaterialGraphWidget* m_graph;
    Connector* m_src;
    Connector* m_dest;
    Link* m_link;
};

#endif // BREAKLINKSCOMMAND_H
