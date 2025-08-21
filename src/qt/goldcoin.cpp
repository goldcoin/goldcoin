// Copyright (c) 2011-2016 The Bitcoin Core developers
#include <filesystem>
// Copyright (c) 2013-2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#if defined(HAVE_CONFIG_H)
#include "config/bitcoin-config.h"
#endif

#include "version_info.h"  // For PACKAGE_NAME
#include "goldcoingui.h"

#include "chainparams.h"
#include "clientmodel.h"
#include "guiconstants.h"
#include "guiutil.h"
#include "intro.h"
#include "networkstyle.h"
#include "optionsmodel.h"
#include "platformstyle.h"
#include "splashscreen.h"
#include "utilitydialog.h"
#include "winshutdownmonitor.h"

#ifdef ENABLE_WALLET
// BIP70 removed - #include "paymentserver.h"  
#include "walletmodel.h"
#endif

#include "init.h"
#include "rpc/server.h"
#include "scheduler.h"
#include "../ui_interface.h"
#include "util.h"
#include "warnings.h"
#include "fs.h"  // For filesystem operations

#ifdef ENABLE_WALLET
#include "wallet/wallet.h"
#endif

#include <stdint.h>

#include <thread>
#include <vector>

#include <QApplication>
#include <QDebug>
#include <QLibraryInfo>
#include <QLocale>
#include <QMessageBox>
#include <QSettings>
#include <QThread>
#include <QTimer>
#include <QTranslator>
// #include <QSslConfiguration> // Qt6 built without SSL support

#if defined(QT_STATICPLUGIN)
#include <QtPlugin>
#if defined(QT_QPA_PLATFORM_XCB)
Q_IMPORT_PLUGIN(QXcbIntegrationPlugin);
#elif defined(QT_QPA_PLATFORM_WINDOWS)
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin);
#elif defined(QT_QPA_PLATFORM_COCOA)
Q_IMPORT_PLUGIN(QCocoaIntegrationPlugin);
#endif
#endif

// Qt 6.9: QTextCodec removed, UTF-8 is the only supported encoding

// Declare meta types used for QMetaObject::invokeMethod
Q_DECLARE_METATYPE(bool*)
// Qt6 already has metatypes for int64_t/long, so CAmount doesn't need declaration

static void InitMessage(const std::string &message)
{
    LogPrintf("init message: %s\n", message);
}

/*
   Translate string to current locale using Qt.
 */
static std::string Translate(const char* psz)
{
    return QCoreApplication::translate("bitcoin-core", psz).toStdString();
}

static QString GetLangTerritory()
{
    QSettings settings;
    // Get desired locale (e.g. "de_DE")
    // 1) System default language
    QString lang_territory = QLocale::system().name();
    // 2) Language from QSettings
    QString lang_territory_qsettings = settings.value("language", "").toString();
    if(!lang_territory_qsettings.isEmpty())
        lang_territory = lang_territory_qsettings;
    // 3) -lang command line argument
    lang_territory = QString::fromStdString(GetArg("-lang", lang_territory.toStdString()));
    return lang_territory;
}

/** Set up translations */
static void initTranslations(QTranslator &qtTranslatorBase, QTranslator &qtTranslator, QTranslator &translatorBase, QTranslator &translator)
{
    // Remove old translators
    QApplication::removeTranslator(&qtTranslatorBase);
    QApplication::removeTranslator(&qtTranslator);
    QApplication::removeTranslator(&translatorBase);
    QApplication::removeTranslator(&translator);

    // Get desired locale (e.g. "de_DE")
    // 1) System default language
    QString lang_territory = GetLangTerritory();

    // Convert to "de" only by truncating "_DE"
    QString lang = lang_territory;
    lang.truncate(lang_territory.lastIndexOf('_'));

    // Load language files for configured locale:
    // - First load the translator for the base language, without territory
    // - Then load the more specific locale translator

    // Load e.g. qt_de.qm
    if (qtTranslatorBase.load("qt_" + lang, QLibraryInfo::path(QLibraryInfo::TranslationsPath)))
        QApplication::installTranslator(&qtTranslatorBase);

    // Load e.g. qt_de_DE.qm
    if (qtTranslator.load("qt_" + lang_territory, QLibraryInfo::path(QLibraryInfo::TranslationsPath)))
        QApplication::installTranslator(&qtTranslator);

    // Load e.g. bitcoin_de.qm (shortcut "de" needs to be defined in bitcoin.qrc)
    if (translatorBase.load(lang, ":/translations/"))
        QApplication::installTranslator(&translatorBase);

    // Load e.g. bitcoin_de_DE.qm (shortcut "de_DE" needs to be defined in bitcoin.qrc)
    if (translator.load(lang_territory, ":/translations/"))
        QApplication::installTranslator(&translator);
}

/* qDebug() message handler --> debug.log */
void DebugMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString &msg)
{
    Q_UNUSED(context);
    const char *category = (type == QtDebugMsg) ? "qt" : nullptr;
    LogPrint(category, "GUI: %s\n", msg.toStdString());
}

/** Class encapsulating Bitcoin Core startup and shutdown.
 * Allows running startup and shutdown in a different thread from the UI thread.
 */
class BitcoinCore: public QObject
{
    Q_OBJECT
public:
    explicit BitcoinCore();

public Q_SLOTS:
    void initialize();
    void shutdown();

Q_SIGNALS:
    void initializeResult(int retval);
    void shutdownResult(int retval);
    void runawayException(const QString &message);

private:
    std::vector<std::thread> threads;
    thread_group threadGroup;
    CScheduler scheduler;

    /// Pass fatal exception message to UI thread
    void handleRunawayException(const std::exception *e);
};

/** Main Bitcoin application object */
class GoldcoinApplication: public QApplication
{
    Q_OBJECT
public:
    explicit GoldcoinApplication(int &argc, char **argv);
    ~GoldcoinApplication();

#ifdef ENABLE_WALLET
    /// Create payment server
    // BIP70 removed - void createPaymentServer();
#endif
    /// parameter interaction/setup based on rules
    void parameterSetup();
    /// Create options model
    void createOptionsModel(bool resetSettings);
    /// Create main window
    void createWindow(const NetworkStyle *networkStyle);
    /// Create splash screen
    void createSplashScreen(const NetworkStyle *networkStyle);

    /// Request core initialization
    void requestInitialize();
    /// Request core shutdown
    void requestShutdown();

    /// Get process return value
    int getReturnValue() { return returnValue; }

    /// Get window identifier of QMainWindow (BitcoinGUI)
    WId getMainWinId() const;

public Q_SLOTS:
    void initializeResult(int retval);
    void shutdownResult(int retval);
    /// Handle runaway exceptions. Shows a message box with the problem and quits the program.
    void handleRunawayException(const QString &message);

Q_SIGNALS:
    void requestedInitialize();
    void requestedShutdown();
    void stopThread();
    void splashFinished(QWidget *window);

private:
    QThread *coreThread;
    OptionsModel *optionsModel;
    ClientModel *clientModel;
    BitcoinGUI *window;
    QTimer *pollShutdownTimer;
#ifdef ENABLE_WALLET
    // BIP70 removed - PaymentServer* paymentServer;
    WalletModel *walletModel;
#endif
    int returnValue;
    const PlatformStyle *platformStyle;
    std::unique_ptr<QWidget> shutdownWindow;

    void startThread();

    static const QString DEFAULT_WALLET;
    static const std::string DEFAULT_UIPLATFORM;
};

#include "goldcoin.moc"

BitcoinCore::BitcoinCore():
    QObject()
{
}

void BitcoinCore::handleRunawayException(const std::exception *e)
{
    PrintExceptionContinue(e, "Runaway exception");
    Q_EMIT runawayException(QString::fromStdString(GetWarnings("gui")));
}

void BitcoinCore::initialize()
{
    try
    {
        qDebug() << __func__ << ": Running AppInit2 in thread";
        if (!AppInitBasicSetup())
        {
            Q_EMIT initializeResult(false);
            return;
        }
        if (!AppInitParameterInteraction())
        {
            Q_EMIT initializeResult(false);
            return;
        }
        if (!AppInitSanityChecks())
        {
            Q_EMIT initializeResult(false);
            return;
        }
        int rv = AppInitMain(threadGroup, scheduler);
        Q_EMIT initializeResult(rv);
    } catch (const std::exception& e) {
        handleRunawayException(&e);
    } catch (...) {
        handleRunawayException(nullptr);
    }
}

void BitcoinCore::shutdown()
{
    try
    {
        qDebug() << __func__ << ": Running Shutdown in thread";
        LogPrintf("Shutting down...\n");
        Interrupt(threadGroup, scheduler);
        
        // Detach threads instead of joining to prevent shutdown hangs
        auto& threads = threadGroup.get_threads();
        for (size_t i = 0; i < threads.size(); ++i) {
            if (threads[i].joinable()) {
                threads[i].detach();
            }
        }
        threads.clear();
        
        // Give detached threads a moment to finish cleanly
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        
        Shutdown();  // This performs all wallet flushes and safe shutdown
        qDebug() << __func__ << ": Shutdown finished";
        Q_EMIT shutdownResult(1);
    } catch (const std::exception& e) {
        LogPrintf("GoldcoinCore::shutdown: Exception caught: %s\n", e.what());
        handleRunawayException(&e);
    } catch (...) {
        LogPrintf("GoldcoinCore::shutdown: Unknown exception caught\n");
        handleRunawayException(nullptr);
    }
}

GoldcoinApplication::GoldcoinApplication(int &argc, char **argv):
    QApplication(argc, argv),
    coreThread(nullptr),
    optionsModel(nullptr),
    clientModel(nullptr),
    window(nullptr),
    pollShutdownTimer(nullptr),
#ifdef ENABLE_WALLET
    // BIP70 removed - paymentServer(nullptr),
    walletModel(nullptr),
#endif
    returnValue(0)
{
    setQuitOnLastWindowClosed(false);

    // UI per-platform customization
    // This must be done inside the GoldcoinApplication constructor, or after it, because
    // PlatformStyle::instantiate requires a QApplication
    std::string platformName;
    platformName = GetArg("-uiplatform", DEFAULT_UIPLATFORM);
    platformStyle = PlatformStyle::instantiate(QString::fromStdString(platformName));
    if (!platformStyle) // Fall back to "other" if specified name not found
        platformStyle = PlatformStyle::instantiate("other");
    assert(platformStyle);
}

GoldcoinApplication::~GoldcoinApplication()
{
    if(coreThread)
    {
        qDebug() << __func__ << ": Stopping thread";
        Q_EMIT stopThread();
        coreThread->wait();
        qDebug() << __func__ << ": Stopped thread";
    }

    delete window;
    window = nullptr;
#ifdef ENABLE_WALLET
    // BIP70 removed
    // delete paymentServer;
    // paymentServer = nullptr;
#endif
    delete optionsModel;
    optionsModel = nullptr;
    delete platformStyle;
    platformStyle = nullptr;
}

#ifdef ENABLE_WALLET
// BIP70 removed
// void GoldcoinApplication::createPaymentServer()
// {
//     paymentServer = new PaymentServer(this);
// }
#endif

void GoldcoinApplication::createOptionsModel(bool resetSettings)
{
    optionsModel = new OptionsModel(nullptr, resetSettings);
}

void GoldcoinApplication::createWindow(const NetworkStyle *networkStyle)
{
    window = new BitcoinGUI(platformStyle, networkStyle, nullptr);

    pollShutdownTimer = new QTimer(window);
    connect(pollShutdownTimer, &QTimer::timeout, window, &BitcoinGUI::detectShutdown);
    pollShutdownTimer->start(200);
}

void GoldcoinApplication::createSplashScreen(const NetworkStyle *networkStyle)
{
    SplashScreen *splash = new SplashScreen(networkStyle);
    // We don't hold a direct pointer to the splash screen after creation, but the splash
    // screen will take care of deleting itself when slotFinish happens.
    splash->show();
    connect(this, &GoldcoinApplication::splashFinished, splash, &SplashScreen::slotFinish);
    connect(this, &GoldcoinApplication::requestedShutdown, splash, &QWidget::close);}

void GoldcoinApplication::startThread()
{
    if(coreThread)
        return;
    coreThread = new QThread(this);
    BitcoinCore *executor = new BitcoinCore();
    executor->moveToThread(coreThread);

    /*  communication to and from thread */
    connect(executor, &BitcoinCore::initializeResult, this, &GoldcoinApplication::initializeResult);
    connect(executor, &BitcoinCore::shutdownResult, this, &GoldcoinApplication::shutdownResult);
    connect(executor, &BitcoinCore::runawayException, this, &GoldcoinApplication::handleRunawayException);
    connect(this, &GoldcoinApplication::requestedInitialize, executor, &BitcoinCore::initialize);
    connect(this, &GoldcoinApplication::requestedShutdown, executor, &BitcoinCore::shutdown);
    /*  make sure executor object is deleted in its own thread */
    connect(this, &GoldcoinApplication::stopThread, executor, &QObject::deleteLater);
    connect(this, &GoldcoinApplication::stopThread, coreThread, &QThread::quit);
    coreThread->start();
}

void GoldcoinApplication::parameterSetup()
{
    InitLogging();
    InitParameterInteraction();
}

void GoldcoinApplication::requestInitialize()
{
    qDebug() << __func__ << ": Requesting initialize";
    startThread();
    Q_EMIT requestedInitialize();
}

void GoldcoinApplication::requestShutdown()
{
    // Show a simple window indicating shutdown status
    // Do this first as some of the steps may take some time below,
    // for example the RPC console may still be executing a command.
    shutdownWindow.reset(ShutdownWindow::showShutdownWindow(window));

    qDebug() << __func__ << ": Requesting shutdown";
    startThread();
    window->hide();
    window->setClientModel(nullptr);
    pollShutdownTimer->stop();

#ifdef ENABLE_WALLET
    window->removeAllWallets();
    delete walletModel;
    walletModel = nullptr;
#endif
    delete clientModel;
    clientModel = nullptr;

    StartShutdown();

    // Request shutdown from core thread
    Q_EMIT requestedShutdown();
}

void GoldcoinApplication::initializeResult(int retval)
{
    qDebug() << __func__ << ": Initialization result: " << retval;
    // Set exit result: 0 if successful, 1 if failure
    returnValue = retval ? 0 : 1;
    if(retval)
    {
        // Log this only after AppInit2 finishes, as then logging setup is guaranteed complete
        qWarning() << "Platform customization:" << platformStyle->getName();
#ifdef ENABLE_WALLET
        // BIP70 removed - PaymentServer::LoadRootCAs();
        // BIP70 removed - paymentServer->setOptionsModel(optionsModel);
#endif

        clientModel = new ClientModel(optionsModel);
        window->setClientModel(clientModel);

#ifdef ENABLE_WALLET
        if(pwalletMain)
        {
            walletModel = new WalletModel(platformStyle, pwalletMain, optionsModel);

            window->addWallet(DEFAULT_WALLET, walletModel);
            window->setCurrentWallet(DEFAULT_WALLET);

            // BIP70 removed
            // connect(walletModel, &WalletModel::coinsSent,
            //                  paymentServer, &PaymentServer::fetchPaymentACK);
        }
#endif

        // If -min option passed, start window minimized.
        if(GetBoolArg("-min", false))
        {
            window->showMinimized();
        }
        else
        {
            window->show();
        }
        Q_EMIT splashFinished(window);

#ifdef ENABLE_WALLET
        // BIP70 removed - Payment request processing
        // Now that initialization/startup is done, process any command-line
        // bitcoin: URIs (payment requests removed with BIP70):
        // connect(paymentServer, &PaymentServer::receivedPaymentRequest,
        //                  window, &BitcoinGUI::handlePaymentRequest);
        // connect(window, &BitcoinGUI::receivedURI,
        //                  paymentServer, &PaymentServer::handleURIOrFile);
        // connect(paymentServer, &PaymentServer::message,
        //                  window, [this](const QString& title, const QString& message, unsigned int style) {
        //                      window->message(title, message, style);
        //                  });
        // QTimer::singleShot(100, paymentServer, &PaymentServer::uiReady);
#endif
    } else {
        quit(); // Exit main loop
    }
}

void GoldcoinApplication::shutdownResult(int retval)
{
    qDebug() << __func__ << ": Shutdown result: " << retval;
    
    // Stop the core thread
    if(coreThread) {
        Q_EMIT stopThread();
        coreThread->quit();
        coreThread->wait(2000);  // Wait max 2 seconds
    }
    
    // Close all windows and exit
    QApplication::closeAllWindows();
    QCoreApplication::exit(0);
    
    // Backup forced exit if the above doesn't work
    QTimer::singleShot(1000, []() {
        std::exit(0);  // Hard exit as last resort (wallet already safe)
    });
}

void GoldcoinApplication::handleRunawayException(const QString &message)
{
    QMessageBox::critical(nullptr, "Runaway exception", GoldcoinApplication::tr("A fatal error occurred. Goldcoin can no longer continue safely and will quit.") + QString("\n\n") + message);
    ::exit(EXIT_FAILURE);
}

WId GoldcoinApplication::getMainWinId() const
{
    if (!window)
        return 0;

    return window->winId();
}

#ifndef BITCOIN_QT_TEST
int main(int argc, char *argv[])
{
    SetupEnvironment();

    /// 1. Parse command-line options. These take precedence over anything else.
    // Command-line options take precedence:
    ParseParameters(argc, argv);

    // Do not refer to data directory yet, this can be overridden by Intro::pickDataDirectory

    /// 2. Basic Qt initialization (not dependent on parameters or configuration)
    // Qt 6.9: UTF-8 is the default encoding, no codec setup needed

    Q_INIT_RESOURCE(bitcoin);
    // Q_INIT_RESOURCE(bitcoin_locale); // Skip locale resources for now

    GoldcoinApplication app(argc, argv);
    // Generate high-dpi pixmaps and high-DPI scaling are always enabled in Qt6
#ifdef Q_OS_MAC
    // Qt 6: AA_DontShowIconsInMenus is removed, icon display in menus is platform-specific
#endif
    // Because of the POODLE attack it is recommended to disable SSLv3 (https://disablessl3.com/),
    // so set SSL protocols to TLS1.0+.
    // Note: Qt6 was built without SSL support, so this is disabled
    // QSslConfiguration sslconf = QSslConfiguration::defaultConfiguration();
    // sslconf.setProtocol(QSsl::TlsV1_0OrLater);
    // QSslConfiguration::setDefaultConfiguration(sslconf);

    // Register meta types used for QMetaObject::invokeMethod
    qRegisterMetaType< bool* >();
    //   Need to pass name here as CAmount is a typedef (see http://qt-project.org/doc/qt-5/qmetatype.html#qRegisterMetaType)
    //   IMPORTANT if it is no longer a typedef use the normal variant above
    // Qt6 already has built-in support for int64_t/long
    qRegisterMetaType< int64_t >("CAmount");

    /// 3. Application identification
    // must be set before OptionsModel is initialized or translations are loaded,
    // as it is used to locate QSettings
    QApplication::setOrganizationName(QAPP_ORG_NAME);
    QApplication::setOrganizationDomain(QAPP_ORG_DOMAIN);
    QApplication::setApplicationName(QAPP_APP_NAME_DEFAULT);
    // GUIUtil::SubstituteFonts(GetLangTerritory());

    /// 4. Initialization of translations, so that intro dialog is in user's language
    // Now that QSettings are accessible, initialize translations
    QTranslator qtTranslatorBase, qtTranslator, translatorBase, translator;
    initTranslations(qtTranslatorBase, qtTranslator, translatorBase, translator);
    translationInterface.Translate.connect(Translate);

    // Show help message immediately after parsing command-line options (for "-lang") and setting locale,
    // but before showing splash screen.
    if (IsArgSet("-?") || IsArgSet("-h") || IsArgSet("-help") || IsArgSet("-version"))
    {
        HelpMessageDialog help(nullptr, IsArgSet("-version"));
        help.showOrPrint();
        return EXIT_SUCCESS;
    }

    /// 5. Now that settings and translations are available, ask user for data directory
    // User language is set up: pick a data directory
    if (!Intro::pickDataDirectory())
        return EXIT_SUCCESS;

    /// 6. Determine availability of data directory and parse bitcoin.conf
    /// - Do not call GetDataDir(true) before this step finishes
    if (!fsbridge::IsDirectory(GetDataDir(false)))
    {
        QMessageBox::critical(nullptr, QObject::tr(PACKAGE_NAME),
                              QObject::tr("Error: Specified data directory \"%1\" does not exist.").arg(QString::fromStdString(GetArg("-datadir", ""))));
        return EXIT_FAILURE;
    }
    try {
        ReadConfigFile(GetArg("-conf", BITCOIN_CONF_FILENAME));
    } catch (const std::exception& e) {
        QMessageBox::critical(nullptr, QObject::tr(PACKAGE_NAME),
                              QObject::tr("Error: Cannot parse configuration file: %1. Only use key=value syntax.").arg(e.what()));
        return EXIT_FAILURE;
    }

    /// 7. Determine network (and switch to network specific options)
    // - Do not call Params() before this step
    // - Do this after parsing the configuration file, as the network can be switched there
    // - QSettings() will use the new application name after this, resulting in network-specific settings
    // - Needs to be done before createOptionsModel

    // Check for -testnet or -regtest parameter (Params() calls are only valid after this clause)
    try {
        SelectParams(ChainNameFromCommandLine());
    } catch(std::exception &e) {
        QMessageBox::critical(nullptr, QObject::tr(PACKAGE_NAME), QObject::tr("Error: %1").arg(e.what()));
        return EXIT_FAILURE;
    }
#ifdef ENABLE_WALLET
    // BIP70 removed - Parse URIs on command line -- this can affect Params()
    // PaymentServer::ipcParseCommandLine(argc, argv);
#endif

    QScopedPointer<const NetworkStyle> networkStyle(NetworkStyle::instantiate(QString::fromStdString(Params().NetworkIDString())));
    assert(!networkStyle.isNull());
    // Allow for separate UI settings for testnets
    QApplication::setApplicationName(networkStyle->getAppName());
    // Re-initialize translations after changing application name (language in network-specific settings can be different)
    initTranslations(qtTranslatorBase, qtTranslator, translatorBase, translator);

#ifdef ENABLE_WALLET
    /// 8. URI IPC sending
    // - Do this early as we don't want to bother initializing if we are just calling IPC
    // - Do this *after* setting up the data directory, as the data directory hash is used in the name
    // of the server.
    // BIP70 removed - IPC command line handling
    // - Do this after creating app and setting up translations, so errors are
    // translated properly.
    // if (PaymentServer::ipcSendCommandLine())
    //     exit(EXIT_SUCCESS);

    // BIP70 removed - Payment server functionality
    // Start up the payment server early, too, so impatient users that click on
    // bitcoin: links repeatedly have their payment requests routed to this process:
    // app.createPaymentServer();
#endif

    /// 9. Main GUI initialization
    // Install global event filter that makes sure that long tooltips can be word-wrapped
    app.installEventFilter(new GUIUtil::ToolTipToRichTextFilter(TOOLTIP_WRAP_THRESHOLD, &app));
#if defined(Q_OS_WIN)
    // Install global event filter for processing Windows session related Windows messages (WM_QUERYENDSESSION and WM_ENDSESSION)
    qApp->installNativeEventFilter(new WinShutdownMonitor());
#endif
    // Install qDebug() message handler to route to debug.log
    qInstallMessageHandler(DebugMessageHandler);
    // Allow parameter interaction before we create the options model
    app.parameterSetup();
    // Load GUI settings from QSettings
    app.createOptionsModel(IsArgSet("-resetguisettings"));

    // Subscribe to global signals from core using modern std::function callbacks
    uiInterface.AddInitMessageCallback([](const std::string& message) {
        InitMessage(message);
    });
    
    // Qt 6.9 signal connections - no boost needed
    uiInterface.InitMessage.connect(InitMessage);

    if (GetBoolArg("-splash", DEFAULT_SPLASHSCREEN) && !GetBoolArg("-min", false))
        app.createSplashScreen(networkStyle.data());

    try
    {
        app.createWindow(networkStyle.data());
        app.requestInitialize();
#if defined(Q_OS_WIN)
        WinShutdownMonitor::registerShutdownBlockReason(QObject::tr("%1 didn't yet exit safely...").arg(QObject::tr(PACKAGE_NAME)), (HWND)app.getMainWinId());
#endif
        app.exec();
        app.requestShutdown();
        app.exec();
    } catch (const std::exception& e) {
        PrintExceptionContinue(&e, "Runaway exception");
        app.handleRunawayException(QString::fromStdString(GetWarnings("gui")));
    } catch (...) {
        PrintExceptionContinue(nullptr, "Runaway exception");
        app.handleRunawayException(QString::fromStdString(GetWarnings("gui")));
    }
    return app.getReturnValue();
}
#endif // BITCOIN_QT_TEST

const QString GoldcoinApplication::DEFAULT_WALLET = QString("~Default");
const std::string GoldcoinApplication::DEFAULT_UIPLATFORM = "other";
