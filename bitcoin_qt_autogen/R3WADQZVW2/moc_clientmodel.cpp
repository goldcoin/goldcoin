/****************************************************************************
** Meta object code from reading C++ file 'clientmodel.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/qt/clientmodel.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'clientmodel.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_ClientModel_t {
    uint offsetsAndSizes[58];
    char stringdata0[12];
    char stringdata1[22];
    char stringdata2[1];
    char stringdata3[6];
    char stringdata4[17];
    char stringdata5[10];
    char stringdata6[22];
    char stringdata7[7];
    char stringdata8[19];
    char stringdata9[7];
    char stringdata10[19];
    char stringdata11[21];
    char stringdata12[14];
    char stringdata13[14];
    char stringdata14[9];
    char stringdata15[13];
    char stringdata16[13];
    char stringdata17[14];
    char stringdata18[8];
    char stringdata19[6];
    char stringdata20[6];
    char stringdata21[13];
    char stringdata22[10];
    char stringdata23[12];
    char stringdata24[21];
    char stringdata25[15];
    char stringdata26[20];
    char stringdata27[12];
    char stringdata28[14];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_ClientModel_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_ClientModel_t qt_meta_stringdata_ClientModel = {
    {
        QT_MOC_LITERAL(0, 11),  // "ClientModel"
        QT_MOC_LITERAL(12, 21),  // "numConnectionsChanged"
        QT_MOC_LITERAL(34, 0),  // ""
        QT_MOC_LITERAL(35, 5),  // "count"
        QT_MOC_LITERAL(41, 16),  // "numBlocksChanged"
        QT_MOC_LITERAL(58, 9),  // "blockDate"
        QT_MOC_LITERAL(68, 21),  // "nVerificationProgress"
        QT_MOC_LITERAL(90, 6),  // "header"
        QT_MOC_LITERAL(97, 18),  // "mempoolSizeChanged"
        QT_MOC_LITERAL(116, 6),  // "size_t"
        QT_MOC_LITERAL(123, 18),  // "mempoolSizeInBytes"
        QT_MOC_LITERAL(142, 20),  // "networkActiveChanged"
        QT_MOC_LITERAL(163, 13),  // "networkActive"
        QT_MOC_LITERAL(177, 13),  // "alertsChanged"
        QT_MOC_LITERAL(191, 8),  // "warnings"
        QT_MOC_LITERAL(200, 12),  // "bytesChanged"
        QT_MOC_LITERAL(213, 12),  // "totalBytesIn"
        QT_MOC_LITERAL(226, 13),  // "totalBytesOut"
        QT_MOC_LITERAL(240, 7),  // "message"
        QT_MOC_LITERAL(248, 5),  // "title"
        QT_MOC_LITERAL(254, 5),  // "style"
        QT_MOC_LITERAL(260, 12),  // "showProgress"
        QT_MOC_LITERAL(273, 9),  // "nProgress"
        QT_MOC_LITERAL(283, 11),  // "updateTimer"
        QT_MOC_LITERAL(295, 20),  // "updateNumConnections"
        QT_MOC_LITERAL(316, 14),  // "numConnections"
        QT_MOC_LITERAL(331, 19),  // "updateNetworkActive"
        QT_MOC_LITERAL(351, 11),  // "updateAlert"
        QT_MOC_LITERAL(363, 13)   // "updateBanlist"
    },
    "ClientModel",
    "numConnectionsChanged",
    "",
    "count",
    "numBlocksChanged",
    "blockDate",
    "nVerificationProgress",
    "header",
    "mempoolSizeChanged",
    "size_t",
    "mempoolSizeInBytes",
    "networkActiveChanged",
    "networkActive",
    "alertsChanged",
    "warnings",
    "bytesChanged",
    "totalBytesIn",
    "totalBytesOut",
    "message",
    "title",
    "style",
    "showProgress",
    "nProgress",
    "updateTimer",
    "updateNumConnections",
    "numConnections",
    "updateNetworkActive",
    "updateAlert",
    "updateBanlist"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_ClientModel[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   92,    2, 0x06,    1 /* Public */,
       4,    4,   95,    2, 0x06,    3 /* Public */,
       8,    2,  104,    2, 0x06,    8 /* Public */,
      11,    1,  109,    2, 0x06,   11 /* Public */,
      13,    1,  112,    2, 0x06,   13 /* Public */,
      15,    2,  115,    2, 0x06,   15 /* Public */,
      18,    3,  120,    2, 0x06,   18 /* Public */,
      21,    2,  127,    2, 0x06,   22 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      23,    0,  132,    2, 0x0a,   25 /* Public */,
      24,    1,  133,    2, 0x0a,   26 /* Public */,
      26,    1,  136,    2, 0x0a,   28 /* Public */,
      27,    0,  139,    2, 0x0a,   30 /* Public */,
      28,    0,  140,    2, 0x0a,   31 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int, QMetaType::QDateTime, QMetaType::Double, QMetaType::Bool,    3,    5,    6,    7,
    QMetaType::Void, QMetaType::Long, 0x80000000 | 9,    3,   10,
    QMetaType::Void, QMetaType::Bool,   12,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::ULongLong,   16,   17,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::UInt,   19,   18,   20,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   19,   22,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   25,
    QMetaType::Void, QMetaType::Bool,   12,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject ClientModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ClientModel.offsetsAndSizes,
    qt_meta_data_ClientModel,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_ClientModel_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ClientModel, std::true_type>,
        // method 'numConnectionsChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'numBlocksChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QDateTime &, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'mempoolSizeChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<long, std::false_type>,
        QtPrivate::TypeAndForceComplete<size_t, std::false_type>,
        // method 'networkActiveChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'alertsChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'bytesChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<quint64, std::false_type>,
        QtPrivate::TypeAndForceComplete<quint64, std::false_type>,
        // method 'message'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<unsigned int, std::false_type>,
        // method 'showProgress'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'updateTimer'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateNumConnections'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'updateNetworkActive'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'updateAlert'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateBanlist'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void ClientModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ClientModel *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->numConnectionsChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->numBlocksChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QDateTime>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[4]))); break;
        case 2: _t->mempoolSizeChanged((*reinterpret_cast< std::add_pointer_t<long>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<size_t>>(_a[2]))); break;
        case 3: _t->networkActiveChanged((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 4: _t->alertsChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 5: _t->bytesChanged((*reinterpret_cast< std::add_pointer_t<quint64>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<quint64>>(_a[2]))); break;
        case 6: _t->message((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<uint>>(_a[3]))); break;
        case 7: _t->showProgress((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 8: _t->updateTimer(); break;
        case 9: _t->updateNumConnections((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 10: _t->updateNetworkActive((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 11: _t->updateAlert(); break;
        case 12: _t->updateBanlist(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ClientModel::*)(int );
            if (_t _q_method = &ClientModel::numConnectionsChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ClientModel::*)(int , const QDateTime & , double , bool );
            if (_t _q_method = &ClientModel::numBlocksChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ClientModel::*)(long , size_t );
            if (_t _q_method = &ClientModel::mempoolSizeChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (ClientModel::*)(bool );
            if (_t _q_method = &ClientModel::networkActiveChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (ClientModel::*)(const QString & );
            if (_t _q_method = &ClientModel::alertsChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (ClientModel::*)(quint64 , quint64 );
            if (_t _q_method = &ClientModel::bytesChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (ClientModel::*)(const QString & , const QString & , unsigned int );
            if (_t _q_method = &ClientModel::message; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (ClientModel::*)(const QString & , int );
            if (_t _q_method = &ClientModel::showProgress; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 7;
                return;
            }
        }
    }
}

const QMetaObject *ClientModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ClientModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ClientModel.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ClientModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void ClientModel::numConnectionsChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ClientModel::numBlocksChanged(int _t1, const QDateTime & _t2, double _t3, bool _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ClientModel::mempoolSizeChanged(long _t1, size_t _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ClientModel::networkActiveChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void ClientModel::alertsChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void ClientModel::bytesChanged(quint64 _t1, quint64 _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void ClientModel::message(const QString & _t1, const QString & _t2, unsigned int _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void ClientModel::showProgress(const QString & _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
