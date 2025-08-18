/****************************************************************************
** Meta object code from reading C++ file 'bitcoingui.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/qt/bitcoingui.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bitcoingui.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_BitcoinGUI_t {
    uint offsetsAndSizes[114];
    char stringdata0[11];
    char stringdata1[12];
    char stringdata2[1];
    char stringdata3[4];
    char stringdata4[18];
    char stringdata5[6];
    char stringdata6[17];
    char stringdata7[14];
    char stringdata8[13];
    char stringdata9[10];
    char stringdata10[22];
    char stringdata11[8];
    char stringdata12[15];
    char stringdata13[8];
    char stringdata14[6];
    char stringdata15[6];
    char stringdata16[6];
    char stringdata17[4];
    char stringdata18[20];
    char stringdata19[7];
    char stringdata20[12];
    char stringdata21[10];
    char stringdata22[21];
    char stringdata23[19];
    char stringdata24[10];
    char stringdata25[20];
    char stringdata26[5];
    char stringdata27[5];
    char stringdata28[8];
    char stringdata29[7];
    char stringdata30[5];
    char stringdata31[8];
    char stringdata32[6];
    char stringdata33[17];
    char stringdata34[16];
    char stringdata35[21];
    char stringdata36[18];
    char stringdata37[5];
    char stringdata38[19];
    char stringdata39[21];
    char stringdata40[12];
    char stringdata41[15];
    char stringdata42[13];
    char stringdata43[16];
    char stringdata44[31];
    char stringdata45[23];
    char stringdata46[18];
    char stringdata47[34];
    char stringdata48[7];
    char stringdata49[22];
    char stringdata50[14];
    char stringdata51[13];
    char stringdata52[13];
    char stringdata53[10];
    char stringdata54[19];
    char stringdata55[20];
    char stringdata56[17];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_BitcoinGUI_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_BitcoinGUI_t qt_meta_stringdata_BitcoinGUI = {
    {
        QT_MOC_LITERAL(0, 10),  // "BitcoinGUI"
        QT_MOC_LITERAL(11, 11),  // "receivedURI"
        QT_MOC_LITERAL(23, 0),  // ""
        QT_MOC_LITERAL(24, 3),  // "uri"
        QT_MOC_LITERAL(28, 17),  // "setNumConnections"
        QT_MOC_LITERAL(46, 5),  // "count"
        QT_MOC_LITERAL(52, 16),  // "setNetworkActive"
        QT_MOC_LITERAL(69, 13),  // "networkActive"
        QT_MOC_LITERAL(83, 12),  // "setNumBlocks"
        QT_MOC_LITERAL(96, 9),  // "blockDate"
        QT_MOC_LITERAL(106, 21),  // "nVerificationProgress"
        QT_MOC_LITERAL(128, 7),  // "headers"
        QT_MOC_LITERAL(136, 14),  // "detectShutdown"
        QT_MOC_LITERAL(151, 7),  // "message"
        QT_MOC_LITERAL(159, 5),  // "title"
        QT_MOC_LITERAL(165, 5),  // "style"
        QT_MOC_LITERAL(171, 5),  // "bool*"
        QT_MOC_LITERAL(177, 3),  // "ret"
        QT_MOC_LITERAL(181, 19),  // "setEncryptionStatus"
        QT_MOC_LITERAL(201, 6),  // "status"
        QT_MOC_LITERAL(208, 11),  // "setHDStatus"
        QT_MOC_LITERAL(220, 9),  // "hdEnabled"
        QT_MOC_LITERAL(230, 20),  // "handlePaymentRequest"
        QT_MOC_LITERAL(251, 18),  // "SendCoinsRecipient"
        QT_MOC_LITERAL(270, 9),  // "recipient"
        QT_MOC_LITERAL(280, 19),  // "incomingTransaction"
        QT_MOC_LITERAL(300, 4),  // "date"
        QT_MOC_LITERAL(305, 4),  // "unit"
        QT_MOC_LITERAL(310, 7),  // "CAmount"
        QT_MOC_LITERAL(318, 6),  // "amount"
        QT_MOC_LITERAL(325, 4),  // "type"
        QT_MOC_LITERAL(330, 7),  // "address"
        QT_MOC_LITERAL(338, 5),  // "label"
        QT_MOC_LITERAL(344, 16),  // "gotoOverviewPage"
        QT_MOC_LITERAL(361, 15),  // "gotoHistoryPage"
        QT_MOC_LITERAL(377, 20),  // "gotoReceiveCoinsPage"
        QT_MOC_LITERAL(398, 17),  // "gotoSendCoinsPage"
        QT_MOC_LITERAL(416, 4),  // "addr"
        QT_MOC_LITERAL(421, 18),  // "gotoSignMessageTab"
        QT_MOC_LITERAL(440, 20),  // "gotoVerifyMessageTab"
        QT_MOC_LITERAL(461, 11),  // "openClicked"
        QT_MOC_LITERAL(473, 14),  // "optionsClicked"
        QT_MOC_LITERAL(488, 12),  // "aboutClicked"
        QT_MOC_LITERAL(501, 15),  // "showDebugWindow"
        QT_MOC_LITERAL(517, 30),  // "showDebugWindowActivateConsole"
        QT_MOC_LITERAL(548, 22),  // "showHelpMessageClicked"
        QT_MOC_LITERAL(571, 17),  // "trayIconActivated"
        QT_MOC_LITERAL(589, 33),  // "QSystemTrayIcon::ActivationRe..."
        QT_MOC_LITERAL(623, 6),  // "reason"
        QT_MOC_LITERAL(630, 21),  // "showNormalIfMinimized"
        QT_MOC_LITERAL(652, 13),  // "fToggleHidden"
        QT_MOC_LITERAL(666, 12),  // "toggleHidden"
        QT_MOC_LITERAL(679, 12),  // "showProgress"
        QT_MOC_LITERAL(692, 9),  // "nProgress"
        QT_MOC_LITERAL(702, 18),  // "setTrayIconVisible"
        QT_MOC_LITERAL(721, 19),  // "toggleNetworkActive"
        QT_MOC_LITERAL(741, 16)   // "showModalOverlay"
    },
    "BitcoinGUI",
    "receivedURI",
    "",
    "uri",
    "setNumConnections",
    "count",
    "setNetworkActive",
    "networkActive",
    "setNumBlocks",
    "blockDate",
    "nVerificationProgress",
    "headers",
    "detectShutdown",
    "message",
    "title",
    "style",
    "bool*",
    "ret",
    "setEncryptionStatus",
    "status",
    "setHDStatus",
    "hdEnabled",
    "handlePaymentRequest",
    "SendCoinsRecipient",
    "recipient",
    "incomingTransaction",
    "date",
    "unit",
    "CAmount",
    "amount",
    "type",
    "address",
    "label",
    "gotoOverviewPage",
    "gotoHistoryPage",
    "gotoReceiveCoinsPage",
    "gotoSendCoinsPage",
    "addr",
    "gotoSignMessageTab",
    "gotoVerifyMessageTab",
    "openClicked",
    "optionsClicked",
    "aboutClicked",
    "showDebugWindow",
    "showDebugWindowActivateConsole",
    "showHelpMessageClicked",
    "trayIconActivated",
    "QSystemTrayIcon::ActivationReason",
    "reason",
    "showNormalIfMinimized",
    "fToggleHidden",
    "toggleHidden",
    "showProgress",
    "nProgress",
    "setTrayIconVisible",
    "toggleNetworkActive",
    "showModalOverlay"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_BitcoinGUI[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      34,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,  218,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       4,    1,  221,    2, 0x0a,    3 /* Public */,
       6,    1,  224,    2, 0x0a,    5 /* Public */,
       8,    4,  227,    2, 0x0a,    7 /* Public */,
      12,    0,  236,    2, 0x0a,   12 /* Public */,
      13,    4,  237,    2, 0x0a,   13 /* Public */,
      13,    3,  246,    2, 0x2a,   18 /* Public | MethodCloned */,
      18,    1,  253,    2, 0x0a,   22 /* Public */,
      20,    1,  256,    2, 0x0a,   24 /* Public */,
      22,    1,  259,    2, 0x0a,   26 /* Public */,
      25,    6,  262,    2, 0x0a,   28 /* Public */,
      33,    0,  275,    2, 0x08,   35 /* Private */,
      34,    0,  276,    2, 0x08,   36 /* Private */,
      35,    0,  277,    2, 0x08,   37 /* Private */,
      36,    1,  278,    2, 0x08,   38 /* Private */,
      36,    0,  281,    2, 0x28,   40 /* Private | MethodCloned */,
      38,    1,  282,    2, 0x08,   41 /* Private */,
      38,    0,  285,    2, 0x28,   43 /* Private | MethodCloned */,
      39,    1,  286,    2, 0x08,   44 /* Private */,
      39,    0,  289,    2, 0x28,   46 /* Private | MethodCloned */,
      40,    0,  290,    2, 0x08,   47 /* Private */,
      41,    0,  291,    2, 0x08,   48 /* Private */,
      42,    0,  292,    2, 0x08,   49 /* Private */,
      43,    0,  293,    2, 0x08,   50 /* Private */,
      44,    0,  294,    2, 0x08,   51 /* Private */,
      45,    0,  295,    2, 0x08,   52 /* Private */,
      46,    1,  296,    2, 0x08,   53 /* Private */,
      49,    1,  299,    2, 0x08,   55 /* Private */,
      49,    0,  302,    2, 0x28,   57 /* Private | MethodCloned */,
      51,    0,  303,    2, 0x08,   58 /* Private */,
      52,    2,  304,    2, 0x08,   59 /* Private */,
      54,    1,  309,    2, 0x08,   62 /* Private */,
      55,    0,  312,    2, 0x08,   64 /* Private */,
      56,    0,  313,    2, 0x08,   65 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Bool,    7,
    QMetaType::Void, QMetaType::Int, QMetaType::QDateTime, QMetaType::Double, QMetaType::Bool,    5,    9,   10,   11,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::UInt, 0x80000000 | 16,   14,   13,   15,   17,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::UInt,   14,   13,   15,
    QMetaType::Void, QMetaType::Int,   19,
    QMetaType::Void, QMetaType::Int,   21,
    QMetaType::Bool, 0x80000000 | 23,   24,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, 0x80000000 | 28, QMetaType::QString, QMetaType::QString, QMetaType::QString,   26,   27,   29,   30,   31,   32,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   37,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   37,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   37,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 47,   48,
    QMetaType::Void, QMetaType::Bool,   50,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   14,   53,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject BitcoinGUI::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_BitcoinGUI.offsetsAndSizes,
    qt_meta_data_BitcoinGUI,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_BitcoinGUI_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<BitcoinGUI, std::true_type>,
        // method 'receivedURI'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'setNumConnections'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'setNetworkActive'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'setNumBlocks'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QDateTime &, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'detectShutdown'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'message'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<unsigned int, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool *, std::false_type>,
        // method 'message'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<unsigned int, std::false_type>,
        // method 'setEncryptionStatus'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'setHDStatus'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'handlePaymentRequest'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const SendCoinsRecipient &, std::false_type>,
        // method 'incomingTransaction'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<const CAmount &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
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
        // method 'openClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'optionsClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'aboutClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showDebugWindow'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showDebugWindowActivateConsole'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showHelpMessageClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'trayIconActivated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QSystemTrayIcon::ActivationReason, std::false_type>,
        // method 'showNormalIfMinimized'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'showNormalIfMinimized'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'toggleHidden'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showProgress'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'setTrayIconVisible'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'toggleNetworkActive'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showModalOverlay'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void BitcoinGUI::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<BitcoinGUI *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->receivedURI((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->setNumConnections((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->setNetworkActive((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 3: _t->setNumBlocks((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QDateTime>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[4]))); break;
        case 4: _t->detectShutdown(); break;
        case 5: _t->message((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<uint>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<bool*>>(_a[4]))); break;
        case 6: _t->message((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<uint>>(_a[3]))); break;
        case 7: _t->setEncryptionStatus((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 8: _t->setHDStatus((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 9: { bool _r = _t->handlePaymentRequest((*reinterpret_cast< std::add_pointer_t<SendCoinsRecipient>>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 10: _t->incomingTransaction((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<CAmount>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[5])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[6]))); break;
        case 11: _t->gotoOverviewPage(); break;
        case 12: _t->gotoHistoryPage(); break;
        case 13: _t->gotoReceiveCoinsPage(); break;
        case 14: _t->gotoSendCoinsPage((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 15: _t->gotoSendCoinsPage(); break;
        case 16: _t->gotoSignMessageTab((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 17: _t->gotoSignMessageTab(); break;
        case 18: _t->gotoVerifyMessageTab((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 19: _t->gotoVerifyMessageTab(); break;
        case 20: _t->openClicked(); break;
        case 21: _t->optionsClicked(); break;
        case 22: _t->aboutClicked(); break;
        case 23: _t->showDebugWindow(); break;
        case 24: _t->showDebugWindowActivateConsole(); break;
        case 25: _t->showHelpMessageClicked(); break;
        case 26: _t->trayIconActivated((*reinterpret_cast< std::add_pointer_t<QSystemTrayIcon::ActivationReason>>(_a[1]))); break;
        case 27: _t->showNormalIfMinimized((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 28: _t->showNormalIfMinimized(); break;
        case 29: _t->toggleHidden(); break;
        case 30: _t->showProgress((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 31: _t->setTrayIconVisible((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 32: _t->toggleNetworkActive(); break;
        case 33: _t->showModalOverlay(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (BitcoinGUI::*)(const QString & );
            if (_t _q_method = &BitcoinGUI::receivedURI; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *BitcoinGUI::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BitcoinGUI::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_BitcoinGUI.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int BitcoinGUI::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 34)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 34;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 34)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 34;
    }
    return _id;
}

// SIGNAL 0
void BitcoinGUI::receivedURI(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
namespace {
struct qt_meta_stringdata_UnitDisplayStatusBarControl_t {
    uint offsetsAndSizes[14];
    char stringdata0[28];
    char stringdata1[18];
    char stringdata2[1];
    char stringdata3[9];
    char stringdata4[16];
    char stringdata5[9];
    char stringdata6[7];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_UnitDisplayStatusBarControl_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_UnitDisplayStatusBarControl_t qt_meta_stringdata_UnitDisplayStatusBarControl = {
    {
        QT_MOC_LITERAL(0, 27),  // "UnitDisplayStatusBarControl"
        QT_MOC_LITERAL(28, 17),  // "updateDisplayUnit"
        QT_MOC_LITERAL(46, 0),  // ""
        QT_MOC_LITERAL(47, 8),  // "newUnits"
        QT_MOC_LITERAL(56, 15),  // "onMenuSelection"
        QT_MOC_LITERAL(72, 8),  // "QAction*"
        QT_MOC_LITERAL(81, 6)   // "action"
    },
    "UnitDisplayStatusBarControl",
    "updateDisplayUnit",
    "",
    "newUnits",
    "onMenuSelection",
    "QAction*",
    "action"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_UnitDisplayStatusBarControl[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   26,    2, 0x08,    1 /* Private */,
       4,    1,   29,    2, 0x08,    3 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, 0x80000000 | 5,    6,

       0        // eod
};

Q_CONSTINIT const QMetaObject UnitDisplayStatusBarControl::staticMetaObject = { {
    QMetaObject::SuperData::link<QLabel::staticMetaObject>(),
    qt_meta_stringdata_UnitDisplayStatusBarControl.offsetsAndSizes,
    qt_meta_data_UnitDisplayStatusBarControl,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_UnitDisplayStatusBarControl_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<UnitDisplayStatusBarControl, std::true_type>,
        // method 'updateDisplayUnit'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onMenuSelection'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QAction *, std::false_type>
    >,
    nullptr
} };

void UnitDisplayStatusBarControl::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<UnitDisplayStatusBarControl *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->updateDisplayUnit((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->onMenuSelection((*reinterpret_cast< std::add_pointer_t<QAction*>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QAction* >(); break;
            }
            break;
        }
    }
}

const QMetaObject *UnitDisplayStatusBarControl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UnitDisplayStatusBarControl::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_UnitDisplayStatusBarControl.stringdata0))
        return static_cast<void*>(this);
    return QLabel::qt_metacast(_clname);
}

int UnitDisplayStatusBarControl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
