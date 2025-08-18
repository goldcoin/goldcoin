// Copyright (c) 2011-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "signverifymessagedialog.h"
#include "ui_signverifymessagedialog.h"

#include "addressbookpage.h"
#include "guiutil.h"
#include "platformstyle.h"
#include "walletmodel.h"

#include "base58.h"
#include "init.h"
#include "validation.h" // For strMessageMagic
#include "wallet/wallet.h"

#include <string>
#include <vector>
#include <optional>
#include <string_view>
#include <span>

#include <QClipboard>

// C++20: Concept for address validation
template<typename T>
concept AddressType = requires(T addr) {
    { addr.IsValid() } -> std::convertible_to<bool>;
    { addr.GetKeyID(std::declval<CKeyID&>()) } -> std::convertible_to<bool>;
};

SignVerifyMessageDialog::SignVerifyMessageDialog(const PlatformStyle *_platformStyle, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SignVerifyMessageDialog),
    model(nullptr),
    platformStyle(_platformStyle)
{
    ui->setupUi(this);

    // C++20: Structured icon setup
    const auto setupIcon = [this](auto* button, std::string_view iconPath) {
        button->setIcon(platformStyle->SingleColorIcon(QString::fromStdString(std::string(iconPath))));
    };
    
    // Sign message icons
    setupIcon(ui->addressBookButton_SM, ":/icons/address-book");
    setupIcon(ui->pasteButton_SM, ":/icons/editpaste");
    setupIcon(ui->copySignatureButton_SM, ":/icons/editcopy");
    setupIcon(ui->signMessageButton_SM, ":/icons/edit");
    setupIcon(ui->clearButton_SM, ":/icons/remove");
    
    // Verify message icons
    setupIcon(ui->addressBookButton_VM, ":/icons/address-book");
    setupIcon(ui->verifyMessageButton_VM, ":/icons/transaction_0");
    setupIcon(ui->clearButton_VM, ":/icons/remove");

    // Qt 6.9: Placeholder text is always supported
    ui->signatureOut_SM->setPlaceholderText(tr("Click \"Sign Message\" to generate signature"));

    GUIUtil::setupAddressWidget(ui->addressIn_SM, this);
    GUIUtil::setupAddressWidget(ui->addressIn_VM, this);

    // C++20: Use array for event filter installation
    const std::array<QWidget*, 6> filterWidgets = {
        ui->addressIn_SM, ui->messageIn_SM, ui->signatureOut_SM,
        ui->addressIn_VM, ui->messageIn_VM, ui->signatureIn_VM
    };
    
    for (auto* widget : filterWidgets) {
        widget->installEventFilter(this);
    }

    ui->signatureOut_SM->setFont(GUIUtil::fixedPitchFont());
    ui->signatureIn_VM->setFont(GUIUtil::fixedPitchFont());
}

SignVerifyMessageDialog::~SignVerifyMessageDialog()
{
    delete ui;
}

void SignVerifyMessageDialog::setModel(WalletModel *_model)
{
    this->model = _model;
}

void SignVerifyMessageDialog::setAddress_SM(const QString &address)
{
    ui->addressIn_SM->setText(address);
    ui->messageIn_SM->setFocus();
}

void SignVerifyMessageDialog::setAddress_VM(const QString &address)
{
    ui->addressIn_VM->setText(address);
    ui->messageIn_VM->setFocus();
}

void SignVerifyMessageDialog::showTab_SM(bool fShow)
{
    ui->tabWidget->setCurrentIndex(0);
    if (fShow) {
        this->show();
    }
}

void SignVerifyMessageDialog::showTab_VM(bool fShow)
{
    ui->tabWidget->setCurrentIndex(1);
    if (fShow) {
        this->show();
    }
}

void SignVerifyMessageDialog::on_addressBookButton_SM_clicked()
{
    if (!model || !model->getAddressTableModel()) return;
    
    AddressBookPage dlg(platformStyle, AddressBookPage::ForSelection, 
                       AddressBookPage::ReceivingTab, this);
    dlg.setModel(model->getAddressTableModel());
    
    if (dlg.exec()) {
        setAddress_SM(dlg.getReturnValue());
    }
}

void SignVerifyMessageDialog::on_pasteButton_SM_clicked()
{
    setAddress_SM(QApplication::clipboard()->text());
}

void SignVerifyMessageDialog::on_signMessageButton_SM_clicked()
{
    if (!model) return;

    // Clear old signature to avoid confusion
    ui->signatureOut_SM->clear();

    // C++20: Lambda for error display
    const auto showError = [this](const QString& message) {
        ui->statusLabel_SM->setStyleSheet("QLabel { color: red; }");
        ui->statusLabel_SM->setText(message);
    };

    // Validate address
    const CBitcoinAddress addr(ui->addressIn_SM->text().toStdString());
    if (!addr.IsValid()) {
        showError(tr("The entered address is invalid.") + " " + 
                 tr("Please check the address and try again."));
        return;
    }
    
    CKeyID keyID;
    if (!addr.GetKeyID(keyID)) {
        ui->addressIn_SM->setValid(false);
        showError(tr("The entered address does not refer to a key.") + " " + 
                 tr("Please check the address and try again."));
        return;
    }

    // Request wallet unlock
    WalletModel::UnlockContext ctx(model->requestUnlock());
    if (!ctx.isValid()) {
        showError(tr("Wallet unlock was cancelled."));
        return;
    }

    // Get private key
    CKey key;
    if (!model->getPrivKey(keyID, key)) {
        showError(tr("Private key for the entered address is not available."));
        return;
    }

    // C++20: Create message hash
    CHashWriter ss(SER_GETHASH, 0);
    ss << strMessageMagic;
    ss << ui->messageIn_SM->document()->toPlainText().toStdString();

    // Sign the message
    std::vector<unsigned char> vchSig;
    if (!key.SignCompact(ss.GetHash(), vchSig)) {
        showError("<nobr>" + tr("Message signing failed.") + "</nobr>");
        return;
    }

    // Success
    ui->statusLabel_SM->setStyleSheet("QLabel { color: green; }");
    ui->statusLabel_SM->setText("<nobr>" + tr("Message signed.") + "</nobr>");

    // C++20: Use span for signature encoding
    ui->signatureOut_SM->setText(QString::fromStdString(
        EncodeBase64(vchSig.data(), vchSig.size())));
}

void SignVerifyMessageDialog::on_copySignatureButton_SM_clicked()
{
    GUIUtil::setClipboard(ui->signatureOut_SM->text());
}

void SignVerifyMessageDialog::on_clearButton_SM_clicked()
{
    // Clear sign message fields
    ui->addressIn_SM->clear();
    ui->messageIn_SM->clear();
    ui->signatureOut_SM->clear();
    
    ui->statusLabel_SM->clear();
    ui->addressIn_SM->setFocus();
}

void SignVerifyMessageDialog::on_addressBookButton_VM_clicked()
{
    if (!model || !model->getAddressTableModel()) return;
    
    AddressBookPage dlg(platformStyle, AddressBookPage::ForSelection, 
                       AddressBookPage::SendingTab, this);
    dlg.setModel(model->getAddressTableModel());
    
    if (dlg.exec()) {
        setAddress_VM(dlg.getReturnValue());
    }
}

void SignVerifyMessageDialog::on_verifyMessageButton_VM_clicked()
{
    // C++20: Lambda for error display
    const auto showError = [this](const QString& message, bool invalidateAddress = false) {
        if (invalidateAddress) {
            ui->addressIn_VM->setValid(false);
        }
        ui->statusLabel_VM->setStyleSheet("QLabel { color: red; }");
        ui->statusLabel_VM->setText(message);
    };

    // Validate address
    const CBitcoinAddress addr(ui->addressIn_VM->text().toStdString());
    if (!addr.IsValid()) {
        showError(tr("The entered address is invalid.") + " " + 
                 tr("Please check the address and try again."));
        return;
    }
    
    CKeyID keyID;
    if (!addr.GetKeyID(keyID)) {
        showError(tr("The entered address does not refer to a key.") + " " + 
                 tr("Please check the address and try again."), true);
        return;
    }

    // Decode signature
    bool fInvalid = false;
    const auto vchSig = DecodeBase64(ui->signatureIn_VM->text().toStdString().c_str(), &fInvalid);

    if (fInvalid) {
        ui->signatureIn_VM->setValid(false);
        showError(tr("The signature could not be decoded.") + " " + 
                 tr("Please check the signature and try again."));
        return;
    }

    // Create message hash
    CHashWriter ss(SER_GETHASH, 0);
    ss << strMessageMagic;
    ss << ui->messageIn_VM->document()->toPlainText().toStdString();

    // Recover public key from signature
    CPubKey pubkey;
    if (!pubkey.RecoverCompact(ss.GetHash(), vchSig)) {
        ui->signatureIn_VM->setValid(false);
        showError(tr("The signature did not match the message digest.") + " " + 
                 tr("Please check the signature and try again."));
        return;
    }

    // Verify address matches recovered key
    if (CBitcoinAddress(pubkey.GetID()) != addr) {
        showError("<nobr>" + tr("Message verification failed.") + "</nobr>");
        return;
    }

    // Success
    ui->statusLabel_VM->setStyleSheet("QLabel { color: green; }");
    ui->statusLabel_VM->setText("<nobr>" + tr("Message verified.") + "</nobr>");
}

void SignVerifyMessageDialog::on_clearButton_VM_clicked()
{
    // C++20: Clear all verify message fields
    const std::array<QWidget*, 3> fields = {
        ui->addressIn_VM, ui->signatureIn_VM, ui->messageIn_VM
    };
    
    for (auto* field : fields) {
        if (auto* lineEdit = qobject_cast<QLineEdit*>(field)) {
            lineEdit->clear();
        } else if (auto* textEdit = qobject_cast<QTextEdit*>(field)) {
            textEdit->clear();
        }
    }
    
    ui->statusLabel_VM->clear();
    ui->addressIn_VM->setFocus();
}

bool SignVerifyMessageDialog::eventFilter(QObject *object, QEvent *event)
{
    // C++20: Early return for non-relevant events
    if (event->type() != QEvent::MouseButtonPress && 
        event->type() != QEvent::FocusIn) {
        return QDialog::eventFilter(object, event);
    }

    const int currentTab = ui->tabWidget->currentIndex();
    
    // Sign message tab
    if (currentTab == 0) {
        ui->statusLabel_SM->clear();
        
        // Auto-select signature on focus
        if (object == ui->signatureOut_SM) {
            ui->signatureOut_SM->selectAll();
            return true;
        }
    }
    // Verify message tab
    else if (currentTab == 1) {
        ui->statusLabel_VM->clear();
    }
    
    return QDialog::eventFilter(object, event);
}
