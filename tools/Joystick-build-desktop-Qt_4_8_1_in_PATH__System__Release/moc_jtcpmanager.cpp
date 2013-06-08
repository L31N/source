/****************************************************************************
** Meta object code from reading C++ file 'jtcpmanager.h'
**
** Created: Sat Jun 8 14:11:14 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../remoteControll/jtcpmanager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'jtcpmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_JTcpManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   13,   12,   12, 0x05,
      49,   41,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      68,   65,   12,   12, 0x0a,
      91,   12,   12,   12, 0x0a,
     116,  110,   12,   12, 0x0a,
     131,  110,   12,   12, 0x0a,
     146,  110,   12,   12, 0x0a,
     168,   12,   12,   12, 0x08,
     189,   12,   12,   12, 0x08,
     234,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_JTcpManager[] = {
    "JTcpManager\0\0message\0newMessage(QString)\0"
    "enabled\0newStatus(bool)\0ip\0"
    "connectServer(QString)\0disconnectServer()\0"
    "value\0updateX(float)\0updateY(float)\0"
    "updateRotation(float)\0connectSuccessfull()\0"
    "connectFailure(QAbstractSocket::SocketError)\0"
    "disconnectSuccessfull()\0"
};

void JTcpManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        JTcpManager *_t = static_cast<JTcpManager *>(_o);
        switch (_id) {
        case 0: _t->newMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->newStatus((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->connectServer((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->disconnectServer(); break;
        case 4: _t->updateX((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 5: _t->updateY((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 6: _t->updateRotation((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 7: _t->connectSuccessfull(); break;
        case 8: _t->connectFailure((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 9: _t->disconnectSuccessfull(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData JTcpManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject JTcpManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_JTcpManager,
      qt_meta_data_JTcpManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &JTcpManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *JTcpManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *JTcpManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_JTcpManager))
        return static_cast<void*>(const_cast< JTcpManager*>(this));
    return QObject::qt_metacast(_clname);
}

int JTcpManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void JTcpManager::newMessage(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void JTcpManager::newStatus(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
