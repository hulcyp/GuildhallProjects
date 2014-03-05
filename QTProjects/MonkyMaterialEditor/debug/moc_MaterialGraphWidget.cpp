/****************************************************************************
** Meta object code from reading C++ file 'MaterialGraphWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../MaterialGraphWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MaterialGraphWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MaterialGraphWidget_t {
    QByteArrayData data[26];
    char stringdata[241];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_MaterialGraphWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_MaterialGraphWidget_t qt_meta_stringdata_MaterialGraphWidget = {
    {
QT_MOC_LITERAL(0, 0, 19),
QT_MOC_LITERAL(1, 20, 9),
QT_MOC_LITERAL(2, 30, 0),
QT_MOC_LITERAL(3, 31, 5),
QT_MOC_LITERAL(4, 37, 9),
QT_MOC_LITERAL(5, 47, 17),
QT_MOC_LITERAL(6, 65, 12),
QT_MOC_LITERAL(7, 78, 4),
QT_MOC_LITERAL(8, 83, 6),
QT_MOC_LITERAL(9, 90, 7),
QT_MOC_LITERAL(10, 98, 15),
QT_MOC_LITERAL(11, 114, 3),
QT_MOC_LITERAL(12, 118, 10),
QT_MOC_LITERAL(13, 129, 4),
QT_MOC_LITERAL(14, 134, 8),
QT_MOC_LITERAL(15, 143, 10),
QT_MOC_LITERAL(16, 154, 12),
QT_MOC_LITERAL(17, 167, 7),
QT_MOC_LITERAL(18, 175, 10),
QT_MOC_LITERAL(19, 186, 3),
QT_MOC_LITERAL(20, 190, 4),
QT_MOC_LITERAL(21, 195, 9),
QT_MOC_LITERAL(22, 205, 5),
QT_MOC_LITERAL(23, 211, 4),
QT_MOC_LITERAL(24, 216, 13),
QT_MOC_LITERAL(25, 230, 9)
    },
    "MaterialGraphWidget\0ItemMoved\0\0Node*\0"
    "movedNode\0moveStartPosition\0NodeSelected\0"
    "node\0zoomIn\0zoomOut\0ShowContextMenu\0"
    "pos\0AddNewNode\0name\0scenePos\0DeleteNode\0"
    "nodeToDelete\0AddLink\0Connector*\0src\0"
    "dest\0BreakLink\0Link*\0link\0BreakAllLinks\0"
    "connector\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MaterialGraphWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   64,    2, 0x06,
       6,    1,   69,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
       8,    0,   72,    2, 0x0a,
       9,    0,   73,    2, 0x0a,
      10,    1,   74,    2, 0x0a,
      12,    2,   77,    2, 0x0a,
      15,    1,   82,    2, 0x0a,
      17,    2,   85,    2, 0x0a,
      21,    1,   90,    2, 0x0a,
      24,    1,   93,    2, 0x0a,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::QPointF,    4,    5,
    QMetaType::Void, 0x80000000 | 3,    7,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QPoint,   11,
    QMetaType::Void, QMetaType::QString, QMetaType::QPointF,   13,   14,
    QMetaType::Void, 0x80000000 | 3,   16,
    QMetaType::Void, 0x80000000 | 18, 0x80000000 | 18,   19,   20,
    QMetaType::Void, 0x80000000 | 22,   23,
    QMetaType::Void, 0x80000000 | 18,   25,

       0        // eod
};

void MaterialGraphWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MaterialGraphWidget *_t = static_cast<MaterialGraphWidget *>(_o);
        switch (_id) {
        case 0: _t->ItemMoved((*reinterpret_cast< Node*(*)>(_a[1])),(*reinterpret_cast< const QPointF(*)>(_a[2]))); break;
        case 1: _t->NodeSelected((*reinterpret_cast< Node*(*)>(_a[1]))); break;
        case 2: _t->zoomIn(); break;
        case 3: _t->zoomOut(); break;
        case 4: _t->ShowContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 5: _t->AddNewNode((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QPointF(*)>(_a[2]))); break;
        case 6: _t->DeleteNode((*reinterpret_cast< Node*(*)>(_a[1]))); break;
        case 7: _t->AddLink((*reinterpret_cast< Connector*(*)>(_a[1])),(*reinterpret_cast< Connector*(*)>(_a[2]))); break;
        case 8: _t->BreakLink((*reinterpret_cast< Link*(*)>(_a[1]))); break;
        case 9: _t->BreakAllLinks((*reinterpret_cast< Connector*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MaterialGraphWidget::*_t)(Node * , const QPointF & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MaterialGraphWidget::ItemMoved)) {
                *result = 0;
            }
        }
        {
            typedef void (MaterialGraphWidget::*_t)(Node * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MaterialGraphWidget::NodeSelected)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject MaterialGraphWidget::staticMetaObject = {
    { &QGraphicsView::staticMetaObject, qt_meta_stringdata_MaterialGraphWidget.data,
      qt_meta_data_MaterialGraphWidget,  qt_static_metacall, 0, 0}
};


const QMetaObject *MaterialGraphWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MaterialGraphWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MaterialGraphWidget.stringdata))
        return static_cast<void*>(const_cast< MaterialGraphWidget*>(this));
    return QGraphicsView::qt_metacast(_clname);
}

int MaterialGraphWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void MaterialGraphWidget::ItemMoved(Node * _t1, const QPointF & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MaterialGraphWidget::NodeSelected(Node * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
