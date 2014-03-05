#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QGraphicsItem>

class Node;
class Link;

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
class c;
QT_END_NAMESPACE

class Connector : public QGraphicsItem
{
public:

    enum ConnectorType
    {
        CT_INPUT,
        CT_OUTPUT,
        CT_COUNT
    };

    Connector( Node* parent, const QString& name, QPointF& posRelToParent, QPointF& size, ConnectorType type, int idx );

    QList<Link*> GetLinks() const;
    void AddLink( Link* link );
    void RemoveLink( Link* link );

    int GetIdx() const { return m_connectorIdx; }

    ConnectorType GetConnectorType() const { return m_connectorType; }

    //Qt overriden functions
    enum { Type = UserType + 3 };
    int type() const { return Type; }

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget );

    Node* GetParent() const { return m_parent; }
    const QString& GetName() const { return m_name; }

    bool CanRecieveLink() const;

signals:
    void AddLink( Connector* src, Connector* dest );
    void BreakLink( Link* link );

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);



private:
    Node* m_parent;
    QPointF m_posRelToParent;
    QList<Link*> m_links;
    QPointF m_size;
    QPointF m_hSize;
    ConnectorType m_connectorType;
    QString m_name;
    QGraphicsTextItem* m_label;
    qreal m_labelPadding;
    int m_connectorIdx;

    QGraphicsLineItem* m_line;
};

#endif // CONNECTOR_H
