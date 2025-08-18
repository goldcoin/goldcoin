/****************************************************************************
** Meta object code from reading C++ file 'addressbookpage.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/qt/addressbookpage.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'addressbookpage.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_AddressBookPage_t {
    uint offsetsAndSizes[38];
    char stringdata0[16];
    char stringdata1[10];
    char stringdata2[1];
    char stringdata3[5];
    char stringdata4[5];
    char stringdata5[7];
    char stringdata6[25];
    char stringdata7[22];
    char stringdata8[23];
    char stringdata9[18];
    char stringdata10[13];
    char stringdata11[24];
    char stringdata12[17];
    char stringdata13[15];
    char stringdata14[6];
    char stringdata15[17];
    char stringdata16[12];
    char stringdata17[7];
    char stringdata18[6];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_AddressBookPage_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_AddressBookPage_t qt_meta_stringdata_AddressBookPage = {
    {
        QT_MOC_LITERAL(0, 15),  // "AddressBookPage"
        QT_MOC_LITERAL(16, 9),  // "sendCoins"
        QT_MOC_LITERAL(26, 0),  // ""
        QT_MOC_LITERAL(27, 4),  // "addr"
        QT_MOC_LITERAL(32, 4),  // "done"
        QT_MOC_LITERAL(37, 6),  // "retval"
        QT_MOC_LITERAL(44, 24),  // "on_deleteAddress_clicked"
        QT_MOC_LITERAL(69, 21),  // "on_newAddress_clicked"
        QT_MOC_LITERAL(91, 22),  // "on_copyAddress_clicked"
        QT_MOC_LITERAL(114, 17),  // "onCopyLabelAction"
        QT_MOC_LITERAL(132, 12),  // "onEditAction"
        QT_MOC_LITERAL(145, 23),  // "on_exportButton_clicked"
        QT_MOC_LITERAL(169, 16),  // "selectionChanged"
        QT_MOC_LITERAL(186, 14),  // "contextualMenu"
        QT_MOC_LITERAL(201, 5),  // "point"
        QT_MOC_LITERAL(207, 16),  // "selectNewAddress"
        QT_MOC_LITERAL(224, 11),  // "QModelIndex"
        QT_MOC_LITERAL(236, 6),  // "parent"
        QT_MOC_LITERAL(243, 5)   // "begin"
    },
    "AddressBookPage",
    "sendCoins",
    "",
    "addr",
    "done",
    "retval",
    "on_deleteAddress_clicked",
    "on_newAddress_clicked",
    "on_copyAddress_clicked",
    "onCopyLabelAction",
    "onEditAction",
    "on_exportButton_clicked",
    "selectionChanged",
    "contextualMenu",
    "point",
    "selectNewAddress",
    "QModelIndex",
    "parent",
    "begin"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_AddressBookPage[] = {

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
       1,    1,   80,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       4,    1,   83,    2, 0x0a,    3 /* Public */,
       6,    0,   86,    2, 0x08,    5 /* Private */,
       7,    0,   87,    2, 0x08,    6 /* Private */,
       8,    0,   88,    2, 0x08,    7 /* Private */,
       9,    0,   89,    2, 0x08,    8 /* Private */,
      10,    0,   90,    2, 0x08,    9 /* Private */,
      11,    0,   91,    2, 0x08,   10 /* Private */,
      12,    0,   92,    2, 0x08,   11 /* Private */,
      13,    1,   93,    2, 0x08,   12 /* Private */,
      15,    3,   96,    2, 0x08,   14 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QPoint,   14,
    QMetaType::Void, 0x80000000 | 16, QMetaType::Int, QMetaType::Int,   17,   18,    2,

       0        // eod
};

Q_CONSTINIT const QMetaObject AddressBookPage::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_AddressBookPage.offsetsAndSizes,
    qt_meta_data_AddressBookPage,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_AddressBookPage_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<AddressBookPage, std::true_type>,
        // method 'sendCoins'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'done'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_deleteAddress_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_newAddress_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_copyAddress_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onCopyLabelAction'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onEditAction'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_exportButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'selectionChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'contextualMenu'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QPoint &, std::false_type>,
        // method 'selectNewAddress'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QModelIndex &, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>
    >,
    nullptr
} };

void AddressBookPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<AddressBookPage *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->sendCoins((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->done((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->on_deleteAddress_clicked(); break;
        case 3: _t->on_newAddress_clicked(); break;
        case 4: _t->on_copyAddress_clicked(); break;
        case 5: _t->onCopyLabelAction(); break;
        case 6: _t->onEditAction(); break;
        case 7: _t->on_exportButton_clicked(); break;
        case 8: _t->selectionChanged(); break;
        case 9: _t->contextualMenu((*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 10: _t->selectNewAddress((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (AddressBookPage::*)(QString );
            if (_t _q_method = &AddressBookPage::sendCoins; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *AddressBookPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AddressBookPage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AddressBookPage.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int AddressBookPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void AddressBookPage::sendCoins(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
