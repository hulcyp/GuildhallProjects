#ifndef BREAKALLLINKSCOMMAND_H
#define BREAKALLLINKSCOMMAND_H

#include <QUndoCommand>

class Connector;
class MaterialGraphWidget;
class Link;

class BreakAllLinksCommand : public QUndoCommand
{
public:
    BreakAllLinksCommand( Connector* connector, MaterialGraphWidget* graph );

    void undo();
    void redo();

private:
    struct LinkInfo
    {
        LinkInfo( Connector* s, Connector* d, Link* l );
        Connector* src;
        Connector* dest;
        Link* link;
    };

    MaterialGraphWidget* m_graph;
    QList<LinkInfo> m_links;

};

#endif // BREAKALLLINKSCOMMAND_H
