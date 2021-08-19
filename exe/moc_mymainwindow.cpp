/****************************************************************************
** Meta object code from reading C++ file 'mymainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Elevator/mymainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mymainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_myMainWindow_t {
    QByteArrayData data[10];
    char stringdata0[157];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_myMainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_myMainWindow_t qt_meta_stringdata_myMainWindow = {
    {
QT_MOC_LITERAL(0, 0, 12), // "myMainWindow"
QT_MOC_LITERAL(1, 13, 19), // "on_emergencyclicked"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 17), // "on_diable1clicked"
QT_MOC_LITERAL(4, 52, 18), // "on_disable2clicked"
QT_MOC_LITERAL(5, 71, 18), // "on_disable3clicked"
QT_MOC_LITERAL(6, 90, 18), // "on_disable4clicked"
QT_MOC_LITERAL(7, 109, 18), // "on_disable5clicked"
QT_MOC_LITERAL(8, 128, 18), // "on_disable6clicked"
QT_MOC_LITERAL(9, 147, 9) // "setIsbusy"

    },
    "myMainWindow\0on_emergencyclicked\0\0"
    "on_diable1clicked\0on_disable2clicked\0"
    "on_disable3clicked\0on_disable4clicked\0"
    "on_disable5clicked\0on_disable6clicked\0"
    "setIsbusy"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_myMainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x08 /* Private */,
       3,    0,   55,    2, 0x08 /* Private */,
       4,    0,   56,    2, 0x08 /* Private */,
       5,    0,   57,    2, 0x08 /* Private */,
       6,    0,   58,    2, 0x08 /* Private */,
       7,    0,   59,    2, 0x08 /* Private */,
       8,    0,   60,    2, 0x08 /* Private */,
       9,    0,   61,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void myMainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        myMainWindow *_t = static_cast<myMainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_emergencyclicked(); break;
        case 1: _t->on_diable1clicked(); break;
        case 2: _t->on_disable2clicked(); break;
        case 3: _t->on_disable3clicked(); break;
        case 4: _t->on_disable4clicked(); break;
        case 5: _t->on_disable5clicked(); break;
        case 6: _t->on_disable6clicked(); break;
        case 7: _t->setIsbusy(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject myMainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_myMainWindow.data,
      qt_meta_data_myMainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *myMainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *myMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_myMainWindow.stringdata0))
        return static_cast<void*>(const_cast< myMainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int myMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
