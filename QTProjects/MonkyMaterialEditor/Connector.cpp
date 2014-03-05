
#include "Link.h"
#include "Node.h"
#include "Connector.h"
#include "MaterialGraphWidget.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QTextBlockFormat>
#include <QTextCursor>
#include <QFont>


Connector::Connector(Node* parent, const QString &name, QPointF& posRelToParent, QPointF& size, ConnectorType type , int idx)
    : QGraphicsItem( parent )
    , m_parent( parent )
    , m_size( size )
    , m_hSize( size * 0.5f )
    , m_connectorType( type )
    , m_name( name )
    , m_labelPadding( 3.0 )
    , m_line(nullptr)
    , m_connectorIdx( idx )
{
    setCacheMode(DeviceCoordinateCache);
    setFlag(ItemIsSelectable);
    setPos( posRelToParent );

    m_label = new QGraphicsTextItem(parent);
    m_label->setPlainText( m_name );
    m_label->setTextWidth( -m_parent->GetWidth() );
    QFont font = m_label->font();
    font.setPointSize(m_size.y());
    font.setLetterSpacing( QFont::PercentageSpacing, 110);
    m_label->setFont( font );
    QTextBlockFormat format;
    if( m_connectorType == CT_INPUT )
    {
        format.setAlignment(Qt::AlignRight);
        QFontMetrics metrics(font);
        qreal width = metrics.boundingRect( m_name ).width();
        m_label->setPos( -width - m_size.x() - m_labelPadding + posRelToParent.x(), -m_size.y() - m_hSize.y() + posRelToParent.y() );
    }
    else
    {
        format.setAlignment(Qt::AlignLeft);
        m_label->setPos( m_labelPadding + posRelToParent.x(), -m_size.y() - m_hSize.y() + posRelToParent.y() );
    }
    QTextCursor cursor = m_label->textCursor();
    cursor.select(QTextCursor::Document);
    cursor.mergeBlockFormat(format);
    cursor.clearSelection();
    m_label->setTextCursor(cursor);
}

QList<Link *> Connector::GetLinks() const
{
    return m_links;
}

void Connector::AddLink( Link* link )
{
   m_links.push_back( link );
}

void Connector::RemoveLink( Link* link )
{
    m_links.removeOne( link );
}

QRectF Connector::boundingRect() const
{
    float adjust = 2;
    return QRectF( -m_hSize.x() - adjust, -m_hSize.y() - adjust, m_size.x() + 2*adjust, m_size.y() + 2*adjust );
}

QPainterPath Connector::shape() const
{
    QPainterPath path;
    path.addRect( -m_hSize.x(), -m_hSize.y(), m_size.x(), m_size.y() );
    return path;
}

void Connector::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->setPen( QPen(Qt::black, 0 ) );
    painter->setBrush( Qt::black );

    painter->drawRect( -m_hSize.x(), -m_hSize.y(), m_size.x(), m_size.y() );
}

bool Connector::CanRecieveLink() const
{
    return ( m_connectorType == CT_OUTPUT ) || ( m_links.size() == 0 && m_connectorType == CT_INPUT );
}

void Connector::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if( event->button() == Qt::LeftButton && CanRecieveLink() && m_line == nullptr )
    {
        m_line = new QGraphicsLineItem( QLineF(event->scenePos(), event->scenePos()));
        m_line->setPen(QPen(Qt::black, 2));
        m_parent->Graph()->scene()->addItem(m_line);
    }
    QGraphicsItem::mousePressEvent( event );
}

void Connector::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event)
    if( m_line != nullptr )
    {
        QList<QGraphicsItem *> endItems = m_parent->Graph()->scene()->items( m_line->line().p2() );
        if (endItems.count() && endItems.first() == m_line)
            endItems.removeFirst();

        if( endItems.size() > 0 )
        {
            Connector* otherConnector = qgraphicsitem_cast<Connector*>( endItems.first() );
            if( otherConnector != nullptr && m_connectorType != otherConnector->m_connectorType )
            {
                if( !otherConnector->CanRecieveLink() )
                {
                    //If the other cannot recieve links then it must have an input link
                    m_parent->Graph()->BreakAllLinks( this );
                }
                Connector* src = nullptr;
                Connector* dest = nullptr;
                if( m_connectorType == CT_INPUT )
                {
                    src = otherConnector;
                    dest = this;
                }
                else
                {
                    src = this;
                    dest = otherConnector;
                }
                m_parent->Graph()->AddLink( src, dest );
             }
        }
        m_parent->Graph()->scene()->removeItem(m_line);
        delete m_line;
        m_line = nullptr;
    }
    QGraphicsItem::mouseReleaseEvent( event );
}

void Connector::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if( m_line != nullptr)
    {
        QLineF newLine( m_line->line().p1(), event->scenePos() );
        m_line->setLine( newLine );
    }
    QGraphicsItem::mouseMoveEvent( event );
}

