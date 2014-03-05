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

#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QList>
#include <QMap>

#include "./CoreEngine/XMLParser.h"
#include "NodeType.h"

class Connector;
class MaterialGraphWidget;
class Link;
class XMLNodeGenerator;

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
class QGraphicsTextItem;
QT_END_NAMESPACE



#define nullptr 0

//! [0]
class Node : public QGraphicsItem
{
public:
    Node(MaterialGraphWidget *graphWidget, const QString& name, NodeType::NodeTypeID typeID, const QList<QString>& inputs, const QList<QString>& outputs, const QList<QString>& attribs );

    MaterialGraphWidget* Graph() const { return m_graph; }

    bool AddInputLink( Link* edge, int idx );
    bool AddOutputLink( Link* edge, int idx );
    const QList<Connector*>& InputConnectors() const;
    const QList<Connector*>& OuputConnectors() const;

    QList< std::pair< QString, QString > >& Attributes() { return m_attributes; }
    const QList< std::pair< QString, QString > >& Attributes() const { return m_attributes; }

    void AttributesUpdated();

    void SetXMLNodeGenerator( XMLNodeGenerator* generator ){ m_xmlNodeGenerator = generator; }

    bool advance();

    float GetWidth() const { return m_width; }
    float GetHeight() const { return m_height; }
    float GetHalfWidth() const { return m_hWidth; }
    float GetHalfHeight() const { return m_hHeight; }
    const QString& GetName() const { return m_name; }
    NodeType::NodeTypeID GetNodeTypeID() const { return m_nodeTypeID; }


    //Qt overriden functions
    enum { Type = UserType + 1 };
    int type() const { return Type; }

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    Monky::XMLNode* GetXMLNode( Monky::XMLDocument& doc, int connectorIdx ) const;
    Monky::XMLNode* GetVariableDeclaration( Monky::XMLDocument& doc ) const;

    const QString& GetVariableName() const { return m_variableName; }
    void SetVariableName( const QString& variableName ) { m_variableName = variableName; }

    void SetNodeID( int nodeID ) { m_nodeID = nodeID; }
    int GetNodeID() const { return m_nodeID; }

    //Used to temporarily store the variable name possibly associated with this node during the saving process
    //I really shouldn't do it this way but I want to get it working... Then I will try to come back and fix this
    mutable QString m_variableName;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    void GenerateConnectorPos(QList<QPointF>& offsetList, int count, float xPos );


    QList<Connector*> m_inputList;
    QList<QPointF> m_inputConnectorPos;
    QList<Connector*> m_outputList;
    QList<QPointF> m_outputConnectorPos;

    //attribute name to value mapping
    QList< std::pair< QString, QString > > m_attributes;

    MaterialGraphWidget *m_graph;
    XMLNodeGenerator* m_xmlNodeGenerator;
    int m_numInputs;
    int m_numOutputs;

    float m_width;
    float m_height;
    float m_hWidth;
    float m_hHeight;

    float m_connectorSpacing;
    QPointF m_connectorSize;

    QGraphicsTextItem* m_title;
    QString m_name;

    QPointF m_newPos;

    int m_nodeID;

    NodeType::NodeTypeID m_nodeTypeID;


};
//! [0]

#endif // NODE_H
