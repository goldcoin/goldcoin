/****************************************************************************
** Meta object code from reading C++ file 'transactiontablemodel.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/qt/transactiontablemodel.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'transactiontablemodel.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_TransactionTableModel_t {
    uint offsetsAndSizes[22];
    char stringdata0[22];
    char stringdata1[18];
    char stringdata2[1];
    char stringdata3[5];
    char stringdata4[7];
    char stringdata5[16];
    char stringdata6[20];
    char stringdata7[18];
    char stringdata8[24];
    char stringdata9[32];
    char stringdata10[6];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_TransactionTableModel_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_TransactionTableModel_t qt_meta_stringdata_TransactionTableModel = {
    {
        QT_MOC_LITERAL(0, 21),  // "TransactionTableModel"
        QT_MOC_LITERAL(22, 17),  // "updateTransaction"
        QT_MOC_LITERAL(40, 0),  // ""
        QT_MOC_LITERAL(41, 4),  // "hash"
        QT_MOC_LITERAL(46, 6),  // "status"
        QT_MOC_LITERAL(53, 15),  // "showTransaction"
        QT_MOC_LITERAL(69, 19),  // "updateConfirmations"
        QT_MOC_LITERAL(89, 17),  // "updateDisplayUnit"
        QT_MOC_LITERAL(107, 23),  // "updateAmountColumnTitle"
        QT_MOC_LITERAL(131, 31),  // "setProcessingQueuedTransactions"
        QT_MOC_LITERAL(163, 5)   // "value"
    },
    "TransactionTableModel",
    "updateTransaction",
    "",
    "hash",
    "status",
    "showTransaction",
    "updateConfirmations",
    "updateDisplayUnit",
    "updateAmountColumnTitle",
    "setProcessingQueuedTransactions",
    "value"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_TransactionTableModel[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    3,   44,    2, 0x0a,    1 /* Public */,
       6,    0,   51,    2, 0x0a,    5 /* Public */,
       7,    0,   52,    2, 0x0a,    6 /* Public */,
       8,    0,   53,    2, 0x0a,    7 /* Public */,
       9,    1,   54,    2, 0x0a,    8 /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Bool,    3,    4,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   10,

       0        // eod
};

Q_CONSTINIT const QMetaObject TransactionTableModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QAbstractTableModel::staticMetaObject>(),
    qt_meta_stringdata_TransactionTableModel.offsetsAndSizes,
    qt_meta_data_TransactionTableModel,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_TransactionTableModel_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<TransactionTableModel, std::true_type>,
        // method 'updateTransaction'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'updateConfirmations'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateDisplayUnit'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateAmountColumnTitle'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'setProcessingQueuedTransactions'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>
    >,
    nullptr
} };

void TransactionTableModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TransactionTableModel *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->updateTransaction((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[3]))); break;
        case 1: _t->updateConfirmations(); break;
        case 2: _t->updateDisplayUnit(); break;
        case 3: _t->updateAmountColumnTitle(); break;
        case 4: _t->setProcessingQueuedTransactions((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *TransactionTableModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TransactionTableModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TransactionTableModel.stringdata0))
        return static_cast<void*>(this);
    return QAbstractTableModel::qt_metacast(_clname);
}

int TransactionTableModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractTableModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
