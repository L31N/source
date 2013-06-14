/****************************************************************************
** Meta object code from reading C++ file 'jmainwindow.h'
**
** Created: Fri Jun 14 08:21:08 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../remoteControll/jmainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'jmainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_JMainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      24,   13,   12,   12, 0x0a,
      60,   50,   12,   12, 0x0a,
      85,   12,   12,   12, 0x08,
     112,   12,   12,   12, 0x08,
     149,  142,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_JMainWindow[] = {
    "JMainWindow\0\0num,status\0"
    "onButtonChanged(int,bool)\0num,value\0"
    "onAxisChanged(int,float)\0"
    "on_buttonConnect_clicked()\0"
    "on_buttonDisconnect_clicked()\0status\0"
    "connect_enabled(bool)\0"
};

void JMainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        JMainWindow *_t = static_cast<JMainWindow *>(_o);
        switch (_id) {
        case 0: _t->onButtonChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 1: _t->onAxisChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 2: _t->on_buttonConnect_clicked(); break;
        case 3: _t->on_buttonDisconnect_clicked(); break;
        case 4: _t->connect_enabled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData JMainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject JMainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_JMainWindow,
      qt_meta_data_JMainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &JMainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *JMainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *JMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_JMainWindow))
        return static_cast<void*>(const_cast< JMainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int JMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
