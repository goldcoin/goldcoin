/****************************************************************************
** Meta object code from reading C++ file 'walletframe.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/qt/walletframe.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'walletframe.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_WalletFrame_t {
    uint offsetsAndSizes[36];
    char stringdata0[12];
    char stringdata1[25];
    char stringdata2[1];
    char stringdata3[17];
    char stringdata4[16];
    char stringdata5[21];
    char stringdata6[18];
    char stringdata7[5];
    char stringdata8[19];
    char stringdata9[21];
    char stringdata10[14];
    char stringdata11[7];
    char stringdata12[13];
    char stringdata13[17];
    char stringdata14[13];
    char stringdata15[21];
    char stringdata16[23];
    char stringdata17[24];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_WalletFrame_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_WalletFrame_t qt_meta_stringdata_WalletFrame = {
    {
        QT_MOC_LITERAL(0, 11),  // "WalletFrame"
        QT_MOC_LITERAL(12, 24),  // "requestedSyncWarningInfo"
        QT_MOC_LITERAL(37, 0),  // ""
        QT_MOC_LITERAL(38, 16),  // "gotoOverviewPage"
        QT_MOC_LITERAL(55, 15),  // "gotoHistoryPage"
        QT_MOC_LITERAL(71, 20),  // "gotoReceiveCoinsPage"
        QT_MOC_LITERAL(92, 17),  // "gotoSendCoinsPage"
        QT_MOC_LITERAL(110, 4),  // "addr"
        QT_MOC_LITERAL(115, 18),  // "gotoSignMessageTab"
        QT_MOC_LITERAL(134, 20),  // "gotoVerifyMessageTab"
        QT_MOC_LITERAL(155, 13),  // "encryptWallet"
        QT_MOC_LITERAL(169, 6),  // "status"
        QT_MOC_LITERAL(176, 12),  // "backupWallet"
        QT_MOC_LITERAL(189, 16),  // "changePassphrase"
        QT_MOC_LITERAL(206, 12),  // "unlockWallet"
        QT_MOC_LITERAL(219, 20),  // "usedSendingAddresses"
        QT_MOC_LITERAL(240, 22),  // "usedReceivingAddresses"
        QT_MOC_LITERAL(263, 23)   // "outOfSyncWarningClicked"
    },
    "WalletFrame",
    "requestedSyncWarningInfo",
    "",
    "gotoOverviewPage",
    "gotoHistoryPage",
    "gotoReceiveCoinsPage",
    "gotoSendCoinsPage",
    "addr",
    "gotoSignMessageTab",
    "gotoVerifyMessageTab",
    "encryptWallet",
    "status",
    "backupWallet",
    "changePassphrase",
    "unlockWallet",
    "usedSendingAddresses",
    "usedReceivingAddresses",
    "outOfSyncWarningClicked"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_WalletFrame[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  116,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       3,    0,  117,    2, 0x0a,    2 /* Public */,
       4,    0,  118,    2, 0x0a,    3 /* Public */,
       5,    0,  119,    2, 0x0a,    4 /* Public */,
       6,    1,  120,    2, 0x0a,    5 /* Public */,
       6,    0,  123,    2, 0x2a,    7 /* Public | MethodCloned */,
       8,    1,  124,    2, 0x0a,    8 /* Public */,
       8,    0,  127,    2, 0x2a,   10 /* Public | MethodCloned */,
       9,    1,  128,    2, 0x0a,   11 /* Public */,
       9,    0,  131,    2, 0x2a,   13 /* Public | MethodCloned */,
      10,    1,  132,    2, 0x0a,   14 /* Public */,
      12,    0,  135,    2, 0x0a,   16 /* Public */,
      13,    0,  136,    2, 0x0a,   17 /* Public */,
      14,    0,  137,    2, 0x0a,   18 /* Public */,
      15,    0,  138,    2, 0x0a,   19 /* Public */,
      16,    0,  139,    2, 0x0a,   20 /* Public */,
      17,    0,  140,    2, 0x0a,   21 /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject WalletFrame::staticMetaObject = { {
    QMetaObject::SuperData::link<QFrame::staticMetaObject>(),
    qt_meta_stringdata_WalletFrame.offsetsAndSizes,
    qt_meta_data_WalletFrame,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_WalletFrame_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<WalletFrame, std::true_type>,
        // method 'requestedSyncWarningInfo'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'gotoOverviewPage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'gotoHistoryPage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'gotoReceiveCoinsPage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'gotoSendCoinsPage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'gotoSendCoinsPage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'gotoSignMessageTab'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'gotoSignMessageTab'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'gotoVerifyMessageTab'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'gotoVerifyMessageTab'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'encryptWallet'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'backupWallet'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'changePassphrase'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'unlockWallet'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'usedSendingAddresses'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'usedReceivingAddresses'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'outOfSyncWarningClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void WalletFrame::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<WalletFrame *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->requestedSyncWarningInfo(); break;
        case 1: _t->gotoOverviewPage(); break;
        case 2: _t->gotoHistoryPage(); break;
        case 3: _t->gotoReceiveCoinsPage(); break;
        case 4: _t->gotoSendCoinsPage((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 5: _t->gotoSendCoinsPage(); break;
        case 6: _t->gotoSignMessageTab((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 7: _t->gotoSignMessageTab(); break;
        case 8: _t->gotoVerifyMessageTab((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 9: _t->gotoVerifyMessageTab(); break;
        case 10: _t->encryptWallet((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 11: _t->backupWallet(); break;
        case 12: _t->changePassphrase(); break;
        case 13: _t->unlockWallet(); break;
        case 14: _t->usedSendingAddresses(); break;
        case 15: _t->usedReceivingAddresses(); break;
        case 16: _t->outOfSyncWarningClicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (WalletFrame::*)();
            if (_t _q_method = &WalletFrame::requestedSyncWarningInfo; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *WalletFrame::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WalletFrame::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_WalletFrame.stringdata0))
        return static_cast<void*>(this);
    return QFrame::qt_metacast(_clname);
}

int WalletFrame::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void WalletFrame::requestedSyncWarningInfo()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
