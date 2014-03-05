/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "Link.h"
#include "Node.h"
#include "MaterialGraphWidget.h"
#include "Connector.h"
#include "XMLNodeGenerator.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QGraphicsTextItem>
#include <QTextBlockFormat>
#include <QTextCursor>
#include <QFont>
#include <QMenu>

#define nullptr 0


Node::Node(MaterialGraphWidget *graphWidget, const QString& name, NodeType::NodeTypeID typeID, const QList<QString> &inputs, const QList<QString> &outputs, const QList<QString> &attribs )
    : m_graph(graphWidget)
    , m_numInputs( inputs.size() )
    , m_numOutputs( outputs.size() )
    , m_width( 200.0f )
    , m_height( 100.0f )
    , m_hWidth( m_width * 0.5f )
    , m_hHeight( m_height * 0.5f )
    , m_connectorSpacing( 8.0f )
    , m_connectorSize( 10.0, 10.0 )
    , m_name( name )
    , m_nodeTypeID( typeID )
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
    setFlag(ItemIsSelectable);

    int maxConnectorsOnOneSide = m_numInputs > m_numOutputs ? m_numInputs : m_numOutputs;
    m_height = ( 2 * maxConnectorsOnOneSide + 4 ) * m_connectorSize.y();
    m_hHeight = m_height * 0.5f;

    GenerateConnectorPos( m_inputConnectorPos, m_numInputs, m_hWidth + m_connectorSize.x() * 0.5f );
    GenerateConnectorPos( m_outputConnectorPos, m_numOutputs, -m_hWidth - m_connectorSize.x() * 0.5f );

    m_inputList.reserve( m_numInputs );
    for( int i = 0; i < m_numInputs; ++i )
        m_inputList.push_back( new Connector(this, inputs[i], m_inputConnectorPos[i], m_connectorSize, Connector::CT_INPUT, i ) );

    m_outputList.reserve( m_numOutputs );
    for( int i = 0; i < m_numOutputs; ++i )
        m_outputList.push_back( new Connector(this, outputs[i], m_outputConnectorPos[i], m_connectorSize, Connector::CT_OUTPUT, i ) );


    for( int i = 0; i < attribs.size(); ++i )
    {
        m_attributes.push_back( std::pair< QString, QString >( attribs[i], "" ) );
    }

    m_title = new QGraphicsTextItem(this);
    m_title->setPlainText( m_name );
    m_title->setTextWidth( m_width );
    m_title->setPos( -m_hWidth, -m_hHeight );
    QTextBlockFormat format;
    format.setAlignment(Qt::AlignCenter);
    QTextCursor cursor = m_title->textCursor();
    cursor.select(QTextCursor::Document);
    cursor.mergeBlockFormat(format);
    cursor.clearSelection();
    m_title->setTextCursor(cursor);
    QFont font = m_title->font();
    font.setLetterSpacing( QFont::PercentageSpacing, 110);
    m_title->setFont( font );
}
//------------------------------------------------------------------------------------------
bool Node::AddInputLink(Link *edge, int idx)
{
    bool added = false;
    if( edge != nullptr && idx < m_numInputs && m_inputList[idx] == nullptr )
    {
        m_inputList[idx]->AddLink( edge );
        edge->adjust();
        added = true;
    }
    return added;
}
//------------------------------------------------------------------------------------------
bool Node::AddOutputLink(Link *edge, int idx)
{
    bool added = false;
    if( edge != nullptr && idx < m_numOutputs && m_outputList[idx] == nullptr )
    {
        m_outputList[idx]->AddLink( edge );
        edge->adjust();
        added = true;
    }
    return added;
}
//------------------------------------------------------------------------------------------
const QList<Connector*>& Node::InputConnectors() const
{
    return m_inputList;
}
//------------------------------------------------------------------------------------------
const QList<Connector*>& Node::OuputConnectors() const
{
    return m_outputList;
}
//------------------------------------------------------------------------------------------
void Node::AttributesUpdated()
{
    if( m_attributes.size() > 0 && m_attributes[0].first.compare( "Name" ) == 0 && !m_attributes[0].second.isEmpty() )
    {
        m_title->setPlainText( m_name + "(" + m_attributes[0].second + ")" );\
        m_title->setTextWidth( m_width );
        m_title->setPos( -m_hWidth, -m_hHeight );
        QTextBlockFormat format;
        format.setAlignment(Qt::AlignCenter);
        QTextCursor cursor = m_title->textCursor();
        cursor.select(QTextCursor::Document);
        cursor.mergeBlockFormat(format);
        cursor.clearSelection();
        m_title->setTextCursor(cursor);
        QFont font = m_title->font();
        font.setLetterSpacing( QFont::PercentageSpacing, 110);
        m_title->setFont( font );
    }
}
//------------------------------------------------------------------------------------------
bool Node::advance()
{
    if (!scene() || scene()->mouseGrabberItem() == this)
    {
        m_newPos = pos();
        setPos(m_newPos);
    }
    else
    {
        QRectF sceneRect = scene()->sceneRect();
        m_newPos = pos();
        m_newPos.setX(qMin(qMax(m_newPos.x(), sceneRect.left() + m_hWidth), sceneRect.right() - m_hWidth));
        m_newPos.setY(qMin(qMax(m_newPos.y(), sceneRect.top() + m_hHeight), sceneRect.bottom() - m_hHeight));
    }
    prepareGeometryChange();
    return true;
}
//------------------------------------------------------------------------------------------
QRectF Node::boundingRect() const
{
    qreal adjust = 0;//m_connectorSize.x();
    return QRectF( -m_hWidth - adjust, -m_hHeight - adjust, m_width + 2*adjust, m_height + 2*adjust);
}
//------------------------------------------------------------------------------------------
QPainterPath Node::shape() const
{
    QPainterPath path;
    path.addRect(-m_hWidth, -m_hHeight, m_width, m_height);
    //path.addRoundedRect(-10,-10,10,10,1,1);
    return path;
}
//------------------------------------------------------------------------------------------
void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{

    QRadialGradient gradient(0, 0, m_hWidth);
    if (option->state & QStyle::State_Sunken) {
        gradient.setCenter(0, 0);
        gradient.setFocalPoint(0, 0);
        gradient.setColorAt(1, QColor(Qt::darkGray).light(120));
        gradient.setColorAt(0, QColor(Qt::darkGray).light(120));
    } else {
        gradient.setColorAt(0, Qt::gray);
        gradient.setColorAt(1, Qt::darkGray);
    }

    if( isSelected() )
    {
        painter->setPen(QPen(Qt::yellow, 2));
    }
    else
    {
        painter->setPen(QPen(Qt::black, 2));
    }
    painter->setBrush(gradient);

    painter->drawRect( -m_hWidth, -m_hHeight, m_width, m_height);
}
//------------------------------------------------------------------------------------------
Monky::XMLNode* Node::GetXMLNode( Monky::XMLDocument &doc, int connectorIdx ) const
{
    if( m_xmlNodeGenerator != nullptr )
        return m_xmlNodeGenerator->GetXMLNode( this, doc, connectorIdx );
    else
        return nullptr;
}
//------------------------------------------------------------------------------------------
Monky::XMLNode* Node::GetVariableDeclaration( Monky::XMLDocument& doc ) const
{
    if( m_xmlNodeGenerator != nullptr )
        return m_xmlNodeGenerator->GetVariableDeclarationXMLNodeFromNode( this, doc );
    else
        return nullptr;
}
//------------------------------------------------------------------------------------------
QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        foreach (Connector *connector, m_inputList)
        {
            if( connector != nullptr )
            {
                foreach( Link* link, connector->GetLinks() )
                {
                    if( link != nullptr )
                        link->adjust();
                }
            }
        }
        foreach (Connector *connector, m_outputList)
        {
            if( connector != nullptr )
            {
                foreach( Link* link, connector->GetLinks() )
                {
                    if( link != nullptr )
                        link->adjust();
                }
            }
        }
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}
//------------------------------------------------------------------------------------------
void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mousePressEvent(event);
}
//------------------------------------------------------------------------------------------
void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
//------------------------------------------------------------------------------------------
void Node::GenerateConnectorPos( QList<QPointF>& offsetList, int count, float xPos )
{
    float spacingPlusHeight = m_connectorSpacing + m_connectorSize.y();
    //Is even
    offsetList.reserve( count );
    if( ( count & 1 ) == 0 )
    {
        int hCount = count / 2;
        float hCountMinusOne = ( count - 1 ) * 0.5f;
        for( int i = 0; i < hCount; ++i )
        {
            offsetList.push_back( QPointF( xPos, ( hCountMinusOne - i ) * spacingPlusHeight ) );
        }

        int c = hCount - 1;
        for( int i = hCount; i < count && c >= 0; ++i, --c)
        {
            offsetList.push_back( QPointF( xPos, -offsetList[c].y() ) );
        }
    }
    else
    {
        int hCount = count / 2;
        for( int i = 0; i < hCount; ++i )
        {
            offsetList.push_back( QPointF( xPos, ( hCount - i ) * spacingPlusHeight ) );
        }
        offsetList.push_back( QPointF( xPos, 0 ) );
        int c = hCount - 1;
        for( int i = hCount + 1; i < count && c >= 0; ++i, --c )
        {
            offsetList.push_back( QPointF( xPos, -offsetList[c].y() ) );
        }
    }
}
