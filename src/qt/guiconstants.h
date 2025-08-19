// Copyright (c) 2011-2016 The Bitcoin Core developers
// Copyright (c) 2013-2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_QT_GUICONSTANTS_H
#define BITCOIN_QT_GUICONSTANTS_H

#include <QColor>
#include <cstdint>
#include <string_view>

// C++23 modernization of Qt constants
namespace goldcoin::qt::constants {

/* Milliseconds between model updates */
inline constexpr int MODEL_UPDATE_DELAY = 250;

/* AskPassphraseDialog -- Maximum passphrase length */
inline constexpr int MAX_PASSPHRASE_SIZE = 1024;

/* BitcoinGUI -- Size of icons in status bar */
inline constexpr int STATUSBAR_ICONSIZE = 16;

/* Default splash screen setting */
inline constexpr bool DEFAULT_SPLASHSCREEN = true;

/* Tooltips longer than this (in characters) are converted into rich text */
inline constexpr int TOOLTIP_WRAP_THRESHOLD = 80;

/* Maximum allowed URI length */
inline constexpr int MAX_URI_LENGTH = 255;

/* QRCodeDialog -- size of exported QR Code image */
inline constexpr int QR_IMAGE_SIZE = 300;

/* Number of frames in spinner animation */
inline constexpr int SPINNER_FRAMES = 36;

/* Organization constants using C++23 string_view */
inline constexpr std::string_view ORG_NAME = "Goldcoin";
inline constexpr std::string_view ORG_DOMAIN = "goldcoin.org";
inline constexpr std::string_view APP_NAME_DEFAULT = "Goldcoin-Qt";
inline constexpr std::string_view APP_NAME_TESTNET = "Goldcoin-Qt-testnet";

/* C++23 consteval for compile-time color creation */
consteval QColor MakeColor(int r, int g, int b) {
    return QColor(r, g, b);
}

/* Transaction list colors as inline constexpr variables */
inline const QColor COLOR_UNCONFIRMED = QColor(128, 128, 128);
inline const QColor COLOR_NEGATIVE = QColor(255, 0, 0);
inline const QColor COLOR_BAREADDRESS = QColor(140, 140, 140);
inline const QColor COLOR_TX_STATUS_OPENUNTILDATE = QColor(64, 64, 255);
inline const QColor COLOR_TX_STATUS_OFFLINE = QColor(192, 192, 192);
inline const QColor COLOR_TX_STATUS_DANGER = QColor(200, 100, 100);
inline const QColor COLOR_BLACK = QColor(0, 0, 0);

/* Invalid field background style */
inline constexpr std::string_view STYLE_INVALID = "background:#FF8080";

} // namespace goldcoin::qt::constants

// Legacy compatibility (to be removed in future)
using namespace goldcoin::qt::constants;

// Legacy macros for backward compatibility
#define QAPP_ORG_NAME "Goldcoin"
#define QAPP_ORG_DOMAIN "goldcoin.org"
#define QAPP_APP_NAME_DEFAULT "Goldcoin-Qt"
#define QAPP_APP_NAME_TESTNET "Goldcoin-Qt-testnet"

#endif // BITCOIN_QT_GUICONSTANTS_H