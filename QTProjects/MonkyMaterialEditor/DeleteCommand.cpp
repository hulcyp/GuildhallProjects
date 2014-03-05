#include "DeleteCommand.h"
#include "MaterialGraphWidget.h"
#include <assert.h>
#include "Node.h"
#include "BreakAllLinksCommand.h"

DeleteCommand::DeleteCommand(Node* node, MaterialGraphWidget* widget, QUndoCommand *parent )
    : QUndoCommand( parent )
    , m_graph( widget )
    , m_nodeToDelete( node )
{
    assert( m_graph != nullptr && m_graph->scene() != nullptr );
    const QList< Connector* >& inputConnectors = node->InputConnectors();
    const QList< Connector* >& outputConnectors = node->OuputConnectors();

    foreach( Connector* connector, inputConnectors )
    {
        m_breakLinksCommands.push_back( new BreakAllLinksCommand( connector, widget ) );
    }
    foreach( Connector* connector, outputConnectors )
    {
        m_breakLinksCommands.push_back( new BreakAllLinksCommand( connector, widget ) );
    }
}
//------------------------------------------------------------------
void DeleteCommand::redo()
{
    m_graph->scene()->removeItem( m_nodeToDelete );
    foreach( BreakAllLinksCommand* command, m_breakLinksCommands )
    {
        command->redo();
    }
    m_graph->scene()->clearSelection();
    m_graph->GraphChanged();
}
//------------------------------------------------------------------
void DeleteCommand::undo()
{
    m_graph->scene()->addItem( m_nodeToDelete );
    foreach( BreakAllLinksCommand* command, m_breakLinksCommands )
    {
        command->undo();
    }
    m_graph->scene()->clearSelection();
    m_graph->scene()->update();
    m_graph->GraphUnChanged();
}
