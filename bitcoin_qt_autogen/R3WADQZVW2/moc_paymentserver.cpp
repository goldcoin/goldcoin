/****************************************************************************
** Meta object code from reading C++ file 'paymentserver.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/qt/paymentserver.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'paymentserver.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_PaymentServer_t {
    uint offsetsAndSizes[46];
    char stringdata0[14];
    char stringdata1[23];
    char stringdata2[1];
    char stringdata3[19];
    char stringdata4[19];
    char stringdata5[14];
    char stringdata6[8];
    char stringdata7[6];
    char stringdata8[6];
    char stringdata9[8];
    char stringdata10[16];
    char stringdata11[9];
    char stringdata12[7];
    char stringdata13[10];
    char stringdata14[12];
    char stringdata15[16];
    char stringdata16[2];
    char stringdata17[20];
    char stringdata18[19];
    char stringdata19[15];
    char stringdata20[16];
    char stringdata21[17];
    char stringdata22[17];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_PaymentServer_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_PaymentServer_t qt_meta_stringdata_PaymentServer = {
    {
        QT_MOC_LITERAL(0, 13),  // "PaymentServer"
        QT_MOC_LITERAL(14, 22),  // "receivedPaymentRequest"
        QT_MOC_LITERAL(37, 0),  // ""
        QT_MOC_LITERAL(38, 18),  // "SendCoinsRecipient"
        QT_MOC_LITERAL(57, 18),  // "receivedPaymentACK"
        QT_MOC_LITERAL(76, 13),  // "paymentACKMsg"
        QT_MOC_LITERAL(90, 7),  // "message"
        QT_MOC_LITERAL(98, 5),  // "title"
        QT_MOC_LITERAL(104, 5),  // "style"
        QT_MOC_LITERAL(110, 7),  // "uiReady"
        QT_MOC_LITERAL(118, 15),  // "fetchPaymentACK"
        QT_MOC_LITERAL(134, 8),  // "CWallet*"
        QT_MOC_LITERAL(143, 6),  // "wallet"
        QT_MOC_LITERAL(150, 9),  // "recipient"
        QT_MOC_LITERAL(160, 11),  // "transaction"
        QT_MOC_LITERAL(172, 15),  // "handleURIOrFile"
        QT_MOC_LITERAL(188, 1),  // "s"
        QT_MOC_LITERAL(190, 19),  // "handleURIConnection"
        QT_MOC_LITERAL(210, 18),  // "netRequestFinished"
        QT_MOC_LITERAL(229, 14),  // "QNetworkReply*"
        QT_MOC_LITERAL(244, 15),  // "reportSslErrors"
        QT_MOC_LITERAL(260, 16),  // "QList<QSslError>"
        QT_MOC_LITERAL(277, 16)   // "handlePaymentACK"
    },
    "PaymentServer",
    "receivedPaymentRequest",
    "",
    "SendCoinsRecipient",
    "receivedPaymentACK",
    "paymentACKMsg",
    "message",
    "title",
    "style",
    "uiReady",
    "fetchPaymentACK",
    "CWallet*",
    "wallet",
    "recipient",
    "transaction",
    "handleURIOrFile",
    "s",
    "handleURIConnection",
    "netRequestFinished",
    "QNetworkReply*",
    "reportSslErrors",
    "QList<QSslError>",
    "handlePaymentACK"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_PaymentServer[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   74,    2, 0x06,    1 /* Public */,
       4,    1,   77,    2, 0x06,    3 /* Public */,
       6,    3,   80,    2, 0x06,    5 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       9,    0,   87,    2, 0x0a,    9 /* Public */,
      10,    3,   88,    2, 0x0a,   10 /* Public */,
      15,    1,   95,    2, 0x0a,   14 /* Public */,
      17,    0,   98,    2, 0x08,   16 /* Private */,
      18,    1,   99,    2, 0x08,   17 /* Private */,
      20,    2,  102,    2, 0x08,   19 /* Private */,
      22,    1,  107,    2, 0x08,   22 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::UInt,    7,    6,    8,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 11, 0x80000000 | 3, QMetaType::QByteArray,   12,   13,   14,
    QMetaType::Void, QMetaType::QString,   16,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 19,    2,
    QMetaType::Void, 0x80000000 | 19, 0x80000000 | 21,    2,    2,
    QMetaType::Void, QMetaType::QString,    5,

       0        // eod
};

Q_CONSTINIT const QMetaObject PaymentServer::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_PaymentServer.offsetsAndSizes,
    qt_meta_data_PaymentServer,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_PaymentServer_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<PaymentServer, std::true_type>,
        // method 'receivedPaymentRequest'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<SendCoinsRecipient, std::false_type>,
        // method 'receivedPaymentACK'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'message'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<unsigned int, std::false_type>,
        // method 'uiReady'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'fetchPaymentACK'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<CWallet *, std::false_type>,
        QtPrivate::TypeAndForceComplete<SendCoinsRecipient, std::false_type>,
        QtPrivate::TypeAndForceComplete<QByteArray, std::false_type>,
        // method 'handleURIOrFile'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'handleURIConnection'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'netRequestFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QNetworkReply *, std::false_type>,
        // method 'reportSslErrors'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QNetworkReply *, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QList<QSslError> &, std::false_type>,
        // method 'handlePaymentACK'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
    >,
    nullptr
} };

void PaymentServer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<PaymentServer *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->receivedPaymentRequest((*reinterpret_cast< std::add_pointer_t<SendCoinsRecipient>>(_a[1]))); break;
        case 1: _t->receivedPaymentACK((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->message((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<uint>>(_a[3]))); break;
        case 3: _t->uiReady(); break;
        case 4: _t->fetchPaymentACK((*reinterpret_cast< std::add_pointer_t<CWallet*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<SendCoinsRecipient>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[3]))); break;
        case 5: _t->handleURIOrFile((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->handleURIConnection(); break;
        case 7: _t->netRequestFinished((*reinterpret_cast< std::add_pointer_t<QNetworkReply*>>(_a[1]))); break;
        case 8: _t->reportSslErrors((*reinterpret_cast< std::add_pointer_t<QNetworkReply*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QList<QSslError>>>(_a[2]))); break;
        case 9: _t->handlePaymentACK((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 1:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<QSslError> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (PaymentServer::*)(SendCoinsRecipient );
            if (_t _q_method = &PaymentServer::receivedPaymentRequest; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (PaymentServer::*)(const QString & );
            if (_t _q_method = &PaymentServer::receivedPaymentACK; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (PaymentServer::*)(const QString & , const QString & , unsigned int );
            if (_t _q_method = &PaymentServer::message; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject *PaymentServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PaymentServer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PaymentServer.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int PaymentServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void PaymentServer::receivedPaymentRequest(SendCoinsRecipient _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void PaymentServer::receivedPaymentACK(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void PaymentServer::message(const QString & _t1, const QString & _t2, unsigned int _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
