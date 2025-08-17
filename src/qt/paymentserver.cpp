// Copyright (c) 2011-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "paymentserver.h"

#include "bitcoinunits.h"
#include "guiutil.h"
#include "optionsmodel.h"

#include "base58.h"
#include "chainparams.h"
#include "policy/policy.h"
#include "../ui_interface.h"
#include "util.h"
#include "wallet/wallet.h"

#include <cstdlib>
#include <filesystem>
#include <boost/filesystem.hpp>

#include <QApplication>
#include <QByteArray>
#include <QDataStream>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QFileOpenEvent>
#include <QFileInfo>
#include <QHash>
#include <QList>
#include <QLocalServer>
#include <QLocalSocket>
#include <QStringList>
#include <QTextDocument>
#include <QUrlQuery>

const int BITCOIN_IPC_CONNECT_TIMEOUT = 1000; // milliseconds
const QString BITCOIN_IPC_PREFIX("goldcoin:");

//
// Create a name that is unique for:
//  testnet / non-testnet
//  data directory
//
static QString ipcServerName()
{
    QString name("GoldcoinQt");

    // Append a simple hash of the datadir
    // Note that GetDataDir(true) returns a different path
    // for -testnet versus main net
    boost::filesystem::path boostPath = GetDataDir(true);
    std::filesystem::path stdPath(boostPath.string());
    QString ddir(GUIUtil::boostPathToQString(stdPath));
    name.append(QString::number(qHash(ddir)));

    return name;
}

//
// We store payment URIs and requests received before
// the main GUI window is up and ready to ask the user
// to send payment.

static QList<QString> savedPaymentRequests;

//
// Sending to the server is done synchronously, at startup.
// If the server isn't already running, startup continues,
// and the items in savedPaymentRequest will be handled
// when uiReady() is called.
//
// Warning: ipcSendCommandLine() is called early in init,
// so don't use "Q_EMIT message()", but "QMessageBox::"!
//
void PaymentServer::ipcParseCommandLine(int argc, char* argv[])
{
    for (int i = 1; i < argc; i++)
    {
        QString arg(argv[i]);
        if (arg.startsWith("-"))
            continue;

        // If the goldcoin: URI contains a payment request, we need to start
        // the network client now to ensure we don't lose any data
        if (arg.startsWith(BITCOIN_IPC_PREFIX, Qt::CaseInsensitive))
        {
            savedPaymentRequests.append(arg);
        }
        else if (QFile::exists(arg)) // Filename
        {
            savedPaymentRequests.append(arg);
            // BIP70 is no longer supported - files are just saved for later
        }
        else
        {
            // Printing to debug.log is about the best we can do here, the
            // GUI hasn't started yet so we can't pop up a message box.
            qWarning() << "PaymentServer::ipcSendCommandLine: Payment request file does not exist: " << arg;
        }
    }
}

//
// Sending to the server is done synchronously, at startup.
// If the server isn't already running, startup continues,
// and the items in savedPaymentRequest will be handled
// when uiReady() is called.
//
bool PaymentServer::ipcSendCommandLine()
{
    bool fResult = false;
    for (const QString& r : savedPaymentRequests)
    {
        QLocalSocket* socket = new QLocalSocket();
        socket->connectToServer(ipcServerName(), QIODevice::WriteOnly);
        if (!socket->waitForConnected(BITCOIN_IPC_CONNECT_TIMEOUT))
        {
            delete socket;
            socket = NULL;
            return false;
        }

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_0);
        out << r;
        out.device()->seek(0);

        socket->write(block);
        socket->flush();
        socket->waitForBytesWritten(BITCOIN_IPC_CONNECT_TIMEOUT);
        socket->disconnectFromServer();

        delete socket;
        socket = NULL;
        fResult = true;
    }

    return fResult;
}

PaymentServer::PaymentServer(QObject* parent, bool startLocalServer) :
    QObject(parent),
    saveURIs(true),
    uriServer(0),
    optionsModel(0)
{
    // Install global event filter to catch QFileOpenEvents
    // on Mac: sent when you click bitcoin: links
    // other OSes: helpful when dealing with payment request files
    if (parent)
        parent->installEventFilter(this);

    QString name = ipcServerName();

    // Clean up old socket leftover from a crash:
    QLocalServer::removeServer(name);

    if (startLocalServer)
    {
        uriServer = new QLocalServer(this);

        if (!uriServer->listen(name)) {
            // constructor is called early in init, so don't use "Q_EMIT message()" here
            QMessageBox::critical(0, tr("Payment request error"),
                tr("Cannot start goldcoin: click-to-pay handler"));
        }
        else {
            connect(uriServer, &QLocalServer::newConnection, this, &PaymentServer::handleURIConnection);
        }
    }
}

PaymentServer::~PaymentServer()
{
}

//
// OSX-specific way of handling bitcoin: URIs and payment request files
//
bool PaymentServer::eventFilter(QObject *object, QEvent *event)
{
    // clicking on bitcoin: URIs creates FileOpen events on the Mac
    if (event->type() == QEvent::FileOpen)
    {
        QFileOpenEvent *fileEvent = static_cast<QFileOpenEvent*>(event);
        if (!fileEvent->file().isEmpty())
            handleURIOrFile(fileEvent->file());
        else if (!fileEvent->url().isEmpty())
            handleURIOrFile(fileEvent->url().toString());

        return true;
    }

    return QObject::eventFilter(object, event);
}

void PaymentServer::uiReady()
{
    saveURIs = false;
    for (const QString& s : savedPaymentRequests)
    {
        handleURIOrFile(s);
    }
    savedPaymentRequests.clear();
}

void PaymentServer::handleURIOrFile(const QString& s)
{
    if (saveURIs)
    {
        savedPaymentRequests.append(s);
        return;
    }

    if (s.startsWith(BITCOIN_IPC_PREFIX, Qt::CaseInsensitive)) // bitcoin: URI
    {
        QUrlQuery uri((QUrl(s)));
        // BIP70 payment requests are no longer supported
        if (uri.hasQueryItem("r"))
        {
            Q_EMIT message(tr("URI handling"),
                tr("BIP70 payment requests are no longer supported."),
                CClientUIInterface::ICON_WARNING);
            return;
        }
        
        // normal URI
        SendCoinsRecipient recipient;
        if (GUIUtil::parseBitcoinURI(s, &recipient))
        {
            CBitcoinAddress address(recipient.address.toStdString());
            if (!address.IsValid()) {
                Q_EMIT message(tr("URI handling"), tr("Invalid payment address %1").arg(recipient.address),
                    CClientUIInterface::MSG_ERROR);
            }
            else
                Q_EMIT receivedPaymentRequest(recipient);
        }
        else
            Q_EMIT message(tr("URI handling"),
                tr("URI cannot be parsed! This can be caused by an invalid Goldcoin address or malformed URI parameters."),
                CClientUIInterface::ICON_WARNING);

        return;
    }

    if (QFile::exists(s)) // payment request file
    {
        // BIP70 payment request files are no longer supported
        Q_EMIT message(tr("Payment request file handling"),
            tr("BIP70 payment request files are no longer supported."),
            CClientUIInterface::ICON_WARNING);
        return;
    }
}

void PaymentServer::handleURIConnection()
{
    QLocalSocket *clientConnection = uriServer->nextPendingConnection();

    while (clientConnection->bytesAvailable() < (int)sizeof(quint32))
        clientConnection->waitForReadyRead();

    connect(clientConnection, &QLocalSocket::disconnected,
            clientConnection, &QObject::deleteLater);

    QDataStream in(clientConnection);
    in.setVersion(QDataStream::Qt_4_0);
    if (clientConnection->bytesAvailable() < (int)sizeof(quint16)) {
        return;
    }
    QString msg;
    in >> msg;

    handleURIOrFile(msg);
}

void PaymentServer::setOptionsModel(OptionsModel *_optionsModel)
{
    this->optionsModel = _optionsModel;
}