#include "AddLinkCommand.h"
#include "Link.h"
#include "Connector.h"
#include "MaterialGraphWidget.h"

AddLinkCommand::AddLinkCommand(  Connector* src, Connector* dest, MaterialGraphWidget* graph )
    : m_graph( graph )
    , m_linkToAdd( nullptr )
    , m_src( src )
    , m_dest( dest )
{
    m_linkToAdd = new Link( m_src, m_dest );
}
//---------------------------------------------------------------------------
AddLinkCommand::~AddLinkCommand()
{
}
//---------------------------------------------------------------------------
void AddLinkCommand::undo()
{
    m_graph->scene()->removeItem( m_linkToAdd );
    m_linkToAdd->SetSource( nullptr );
    m_linkToAdd->SetDest( nullptr );
    m_graph->GraphUnChanged();
}
//---------------------------------------------------------------------------
void AddLinkCommand::redo()
{
    m_graph->scene()->addItem( m_linkToAdd );
    m_linkToAdd->SetSource( m_src );
    m_linkToAdd->SetDest( m_dest );
    m_linkToAdd->adjust();
    m_graph->GraphChanged();
}
