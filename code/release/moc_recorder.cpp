/****************************************************************************
** Meta object code from reading C++ file 'recorder.h'
**
** Created: Tue 2. Jan 15:30:34 2007
**      by: The Qt Meta Object Compiler version 59 (Qt 4.1.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../recorder.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'recorder.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.1.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_Recorder[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x05,
      29,    9,    9,    9, 0x05,
      49,    9,    9,    9, 0x05,
      64,    9,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
      82,    9,    9,    9, 0x0a,
      99,    9,    9,    9, 0x0a,
     115,    9,    9,    9, 0x0a,
     129,    9,    9,    9, 0x0a,
     143,    9,    9,    9, 0x0a,
     160,    9,    9,    9, 0x0a,
     173,    9,    9,    9, 0x0a,
     186,    9,    9,    9, 0x0a,
     201,    9,    9,    9, 0x0a,
     220,    9,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Recorder[] = {
    "Recorder\0\0RemakeRecordName()\0GetSettingsSignal()\0SavedComment()\0"
    "passwordMatches()\0MakeRecordName()\0CreateComment()\0GetSettings()\0"
    "SaveComment()\0ShowHiddenMenu()\0DoActivate()\0DoPlayback()\0"
    "DoFMODConfig()\0DoVoiceLogConfig()\0CheckPassMatches()\0"
};

const QMetaObject Recorder::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Recorder,
      qt_meta_data_Recorder, 0 }
};

const QMetaObject *Recorder::metaObject() const
{
    return &staticMetaObject;
}

void *Recorder::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Recorder))
	return static_cast<void*>(const_cast<Recorder*>(this));
    return QWidget::qt_metacast(_clname);
}

int Recorder::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: RemakeRecordName(); break;
        case 1: GetSettingsSignal(); break;
        case 2: SavedComment(); break;
        case 3: passwordMatches(); break;
        case 4: MakeRecordName(); break;
        case 5: CreateComment(); break;
        case 6: GetSettings(); break;
        case 7: SaveComment(); break;
        case 8: ShowHiddenMenu(); break;
        case 9: DoActivate(); break;
        case 10: DoPlayback(); break;
        case 11: DoFMODConfig(); break;
        case 12: DoVoiceLogConfig(); break;
        case 13: CheckPassMatches(); break;
        }
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void Recorder::RemakeRecordName()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void Recorder::GetSettingsSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void Recorder::SavedComment()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void Recorder::passwordMatches()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
