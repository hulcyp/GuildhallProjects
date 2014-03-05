#include "AddNodeCommand.h"
#include "MaterialGraphWidget.h"
#include "Node.h"
#include <assert.h>
#include "NodeType.h"

AddNodeCommand::AddNodeCommand( const QString& nodeName, MaterialGraphWidget* graph, const QPointF& scenePos, QUndoCommand* parent )
    : QUndoCommand(parent)
    , m_graph(graph)
    , m_initialScenePos( scenePos )
{
    assert( m_graph != nullptr && m_graph->scene() != nullptr );
    QList<QString> inputs;
    QList<QString> outputs;


    m_nodeToAdd = NodeType::ConstructNodeFromType( nodeName, graph );
    m_graph->scene()->update();

}
//----------------------------------------------------------------
AddNodeCommand::~AddNodeCommand()
{
}
//----------------------------------------------------------------
void AddNodeCommand::undo()
{
    m_graph->scene()->removeItem( m_nodeToAdd );
    m_graph->scene()->clearSelection();
    m_graph->scene()->update();
    m_graph->GraphUnChanged();
}
//----------------------------------------------------------------
void AddNodeCommand::redo()
{
    m_graph->scene()->addItem( m_nodeToAdd );
    m_nodeToAdd->setPos( m_initialScenePos );
    m_graph->scene()->clearSelection();
    m_graph->scene()->update();
    m_graph->GraphChanged();
}
