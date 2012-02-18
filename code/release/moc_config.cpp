/****************************************************************************
** Meta object code from reading C++ file 'config.h'
**
** Created: Sat 30. Dec 16:59:44 2006
**      by: The Qt Meta Object Compiler version 59 (Qt 4.1.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../config.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'config.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.1.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_GeneralTab[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_GeneralTab[] = {
    "GeneralTab\0"
};

const QMetaObject GeneralTab::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_GeneralTab,
      qt_meta_data_GeneralTab, 0 }
};

const QMetaObject *GeneralTab::metaObject() const
{
    return &staticMetaObject;
}

void *GeneralTab::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GeneralTab))
	return static_cast<void*>(const_cast<GeneralTab*>(this));
    return QWidget::qt_metacast(_clname);
}

int GeneralTab::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_RecordingTab[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_RecordingTab[] = {
    "RecordingTab\0\0FindFile()\0"
};

const QMetaObject RecordingTab::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_RecordingTab,
      qt_meta_data_RecordingTab, 0 }
};

const QMetaObject *RecordingTab::metaObject() const
{
    return &staticMetaObject;
}

void *RecordingTab::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RecordingTab))
	return static_cast<void*>(const_cast<RecordingTab*>(this));
    return QWidget::qt_metacast(_clname);
}

int RecordingTab::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: FindFile(); break;
        }
        _id -= 1;
    }
    return _id;
}
static const uint qt_meta_data_UserTab[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
      27,    8,    8,    8, 0x08,
      48,    8,    8,    8, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_UserTab[] = {
    "UserTab\0\0userPassMatches()\0ChangeUserPassword()\0CheckPassMatches()\0"
};

const QMetaObject UserTab::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_UserTab,
      qt_meta_data_UserTab, 0 }
};

const QMetaObject *UserTab::metaObject() const
{
    return &staticMetaObject;
}

void *UserTab::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_UserTab))
	return static_cast<void*>(const_cast<UserTab*>(this));
    return QWidget::qt_metacast(_clname);
}

int UserTab::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: userPassMatches(); break;
        case 1: ChangeUserPassword(); break;
        case 2: CheckPassMatches(); break;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void UserTab::userPassMatches()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
static const uint qt_meta_data_Config[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x05,

 // slots: signature, parameters, type, tag, flags
      24,    7,    7,    7, 0x0a,
      37,    7,    7,    7, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Config[] = {
    "Config\0\0SavedSettings()\0ShowConfig()\0SaveSettings()\0"
};

const QMetaObject Config::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Config,
      qt_meta_data_Config, 0 }
};

const QMetaObject *Config::metaObject() const
{
    return &staticMetaObject;
}

void *Config::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Config))
	return static_cast<void*>(const_cast<Config*>(this));
    return QWidget::qt_metacast(_clname);
}

int Config::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: SavedSettings(); break;
        case 1: ShowConfig(); break;
        case 2: SaveSettings(); break;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void Config::SavedSettings()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
