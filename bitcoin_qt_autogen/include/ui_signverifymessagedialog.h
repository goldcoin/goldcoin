/********************************************************************************
** Form generated from reading UI file 'signverifymessagedialog.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIGNVERIFYMESSAGEDIALOG_H
#define UI_SIGNVERIFYMESSAGEDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qvalidatedlineedit.h"

QT_BEGIN_NAMESPACE

class Ui_SignVerifyMessageDialog
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tabSignMessage;
    QVBoxLayout *verticalLayout_SM;
    QLabel *infoLabel_SM;
    QHBoxLayout *horizontalLayout_1_SM;
    QValidatedLineEdit *addressIn_SM;
    QPushButton *addressBookButton_SM;
    QPushButton *pasteButton_SM;
    QPlainTextEdit *messageIn_SM;
    QLabel *signatureLabel_SM;
    QHBoxLayout *horizontalLayout_2_SM;
    QLineEdit *signatureOut_SM;
    QPushButton *copySignatureButton_SM;
    QHBoxLayout *horizontalLayout_3_SM;
    QPushButton *signMessageButton_SM;
    QPushButton *clearButton_SM;
    QSpacerItem *horizontalSpacer_1_SM;
    QLabel *statusLabel_SM;
    QSpacerItem *horizontalSpacer_2_SM;
    QWidget *tabVerifyMessage;
    QVBoxLayout *verticalLayout_VM;
    QLabel *infoLabel_VM;
    QHBoxLayout *horizontalLayout_1_VM;
    QValidatedLineEdit *addressIn_VM;
    QPushButton *addressBookButton_VM;
    QPlainTextEdit *messageIn_VM;
    QValidatedLineEdit *signatureIn_VM;
    QHBoxLayout *horizontalLayout_2_VM;
    QPushButton *verifyMessageButton_VM;
    QPushButton *clearButton_VM;
    QSpacerItem *horizontalSpacer_1_VM;
    QLabel *statusLabel_VM;
    QSpacerItem *horizontalSpacer_2_VM;

    void setupUi(QDialog *SignVerifyMessageDialog)
    {
        if (SignVerifyMessageDialog->objectName().isEmpty())
            SignVerifyMessageDialog->setObjectName("SignVerifyMessageDialog");
        SignVerifyMessageDialog->resize(700, 380);
        SignVerifyMessageDialog->setModal(true);
        verticalLayout = new QVBoxLayout(SignVerifyMessageDialog);
        verticalLayout->setObjectName("verticalLayout");
        tabWidget = new QTabWidget(SignVerifyMessageDialog);
        tabWidget->setObjectName("tabWidget");
        tabSignMessage = new QWidget();
        tabSignMessage->setObjectName("tabSignMessage");
        verticalLayout_SM = new QVBoxLayout(tabSignMessage);
        verticalLayout_SM->setObjectName("verticalLayout_SM");
        infoLabel_SM = new QLabel(tabSignMessage);
        infoLabel_SM->setObjectName("infoLabel_SM");
        infoLabel_SM->setTextFormat(Qt::PlainText);
        infoLabel_SM->setWordWrap(true);

        verticalLayout_SM->addWidget(infoLabel_SM);

        horizontalLayout_1_SM = new QHBoxLayout();
        horizontalLayout_1_SM->setSpacing(0);
        horizontalLayout_1_SM->setObjectName("horizontalLayout_1_SM");
        addressIn_SM = new QValidatedLineEdit(tabSignMessage);
        addressIn_SM->setObjectName("addressIn_SM");

        horizontalLayout_1_SM->addWidget(addressIn_SM);

        addressBookButton_SM = new QPushButton(tabSignMessage);
        addressBookButton_SM->setObjectName("addressBookButton_SM");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/address-book"), QSize(), QIcon::Normal, QIcon::Off);
        addressBookButton_SM->setIcon(icon);
        addressBookButton_SM->setAutoDefault(false);

        horizontalLayout_1_SM->addWidget(addressBookButton_SM);

        pasteButton_SM = new QPushButton(tabSignMessage);
        pasteButton_SM->setObjectName("pasteButton_SM");
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/editpaste"), QSize(), QIcon::Normal, QIcon::Off);
        pasteButton_SM->setIcon(icon1);
        pasteButton_SM->setAutoDefault(false);

        horizontalLayout_1_SM->addWidget(pasteButton_SM);


        verticalLayout_SM->addLayout(horizontalLayout_1_SM);

        messageIn_SM = new QPlainTextEdit(tabSignMessage);
        messageIn_SM->setObjectName("messageIn_SM");

        verticalLayout_SM->addWidget(messageIn_SM);

        signatureLabel_SM = new QLabel(tabSignMessage);
        signatureLabel_SM->setObjectName("signatureLabel_SM");
        signatureLabel_SM->setTextFormat(Qt::PlainText);

        verticalLayout_SM->addWidget(signatureLabel_SM);

        horizontalLayout_2_SM = new QHBoxLayout();
        horizontalLayout_2_SM->setSpacing(0);
        horizontalLayout_2_SM->setObjectName("horizontalLayout_2_SM");
        signatureOut_SM = new QLineEdit(tabSignMessage);
        signatureOut_SM->setObjectName("signatureOut_SM");
        QFont font;
        font.setItalic(true);
        signatureOut_SM->setFont(font);
        signatureOut_SM->setReadOnly(true);

        horizontalLayout_2_SM->addWidget(signatureOut_SM);

        copySignatureButton_SM = new QPushButton(tabSignMessage);
        copySignatureButton_SM->setObjectName("copySignatureButton_SM");
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/editcopy"), QSize(), QIcon::Normal, QIcon::Off);
        copySignatureButton_SM->setIcon(icon2);
        copySignatureButton_SM->setAutoDefault(false);

        horizontalLayout_2_SM->addWidget(copySignatureButton_SM);


        verticalLayout_SM->addLayout(horizontalLayout_2_SM);

        horizontalLayout_3_SM = new QHBoxLayout();
        horizontalLayout_3_SM->setObjectName("horizontalLayout_3_SM");
        signMessageButton_SM = new QPushButton(tabSignMessage);
        signMessageButton_SM->setObjectName("signMessageButton_SM");
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/edit"), QSize(), QIcon::Normal, QIcon::Off);
        signMessageButton_SM->setIcon(icon3);
        signMessageButton_SM->setAutoDefault(false);

        horizontalLayout_3_SM->addWidget(signMessageButton_SM);

        clearButton_SM = new QPushButton(tabSignMessage);
        clearButton_SM->setObjectName("clearButton_SM");
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icons/remove"), QSize(), QIcon::Normal, QIcon::Off);
        clearButton_SM->setIcon(icon4);
        clearButton_SM->setAutoDefault(false);

        horizontalLayout_3_SM->addWidget(clearButton_SM);

        horizontalSpacer_1_SM = new QSpacerItem(40, 48, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3_SM->addItem(horizontalSpacer_1_SM);

        statusLabel_SM = new QLabel(tabSignMessage);
        statusLabel_SM->setObjectName("statusLabel_SM");
        QFont font1;
        font1.setBold(true);
        statusLabel_SM->setFont(font1);
        statusLabel_SM->setWordWrap(true);

        horizontalLayout_3_SM->addWidget(statusLabel_SM);

        horizontalSpacer_2_SM = new QSpacerItem(40, 48, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3_SM->addItem(horizontalSpacer_2_SM);


        verticalLayout_SM->addLayout(horizontalLayout_3_SM);

        tabWidget->addTab(tabSignMessage, QString());
        tabVerifyMessage = new QWidget();
        tabVerifyMessage->setObjectName("tabVerifyMessage");
        verticalLayout_VM = new QVBoxLayout(tabVerifyMessage);
        verticalLayout_VM->setObjectName("verticalLayout_VM");
        infoLabel_VM = new QLabel(tabVerifyMessage);
        infoLabel_VM->setObjectName("infoLabel_VM");
        infoLabel_VM->setTextFormat(Qt::PlainText);
        infoLabel_VM->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        infoLabel_VM->setWordWrap(true);

        verticalLayout_VM->addWidget(infoLabel_VM);

        horizontalLayout_1_VM = new QHBoxLayout();
        horizontalLayout_1_VM->setSpacing(0);
        horizontalLayout_1_VM->setObjectName("horizontalLayout_1_VM");
        addressIn_VM = new QValidatedLineEdit(tabVerifyMessage);
        addressIn_VM->setObjectName("addressIn_VM");

        horizontalLayout_1_VM->addWidget(addressIn_VM);

        addressBookButton_VM = new QPushButton(tabVerifyMessage);
        addressBookButton_VM->setObjectName("addressBookButton_VM");
        addressBookButton_VM->setIcon(icon);
        addressBookButton_VM->setAutoDefault(false);

        horizontalLayout_1_VM->addWidget(addressBookButton_VM);


        verticalLayout_VM->addLayout(horizontalLayout_1_VM);

        messageIn_VM = new QPlainTextEdit(tabVerifyMessage);
        messageIn_VM->setObjectName("messageIn_VM");

        verticalLayout_VM->addWidget(messageIn_VM);

        signatureIn_VM = new QValidatedLineEdit(tabVerifyMessage);
        signatureIn_VM->setObjectName("signatureIn_VM");

        verticalLayout_VM->addWidget(signatureIn_VM);

        horizontalLayout_2_VM = new QHBoxLayout();
        horizontalLayout_2_VM->setObjectName("horizontalLayout_2_VM");
        verifyMessageButton_VM = new QPushButton(tabVerifyMessage);
        verifyMessageButton_VM->setObjectName("verifyMessageButton_VM");
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/icons/transaction_0"), QSize(), QIcon::Normal, QIcon::Off);
        verifyMessageButton_VM->setIcon(icon5);
        verifyMessageButton_VM->setAutoDefault(false);

        horizontalLayout_2_VM->addWidget(verifyMessageButton_VM);

        clearButton_VM = new QPushButton(tabVerifyMessage);
        clearButton_VM->setObjectName("clearButton_VM");
        clearButton_VM->setIcon(icon4);
        clearButton_VM->setAutoDefault(false);

        horizontalLayout_2_VM->addWidget(clearButton_VM);

        horizontalSpacer_1_VM = new QSpacerItem(40, 48, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2_VM->addItem(horizontalSpacer_1_VM);

        statusLabel_VM = new QLabel(tabVerifyMessage);
        statusLabel_VM->setObjectName("statusLabel_VM");
        statusLabel_VM->setFont(font1);
        statusLabel_VM->setWordWrap(true);

        horizontalLayout_2_VM->addWidget(statusLabel_VM);

        horizontalSpacer_2_VM = new QSpacerItem(40, 48, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2_VM->addItem(horizontalSpacer_2_VM);


        verticalLayout_VM->addLayout(horizontalLayout_2_VM);

        tabWidget->addTab(tabVerifyMessage, QString());

        verticalLayout->addWidget(tabWidget);


        retranslateUi(SignVerifyMessageDialog);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(SignVerifyMessageDialog);
    } // setupUi

    void retranslateUi(QDialog *SignVerifyMessageDialog)
    {
        SignVerifyMessageDialog->setWindowTitle(QCoreApplication::translate("SignVerifyMessageDialog", "Signatures - Sign / Verify a Message", nullptr));
        infoLabel_SM->setText(QCoreApplication::translate("SignVerifyMessageDialog", "You can sign messages/agreements with your addresses to prove you can receive goldcoins sent to them. Be careful not to sign anything vague or random, as phishing attacks may try to trick you into signing your identity over to them. Only sign fully-detailed statements you agree to.", nullptr));
#if QT_CONFIG(tooltip)
        addressIn_SM->setToolTip(QCoreApplication::translate("SignVerifyMessageDialog", "The Goldcoin address to sign the message with", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        addressBookButton_SM->setToolTip(QCoreApplication::translate("SignVerifyMessageDialog", "Choose previously used address", nullptr));
#endif // QT_CONFIG(tooltip)
        addressBookButton_SM->setText(QString());
#if QT_CONFIG(shortcut)
        addressBookButton_SM->setShortcut(QCoreApplication::translate("SignVerifyMessageDialog", "Alt+A", nullptr));
#endif // QT_CONFIG(shortcut)
#if QT_CONFIG(tooltip)
        pasteButton_SM->setToolTip(QCoreApplication::translate("SignVerifyMessageDialog", "Paste address from clipboard", nullptr));
#endif // QT_CONFIG(tooltip)
        pasteButton_SM->setText(QString());
#if QT_CONFIG(shortcut)
        pasteButton_SM->setShortcut(QCoreApplication::translate("SignVerifyMessageDialog", "Alt+P", nullptr));
#endif // QT_CONFIG(shortcut)
#if QT_CONFIG(tooltip)
        messageIn_SM->setToolTip(QCoreApplication::translate("SignVerifyMessageDialog", "Enter the message you want to sign here", nullptr));
#endif // QT_CONFIG(tooltip)
        signatureLabel_SM->setText(QCoreApplication::translate("SignVerifyMessageDialog", "Signature", nullptr));
#if QT_CONFIG(tooltip)
        copySignatureButton_SM->setToolTip(QCoreApplication::translate("SignVerifyMessageDialog", "Copy the current signature to the system clipboard", nullptr));
#endif // QT_CONFIG(tooltip)
        copySignatureButton_SM->setText(QString());
#if QT_CONFIG(tooltip)
        signMessageButton_SM->setToolTip(QCoreApplication::translate("SignVerifyMessageDialog", "Sign the message to prove you own this Goldcoin address", nullptr));
#endif // QT_CONFIG(tooltip)
        signMessageButton_SM->setText(QCoreApplication::translate("SignVerifyMessageDialog", "Sign &Message", nullptr));
#if QT_CONFIG(tooltip)
        clearButton_SM->setToolTip(QCoreApplication::translate("SignVerifyMessageDialog", "Reset all sign message fields", nullptr));
#endif // QT_CONFIG(tooltip)
        clearButton_SM->setText(QCoreApplication::translate("SignVerifyMessageDialog", "Clear &All", nullptr));
        statusLabel_SM->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tabSignMessage), QCoreApplication::translate("SignVerifyMessageDialog", "&Sign Message", nullptr));
        infoLabel_VM->setText(QCoreApplication::translate("SignVerifyMessageDialog", "Enter the receiver's address, message (ensure you copy line breaks, spaces, tabs, etc. exactly) and signature below to verify the message. Be careful not to read more into the signature than what is in the signed message itself, to avoid being tricked by a man-in-the-middle attack. Note that this only proves the signing party receives with the address, it cannot prove sendership of any transaction!", nullptr));
#if QT_CONFIG(tooltip)
        addressIn_VM->setToolTip(QCoreApplication::translate("SignVerifyMessageDialog", "The Goldcoin address the message was signed with", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        addressBookButton_VM->setToolTip(QCoreApplication::translate("SignVerifyMessageDialog", "Choose previously used address", nullptr));
#endif // QT_CONFIG(tooltip)
        addressBookButton_VM->setText(QString());
#if QT_CONFIG(shortcut)
        addressBookButton_VM->setShortcut(QCoreApplication::translate("SignVerifyMessageDialog", "Alt+A", nullptr));
#endif // QT_CONFIG(shortcut)
#if QT_CONFIG(tooltip)
        verifyMessageButton_VM->setToolTip(QCoreApplication::translate("SignVerifyMessageDialog", "Verify the message to ensure it was signed with the specified Goldcoin address", nullptr));
#endif // QT_CONFIG(tooltip)
        verifyMessageButton_VM->setText(QCoreApplication::translate("SignVerifyMessageDialog", "Verify &Message", nullptr));
#if QT_CONFIG(tooltip)
        clearButton_VM->setToolTip(QCoreApplication::translate("SignVerifyMessageDialog", "Reset all verify message fields", nullptr));
#endif // QT_CONFIG(tooltip)
        clearButton_VM->setText(QCoreApplication::translate("SignVerifyMessageDialog", "Clear &All", nullptr));
        statusLabel_VM->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tabVerifyMessage), QCoreApplication::translate("SignVerifyMessageDialog", "&Verify Message", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SignVerifyMessageDialog: public Ui_SignVerifyMessageDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIGNVERIFYMESSAGEDIALOG_H
