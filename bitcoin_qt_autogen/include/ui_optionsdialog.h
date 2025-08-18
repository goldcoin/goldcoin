/********************************************************************************
** Form generated from reading UI file 'optionsdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPTIONSDIALOG_H
#define UI_OPTIONSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qvalidatedlineedit.h"
#include "qvaluecombobox.h"

QT_BEGIN_NAMESPACE

class Ui_OptionsDialog
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tabMain;
    QVBoxLayout *verticalLayout_Main;
    QCheckBox *bitcoinAtStartup;
    QHBoxLayout *horizontalLayout_2_Main;
    QLabel *databaseCacheLabel;
    QSpinBox *databaseCache;
    QLabel *databaseCacheUnitLabel;
    QSpacerItem *horizontalSpacer_2_Main;
    QHBoxLayout *horizontalLayout_3_Main;
    QLabel *threadsScriptVerifLabel;
    QSpinBox *threadsScriptVerif;
    QSpacerItem *horizontalSpacer_3_Main;
    QSpacerItem *verticalSpacer_Main;
    QWidget *tabWallet;
    QVBoxLayout *verticalLayout_Wallet;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QCheckBox *coinControlFeatures;
    QCheckBox *spendZeroConfChange;
    QSpacerItem *verticalSpacer_Wallet;
    QWidget *tabNetwork;
    QVBoxLayout *verticalLayout_Network;
    QCheckBox *mapPortUpnp;
    QCheckBox *allowIncoming;
    QCheckBox *connectSocks;
    QHBoxLayout *horizontalLayout_1_Network;
    QLabel *proxyIpLabel;
    QValidatedLineEdit *proxyIp;
    QLabel *proxyPortLabel;
    QLineEdit *proxyPort;
    QSpacerItem *horizontalSpacer_1_Network;
    QHBoxLayout *horizontalLayout_2_Network;
    QLabel *proxyActiveNets;
    QCheckBox *proxyReachIPv4;
    QLabel *proxyReachIPv4Label;
    QCheckBox *proxyReachIPv6;
    QLabel *proxyReachIPv6Label;
    QCheckBox *proxyReachTor;
    QLabel *proxyReachTorLabel;
    QSpacerItem *horizontalSpacer_2_Network;
    QCheckBox *connectSocksTor;
    QHBoxLayout *horizontalLayout_3_Network;
    QLabel *proxyIpTorLabel;
    QValidatedLineEdit *proxyIpTor;
    QLabel *proxyPortTorLabel;
    QLineEdit *proxyPortTor;
    QSpacerItem *horizontalSpacer_4_Network;
    QSpacerItem *verticalSpacer_Network;
    QWidget *tabWindow;
    QVBoxLayout *verticalLayout_Window;
    QCheckBox *hideTrayIcon;
    QCheckBox *minimizeToTray;
    QCheckBox *minimizeOnClose;
    QSpacerItem *verticalSpacer_Window;
    QWidget *tabDisplay;
    QVBoxLayout *verticalLayout_Display;
    QHBoxLayout *horizontalLayout_1_Display;
    QLabel *langLabel;
    QValueComboBox *lang;
    QHBoxLayout *horizontalLayout_2_Display;
    QLabel *unitLabel;
    QValueComboBox *unit;
    QHBoxLayout *horizontalLayout_3_Display;
    QLabel *thirdPartyTxUrlsLabel;
    QLineEdit *thirdPartyTxUrls;
    QSpacerItem *verticalSpacer_Display;
    QFrame *frame;
    QVBoxLayout *verticalLayout_Bottom;
    QHBoxLayout *horizontalLayout_Bottom;
    QLabel *overriddenByCommandLineInfoLabel;
    QSpacerItem *horizontalSpacer_Bottom;
    QLabel *overriddenByCommandLineLabel;
    QHBoxLayout *horizontalLayout_Buttons;
    QPushButton *resetButton;
    QSpacerItem *horizontalSpacer_1;
    QLabel *statusLabel;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *OptionsDialog)
    {
        if (OptionsDialog->objectName().isEmpty())
            OptionsDialog->setObjectName("OptionsDialog");
        OptionsDialog->resize(560, 440);
        OptionsDialog->setModal(true);
        verticalLayout = new QVBoxLayout(OptionsDialog);
        verticalLayout->setObjectName("verticalLayout");
        tabWidget = new QTabWidget(OptionsDialog);
        tabWidget->setObjectName("tabWidget");
        tabMain = new QWidget();
        tabMain->setObjectName("tabMain");
        verticalLayout_Main = new QVBoxLayout(tabMain);
        verticalLayout_Main->setObjectName("verticalLayout_Main");
        bitcoinAtStartup = new QCheckBox(tabMain);
        bitcoinAtStartup->setObjectName("bitcoinAtStartup");

        verticalLayout_Main->addWidget(bitcoinAtStartup);

        horizontalLayout_2_Main = new QHBoxLayout();
        horizontalLayout_2_Main->setObjectName("horizontalLayout_2_Main");
        databaseCacheLabel = new QLabel(tabMain);
        databaseCacheLabel->setObjectName("databaseCacheLabel");
        databaseCacheLabel->setTextFormat(Qt::PlainText);

        horizontalLayout_2_Main->addWidget(databaseCacheLabel);

        databaseCache = new QSpinBox(tabMain);
        databaseCache->setObjectName("databaseCache");

        horizontalLayout_2_Main->addWidget(databaseCache);

        databaseCacheUnitLabel = new QLabel(tabMain);
        databaseCacheUnitLabel->setObjectName("databaseCacheUnitLabel");
        databaseCacheUnitLabel->setTextFormat(Qt::PlainText);

        horizontalLayout_2_Main->addWidget(databaseCacheUnitLabel);

        horizontalSpacer_2_Main = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2_Main->addItem(horizontalSpacer_2_Main);


        verticalLayout_Main->addLayout(horizontalLayout_2_Main);

        horizontalLayout_3_Main = new QHBoxLayout();
        horizontalLayout_3_Main->setObjectName("horizontalLayout_3_Main");
        threadsScriptVerifLabel = new QLabel(tabMain);
        threadsScriptVerifLabel->setObjectName("threadsScriptVerifLabel");
        threadsScriptVerifLabel->setTextFormat(Qt::PlainText);

        horizontalLayout_3_Main->addWidget(threadsScriptVerifLabel);

        threadsScriptVerif = new QSpinBox(tabMain);
        threadsScriptVerif->setObjectName("threadsScriptVerif");

        horizontalLayout_3_Main->addWidget(threadsScriptVerif);

        horizontalSpacer_3_Main = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3_Main->addItem(horizontalSpacer_3_Main);


        verticalLayout_Main->addLayout(horizontalLayout_3_Main);

        verticalSpacer_Main = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_Main->addItem(verticalSpacer_Main);

        tabWidget->addTab(tabMain, QString());
        tabWallet = new QWidget();
        tabWallet->setObjectName("tabWallet");
        verticalLayout_Wallet = new QVBoxLayout(tabWallet);
        verticalLayout_Wallet->setObjectName("verticalLayout_Wallet");
        groupBox = new QGroupBox(tabWallet);
        groupBox->setObjectName("groupBox");
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setObjectName("verticalLayout_2");
        coinControlFeatures = new QCheckBox(groupBox);
        coinControlFeatures->setObjectName("coinControlFeatures");

        verticalLayout_2->addWidget(coinControlFeatures);

        spendZeroConfChange = new QCheckBox(groupBox);
        spendZeroConfChange->setObjectName("spendZeroConfChange");

        verticalLayout_2->addWidget(spendZeroConfChange);


        verticalLayout_Wallet->addWidget(groupBox);

        verticalSpacer_Wallet = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_Wallet->addItem(verticalSpacer_Wallet);

        tabWidget->addTab(tabWallet, QString());
        tabNetwork = new QWidget();
        tabNetwork->setObjectName("tabNetwork");
        verticalLayout_Network = new QVBoxLayout(tabNetwork);
        verticalLayout_Network->setObjectName("verticalLayout_Network");
        mapPortUpnp = new QCheckBox(tabNetwork);
        mapPortUpnp->setObjectName("mapPortUpnp");

        verticalLayout_Network->addWidget(mapPortUpnp);

        allowIncoming = new QCheckBox(tabNetwork);
        allowIncoming->setObjectName("allowIncoming");

        verticalLayout_Network->addWidget(allowIncoming);

        connectSocks = new QCheckBox(tabNetwork);
        connectSocks->setObjectName("connectSocks");

        verticalLayout_Network->addWidget(connectSocks);

        horizontalLayout_1_Network = new QHBoxLayout();
        horizontalLayout_1_Network->setObjectName("horizontalLayout_1_Network");
        proxyIpLabel = new QLabel(tabNetwork);
        proxyIpLabel->setObjectName("proxyIpLabel");
        proxyIpLabel->setTextFormat(Qt::PlainText);

        horizontalLayout_1_Network->addWidget(proxyIpLabel);

        proxyIp = new QValidatedLineEdit(tabNetwork);
        proxyIp->setObjectName("proxyIp");
        proxyIp->setMinimumSize(QSize(140, 0));
        proxyIp->setMaximumSize(QSize(140, 16777215));

        horizontalLayout_1_Network->addWidget(proxyIp);

        proxyPortLabel = new QLabel(tabNetwork);
        proxyPortLabel->setObjectName("proxyPortLabel");
        proxyPortLabel->setTextFormat(Qt::PlainText);

        horizontalLayout_1_Network->addWidget(proxyPortLabel);

        proxyPort = new QLineEdit(tabNetwork);
        proxyPort->setObjectName("proxyPort");
        proxyPort->setMinimumSize(QSize(55, 0));
        proxyPort->setMaximumSize(QSize(55, 16777215));

        horizontalLayout_1_Network->addWidget(proxyPort);

        horizontalSpacer_1_Network = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_1_Network->addItem(horizontalSpacer_1_Network);


        verticalLayout_Network->addLayout(horizontalLayout_1_Network);

        horizontalLayout_2_Network = new QHBoxLayout();
        horizontalLayout_2_Network->setObjectName("horizontalLayout_2_Network");
        proxyActiveNets = new QLabel(tabNetwork);
        proxyActiveNets->setObjectName("proxyActiveNets");
        proxyActiveNets->setTextFormat(Qt::PlainText);

        horizontalLayout_2_Network->addWidget(proxyActiveNets);

        proxyReachIPv4 = new QCheckBox(tabNetwork);
        proxyReachIPv4->setObjectName("proxyReachIPv4");
        proxyReachIPv4->setEnabled(false);

        horizontalLayout_2_Network->addWidget(proxyReachIPv4);

        proxyReachIPv4Label = new QLabel(tabNetwork);
        proxyReachIPv4Label->setObjectName("proxyReachIPv4Label");
        proxyReachIPv4Label->setTextFormat(Qt::PlainText);

        horizontalLayout_2_Network->addWidget(proxyReachIPv4Label);

        proxyReachIPv6 = new QCheckBox(tabNetwork);
        proxyReachIPv6->setObjectName("proxyReachIPv6");
        proxyReachIPv6->setEnabled(false);

        horizontalLayout_2_Network->addWidget(proxyReachIPv6);

        proxyReachIPv6Label = new QLabel(tabNetwork);
        proxyReachIPv6Label->setObjectName("proxyReachIPv6Label");
        proxyReachIPv6Label->setTextFormat(Qt::PlainText);

        horizontalLayout_2_Network->addWidget(proxyReachIPv6Label);

        proxyReachTor = new QCheckBox(tabNetwork);
        proxyReachTor->setObjectName("proxyReachTor");
        proxyReachTor->setEnabled(false);

        horizontalLayout_2_Network->addWidget(proxyReachTor);

        proxyReachTorLabel = new QLabel(tabNetwork);
        proxyReachTorLabel->setObjectName("proxyReachTorLabel");
        proxyReachTorLabel->setTextFormat(Qt::PlainText);

        horizontalLayout_2_Network->addWidget(proxyReachTorLabel);

        horizontalSpacer_2_Network = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2_Network->addItem(horizontalSpacer_2_Network);


        verticalLayout_Network->addLayout(horizontalLayout_2_Network);

        connectSocksTor = new QCheckBox(tabNetwork);
        connectSocksTor->setObjectName("connectSocksTor");

        verticalLayout_Network->addWidget(connectSocksTor);

        horizontalLayout_3_Network = new QHBoxLayout();
        horizontalLayout_3_Network->setObjectName("horizontalLayout_3_Network");
        proxyIpTorLabel = new QLabel(tabNetwork);
        proxyIpTorLabel->setObjectName("proxyIpTorLabel");
        proxyIpTorLabel->setTextFormat(Qt::PlainText);

        horizontalLayout_3_Network->addWidget(proxyIpTorLabel);

        proxyIpTor = new QValidatedLineEdit(tabNetwork);
        proxyIpTor->setObjectName("proxyIpTor");
        proxyIpTor->setMinimumSize(QSize(140, 0));
        proxyIpTor->setMaximumSize(QSize(140, 16777215));

        horizontalLayout_3_Network->addWidget(proxyIpTor);

        proxyPortTorLabel = new QLabel(tabNetwork);
        proxyPortTorLabel->setObjectName("proxyPortTorLabel");
        proxyPortTorLabel->setTextFormat(Qt::PlainText);

        horizontalLayout_3_Network->addWidget(proxyPortTorLabel);

        proxyPortTor = new QLineEdit(tabNetwork);
        proxyPortTor->setObjectName("proxyPortTor");
        proxyPortTor->setMinimumSize(QSize(55, 0));
        proxyPortTor->setMaximumSize(QSize(55, 16777215));

        horizontalLayout_3_Network->addWidget(proxyPortTor);

        horizontalSpacer_4_Network = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3_Network->addItem(horizontalSpacer_4_Network);


        verticalLayout_Network->addLayout(horizontalLayout_3_Network);

        verticalSpacer_Network = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_Network->addItem(verticalSpacer_Network);

        tabWidget->addTab(tabNetwork, QString());
        tabWindow = new QWidget();
        tabWindow->setObjectName("tabWindow");
        verticalLayout_Window = new QVBoxLayout(tabWindow);
        verticalLayout_Window->setObjectName("verticalLayout_Window");
        hideTrayIcon = new QCheckBox(tabWindow);
        hideTrayIcon->setObjectName("hideTrayIcon");

        verticalLayout_Window->addWidget(hideTrayIcon);

        minimizeToTray = new QCheckBox(tabWindow);
        minimizeToTray->setObjectName("minimizeToTray");

        verticalLayout_Window->addWidget(minimizeToTray);

        minimizeOnClose = new QCheckBox(tabWindow);
        minimizeOnClose->setObjectName("minimizeOnClose");

        verticalLayout_Window->addWidget(minimizeOnClose);

        verticalSpacer_Window = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_Window->addItem(verticalSpacer_Window);

        tabWidget->addTab(tabWindow, QString());
        tabDisplay = new QWidget();
        tabDisplay->setObjectName("tabDisplay");
        verticalLayout_Display = new QVBoxLayout(tabDisplay);
        verticalLayout_Display->setObjectName("verticalLayout_Display");
        horizontalLayout_1_Display = new QHBoxLayout();
        horizontalLayout_1_Display->setObjectName("horizontalLayout_1_Display");
        langLabel = new QLabel(tabDisplay);
        langLabel->setObjectName("langLabel");
        langLabel->setTextFormat(Qt::PlainText);

        horizontalLayout_1_Display->addWidget(langLabel);

        lang = new QValueComboBox(tabDisplay);
        lang->setObjectName("lang");

        horizontalLayout_1_Display->addWidget(lang);


        verticalLayout_Display->addLayout(horizontalLayout_1_Display);

        horizontalLayout_2_Display = new QHBoxLayout();
        horizontalLayout_2_Display->setObjectName("horizontalLayout_2_Display");
        unitLabel = new QLabel(tabDisplay);
        unitLabel->setObjectName("unitLabel");
        unitLabel->setTextFormat(Qt::PlainText);

        horizontalLayout_2_Display->addWidget(unitLabel);

        unit = new QValueComboBox(tabDisplay);
        unit->setObjectName("unit");

        horizontalLayout_2_Display->addWidget(unit);


        verticalLayout_Display->addLayout(horizontalLayout_2_Display);

        horizontalLayout_3_Display = new QHBoxLayout();
        horizontalLayout_3_Display->setObjectName("horizontalLayout_3_Display");
        thirdPartyTxUrlsLabel = new QLabel(tabDisplay);
        thirdPartyTxUrlsLabel->setObjectName("thirdPartyTxUrlsLabel");

        horizontalLayout_3_Display->addWidget(thirdPartyTxUrlsLabel);

        thirdPartyTxUrls = new QLineEdit(tabDisplay);
        thirdPartyTxUrls->setObjectName("thirdPartyTxUrls");

        horizontalLayout_3_Display->addWidget(thirdPartyTxUrls);


        verticalLayout_Display->addLayout(horizontalLayout_3_Display);

        verticalSpacer_Display = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_Display->addItem(verticalSpacer_Display);

        tabWidget->addTab(tabDisplay, QString());

        verticalLayout->addWidget(tabWidget);

        frame = new QFrame(OptionsDialog);
        frame->setObjectName("frame");
        verticalLayout_Bottom = new QVBoxLayout(frame);
        verticalLayout_Bottom->setObjectName("verticalLayout_Bottom");
        horizontalLayout_Bottom = new QHBoxLayout();
        horizontalLayout_Bottom->setObjectName("horizontalLayout_Bottom");
        overriddenByCommandLineInfoLabel = new QLabel(frame);
        overriddenByCommandLineInfoLabel->setObjectName("overriddenByCommandLineInfoLabel");
        overriddenByCommandLineInfoLabel->setTextFormat(Qt::PlainText);

        horizontalLayout_Bottom->addWidget(overriddenByCommandLineInfoLabel);

        horizontalSpacer_Bottom = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_Bottom->addItem(horizontalSpacer_Bottom);


        verticalLayout_Bottom->addLayout(horizontalLayout_Bottom);

        overriddenByCommandLineLabel = new QLabel(frame);
        overriddenByCommandLineLabel->setObjectName("overriddenByCommandLineLabel");
        overriddenByCommandLineLabel->setTextFormat(Qt::PlainText);
        overriddenByCommandLineLabel->setWordWrap(true);

        verticalLayout_Bottom->addWidget(overriddenByCommandLineLabel);


        verticalLayout->addWidget(frame);

        horizontalLayout_Buttons = new QHBoxLayout();
        horizontalLayout_Buttons->setObjectName("horizontalLayout_Buttons");
        resetButton = new QPushButton(OptionsDialog);
        resetButton->setObjectName("resetButton");
        resetButton->setAutoDefault(false);

        horizontalLayout_Buttons->addWidget(resetButton);

        horizontalSpacer_1 = new QSpacerItem(40, 48, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_Buttons->addItem(horizontalSpacer_1);

        statusLabel = new QLabel(OptionsDialog);
        statusLabel->setObjectName("statusLabel");
        statusLabel->setMinimumSize(QSize(200, 0));
        QFont font;
        font.setBold(true);
        statusLabel->setFont(font);
        statusLabel->setTextFormat(Qt::PlainText);
        statusLabel->setWordWrap(true);

        horizontalLayout_Buttons->addWidget(statusLabel);

        horizontalSpacer_2 = new QSpacerItem(40, 48, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_Buttons->addItem(horizontalSpacer_2);

        okButton = new QPushButton(OptionsDialog);
        okButton->setObjectName("okButton");
        okButton->setAutoDefault(false);

        horizontalLayout_Buttons->addWidget(okButton);

        cancelButton = new QPushButton(OptionsDialog);
        cancelButton->setObjectName("cancelButton");
        cancelButton->setAutoDefault(false);

        horizontalLayout_Buttons->addWidget(cancelButton);


        verticalLayout->addLayout(horizontalLayout_Buttons);

#if QT_CONFIG(shortcut)
        databaseCacheLabel->setBuddy(databaseCache);
        threadsScriptVerifLabel->setBuddy(threadsScriptVerif);
        proxyIpLabel->setBuddy(proxyIp);
        proxyPortLabel->setBuddy(proxyPort);
        proxyIpTorLabel->setBuddy(proxyIpTor);
        proxyPortTorLabel->setBuddy(proxyPortTor);
        langLabel->setBuddy(lang);
        unitLabel->setBuddy(unit);
        thirdPartyTxUrlsLabel->setBuddy(thirdPartyTxUrls);
#endif // QT_CONFIG(shortcut)

        retranslateUi(OptionsDialog);

        tabWidget->setCurrentIndex(0);
        okButton->setDefault(true);


        QMetaObject::connectSlotsByName(OptionsDialog);
    } // setupUi

    void retranslateUi(QDialog *OptionsDialog)
    {
        OptionsDialog->setWindowTitle(QCoreApplication::translate("OptionsDialog", "Options", nullptr));
#if QT_CONFIG(tooltip)
        bitcoinAtStartup->setToolTip(QCoreApplication::translate("OptionsDialog", "Automatically start %1 after logging in to the system.", nullptr));
#endif // QT_CONFIG(tooltip)
        bitcoinAtStartup->setText(QCoreApplication::translate("OptionsDialog", "&Start %1 on system login", nullptr));
        databaseCacheLabel->setText(QCoreApplication::translate("OptionsDialog", "Size of &database cache", nullptr));
        databaseCacheUnitLabel->setText(QCoreApplication::translate("OptionsDialog", "MB", nullptr));
        threadsScriptVerifLabel->setText(QCoreApplication::translate("OptionsDialog", "Number of script &verification threads", nullptr));
#if QT_CONFIG(tooltip)
        threadsScriptVerif->setToolTip(QCoreApplication::translate("OptionsDialog", "(0 = auto, <0 = leave that many cores free)", nullptr));
#endif // QT_CONFIG(tooltip)
        tabWidget->setTabText(tabWidget->indexOf(tabMain), QCoreApplication::translate("OptionsDialog", "&Main", nullptr));
        groupBox->setTitle(QCoreApplication::translate("OptionsDialog", "Expert", nullptr));
#if QT_CONFIG(tooltip)
        coinControlFeatures->setToolTip(QCoreApplication::translate("OptionsDialog", "Whether to show coin control features or not.", nullptr));
#endif // QT_CONFIG(tooltip)
        coinControlFeatures->setText(QCoreApplication::translate("OptionsDialog", "Enable coin &control features", nullptr));
#if QT_CONFIG(tooltip)
        spendZeroConfChange->setToolTip(QCoreApplication::translate("OptionsDialog", "If you disable the spending of unconfirmed change, the change from a transaction cannot be used until that transaction has at least one confirmation. This also affects how your balance is computed.", nullptr));
#endif // QT_CONFIG(tooltip)
        spendZeroConfChange->setText(QCoreApplication::translate("OptionsDialog", "&Spend unconfirmed change", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabWallet), QCoreApplication::translate("OptionsDialog", "W&allet", nullptr));
#if QT_CONFIG(tooltip)
        mapPortUpnp->setToolTip(QCoreApplication::translate("OptionsDialog", "Automatically open the Goldcoin client port on the router. This only works when your router supports UPnP and it is enabled.", nullptr));
#endif // QT_CONFIG(tooltip)
        mapPortUpnp->setText(QCoreApplication::translate("OptionsDialog", "Map port using &UPnP", nullptr));
#if QT_CONFIG(tooltip)
        allowIncoming->setToolTip(QCoreApplication::translate("OptionsDialog", "Accept connections from outside", nullptr));
#endif // QT_CONFIG(tooltip)
        allowIncoming->setText(QCoreApplication::translate("OptionsDialog", "Allow incoming connections", nullptr));
#if QT_CONFIG(tooltip)
        connectSocks->setToolTip(QCoreApplication::translate("OptionsDialog", "Connect to the Goldcoin network through a SOCKS5 proxy.", nullptr));
#endif // QT_CONFIG(tooltip)
        connectSocks->setText(QCoreApplication::translate("OptionsDialog", "&Connect through SOCKS5 proxy (default proxy):", nullptr));
        proxyIpLabel->setText(QCoreApplication::translate("OptionsDialog", "Proxy &IP:", nullptr));
#if QT_CONFIG(tooltip)
        proxyIp->setToolTip(QCoreApplication::translate("OptionsDialog", "IP address of the proxy (e.g. IPv4: 127.0.0.1 / IPv6: ::1)", nullptr));
#endif // QT_CONFIG(tooltip)
        proxyPortLabel->setText(QCoreApplication::translate("OptionsDialog", "&Port:", nullptr));
#if QT_CONFIG(tooltip)
        proxyPort->setToolTip(QCoreApplication::translate("OptionsDialog", "Port of the proxy (e.g. 9050)", nullptr));
#endif // QT_CONFIG(tooltip)
        proxyActiveNets->setText(QCoreApplication::translate("OptionsDialog", "Used for reaching peers via:", nullptr));
#if QT_CONFIG(tooltip)
        proxyReachIPv4->setToolTip(QCoreApplication::translate("OptionsDialog", "Shows, if the supplied default SOCKS5 proxy is used to reach peers via this network type.", nullptr));
#endif // QT_CONFIG(tooltip)
        proxyReachIPv4->setText(QString());
        proxyReachIPv4Label->setText(QCoreApplication::translate("OptionsDialog", "IPv4", nullptr));
#if QT_CONFIG(tooltip)
        proxyReachIPv6->setToolTip(QCoreApplication::translate("OptionsDialog", "Shows, if the supplied default SOCKS5 proxy is used to reach peers via this network type.", nullptr));
#endif // QT_CONFIG(tooltip)
        proxyReachIPv6->setText(QString());
        proxyReachIPv6Label->setText(QCoreApplication::translate("OptionsDialog", "IPv6", nullptr));
#if QT_CONFIG(tooltip)
        proxyReachTor->setToolTip(QCoreApplication::translate("OptionsDialog", "Shows, if the supplied default SOCKS5 proxy is used to reach peers via this network type.", nullptr));
#endif // QT_CONFIG(tooltip)
        proxyReachTor->setText(QString());
        proxyReachTorLabel->setText(QCoreApplication::translate("OptionsDialog", "Tor", nullptr));
#if QT_CONFIG(tooltip)
        connectSocksTor->setToolTip(QCoreApplication::translate("OptionsDialog", "Connect to the Goldcoin network through a separate SOCKS5 proxy for Tor hidden services.", nullptr));
#endif // QT_CONFIG(tooltip)
        connectSocksTor->setText(QCoreApplication::translate("OptionsDialog", "Use separate SOCKS5 proxy to reach peers via Tor hidden services:", nullptr));
        proxyIpTorLabel->setText(QCoreApplication::translate("OptionsDialog", "Proxy &IP:", nullptr));
#if QT_CONFIG(tooltip)
        proxyIpTor->setToolTip(QCoreApplication::translate("OptionsDialog", "IP address of the proxy (e.g. IPv4: 127.0.0.1 / IPv6: ::1)", nullptr));
#endif // QT_CONFIG(tooltip)
        proxyPortTorLabel->setText(QCoreApplication::translate("OptionsDialog", "&Port:", nullptr));
#if QT_CONFIG(tooltip)
        proxyPortTor->setToolTip(QCoreApplication::translate("OptionsDialog", "Port of the proxy (e.g. 9050)", nullptr));
#endif // QT_CONFIG(tooltip)
        tabWidget->setTabText(tabWidget->indexOf(tabNetwork), QCoreApplication::translate("OptionsDialog", "&Network", nullptr));
#if QT_CONFIG(tooltip)
        hideTrayIcon->setToolTip(QCoreApplication::translate("OptionsDialog", "&Hide the icon from the system tray.", nullptr));
#endif // QT_CONFIG(tooltip)
        hideTrayIcon->setText(QCoreApplication::translate("OptionsDialog", "Hide tray icon", nullptr));
#if QT_CONFIG(tooltip)
        minimizeToTray->setToolTip(QCoreApplication::translate("OptionsDialog", "Show only a tray icon after minimizing the window.", nullptr));
#endif // QT_CONFIG(tooltip)
        minimizeToTray->setText(QCoreApplication::translate("OptionsDialog", "&Minimize to the tray instead of the taskbar", nullptr));
#if QT_CONFIG(tooltip)
        minimizeOnClose->setToolTip(QCoreApplication::translate("OptionsDialog", "Minimize instead of exit the application when the window is closed. When this option is enabled, the application will be closed only after selecting Exit in the menu.", nullptr));
#endif // QT_CONFIG(tooltip)
        minimizeOnClose->setText(QCoreApplication::translate("OptionsDialog", "M&inimize on close", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabWindow), QCoreApplication::translate("OptionsDialog", "&Window", nullptr));
        langLabel->setText(QCoreApplication::translate("OptionsDialog", "User Interface &language:", nullptr));
#if QT_CONFIG(tooltip)
        lang->setToolTip(QCoreApplication::translate("OptionsDialog", "The user interface language can be set here. This setting will take effect after restarting %1.", nullptr));
#endif // QT_CONFIG(tooltip)
        unitLabel->setText(QCoreApplication::translate("OptionsDialog", "&Unit to show amounts in:", nullptr));
#if QT_CONFIG(tooltip)
        unit->setToolTip(QCoreApplication::translate("OptionsDialog", "Choose the default subdivision unit to show in the interface and when sending coins.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        thirdPartyTxUrlsLabel->setToolTip(QCoreApplication::translate("OptionsDialog", "Third party URLs (e.g. a block explorer) that appear in the transactions tab as context menu items. %s in the URL is replaced by transaction hash. Multiple URLs are separated by vertical bar |.", nullptr));
#endif // QT_CONFIG(tooltip)
        thirdPartyTxUrlsLabel->setText(QCoreApplication::translate("OptionsDialog", "Third party transaction URLs", nullptr));
#if QT_CONFIG(tooltip)
        thirdPartyTxUrls->setToolTip(QCoreApplication::translate("OptionsDialog", "Third party URLs (e.g. a block explorer) that appear in the transactions tab as context menu items. %s in the URL is replaced by transaction hash. Multiple URLs are separated by vertical bar |.", nullptr));
#endif // QT_CONFIG(tooltip)
        tabWidget->setTabText(tabWidget->indexOf(tabDisplay), QCoreApplication::translate("OptionsDialog", "&Display", nullptr));
        overriddenByCommandLineInfoLabel->setText(QCoreApplication::translate("OptionsDialog", "Active command-line options that override above options:", nullptr));
        overriddenByCommandLineLabel->setText(QString());
#if QT_CONFIG(tooltip)
        resetButton->setToolTip(QCoreApplication::translate("OptionsDialog", "Reset all client options to default.", nullptr));
#endif // QT_CONFIG(tooltip)
        resetButton->setText(QCoreApplication::translate("OptionsDialog", "&Reset Options", nullptr));
        statusLabel->setText(QString());
        okButton->setText(QCoreApplication::translate("OptionsDialog", "&OK", nullptr));
        cancelButton->setText(QCoreApplication::translate("OptionsDialog", "&Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class OptionsDialog: public Ui_OptionsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPTIONSDIALOG_H
