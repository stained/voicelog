/****************************************************************************
** Meta object code from reading C++ file 'vumeter.h'
**
** Created: Sat 30. Dec 17:00:10 2006
**      by: The Qt Meta Object Compiler version 59 (Qt 4.1.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../vumeter.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'vumeter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.1.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_VUMeter[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x05,
      25,    8,    8,    8, 0x05,
      47,   42,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
      69,   63,    8,    8, 0x0a,
      84,   80,    8,    8, 0x0a,
      96,   80,    8,    8, 0x0a,
     112,    8,    8,    8, 0x0a,
     125,    8,    8,    8, 0x0a,
     138,    8,    8,    8, 0x0a,
     153,    8,    8,    8, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_VUMeter[] = {
    "VUMeter\0\0overThreshold()\0underThreshold()\0code\0dtmfCode(char*)\0"
    "point\0setVU(int)\0len\0setLen(int)\0setFileLen(int)\0showRecord()\0"
    "hideRecord()\0showPlayback()\0updateThresholdTimer()\0"
};

const QMetaObject VUMeter::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_VUMeter,
      qt_meta_data_VUMeter, 0 }
};

const QMetaObject *VUMeter::metaObject() const
{
    return &staticMetaObject;
}

void *VUMeter::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_VUMeter))
	return static_cast<void*>(const_cast<VUMeter*>(this));
    return QWidget::qt_metacast(_clname);
}

int VUMeter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: overThreshold(); break;
        case 1: underThreshold(); break;
        case 2: dtmfCode((*reinterpret_cast< char*(*)>(_a[1]))); break;
        case 3: setVU((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: setLen((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: setFileLen((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: showRecord(); break;
        case 7: hideRecord(); break;
        case 8: showPlayback(); break;
        case 9: updateThresholdTimer(); break;
        }
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void VUMeter::overThreshold()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void VUMeter::underThreshold()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void VUMeter::dtmfCode(char * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
