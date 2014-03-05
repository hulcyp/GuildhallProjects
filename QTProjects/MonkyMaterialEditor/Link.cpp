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
#include "Connector.h"

#include <math.h>
#include <assert.h>

#include <QPainter>
#include <QStyleOption>

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

Link::Link(Connector *sourceConnector, Connector *destConnector )
    : m_arrowSize(10)
{
    assert( sourceConnector != nullptr && destConnector != nullptr );
    setAcceptedMouseButtons(0);
    m_source = sourceConnector;
    m_dest = destConnector;
    m_source->AddLink( this );
    m_dest->AddLink( this );
    adjust();
}
//-------------------------------------------------------------------
Link::~Link()
{
}
//-------------------------------------------------------------------
Connector *Link::SourceConnector() const
{
    return m_source;
}
//-------------------------------------------------------------------
Connector *Link::DestConnector() const
{
    return m_dest;
}
//-------------------------------------------------------------------
void Link::SetSource( Connector* src )
{
    if( m_source != nullptr )
        m_source->RemoveLink( this );
    m_source = src;
    if( m_source != nullptr )
        m_source->AddLink( this );

}
//-------------------------------------------------------------------
void Link::SetDest( Connector* dest )
{
    if( m_dest != nullptr )
       m_dest->RemoveLink( this );
    m_dest = dest;
    if( m_dest != nullptr )
        m_dest->AddLink( this );
}
//-------------------------------------------------------------------
void Link::adjust()
{
    if (!m_source || !m_dest)
        return;

    QLineF line(mapFromItem(m_source, 0, 0 ), mapFromItem(m_dest, 0, 0 ) );

    prepareGeometryChange();

    m_sourcePoint = line.p1();
    m_destPoint = line.p2();
}
//-------------------------------------------------------------------
QRectF Link::boundingRect() const
{
    if (!m_source || !m_dest)
        return QRectF();

    qreal penWidth = 1;
    qreal extra = (penWidth + m_arrowSize) / 2.0;

    return QRectF(m_sourcePoint, QSizeF(m_destPoint.x() - m_sourcePoint.x(),
                                      m_destPoint.y() - m_sourcePoint.y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}
//-------------------------------------------------------------------
QPainterPath Link::shape() const
{
    QPainterPath path;
    path.addPolygon(m_arrow);
    return path;
}
//-------------------------------------------------------------------
void Link::paint(QPainter *painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
    Q_UNUSED(widget)
    if (!m_source || !m_dest)
        return;

    QLineF line(m_sourcePoint, m_destPoint);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;


    // Draw the line itself
    if( option->state & QStyle::State_Sunken )
    {
        painter->setPen(QPen(Qt::yellow, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    }
    else
    {
        painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    }
    painter->drawLine(line);


    // Draw the arrows
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
        angle = TwoPi - angle;

//    QPointF sourceArrowP1 = sourcePoint + QPointF(sin(angle + Pi / 3) * arrowSize,
//                                                  cos(angle + Pi / 3) * arrowSize);
//    QPointF sourceArrowP2 = sourcePoint + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
//                                                  cos(angle + Pi - Pi / 3) * arrowSize);
    QPointF destArrowP1 = m_destPoint + QPointF(sin(angle - Pi / 3) * m_arrowSize,
                                              cos(angle - Pi / 3) * m_arrowSize);
    QPointF destArrowP2 = m_destPoint + QPointF(sin(angle - Pi + Pi / 3) * m_arrowSize,
                                              cos(angle - Pi + Pi / 3) * m_arrowSize);

    painter->setPen( QPen( Qt::gray, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin ) );
    if( option->state & QStyle::State_Sunken )
    {
        painter->setBrush(Qt::yellow);
    }
    else
    {
        painter->setBrush(Qt::black);
    }
    m_arrow.clear();
    m_arrow << line.p2() << destArrowP1 << destArrowP2;
    painter->drawPolygon( m_arrow );
}
