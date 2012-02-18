/****************************************************************************
** Meta object code from reading C++ file 'fmod.h'
**
** Created: Tue 2. Jan 15:30:44 2007
**      by: The Qt Meta Object Compiler version 59 (Qt 4.1.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../fmod/fmod.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'fmod.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.1.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_FMod[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      15,    6,    5,    5, 0x05,
      42,   32,    5,    5, 0x05,
      66,   61,    5,    5, 0x05,
      90,   82,    5,    5, 0x05,
     103,    5,    5,    5, 0x05,
     127,  119,    5,    5, 0x05,
     143,  119,    5,    5, 0x05,
     163,    5,    5,    5, 0x05,

 // slots: signature, parameters, type, tag, flags
     178,    5,    5,    5, 0x0a,
     191,    5,    5,    5, 0x0a,
     206,    5,    5,    5, 0x0a,
     224,    5,  220,    5, 0x0a,
     244,    5,    5,    5, 0x0a,
     271,  263,    5,    5, 0x08,
     289,    5,    5,    5, 0x08,
     301,    5,    5,    5, 0x08,
     321,    5,    5,    5, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FMod[] = {
    "FMod\0\0waveForm\0waveChanged(int)\0frequency\0freqChanged(float)\0code\0"
    "dtmfCode(char*)\0waveLen\0waveLen(int)\0SavedSettings()\0filelen\0"
    "lenChanged(int)\0lenFileChanged(int)\0playBackDone()\0showConfig()\0"
    "startMonitor()\0stopMonitor()\0int\0startRecordToFile()\0"
    "stopRecordToFile()\0checked\0listDrivers(bool)\0setDriver()\0"
    "timedRecordToFile()\0timedSpectrum()\0"
};

const QMetaObject FMod::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_FMod,
      qt_meta_data_FMod, 0 }
};

const QMetaObject *FMod::metaObject() const
{
    return &staticMetaObject;
}

void *FMod::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FMod))
	return static_cast<void*>(const_cast<FMod*>(this));
    return QWidget::qt_metacast(_clname);
}

int FMod::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: waveChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: freqChanged((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 2: dtmfCode((*reinterpret_cast< char*(*)>(_a[1]))); break;
        case 3: waveLen((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: SavedSettings(); break;
        case 5: lenChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: lenFileChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: playBackDone(); break;
        case 8: showConfig(); break;
        case 9: startMonitor(); break;
        case 10: stopMonitor(); break;
        case 11: { int _r = startRecordToFile();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 12: stopRecordToFile(); break;
        case 13: listDrivers((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: setDriver(); break;
        case 15: timedRecordToFile(); break;
        case 16: timedSpectrum(); break;
        }
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void FMod::waveChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void FMod::freqChanged(float _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void FMod::dtmfCode(char * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void FMod::waveLen(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void FMod::SavedSettings()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void FMod::lenChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void FMod::lenFileChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void FMod::playBackDone()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}
