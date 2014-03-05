#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>

QT_BEGIN_NAMESPACE
class QMenu;
QT_END_NAMESPACE

#define nullptr 0

#define MATERIAL_VERSION_NUMBER 1

class Node;
class Connector;
class MainWindow;
class Link;
class GraphLayoutParser;


class MaterialGraphWidget : public QGraphicsView
{
    Q_OBJECT

public:
    friend class GraphLayoutParser;

    MaterialGraphWidget(MainWindow *parent = 0);

    const Node* GetOutputChannelsNode() const { return m_outputChannels; }

    void NewMaterial();
    void SaveMaterial( const QString& filePath );
    void OpenMaterial( const QString& filePath );

    Node* GetSelectedNode() const { return m_selectedNode; }

    int GetNextNodeID() { return m_nextNodeID++; }

    void GraphChanged();
    void GraphUnChanged();
    bool HasGraphChangedSinceLastSave() const { return m_graphChangedSinceLastSave; }

signals:
    void ItemMoved( Node* movedNode, const QPointF &moveStartPosition );
    void NodeSelected( Node* node );

public slots:
    void zoomIn();
    void zoomOut();
    void ShowContextMenu( const QPoint& pos );
    void AddNewNode( const QString& name, const QPointF& scenePos );
    void DeleteNode( Node* nodeToDelete );
    void AddLink( Connector* src, Connector* dest );
    void BreakLink( Link* link );
    void BreakAllLinks( Connector* connector );

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent * event);
    void timerEvent(QTimerEvent *event);
#ifndef QT_NO_WHEELEVENT
    void wheelEvent(QWheelEvent *event);
#endif
    void drawBackground(QPainter *painter, const QRectF &rect);

    void scaleView(qreal scaleFactor);
    void mousePressEvent( QMouseEvent* event );
    void mouseReleaseEvent( QMouseEvent* event );

    void ShowAddNodeContextMenu( const QPoint& pos );
    void ShowDeleteNodeContextMenu( const QPoint& pos, Node* node );
    void ShowBreakLinkContextMenu( const QPoint& pos, Connector* connector );

    void ConstructConextMenus();

    void RemoveAllItems();

private:
    int timerId;
    MainWindow* m_mainWindow;
    QMenu* m_addNodeContextMenu;
    QPoint m_sceneRect;
    Node* m_selectedNode;
    Node* m_movingNode;
    Node* m_outputChannels;
    QPointF m_oldMovingNodePos;
    qreal m_lastZValue;

    int m_nextNodeID;
    bool m_graphChangedSinceLastSave;

};
//! [0]

#endif // GRAPHWIDGET_H
