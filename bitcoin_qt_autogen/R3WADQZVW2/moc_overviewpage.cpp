/****************************************************************************
** Meta object code from reading C++ file 'overviewpage.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/qt/overviewpage.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'overviewpage.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_OverviewPage_t {
    uint offsetsAndSizes[42];
    char stringdata0[13];
    char stringdata1[19];
    char stringdata2[1];
    char stringdata3[12];
    char stringdata4[6];
    char stringdata5[24];
    char stringdata6[11];
    char stringdata7[8];
    char stringdata8[8];
    char stringdata9[19];
    char stringdata10[16];
    char stringdata11[17];
    char stringdata12[19];
    char stringdata13[21];
    char stringdata14[18];
    char stringdata15[25];
    char stringdata16[13];
    char stringdata17[9];
    char stringdata18[22];
    char stringdata19[14];
    char stringdata20[29];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_OverviewPage_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_OverviewPage_t qt_meta_stringdata_OverviewPage = {
    {
        QT_MOC_LITERAL(0, 12),  // "OverviewPage"
        QT_MOC_LITERAL(13, 18),  // "transactionClicked"
        QT_MOC_LITERAL(32, 0),  // ""
        QT_MOC_LITERAL(33, 11),  // "QModelIndex"
        QT_MOC_LITERAL(45, 5),  // "index"
        QT_MOC_LITERAL(51, 23),  // "outOfSyncWarningClicked"
        QT_MOC_LITERAL(75, 10),  // "setBalance"
        QT_MOC_LITERAL(86, 7),  // "CAmount"
        QT_MOC_LITERAL(94, 7),  // "balance"
        QT_MOC_LITERAL(102, 18),  // "unconfirmedBalance"
        QT_MOC_LITERAL(121, 15),  // "immatureBalance"
        QT_MOC_LITERAL(137, 16),  // "watchOnlyBalance"
        QT_MOC_LITERAL(154, 18),  // "watchUnconfBalance"
        QT_MOC_LITERAL(173, 20),  // "watchImmatureBalance"
        QT_MOC_LITERAL(194, 17),  // "updateDisplayUnit"
        QT_MOC_LITERAL(212, 24),  // "handleTransactionClicked"
        QT_MOC_LITERAL(237, 12),  // "updateAlerts"
        QT_MOC_LITERAL(250, 8),  // "warnings"
        QT_MOC_LITERAL(259, 21),  // "updateWatchOnlyLabels"
        QT_MOC_LITERAL(281, 13),  // "showWatchOnly"
        QT_MOC_LITERAL(295, 28)   // "handleOutOfSyncWarningClicks"
    },
    "OverviewPage",
    "transactionClicked",
    "",
    "QModelIndex",
    "index",
    "outOfSyncWarningClicked",
    "setBalance",
    "CAmount",
    "balance",
    "unconfirmedBalance",
    "immatureBalance",
    "watchOnlyBalance",
    "watchUnconfBalance",
    "watchImmatureBalance",
    "updateDisplayUnit",
    "handleTransactionClicked",
    "updateAlerts",
    "warnings",
    "updateWatchOnlyLabels",
    "showWatchOnly",
    "handleOutOfSyncWarningClicks"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_OverviewPage[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   62,    2, 0x06,    1 /* Public */,
       5,    0,   65,    2, 0x06,    3 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       6,    6,   66,    2, 0x0a,    4 /* Public */,
      14,    0,   79,    2, 0x08,   11 /* Private */,
      15,    1,   80,    2, 0x08,   12 /* Private */,
      16,    1,   83,    2, 0x08,   14 /* Private */,
      18,    1,   86,    2, 0x08,   16 /* Private */,
      20,    0,   89,    2, 0x08,   18 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 7, 0x80000000 | 7, 0x80000000 | 7, 0x80000000 | 7, 0x80000000 | 7, 0x80000000 | 7,    8,    9,   10,   11,   12,   13,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QString,   17,
    QMetaType::Void, QMetaType::Bool,   19,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject OverviewPage::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_OverviewPage.offsetsAndSizes,
    qt_meta_data_OverviewPage,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_OverviewPage_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<OverviewPage, std::true_type>,
        // method 'transactionClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QModelIndex &, std::false_type>,
        // method 'outOfSyncWarningClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'setBalance'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const CAmount &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const CAmount &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const CAmount &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const CAmount &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const CAmount &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const CAmount &, std::false_type>,
        // method 'updateDisplayUnit'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'handleTransactionClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QModelIndex &, std::false_type>,
        // method 'updateAlerts'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'updateWatchOnlyLabels'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'handleOutOfSyncWarningClicks'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void OverviewPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<OverviewPage *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->transactionClicked((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 1: _t->outOfSyncWarningClicked(); break;
        case 2: _t->setBalance((*reinterpret_cast< std::add_pointer_t<CAmount>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<CAmount>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<CAmount>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<CAmount>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<CAmount>>(_a[5])),(*reinterpret_cast< std::add_pointer_t<CAmount>>(_a[6]))); break;
        case 3: _t->updateDisplayUnit(); break;
        case 4: _t->handleTransactionClicked((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 5: _t->updateAlerts((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->updateWatchOnlyLabels((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 7: _t->handleOutOfSyncWarningClicks(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (OverviewPage::*)(const QModelIndex & );
            if (_t _q_method = &OverviewPage::transactionClicked; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (OverviewPage::*)();
            if (_t _q_method = &OverviewPage::outOfSyncWarningClicked; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject *OverviewPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OverviewPage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_OverviewPage.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int OverviewPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void OverviewPage::transactionClicked(const QModelIndex & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void OverviewPage::outOfSyncWarningClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
