/****************************************************************************
** Meta object code from reading C++ file 'jbotsimulator.h'
**
** Created: Sat Jun 8 14:11:12 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../remoteControll/jbotsimulator.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'jbotsimulator.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_JBotSimulator[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      28,   15,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      57,   14,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_JBotSimulator[] = {
    "JBotSimulator\0\0rotation,pos\0"
    "positionChanged(int,JVector)\0updatePos()\0"
};

void JBotSimulator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        JBotSimulator *_t = static_cast<JBotSimulator *>(_o);
        switch (_id) {
        case 0: _t->positionChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< JVector(*)>(_a[2]))); break;
        case 1: _t->updatePos(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData JBotSimulator::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject JBotSimulator::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_JBotSimulator,
      qt_meta_data_JBotSimulator, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &JBotSimulator::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *JBotSimulator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *JBotSimulator::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_JBotSimulator))
        return static_cast<void*>(const_cast< JBotSimulator*>(this));
    return QObject::qt_metacast(_clname);
}

int JBotSimulator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void JBotSimulator::positionChanged(int _t1, JVector _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
