/****************************************************************************
** Meta object code from reading C++ file 'jjoystick.h'
**
** Created: Sat Jun 8 14:11:11 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../remoteControll/jjoystick.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'jjoystick.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_JJoystick[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   11,   10,   10, 0x05,
      54,   44,   10,   10, 0x05,
      87,   79,   10,   10, 0x05,
     129,  117,   10,   10, 0x05,
     160,  153,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
     183,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_JJoystick[] = {
    "JJoystick\0\0num,value\0axisChanged(int,float)\0"
    "num,dx,dy\0ballChanged(int,int,int)\0"
    "num,val\0hatChanged(int,unsigned char)\0"
    "num,pressed\0buttonChanged(int,bool)\0"
    "status\0statusChanged(QString)\0timeout()\0"
};

void JJoystick::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        JJoystick *_t = static_cast<JJoystick *>(_o);
        switch (_id) {
        case 0: _t->axisChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 1: _t->ballChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 2: _t->hatChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< unsigned char(*)>(_a[2]))); break;
        case 3: _t->buttonChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 4: _t->statusChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->timeout(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData JJoystick::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject JJoystick::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_JJoystick,
      qt_meta_data_JJoystick, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &JJoystick::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *JJoystick::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *JJoystick::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_JJoystick))
        return static_cast<void*>(const_cast< JJoystick*>(this));
    return QThread::qt_metacast(_clname);
}

int JJoystick::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void JJoystick::axisChanged(int _t1, float _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void JJoystick::ballChanged(int _t1, int _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void JJoystick::hatChanged(int _t1, unsigned char _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void JJoystick::buttonChanged(int _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void JJoystick::statusChanged(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
