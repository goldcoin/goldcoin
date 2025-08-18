/****************************************************************************
** Meta object code from reading C++ file 'modaloverlay.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/qt/modaloverlay.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'modaloverlay.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_ModalOverlay_t {
    uint offsetsAndSizes[26];
    char stringdata0[13];
    char stringdata1[10];
    char stringdata2[1];
    char stringdata3[6];
    char stringdata4[10];
    char stringdata5[22];
    char stringdata6[19];
    char stringdata7[17];
    char stringdata8[9];
    char stringdata9[5];
    char stringdata10[14];
    char stringdata11[13];
    char stringdata12[15];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_ModalOverlay_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_ModalOverlay_t qt_meta_stringdata_ModalOverlay = {
    {
        QT_MOC_LITERAL(0, 12),  // "ModalOverlay"
        QT_MOC_LITERAL(13, 9),  // "tipUpdate"
        QT_MOC_LITERAL(23, 0),  // ""
        QT_MOC_LITERAL(24, 5),  // "count"
        QT_MOC_LITERAL(30, 9),  // "blockDate"
        QT_MOC_LITERAL(40, 21),  // "nVerificationProgress"
        QT_MOC_LITERAL(62, 18),  // "setKnownBestHeight"
        QT_MOC_LITERAL(81, 16),  // "toggleVisibility"
        QT_MOC_LITERAL(98, 8),  // "showHide"
        QT_MOC_LITERAL(107, 4),  // "hide"
        QT_MOC_LITERAL(112, 13),  // "userRequested"
        QT_MOC_LITERAL(126, 12),  // "closeClicked"
        QT_MOC_LITERAL(139, 14)   // "isLayerVisible"
    },
    "ModalOverlay",
    "tipUpdate",
    "",
    "count",
    "blockDate",
    "nVerificationProgress",
    "setKnownBestHeight",
    "toggleVisibility",
    "showHide",
    "hide",
    "userRequested",
    "closeClicked",
    "isLayerVisible"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_ModalOverlay[] = {

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
       1,    3,   62,    2, 0x0a,    1 /* Public */,
       6,    2,   69,    2, 0x0a,    5 /* Public */,
       7,    0,   74,    2, 0x0a,    8 /* Public */,
       8,    2,   75,    2, 0x0a,    9 /* Public */,
       8,    1,   80,    2, 0x2a,   12 /* Public | MethodCloned */,
       8,    0,   83,    2, 0x2a,   14 /* Public | MethodCloned */,
      11,    0,   84,    2, 0x0a,   15 /* Public */,
      12,    0,   85,    2, 0x0a,   16 /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QDateTime, QMetaType::Double,    3,    4,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::QDateTime,    3,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool, QMetaType::Bool,    9,   10,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool,

       0        // eod
};

Q_CONSTINIT const QMetaObject ModalOverlay::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_ModalOverlay.offsetsAndSizes,
    qt_meta_data_ModalOverlay,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_ModalOverlay_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ModalOverlay, std::true_type>,
        // method 'tipUpdate'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QDateTime &, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'setKnownBestHeight'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QDateTime &, std::false_type>,
        // method 'toggleVisibility'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showHide'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'showHide'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'showHide'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'closeClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'isLayerVisible'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>
    >,
    nullptr
} };

void ModalOverlay::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ModalOverlay *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->tipUpdate((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QDateTime>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[3]))); break;
        case 1: _t->setKnownBestHeight((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QDateTime>>(_a[2]))); break;
        case 2: _t->toggleVisibility(); break;
        case 3: _t->showHide((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        case 4: _t->showHide((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 5: _t->showHide(); break;
        case 6: _t->closeClicked(); break;
        case 7: { bool _r = _t->isLayerVisible();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

const QMetaObject *ModalOverlay::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ModalOverlay::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ModalOverlay.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ModalOverlay::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
