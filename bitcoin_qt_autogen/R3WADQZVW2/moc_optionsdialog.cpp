/****************************************************************************
** Meta object code from reading C++ file 'optionsdialog.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/qt/optionsdialog.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'optionsdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_ProxyAddressValidator_t {
    uint offsetsAndSizes[2];
    char stringdata0[22];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_ProxyAddressValidator_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_ProxyAddressValidator_t qt_meta_stringdata_ProxyAddressValidator = {
    {
        QT_MOC_LITERAL(0, 21)   // "ProxyAddressValidator"
    },
    "ProxyAddressValidator"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_ProxyAddressValidator[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

Q_CONSTINIT const QMetaObject ProxyAddressValidator::staticMetaObject = { {
    QMetaObject::SuperData::link<QValidator::staticMetaObject>(),
    qt_meta_stringdata_ProxyAddressValidator.offsetsAndSizes,
    qt_meta_data_ProxyAddressValidator,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_ProxyAddressValidator_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ProxyAddressValidator, std::true_type>
    >,
    nullptr
} };

void ProxyAddressValidator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    (void)_o;
    (void)_id;
    (void)_c;
    (void)_a;
}

const QMetaObject *ProxyAddressValidator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ProxyAddressValidator::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ProxyAddressValidator.stringdata0))
        return static_cast<void*>(this);
    return QValidator::qt_metacast(_clname);
}

int ProxyAddressValidator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QValidator::qt_metacall(_c, _id, _a);
    return _id;
}
namespace {
struct qt_meta_stringdata_OptionsDialog_t {
    uint offsetsAndSizes[34];
    char stringdata0[14];
    char stringdata1[14];
    char stringdata2[1];
    char stringdata3[20];
    char stringdata4[11];
    char stringdata5[11];
    char stringdata6[17];
    char stringdata7[7];
    char stringdata8[23];
    char stringdata9[20];
    char stringdata10[24];
    char stringdata11[29];
    char stringdata12[19];
    char stringdata13[12];
    char stringdata14[17];
    char stringdata15[27];
    char stringdata16[23];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_OptionsDialog_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_OptionsDialog_t qt_meta_stringdata_OptionsDialog = {
    {
        QT_MOC_LITERAL(0, 13),  // "OptionsDialog"
        QT_MOC_LITERAL(14, 13),  // "proxyIpChecks"
        QT_MOC_LITERAL(28, 0),  // ""
        QT_MOC_LITERAL(29, 19),  // "QValidatedLineEdit*"
        QT_MOC_LITERAL(49, 10),  // "pUiProxyIp"
        QT_MOC_LITERAL(60, 10),  // "nProxyPort"
        QT_MOC_LITERAL(71, 16),  // "setOkButtonState"
        QT_MOC_LITERAL(88, 6),  // "fState"
        QT_MOC_LITERAL(95, 22),  // "on_resetButton_clicked"
        QT_MOC_LITERAL(118, 19),  // "on_okButton_clicked"
        QT_MOC_LITERAL(138, 23),  // "on_cancelButton_clicked"
        QT_MOC_LITERAL(162, 28),  // "on_hideTrayIcon_stateChanged"
        QT_MOC_LITERAL(191, 18),  // "showRestartWarning"
        QT_MOC_LITERAL(210, 11),  // "fPersistent"
        QT_MOC_LITERAL(222, 16),  // "clearStatusLabel"
        QT_MOC_LITERAL(239, 26),  // "updateProxyValidationState"
        QT_MOC_LITERAL(266, 22)   // "updateDefaultProxyNets"
    },
    "OptionsDialog",
    "proxyIpChecks",
    "",
    "QValidatedLineEdit*",
    "pUiProxyIp",
    "nProxyPort",
    "setOkButtonState",
    "fState",
    "on_resetButton_clicked",
    "on_okButton_clicked",
    "on_cancelButton_clicked",
    "on_hideTrayIcon_stateChanged",
    "showRestartWarning",
    "fPersistent",
    "clearStatusLabel",
    "updateProxyValidationState",
    "updateDefaultProxyNets"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_OptionsDialog[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,   80,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       6,    1,   85,    2, 0x08,    4 /* Private */,
       8,    0,   88,    2, 0x08,    6 /* Private */,
       9,    0,   89,    2, 0x08,    7 /* Private */,
      10,    0,   90,    2, 0x08,    8 /* Private */,
      11,    1,   91,    2, 0x08,    9 /* Private */,
      12,    1,   94,    2, 0x08,   11 /* Private */,
      12,    0,   97,    2, 0x28,   13 /* Private | MethodCloned */,
      14,    0,   98,    2, 0x08,   14 /* Private */,
      15,    0,   99,    2, 0x08,   15 /* Private */,
      16,    0,  100,    2, 0x08,   16 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int,    4,    5,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Bool,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject OptionsDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_OptionsDialog.offsetsAndSizes,
    qt_meta_data_OptionsDialog,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_OptionsDialog_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<OptionsDialog, std::true_type>,
        // method 'proxyIpChecks'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QValidatedLineEdit *, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'setOkButtonState'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'on_resetButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_okButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_cancelButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_hideTrayIcon_stateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'showRestartWarning'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'showRestartWarning'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'clearStatusLabel'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateProxyValidationState'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateDefaultProxyNets'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void OptionsDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<OptionsDialog *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->proxyIpChecks((*reinterpret_cast< std::add_pointer_t<QValidatedLineEdit*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 1: _t->setOkButtonState((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 2: _t->on_resetButton_clicked(); break;
        case 3: _t->on_okButton_clicked(); break;
        case 4: _t->on_cancelButton_clicked(); break;
        case 5: _t->on_hideTrayIcon_stateChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->showRestartWarning((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 7: _t->showRestartWarning(); break;
        case 8: _t->clearStatusLabel(); break;
        case 9: _t->updateProxyValidationState(); break;
        case 10: _t->updateDefaultProxyNets(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (OptionsDialog::*)(QValidatedLineEdit * , int );
            if (_t _q_method = &OptionsDialog::proxyIpChecks; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *OptionsDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OptionsDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_OptionsDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int OptionsDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void OptionsDialog::proxyIpChecks(QValidatedLineEdit * _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
