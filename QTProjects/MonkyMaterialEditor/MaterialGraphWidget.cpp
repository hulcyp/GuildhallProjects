#include "MaterialGraphWidget.h"
#include "Link.h"
#include "Node.h"
#include "Connector.h"
#include "AddNodeCommand.h"
#include "DeleteCommand.h"
#include "BreakLinkCommand.h"
#include "mainwindow.h"
#include "AddLinkCommand.h"
#include "BreakLinkCommand.h"
#include "NodeType.h"
#include "BreakAllLinksCommand.h"
#include "GraphLayoutGenerator.h"
#include "GraphLayoutParser.h"

#include <math.h>

#include <QKeyEvent>
#include <QMenu>
#include <QAction>
#include <QUndoStack>
#include <QMessageBox>


MaterialGraphWidget::MaterialGraphWidget(MainWindow *parent)
    : QGraphicsView( parent )
    , timerId( 0 )
    , m_mainWindow( parent )
    , m_addNodeContextMenu( nullptr )
    , m_sceneRect( 5000, 5000 )
    , m_selectedNode( nullptr )
    , m_movingNode( nullptr )
    , m_lastZValue( 0.0 )
    , m_nextNodeID( 0 )
    , m_graphChangedSinceLastSave( true )
{
    QGraphicsScene* scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(-m_sceneRect.x() * 0.5f, -m_sceneRect.x() * 0.5f, m_sceneRect.x(), m_sceneRect.y());

    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(0.8), qreal(0.8));

    ConstructConextMenus();

    m_outputChannels = NodeType::ConstructNodeFromType( "OutputChannels", this );
    scene->addItem( m_outputChannels );
    scene->update();
}

//-----------------------------------------------------------------
void MaterialGraphWidget::NewMaterial()
{
    RemoveAllItems();
    m_outputChannels = nullptr;
    m_selectedNode = nullptr;
    m_movingNode = nullptr;
    m_mainWindow->UndoStack()->clear();
    m_outputChannels = NodeType::ConstructNodeFromType( "OutputChannels", this );
    scene()->addItem( m_outputChannels );
    scene()->update();
}
//-----------------------------------------------------------------
void MaterialGraphWidget::SaveMaterial( const QString& filePath )
{
    std::vector< std::string > tokens;
    Monky::stringTokenizer( filePath.toLocal8Bit().constData(), tokens, "/" );
    std::string matName;
    if( tokens.size() > 0 )
        matName = tokens[ tokens.size() - 1 ];

    Monky::XMLDocument doc;
    Monky::XMLNode* root = doc.NewElement( "Material" );
    root->SetAttribute( "name", matName.c_str() );

    std::vector<Node*> nodeList;

    if( m_outputChannels != nullptr )
    {
        QPointF rootPos = m_outputChannels->pos();


        QList<Connector*> outputChannels = m_outputChannels->InputConnectors();
        QList<QGraphicsItem*> allItems = items();
        for( int i = 0; i < allItems.size(); ++i )
        {
            Node* tempNode = qgraphicsitem_cast<Node*>( allItems[i] );
            if( tempNode )
            {
                nodeList.push_back( tempNode );
                Monky::XMLNode* variableDec = tempNode->GetVariableDeclaration( doc );
                if( variableDec )
                    root->InsertEndChild( variableDec );
            }
        }

        for( int i = 0; i < outputChannels.size(); ++i )
        {
            QList<Link*> outputChannelLinks = outputChannels[i]->GetLinks();
            if( outputChannelLinks.size() > 0 )
            {
                for( int link = 0; link < outputChannelLinks.size(); ++link )
                {
                    if( outputChannelLinks[link] != nullptr )
                    {
                        Monky::XMLNode* outputChannelXMLNode = doc.NewElement( outputChannels[i]->GetName().toLocal8Bit().constData() );
                        //Since the link as a dest it is guaranteed to have a source
                        Node* tempNode = outputChannelLinks[link]->SourceConnector()->GetParent();
                        if( tempNode )
                        {
                            Monky::XMLNode* xmlNode = tempNode->GetXMLNode( doc, outputChannelLinks[link]->SourceConnector()->GetIdx() );
                            if( xmlNode )
                            {
                                outputChannelXMLNode->InsertEndChild( xmlNode );
                                root->InsertEndChild( outputChannelXMLNode );
                            }
                        }
                    }
                }
            }

        }
    }

    GraphLayoutGenerator layoutGenerator;
    int bufferSize = 0;
    char* layoutBuffer = layoutGenerator.GenerateGraphLayoutBuffer( nodeList, bufferSize );

    FILE* materialFile = fopen( filePath.toLocal8Bit().constData(), "wb" );
    if( materialFile )
    {
        fwrite( reinterpret_cast< int* >( &bufferSize ), sizeof(int), 1, materialFile );
        fwrite( layoutBuffer, sizeof(char), bufferSize, materialFile );
        fclose( materialFile );

        materialFile = fopen( filePath.toLocal8Bit().constData(), "a" );

        doc.InsertEndChild( root );
        if( doc.SaveFile( materialFile ) == tinyxml2::XML_NO_ERROR )
        {
            QMessageBox::information( this, "Success", "Successfully saved file" );
        }
        fclose( materialFile );
        m_graphChangedSinceLastSave = false;
    }
    else
    {
        QMessageBox::critical( this, "Error", "Failed to save the file" );
    }
}
//-----------------------------------------------------------------
void MaterialGraphWidget::OpenMaterial( const QString& filePath )
{
    RemoveAllItems();
    m_outputChannels = nullptr;
    m_selectedNode = nullptr;
    m_movingNode = nullptr;

    bool success = false;

    FILE* materialFile = fopen( filePath.toLocal8Bit().constData(), "rb" );
    int bufferSize = 0;
    if( materialFile )
    {
        fread( reinterpret_cast< void* >( &bufferSize ), sizeof(int), 1, materialFile );

        if( bufferSize > 0 )
        {
            char* bufferLayout = new char[bufferSize];
            fread( bufferLayout, sizeof( char ), bufferSize, materialFile );

            GraphLayoutParser parser;
            parser.ParseGraphLayout( bufferLayout, bufferSize, this );
            success = true;
        }
    }

    if( !success )
    {
        QMessageBox::critical( this, "Error", "Failed to open file" );
    }
}
//-----------------------------------------------------------------
void MaterialGraphWidget::GraphChanged()
{
    m_graphChangedSinceLastSave = true;
    m_mainWindow->GraphChanged( !m_graphChangedSinceLastSave );
}
//-----------------------------------------------------------------
void MaterialGraphWidget::GraphUnChanged()
{
    //m_graphChangedSinceLastSave = false;
    //m_mainWindow->GraphChanged( !m_graphChangedSinceLastSave );

}
//-----------------------------------------------------------------
// Slots
//-----------------------------------------------------------------
void MaterialGraphWidget::zoomIn()
{
    scaleView(qreal(1.2));
}
//-----------------------------------------------------------------
void MaterialGraphWidget::zoomOut()
{
    scaleView(1 / qreal(1.2));
}
//-----------------------------------------------------------------
void MaterialGraphWidget::ShowContextMenu( const QPoint& pos )
{
    QGraphicsItem* item = itemAt(pos);
    if( item == nullptr )
    {
        ShowAddNodeContextMenu(pos);
    }
    else
    {
        Node* possibleNode = qgraphicsitem_cast<Node*>(item);
        if( possibleNode != nullptr && possibleNode != m_outputChannels )
        {
            ShowDeleteNodeContextMenu(pos,possibleNode);
        }
        else
        {
            Connector* possibleConnector = qgraphicsitem_cast<Connector*>(item);
            if( possibleConnector )
            {
                ShowBreakLinkContextMenu(pos,possibleConnector);
            }
        }
    }
}
//-----------------------------------------------------------------
void MaterialGraphWidget::AddNewNode(const QString& name , const QPointF &scenePos)
{
    QUndoCommand* addCommand = new AddNodeCommand( name, this, scenePos );
    m_mainWindow->UndoStack()->push( addCommand );
}
//-----------------------------------------------------------------
void MaterialGraphWidget::DeleteNode( Node *nodeToDelete )
{
    if( nodeToDelete != m_outputChannels )
    {        
        QUndoCommand* deleteCommand = new DeleteCommand( nodeToDelete, this );
        m_mainWindow->UndoStack()->push(deleteCommand);
    }
}
//-----------------------------------------------------------------
void MaterialGraphWidget::AddLink( Connector* src, Connector* dest )
{
    QUndoCommand* addLinkCmd = new AddLinkCommand( src, dest, this );
    m_mainWindow->UndoStack()->push( addLinkCmd );
}
//-----------------------------------------------------------------
void MaterialGraphWidget::BreakLink( Link* link )
{
    QUndoCommand* breaklinkCmd = new BreakLinkCommand( link, this );
    m_mainWindow->UndoStack()->push( breaklinkCmd );
}
//-----------------------------------------------------------------
void MaterialGraphWidget::BreakAllLinks( Connector *connector )
{
    QUndoCommand* breaklinkCmd = new BreakAllLinksCommand( connector, this );
    m_mainWindow->UndoStack()->push( breaklinkCmd );
}
//-----------------------------------------------------------------
// Protected members
//-----------------------------------------------------------------
//-----------------------------------------------------------------
void MaterialGraphWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Plus:
        zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
    case Qt::Key_Space:
    case Qt::Key_Enter:
        break;
    case Qt::Key_Alt:
        setDragMode(ScrollHandDrag);
    default:
        QGraphicsView::keyPressEvent(event);
    }
}
//-----------------------------------------------------------------
void MaterialGraphWidget::keyReleaseEvent(QKeyEvent * event)
{
    switch(event->key())
    {
    case Qt::Key_Alt:
        setDragMode(NoDrag);
        break;
    default:
        QGraphicsView::keyReleaseEvent( event );
    }
}
//-----------------------------------------------------------------
void MaterialGraphWidget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    QList<Node *> nodes;
    foreach (QGraphicsItem *item, scene()->items()) {
        if (Node *node = qgraphicsitem_cast<Node *>(item))
            nodes << node;
    }

    bool itemsMoved = false;
    foreach (Node *node, nodes) {
        if (node->advance())
            itemsMoved = true;
    }

    if (!itemsMoved) {
        killTimer(timerId);
        timerId = 0;
    }
}
//-----------------------------------------------------------------
#ifndef QT_NO_WHEELEVENT
//! [5]
void MaterialGraphWidget::wheelEvent(QWheelEvent *event)
{
    scaleView(pow((double)2, -event->delta() / 240.0));
}
//! [5]
#endif
//-----------------------------------------------------------------
void MaterialGraphWidget::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);

    // Shadow
    QRectF sceneRect = this->sceneRect();
    QRectF rightShadow(sceneRect.right(), sceneRect.top() + 5, 5, sceneRect.height());
    QRectF bottomShadow(sceneRect.left() + 5, sceneRect.bottom(), sceneRect.width(), 5);
    if (rightShadow.intersects(rect) || rightShadow.contains(rect))
    painter->fillRect(rightShadow, Qt::darkGray);
    if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
    painter->fillRect(bottomShadow, Qt::darkGray);

    // Fill
    QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, Qt::lightGray);
    painter->fillRect(rect.intersected(sceneRect), gradient);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(sceneRect);

    // Text
    QRectF textRect(sceneRect.left() + 4, sceneRect.top() + 4,
                    sceneRect.width() - 4, sceneRect.height() - 4);
    QString message;//(tr("Click and drag the nodes around, and zoom with the mouse "
                     //  "wheel or the '+' and '-' keys"));

    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(14);
    painter->setFont(font);
    painter->setPen(Qt::lightGray);
    painter->drawText(textRect.translated(2, 2), message);
    painter->setPen(Qt::black);
    painter->drawText(textRect, message);
}
//-----------------------------------------------------------------
void MaterialGraphWidget::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}
//-----------------------------------------------------------------
void MaterialGraphWidget::mousePressEvent( QMouseEvent* event )
{
    if( event->button() == Qt::LeftButton )
    {
        QGraphicsItem* item = itemAt(event->pos());
        Node* possibleNode = qgraphicsitem_cast<Node*>(item);
        Node* possibleNodeParent = nullptr;
        if( item != nullptr )
            possibleNodeParent = qgraphicsitem_cast<Node*>( item->parentItem() );
        if( possibleNode == nullptr )
            possibleNode = possibleNodeParent;
        if( possibleNode != nullptr )
        {
           m_selectedNode = possibleNode;
           m_selectedNode->setZValue( m_lastZValue + 0.1f );
           m_movingNode = possibleNode;
           m_oldMovingNodePos = m_movingNode->pos();
           emit NodeSelected( m_selectedNode );
        }
        else
            emit NodeSelected( nullptr );
    }
    else if( event->button() == Qt::RightButton )
    {
        ShowContextMenu(event->pos());
    }
    QGraphicsView::mousePressEvent(event);
}
//-----------------------------------------------------------------
void MaterialGraphWidget::mouseReleaseEvent( QMouseEvent* event )
{
    if (m_movingNode != nullptr && event->button() == Qt::LeftButton) {
            if ( m_oldMovingNodePos != m_movingNode->pos() )
                emit ItemMoved (m_movingNode, m_oldMovingNodePos );
            m_movingNode = nullptr;
        }
    QGraphicsView::mouseReleaseEvent(event);
}
//-----------------------------------------------------------------
void MaterialGraphWidget::ShowAddNodeContextMenu( const QPoint& pos )
{
    QPoint globalPos = mapToGlobal(pos);

    QAction* selectedItem = m_addNodeContextMenu->exec(globalPos);
    if( selectedItem )
    {
        AddNewNode( selectedItem->iconText(), mapToScene(pos) );
    }
    else
    {}
}
//-----------------------------------------------------------------
void MaterialGraphWidget::ShowDeleteNodeContextMenu(const QPoint& pos , Node *node)
{
    QPoint globalPos = mapToGlobal(pos);

    QMenu myMenu;
    myMenu.addAction( "Delete Node" );

    QAction* selectedItem = myMenu.exec(globalPos);
    if( selectedItem )
    {
        DeleteNode( node );
    }
    else
    {}
}
//----------------------------------------------------------------
void MaterialGraphWidget::ShowBreakLinkContextMenu(const QPoint& pos , Connector *connector)
{
    QPoint globalPos = mapToGlobal(pos);

    QMenu myMenu;
    myMenu.addAction( "Break all links" );

    QAction* selectedItem = myMenu.exec(globalPos);
    if( selectedItem )
    {
        if( connector != nullptr && !connector->GetLinks().isEmpty() )
            BreakAllLinks( connector );
    }
    else
    {}
}
//----------------------------------------------------------------
void MaterialGraphWidget::ConstructConextMenus()
{
    m_addNodeContextMenu = new QMenu();
    const QMap< QString, NodeType >& nodeTypes = NodeType::GetNodeTypes();

    for( auto iter = nodeTypes.begin(); iter != nodeTypes.end(); ++iter )
    {
        if( iter.value().shouldShowInMenu )
            m_addNodeContextMenu->addAction( iter.key() );
    }
}
//----------------------------------------------------------------
void MaterialGraphWidget::RemoveAllItems()
{
    QList< QGraphicsItem* > itemsList = scene()->items();
    foreach( QGraphicsItem* item, itemsList )
    {
        if( item->scene() == scene() )
            scene()->removeItem( item );
    }
}


