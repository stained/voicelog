/****************************************************************************
** Meta object code from reading C++ file 'playback.h'
**
** Created: Tue 2. Jan 15:30:25 2007
**      by: The Qt Meta Object Compiler version 59 (Qt 4.1.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../playback.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'playback.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.1.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_PlayBack[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x0a,
      33,   25,    9,    9, 0x0a,
      50,    9,    9,    9, 0x0a,
      63,    9,    9,    9, 0x0a,
      79,    9,    9,    9, 0x0a,
      90,    9,    9,    9, 0x08,
     103,    9,    9,    9, 0x08,
     117,    9,    9,    9, 0x08,
     130,    9,    9,    9, 0x08,
     146,    9,    9,    9, 0x08,
     166,  162,    9,    9, 0x08,
     185,  181,    9,    9, 0x08,
     203,    9,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_PlayBack[] = {
    "PlayBack\0\0ShowPlayBack()\0row,col\0SetCell(int,int)\0DeleteFile()\0"
    "GetProperties()\0PlayFile()\0FModDoPlay()\0FModDoPause()\0FModDoStop()\0"
    "FModDoSpeedDn()\0FModDoSpeedUp()\0pos\0FModDoPos(int)\0len\0"
    "SetSliderLen(int)\0KillWindow()\0"
};

const QMetaObject PlayBack::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_PlayBack,
      qt_meta_data_PlayBack, 0 }
};

const QMetaObject *PlayBack::metaObject() const
{
    return &staticMetaObject;
}

void *PlayBack::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PlayBack))
	return static_cast<void*>(const_cast<PlayBack*>(this));
    return QWidget::qt_metacast(_clname);
}

int PlayBack::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: ShowPlayBack(); break;
        case 1: SetCell((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: DeleteFile(); break;
        case 3: GetProperties(); break;
        case 4: PlayFile(); break;
        case 5: FModDoPlay(); break;
        case 6: FModDoPause(); break;
        case 7: FModDoStop(); break;
        case 8: FModDoSpeedDn(); break;
        case 9: FModDoSpeedUp(); break;
        case 10: FModDoPos((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: SetSliderLen((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: KillWindow(); break;
        }
        _id -= 13;
    }
    return _id;
}
