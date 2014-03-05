/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[17];
    char stringdata[218];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_MainWindow_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 10),
QT_MOC_LITERAL(2, 22, 0),
QT_MOC_LITERAL(3, 23, 9),
QT_MOC_LITERAL(4, 33, 5),
QT_MOC_LITERAL(5, 39, 9),
QT_MOC_LITERAL(6, 49, 17),
QT_MOC_LITERAL(7, 67, 29),
QT_MOC_LITERAL(8, 97, 12),
QT_MOC_LITERAL(9, 110, 4),
QT_MOC_LITERAL(10, 115, 19),
QT_MOC_LITERAL(11, 135, 19),
QT_MOC_LITERAL(12, 155, 11),
QT_MOC_LITERAL(13, 167, 12),
QT_MOC_LITERAL(14, 180, 14),
QT_MOC_LITERAL(15, 195, 12),
QT_MOC_LITERAL(16, 208, 8)
    },
    "MainWindow\0DeleteNode\0\0ItemMoved\0Node*\0"
    "movedNode\0moveStartPosition\0"
    "FinishedEditingAttributeEntry\0"
    "NodeSelected\0node\0EditMenuAboutToShow\0"
    "EditMenuAboutToHide\0NewFileSlot\0"
    "SaveFileSlot\0SaveAsFileSlot\0OpenFileSlot\0"
    "QuitSlot\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x0a,
       3,    2,   70,    2, 0x0a,
       7,    0,   75,    2, 0x0a,
       8,    1,   76,    2, 0x0a,
      10,    0,   79,    2, 0x08,
      11,    0,   80,    2, 0x08,
      12,    0,   81,    2, 0x08,
      13,    0,   82,    2, 0x08,
      14,    0,   83,    2, 0x08,
      15,    0,   84,    2, 0x08,
      16,    0,   85,    2, 0x08,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4, QMetaType::QPointF,    5,    6,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->DeleteNode(); break;
        case 1: _t->ItemMoved((*reinterpret_cast< Node*(*)>(_a[1])),(*reinterpret_cast< const QPointF(*)>(_a[2]))); break;
        case 2: _t->FinishedEditingAttributeEntry(); break;
        case 3: _t->NodeSelected((*reinterpret_cast< Node*(*)>(_a[1]))); break;
        case 4: _t->EditMenuAboutToShow(); break;
        case 5: _t->EditMenuAboutToHide(); break;
        case 6: _t->NewFileSlot(); break;
        case 7: _t->SaveFileSlot(); break;
        case 8: _t->SaveAsFileSlot(); break;
        case 9: _t->OpenFileSlot(); break;
        case 10: _t->QuitSlot(); break;
        default: ;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, 0, 0}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
