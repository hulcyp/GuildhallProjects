#include "MoveCommand.h"
#include "Node.h"
#include <assert.h>
#include <QGraphicsScene>
#include "MaterialGraphWidget.h"
MoveCommand::MoveCommand( Node* node, const QPointF& oldPos, QUndoCommand *parent )
    : QUndoCommand( parent )
    , m_nodeToMove( node )
    , m_oldPos( oldPos )
{
    assert( node != nullptr );
    m_nodeToMove = node;
    m_newPos = m_nodeToMove->pos();
}
//--------------------------------------------------------------------
void MoveCommand::undo()
{
    m_nodeToMove->setPos( m_oldPos );
    m_nodeToMove->scene()->update();
    m_nodeToMove->Graph()->GraphUnChanged();
}
//--------------------------------------------------------------------
void MoveCommand::redo()
{
    m_nodeToMove->setPos( m_newPos );
    m_nodeToMove->scene()->update();
    m_nodeToMove->Graph()->GraphChanged();
}
//--------------------------------------------------------------------
bool MoveCommand::mergeWith(const QUndoCommand *command)
{
    bool merged = false;
    // This is safe because merge only runs if the command id's are the same. Since we override the id function and give our MoveCommand
    // a unique id it is guaranteed that the command passed down is of type MoveCommand*
    const MoveCommand* moveCommand = static_cast<const MoveCommand* >( command );

    if( m_nodeToMove == moveCommand->m_nodeToMove )
    {
        m_newPos = moveCommand->m_newPos;
        merged = true;
    }

    return merged;
}
