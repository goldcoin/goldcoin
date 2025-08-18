/****************************************************************************
** Meta object code from reading C++ file 'recentrequeststablemodel.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/qt/recentrequeststablemodel.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'recentrequeststablemodel.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_RecentRequestsTableModel_t {
    uint offsetsAndSizes[14];
    char stringdata0[25];
    char stringdata1[5];
    char stringdata2[1];
    char stringdata3[7];
    char stringdata4[14];
    char stringdata5[6];
    char stringdata6[18];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_RecentRequestsTableModel_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_RecentRequestsTableModel_t qt_meta_stringdata_RecentRequestsTableModel = {
    {
        QT_MOC_LITERAL(0, 24),  // "RecentRequestsTableModel"
        QT_MOC_LITERAL(25, 4),  // "sort"
        QT_MOC_LITERAL(30, 0),  // ""
        QT_MOC_LITERAL(31, 6),  // "column"
        QT_MOC_LITERAL(38, 13),  // "Qt::SortOrder"
        QT_MOC_LITERAL(52, 5),  // "order"
        QT_MOC_LITERAL(58, 17)   // "updateDisplayUnit"
    },
    "RecentRequestsTableModel",
    "sort",
    "",
    "column",
    "Qt::SortOrder",
    "order",
    "updateDisplayUnit"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_RecentRequestsTableModel[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,   32,    2, 0x0a,    1 /* Public */,
       1,    1,   37,    2, 0x2a,    4 /* Public | MethodCloned */,
       6,    0,   40,    2, 0x0a,    6 /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, 0x80000000 | 4,    3,    5,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject RecentRequestsTableModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QAbstractTableModel::staticMetaObject>(),
    qt_meta_stringdata_RecentRequestsTableModel.offsetsAndSizes,
    qt_meta_data_RecentRequestsTableModel,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_RecentRequestsTableModel_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<RecentRequestsTableModel, std::true_type>,
        // method 'sort'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<Qt::SortOrder, std::false_type>,
        // method 'sort'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'updateDisplayUnit'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void RecentRequestsTableModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RecentRequestsTableModel *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->sort((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<Qt::SortOrder>>(_a[2]))); break;
        case 1: _t->sort((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->updateDisplayUnit(); break;
        default: ;
        }
    }
}

const QMetaObject *RecentRequestsTableModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RecentRequestsTableModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RecentRequestsTableModel.stringdata0))
        return static_cast<void*>(this);
    return QAbstractTableModel::qt_metacast(_clname);
}

int RecentRequestsTableModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractTableModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
