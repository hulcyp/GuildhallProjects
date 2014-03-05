#include "BreakLinkCommand.h"
#include "Link.h"
#include "Connector.h"
#include "MaterialGraphWidget.h"

BreakLinkCommand::BreakLinkCommand( Link* link, MaterialGraphWidget* widget )
    : m_graph( widget )
    , m_link( link )
{
    m_src = m_link->SourceConnector();
    m_dest = m_link->DestConnector();
}
//---------------------------------------------------------------
void BreakLinkCommand::undo()
{
    m_graph->scene()->addItem( m_link );
    m_link->SetSource( m_src );
    m_link->SetDest( m_dest );
    m_graph->scene()->update();
    m_link->adjust();
    m_graph->GraphUnChanged();
}
//---------------------------------------------------------------
void BreakLinkCommand::redo()
{
    m_graph->scene()->removeItem( m_link );
    m_graph->scene()->update();
    m_link->SetSource( nullptr );
    m_link->SetDest( nullptr );
    m_graph->GraphChanged();
}
