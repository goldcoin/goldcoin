/****************************************************************************
** Meta object code from reading C++ file 'signverifymessagedialog.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/qt/signverifymessagedialog.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'signverifymessagedialog.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_SignVerifyMessageDialog_t {
    uint offsetsAndSizes[20];
    char stringdata0[24];
    char stringdata1[32];
    char stringdata2[1];
    char stringdata3[26];
    char stringdata4[32];
    char stringdata5[34];
    char stringdata6[26];
    char stringdata7[32];
    char stringdata8[34];
    char stringdata9[26];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_SignVerifyMessageDialog_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_SignVerifyMessageDialog_t qt_meta_stringdata_SignVerifyMessageDialog = {
    {
        QT_MOC_LITERAL(0, 23),  // "SignVerifyMessageDialog"
        QT_MOC_LITERAL(24, 31),  // "on_addressBookButton_SM_clicked"
        QT_MOC_LITERAL(56, 0),  // ""
        QT_MOC_LITERAL(57, 25),  // "on_pasteButton_SM_clicked"
        QT_MOC_LITERAL(83, 31),  // "on_signMessageButton_SM_clicked"
        QT_MOC_LITERAL(115, 33),  // "on_copySignatureButton_SM_cli..."
        QT_MOC_LITERAL(149, 25),  // "on_clearButton_SM_clicked"
        QT_MOC_LITERAL(175, 31),  // "on_addressBookButton_VM_clicked"
        QT_MOC_LITERAL(207, 33),  // "on_verifyMessageButton_VM_cli..."
        QT_MOC_LITERAL(241, 25)   // "on_clearButton_VM_clicked"
    },
    "SignVerifyMessageDialog",
    "on_addressBookButton_SM_clicked",
    "",
    "on_pasteButton_SM_clicked",
    "on_signMessageButton_SM_clicked",
    "on_copySignatureButton_SM_clicked",
    "on_clearButton_SM_clicked",
    "on_addressBookButton_VM_clicked",
    "on_verifyMessageButton_VM_clicked",
    "on_clearButton_VM_clicked"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_SignVerifyMessageDialog[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   62,    2, 0x08,    1 /* Private */,
       3,    0,   63,    2, 0x08,    2 /* Private */,
       4,    0,   64,    2, 0x08,    3 /* Private */,
       5,    0,   65,    2, 0x08,    4 /* Private */,
       6,    0,   66,    2, 0x08,    5 /* Private */,
       7,    0,   67,    2, 0x08,    6 /* Private */,
       8,    0,   68,    2, 0x08,    7 /* Private */,
       9,    0,   69,    2, 0x08,    8 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject SignVerifyMessageDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_SignVerifyMessageDialog.offsetsAndSizes,
    qt_meta_data_SignVerifyMessageDialog,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_SignVerifyMessageDialog_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<SignVerifyMessageDialog, std::true_type>,
        // method 'on_addressBookButton_SM_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_pasteButton_SM_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_signMessageButton_SM_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_copySignatureButton_SM_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_clearButton_SM_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_addressBookButton_VM_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_verifyMessageButton_VM_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_clearButton_VM_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void SignVerifyMessageDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SignVerifyMessageDialog *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_addressBookButton_SM_clicked(); break;
        case 1: _t->on_pasteButton_SM_clicked(); break;
        case 2: _t->on_signMessageButton_SM_clicked(); break;
        case 3: _t->on_copySignatureButton_SM_clicked(); break;
        case 4: _t->on_clearButton_SM_clicked(); break;
        case 5: _t->on_addressBookButton_VM_clicked(); break;
        case 6: _t->on_verifyMessageButton_VM_clicked(); break;
        case 7: _t->on_clearButton_VM_clicked(); break;
        default: ;
        }
    }
    (void)_a;
}

const QMetaObject *SignVerifyMessageDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SignVerifyMessageDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SignVerifyMessageDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int SignVerifyMessageDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
