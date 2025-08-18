/****************************************************************************
** Meta object code from reading C++ file 'rpcconsole.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/qt/rpcconsole.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'rpcconsole.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_RPCConsole_t {
    uint offsetsAndSizes[126];
    char stringdata0[11];
    char stringdata1[13];
    char stringdata2[1];
    char stringdata3[11];
    char stringdata4[8];
    char stringdata5[26];
    char stringdata6[28];
    char stringdata7[6];
    char stringdata8[34];
    char stringdata9[30];
    char stringdata10[6];
    char stringdata11[19];
    char stringdata12[13];
    char stringdata13[14];
    char stringdata14[12];
    char stringdata15[14];
    char stringdata16[6];
    char stringdata17[10];
    char stringdata18[12];
    char stringdata19[10];
    char stringdata20[12];
    char stringdata21[26];
    char stringdata22[6];
    char stringdata23[24];
    char stringdata24[29];
    char stringdata25[18];
    char stringdata26[6];
    char stringdata27[13];
    char stringdata28[11];
    char stringdata29[12];
    char stringdata30[12];
    char stringdata31[8];
    char stringdata32[8];
    char stringdata33[9];
    char stringdata34[5];
    char stringdata35[18];
    char stringdata36[6];
    char stringdata37[17];
    char stringdata38[14];
    char stringdata39[13];
    char stringdata40[10];
    char stringdata41[22];
    char stringdata42[8];
    char stringdata43[15];
    char stringdata44[12];
    char stringdata45[7];
    char stringdata46[9];
    char stringdata47[14];
    char stringdata48[7];
    char stringdata49[12];
    char stringdata50[13];
    char stringdata51[15];
    char stringdata52[9];
    char stringdata53[11];
    char stringdata54[24];
    char stringdata55[18];
    char stringdata56[23];
    char stringdata57[16];
    char stringdata58[8];
    char stringdata59[18];
    char stringdata60[12];
    char stringdata61[9];
    char stringdata62[8];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_RPCConsole_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_RPCConsole_t qt_meta_stringdata_RPCConsole = {
    {
        QT_MOC_LITERAL(0, 10),  // "RPCConsole"
        QT_MOC_LITERAL(11, 12),  // "stopExecutor"
        QT_MOC_LITERAL(24, 0),  // ""
        QT_MOC_LITERAL(25, 10),  // "cmdRequest"
        QT_MOC_LITERAL(36, 7),  // "command"
        QT_MOC_LITERAL(44, 25),  // "on_lineEdit_returnPressed"
        QT_MOC_LITERAL(70, 27),  // "on_tabWidget_currentChanged"
        QT_MOC_LITERAL(98, 5),  // "index"
        QT_MOC_LITERAL(104, 33),  // "on_openDebugLogfileButton_cli..."
        QT_MOC_LITERAL(138, 29),  // "on_sldGraphRange_valueChanged"
        QT_MOC_LITERAL(168, 5),  // "value"
        QT_MOC_LITERAL(174, 18),  // "updateTrafficStats"
        QT_MOC_LITERAL(193, 12),  // "totalBytesIn"
        QT_MOC_LITERAL(206, 13),  // "totalBytesOut"
        QT_MOC_LITERAL(220, 11),  // "resizeEvent"
        QT_MOC_LITERAL(232, 13),  // "QResizeEvent*"
        QT_MOC_LITERAL(246, 5),  // "event"
        QT_MOC_LITERAL(252, 9),  // "showEvent"
        QT_MOC_LITERAL(262, 11),  // "QShowEvent*"
        QT_MOC_LITERAL(274, 9),  // "hideEvent"
        QT_MOC_LITERAL(284, 11),  // "QHideEvent*"
        QT_MOC_LITERAL(296, 25),  // "showPeersTableContextMenu"
        QT_MOC_LITERAL(322, 5),  // "point"
        QT_MOC_LITERAL(328, 23),  // "showBanTableContextMenu"
        QT_MOC_LITERAL(352, 28),  // "showOrHideBanTableIfRequired"
        QT_MOC_LITERAL(381, 17),  // "clearSelectedNode"
        QT_MOC_LITERAL(399, 5),  // "clear"
        QT_MOC_LITERAL(405, 12),  // "clearHistory"
        QT_MOC_LITERAL(418, 10),  // "fontBigger"
        QT_MOC_LITERAL(429, 11),  // "fontSmaller"
        QT_MOC_LITERAL(441, 11),  // "setFontSize"
        QT_MOC_LITERAL(453, 7),  // "newSize"
        QT_MOC_LITERAL(461, 7),  // "message"
        QT_MOC_LITERAL(469, 8),  // "category"
        QT_MOC_LITERAL(478, 4),  // "html"
        QT_MOC_LITERAL(483, 17),  // "setNumConnections"
        QT_MOC_LITERAL(501, 5),  // "count"
        QT_MOC_LITERAL(507, 16),  // "setNetworkActive"
        QT_MOC_LITERAL(524, 13),  // "networkActive"
        QT_MOC_LITERAL(538, 12),  // "setNumBlocks"
        QT_MOC_LITERAL(551, 9),  // "blockDate"
        QT_MOC_LITERAL(561, 21),  // "nVerificationProgress"
        QT_MOC_LITERAL(583, 7),  // "headers"
        QT_MOC_LITERAL(591, 14),  // "setMempoolSize"
        QT_MOC_LITERAL(606, 11),  // "numberOfTxs"
        QT_MOC_LITERAL(618, 6),  // "size_t"
        QT_MOC_LITERAL(625, 8),  // "dynUsage"
        QT_MOC_LITERAL(634, 13),  // "browseHistory"
        QT_MOC_LITERAL(648, 6),  // "offset"
        QT_MOC_LITERAL(655, 11),  // "scrollToEnd"
        QT_MOC_LITERAL(667, 12),  // "peerSelected"
        QT_MOC_LITERAL(680, 14),  // "QItemSelection"
        QT_MOC_LITERAL(695, 8),  // "selected"
        QT_MOC_LITERAL(704, 10),  // "deselected"
        QT_MOC_LITERAL(715, 23),  // "peerLayoutAboutToChange"
        QT_MOC_LITERAL(739, 17),  // "peerLayoutChanged"
        QT_MOC_LITERAL(757, 22),  // "disconnectSelectedNode"
        QT_MOC_LITERAL(780, 15),  // "banSelectedNode"
        QT_MOC_LITERAL(796, 7),  // "bantime"
        QT_MOC_LITERAL(804, 17),  // "unbanSelectedNode"
        QT_MOC_LITERAL(822, 11),  // "setTabFocus"
        QT_MOC_LITERAL(834, 8),  // "TabTypes"
        QT_MOC_LITERAL(843, 7)   // "tabType"
    },
    "RPCConsole",
    "stopExecutor",
    "",
    "cmdRequest",
    "command",
    "on_lineEdit_returnPressed",
    "on_tabWidget_currentChanged",
    "index",
    "on_openDebugLogfileButton_clicked",
    "on_sldGraphRange_valueChanged",
    "value",
    "updateTrafficStats",
    "totalBytesIn",
    "totalBytesOut",
    "resizeEvent",
    "QResizeEvent*",
    "event",
    "showEvent",
    "QShowEvent*",
    "hideEvent",
    "QHideEvent*",
    "showPeersTableContextMenu",
    "point",
    "showBanTableContextMenu",
    "showOrHideBanTableIfRequired",
    "clearSelectedNode",
    "clear",
    "clearHistory",
    "fontBigger",
    "fontSmaller",
    "setFontSize",
    "newSize",
    "message",
    "category",
    "html",
    "setNumConnections",
    "count",
    "setNetworkActive",
    "networkActive",
    "setNumBlocks",
    "blockDate",
    "nVerificationProgress",
    "headers",
    "setMempoolSize",
    "numberOfTxs",
    "size_t",
    "dynUsage",
    "browseHistory",
    "offset",
    "scrollToEnd",
    "peerSelected",
    "QItemSelection",
    "selected",
    "deselected",
    "peerLayoutAboutToChange",
    "peerLayoutChanged",
    "disconnectSelectedNode",
    "banSelectedNode",
    "bantime",
    "unbanSelectedNode",
    "setTabFocus",
    "TabTypes",
    "tabType"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_RPCConsole[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      34,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  218,    2, 0x06,    1 /* Public */,
       3,    1,  219,    2, 0x06,    2 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    0,  222,    2, 0x08,    4 /* Private */,
       6,    1,  223,    2, 0x08,    5 /* Private */,
       8,    0,  226,    2, 0x08,    7 /* Private */,
       9,    1,  227,    2, 0x08,    8 /* Private */,
      11,    2,  230,    2, 0x08,   10 /* Private */,
      14,    1,  235,    2, 0x08,   13 /* Private */,
      17,    1,  238,    2, 0x08,   15 /* Private */,
      19,    1,  241,    2, 0x08,   17 /* Private */,
      21,    1,  244,    2, 0x08,   19 /* Private */,
      23,    1,  247,    2, 0x08,   21 /* Private */,
      24,    0,  250,    2, 0x08,   23 /* Private */,
      25,    0,  251,    2, 0x08,   24 /* Private */,
      26,    1,  252,    2, 0x0a,   25 /* Public */,
      26,    0,  255,    2, 0x2a,   27 /* Public | MethodCloned */,
      28,    0,  256,    2, 0x0a,   28 /* Public */,
      29,    0,  257,    2, 0x0a,   29 /* Public */,
      30,    1,  258,    2, 0x0a,   30 /* Public */,
      32,    3,  261,    2, 0x0a,   32 /* Public */,
      32,    2,  268,    2, 0x2a,   36 /* Public | MethodCloned */,
      35,    1,  273,    2, 0x0a,   39 /* Public */,
      37,    1,  276,    2, 0x0a,   41 /* Public */,
      39,    4,  279,    2, 0x0a,   43 /* Public */,
      43,    2,  288,    2, 0x0a,   48 /* Public */,
      47,    1,  293,    2, 0x0a,   51 /* Public */,
      49,    0,  296,    2, 0x0a,   53 /* Public */,
      50,    2,  297,    2, 0x0a,   54 /* Public */,
      54,    0,  302,    2, 0x0a,   57 /* Public */,
      55,    0,  303,    2, 0x0a,   58 /* Public */,
      56,    0,  304,    2, 0x0a,   59 /* Public */,
      57,    1,  305,    2, 0x0a,   60 /* Public */,
      59,    0,  308,    2, 0x0a,   62 /* Public */,
      60,    1,  309,    2, 0x0a,   63 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::ULongLong,   12,   13,
    QMetaType::Void, 0x80000000 | 15,   16,
    QMetaType::Void, 0x80000000 | 18,   16,
    QMetaType::Void, 0x80000000 | 20,   16,
    QMetaType::Void, QMetaType::QPoint,   22,
    QMetaType::Void, QMetaType::QPoint,   22,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   27,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   31,
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::Bool,   33,   32,   34,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   33,   32,
    QMetaType::Void, QMetaType::Int,   36,
    QMetaType::Void, QMetaType::Bool,   38,
    QMetaType::Void, QMetaType::Int, QMetaType::QDateTime, QMetaType::Double, QMetaType::Bool,   36,   40,   41,   42,
    QMetaType::Void, QMetaType::Long, 0x80000000 | 45,   44,   46,
    QMetaType::Void, QMetaType::Int,   48,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 51, 0x80000000 | 51,   52,   53,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   58,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 61,   62,

       0        // eod
};

Q_CONSTINIT const QMetaObject RPCConsole::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_RPCConsole.offsetsAndSizes,
    qt_meta_data_RPCConsole,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_RPCConsole_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<RPCConsole, std::true_type>,
        // method 'stopExecutor'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'cmdRequest'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'on_lineEdit_returnPressed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_tabWidget_currentChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_openDebugLogfileButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_sldGraphRange_valueChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'updateTrafficStats'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<quint64, std::false_type>,
        QtPrivate::TypeAndForceComplete<quint64, std::false_type>,
        // method 'resizeEvent'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QResizeEvent *, std::false_type>,
        // method 'showEvent'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QShowEvent *, std::false_type>,
        // method 'hideEvent'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QHideEvent *, std::false_type>,
        // method 'showPeersTableContextMenu'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QPoint &, std::false_type>,
        // method 'showBanTableContextMenu'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QPoint &, std::false_type>,
        // method 'showOrHideBanTableIfRequired'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'clearSelectedNode'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'clear'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'clear'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'fontBigger'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'fontSmaller'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'setFontSize'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'message'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'message'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
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
        // method 'setMempoolSize'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<long, std::false_type>,
        QtPrivate::TypeAndForceComplete<size_t, std::false_type>,
        // method 'browseHistory'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'scrollToEnd'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'peerSelected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QItemSelection &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QItemSelection &, std::false_type>,
        // method 'peerLayoutAboutToChange'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'peerLayoutChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'disconnectSelectedNode'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'banSelectedNode'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'unbanSelectedNode'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'setTabFocus'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<TabTypes, std::false_type>
    >,
    nullptr
} };

void RPCConsole::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RPCConsole *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->stopExecutor(); break;
        case 1: _t->cmdRequest((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->on_lineEdit_returnPressed(); break;
        case 3: _t->on_tabWidget_currentChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->on_openDebugLogfileButton_clicked(); break;
        case 5: _t->on_sldGraphRange_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->updateTrafficStats((*reinterpret_cast< std::add_pointer_t<quint64>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<quint64>>(_a[2]))); break;
        case 7: _t->resizeEvent((*reinterpret_cast< std::add_pointer_t<QResizeEvent*>>(_a[1]))); break;
        case 8: _t->showEvent((*reinterpret_cast< std::add_pointer_t<QShowEvent*>>(_a[1]))); break;
        case 9: _t->hideEvent((*reinterpret_cast< std::add_pointer_t<QHideEvent*>>(_a[1]))); break;
        case 10: _t->showPeersTableContextMenu((*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 11: _t->showBanTableContextMenu((*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 12: _t->showOrHideBanTableIfRequired(); break;
        case 13: _t->clearSelectedNode(); break;
        case 14: _t->clear((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 15: _t->clear(); break;
        case 16: _t->fontBigger(); break;
        case 17: _t->fontSmaller(); break;
        case 18: _t->setFontSize((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 19: _t->message((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[3]))); break;
        case 20: _t->message((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 21: _t->setNumConnections((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 22: _t->setNetworkActive((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 23: _t->setNumBlocks((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QDateTime>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[4]))); break;
        case 24: _t->setMempoolSize((*reinterpret_cast< std::add_pointer_t<long>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<size_t>>(_a[2]))); break;
        case 25: _t->browseHistory((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 26: _t->scrollToEnd(); break;
        case 27: _t->peerSelected((*reinterpret_cast< std::add_pointer_t<QItemSelection>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QItemSelection>>(_a[2]))); break;
        case 28: _t->peerLayoutAboutToChange(); break;
        case 29: _t->peerLayoutChanged(); break;
        case 30: _t->disconnectSelectedNode(); break;
        case 31: _t->banSelectedNode((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 32: _t->unbanSelectedNode(); break;
        case 33: _t->setTabFocus((*reinterpret_cast< std::add_pointer_t<TabTypes>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 27:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 1:
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QItemSelection >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (RPCConsole::*)();
            if (_t _q_method = &RPCConsole::stopExecutor; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (RPCConsole::*)(const QString & );
            if (_t _q_method = &RPCConsole::cmdRequest; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject *RPCConsole::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RPCConsole::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RPCConsole.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int RPCConsole::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 34)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 34;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 34)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 34;
    }
    return _id;
}

// SIGNAL 0
void RPCConsole::stopExecutor()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void RPCConsole::cmdRequest(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
