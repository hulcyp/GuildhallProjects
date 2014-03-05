#include "BreakAllLinksCommand.h"
#include "MaterialGraphWidget.h"
#include "Connector.h"
#include "Link.h"

BreakAllLinksCommand::BreakAllLinksCommand( Connector* connector, MaterialGraphWidget* graph )
    :   m_graph( graph )
{
    if( connector != nullptr )
    {
        QList<Link*> links = connector->GetLinks();
        foreach( Link* link, links )
        {
            if( link != nullptr )
            {
                m_links.push_back( LinkInfo( link->SourceConnector(), link->DestConnector(), link ) );
            }
        }
    }
}
//---------------------------------------------------------------
void BreakAllLinksCommand::undo()
{
    foreach( const LinkInfo& linkInfo, m_links )
    {
        m_graph->scene()->addItem( linkInfo.link );
        linkInfo.link->SetSource( linkInfo.src );
        linkInfo.link->SetDest( linkInfo.dest );
        m_graph->scene()->update();
        linkInfo.link->adjust();
    }
    m_graph->GraphUnChanged();
}
//---------------------------------------------------------------
void BreakAllLinksCommand::redo()
{
    foreach( const LinkInfo& linkInfo, m_links )
    {
        m_graph->scene()->removeItem( linkInfo.link );
        linkInfo.link->SetSource( nullptr );
        linkInfo.link->SetDest( nullptr );
        m_graph->scene()->update();
    }
    m_graph->GraphChanged();
}
//---------------------------------------------------------------
//---------------------------------------------------------------
BreakAllLinksCommand::LinkInfo::LinkInfo( Connector* s, Connector* d, Link* l )
    :   src( s )
    ,   dest( d )
    ,   link( l )
{}
