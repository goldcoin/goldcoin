// Copyright (c) 2011-2016 The Bitcoin Core developers
// Copyright (c) 2013-2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#if defined(HAVE_CONFIG_H)
#include "config/bitcoin-config.h"
#endif

#include "askpassphrasedialog.h"
#include "ui_askpassphrasedialog.h"

#include "version_info.h"  // For PACKAGE_NAME
#include "guiconstants.h"
#include "walletmodel.h"

#include "support/allocators/secure.h"

#include <QKeyEvent>
#include <QMessageBox>
#include <QPushButton>

#include <string_view>
#include <optional>
#include <algorithm>
#include <memory>

// C++20: Concept for secure string validation
template<typename T>
concept SecureStringType = requires(T str) {
    { str.empty() } -> std::convertible_to<bool>;
    { str.clear() };
    { str.size() } -> std::convertible_to<size_t>;
};

AskPassphraseDialog::AskPassphraseDialog(Mode _mode, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AskPassphraseDialog),
    mode(_mode),
    model(nullptr),
    fCapsLock(false)
{
    ui->setupUi(this);

    ui->passEdit1->setMinimumSize(ui->passEdit1->sizeHint());
    ui->passEdit2->setMinimumSize(ui->passEdit2->sizeHint());
    ui->passEdit3->setMinimumSize(ui->passEdit3->sizeHint());

    ui->passEdit1->setMaxLength(MAX_PASSPHRASE_SIZE);
    ui->passEdit2->setMaxLength(MAX_PASSPHRASE_SIZE);
    ui->passEdit3->setMaxLength(MAX_PASSPHRASE_SIZE);

    // Setup Caps Lock detection.
    ui->passEdit1->installEventFilter(this);
    ui->passEdit2->installEventFilter(this);
    ui->passEdit3->installEventFilter(this);

    // C++20: Use structured bindings and lambda for UI setup
    const auto setupDialogMode = [this](const QString& warning, const QString& title, 
                                        bool showPass1, bool showPass2, bool showPass3) {
        ui->warningLabel->setText(warning);
        setWindowTitle(title);
        ui->passLabel1->setVisible(showPass1);
        ui->passEdit1->setVisible(showPass1);
        ui->passLabel2->setVisible(showPass2);
        ui->passEdit2->setVisible(showPass2);
        ui->passLabel3->setVisible(showPass3);
        ui->passEdit3->setVisible(showPass3);
    };

    switch(mode) {
        case Encrypt: // Ask passphrase x2
            setupDialogMode(
                tr("Enter the new passphrase to the wallet.<br/>Please use a passphrase of <b>ten or more random characters</b>, or <b>eight or more words</b>."),
                tr("Encrypt wallet"),
                false, true, true);
            break;
        case Unlock: // Ask passphrase
            setupDialogMode(
                tr("This operation needs your wallet passphrase to unlock the wallet."),
                tr("Unlock wallet"),
                true, false, false);
            break;
        case Decrypt: // Ask passphrase
            setupDialogMode(
                tr("This operation needs your wallet passphrase to decrypt the wallet."),
                tr("Decrypt wallet"),
                true, false, false);
            break;
        case ChangePass: // Ask old passphrase + new passphrase x2
            setupDialogMode(
                tr("Enter the old passphrase and new passphrase to the wallet."),
                tr("Change passphrase"),
                true, true, true);
            break;
    }
    textChanged();
    
    // Qt 6.9: Modern signal connections with lambda
    const auto connectPassphraseField = [this](QLineEdit* field) {
        connect(field, &QLineEdit::textChanged, this, [this]() {
            textChanged();
        });
    };
    
    connectPassphraseField(ui->passEdit1);
    connectPassphraseField(ui->passEdit2);
    connectPassphraseField(ui->passEdit3);
}

AskPassphraseDialog::~AskPassphraseDialog()
{
    secureClearPassFields();
    delete ui;
}

void AskPassphraseDialog::setModel(WalletModel *_model)
{
    this->model = _model;
}

void AskPassphraseDialog::accept()
{
    if (!model) return;

    // C++20: RAII pattern for secure string management
    struct SecurePassphrases {
        SecureString oldpass, newpass1, newpass2;
        SecurePassphrases() {
            oldpass.reserve(MAX_PASSPHRASE_SIZE);
            newpass1.reserve(MAX_PASSPHRASE_SIZE);
            newpass2.reserve(MAX_PASSPHRASE_SIZE);
        }
    } passphrases;
    
    // Extract passphrases securely
    const auto extractPassphrase = [](QLineEdit* edit, SecureString& target) {
        const auto text = edit->text().toStdString();
        target.assign(text.c_str());
    };
    
    extractPassphrase(ui->passEdit1, passphrases.oldpass);
    extractPassphrase(ui->passEdit2, passphrases.newpass1);
    extractPassphrase(ui->passEdit3, passphrases.newpass2);

    secureClearPassFields();

    // C++20: Use references to avoid copies
    const auto& [oldpass, newpass1, newpass2] = std::tie(
        passphrases.oldpass, passphrases.newpass1, passphrases.newpass2);

    switch(mode) {
    case Encrypt: {
        if (newpass1.empty() || newpass2.empty()) {
            // Cannot encrypt with empty passphrase
            break;
        }
        // C++20: Structured message composition
        const auto warningMessage = tr("Warning: If you encrypt your wallet and lose your passphrase, you will <b>LOSE ALL OF YOUR GOLDCOINS</b>!")
                                  + "<br><br>"
                                  + tr("Are you sure you wish to encrypt your wallet?");
        
        const auto retval = QMessageBox::question(this, 
            tr("Confirm wallet encryption"),
            warningMessage,
            QMessageBox::Yes | QMessageBox::Cancel,
            QMessageBox::Cancel);
        if(retval == QMessageBox::Yes)
        {
            if(newpass1 == newpass2)
            {
                if(model->setWalletEncrypted(true, newpass1))
                {
                    QMessageBox::warning(this, tr("Wallet encrypted"),
                                         "<qt>" +
                                         tr("%1 will close now to finish the encryption process. "
                                         "Remember that encrypting your wallet cannot fully protect "
                                         "your goldcoins from being stolen by malware infecting your computer.").arg(tr(PACKAGE_NAME)) +
                                         "<br><br><b>" +
                                         tr("IMPORTANT: Any previous backups you have made of your wallet file "
                                         "should be replaced with the newly generated, encrypted wallet file. "
                                         "For security reasons, previous backups of the unencrypted wallet file "
                                         "will become useless as soon as you start using the new, encrypted wallet.") +
                                         "</b></qt>");
                    QApplication::quit();
                }
                else
                {
                    QMessageBox::critical(this, tr("Wallet encryption failed"),
                                         tr("Wallet encryption failed due to an internal error. Your wallet was not encrypted."));
                }
                QDialog::accept(); // Success
            }
            else
            {
                QMessageBox::critical(this, tr("Wallet encryption failed"),
                                     tr("The supplied passphrases do not match."));
            }
        }
        else
        {
            QDialog::reject(); // Cancelled
        }
        } break;
    case Unlock:
        if(!model->setWalletLocked(false, oldpass))
        {
            QMessageBox::critical(this, tr("Wallet unlock failed"),
                                  tr("The passphrase entered for the wallet decryption was incorrect."));
        }
        else
        {
            QDialog::accept(); // Success
        }
        break;
    case Decrypt:
        if(!model->setWalletEncrypted(false, oldpass))
        {
            QMessageBox::critical(this, tr("Wallet decryption failed"),
                                  tr("The passphrase entered for the wallet decryption was incorrect."));
        }
        else
        {
            QDialog::accept(); // Success
        }
        break;
    case ChangePass:
        if(newpass1 == newpass2)
        {
            if(model->changePassphrase(oldpass, newpass1))
            {
                QMessageBox::information(this, tr("Wallet encrypted"),
                                     tr("Wallet passphrase was successfully changed."));
                QDialog::accept(); // Success
            }
            else
            {
                QMessageBox::critical(this, tr("Wallet encryption failed"),
                                     tr("The passphrase entered for the wallet decryption was incorrect."));
            }
        }
        else
        {
            QMessageBox::critical(this, tr("Wallet encryption failed"),
                                 tr("The supplied passphrases do not match."));
        }
        break;
    }
}

void AskPassphraseDialog::textChanged()
{
    // C++20: Use lambda for validation logic
    const auto isFieldValid = [](QLineEdit* field) -> bool {
        return !field->text().isEmpty();
    };
    
    // C++20: Pattern matching style validation
    const bool acceptable = [this, &isFieldValid]() -> bool {
        switch(mode) {
        case Encrypt: // New passphrase x2
            return isFieldValid(ui->passEdit2) && isFieldValid(ui->passEdit3);
        case Unlock: // Old passphrase x1
        case Decrypt:
            return isFieldValid(ui->passEdit1);
        case ChangePass: // Old passphrase x1, new passphrase x2
            return isFieldValid(ui->passEdit1) && 
                   isFieldValid(ui->passEdit2) && 
                   isFieldValid(ui->passEdit3);
        default:
            return false;
        }
    }();
    
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(acceptable);
}

bool AskPassphraseDialog::event(QEvent *event)
{
    // C++20: Early return pattern
    if (event->type() != QEvent::KeyPress) {
        return QWidget::event(event);
    }
    
    // C++20: Use auto for cast
    if (const auto* ke = static_cast<QKeyEvent*>(event); 
        ke->key() == Qt::Key_CapsLock) {
        fCapsLock = !fCapsLock;
        
        // Update caps lock warning
        ui->capsLabel->setText(fCapsLock 
            ? tr("Warning: The Caps Lock key is on!") 
            : QString());
    }
    
    return QWidget::event(event);
}

bool AskPassphraseDialog::eventFilter(QObject *object, QEvent *event)
{
    // C++20: Early return for non-key events
    if (event->type() != QEvent::KeyPress) {
        return QDialog::eventFilter(object, event);
    }
    
    const auto* ke = static_cast<QKeyEvent*>(event);
    const auto str = ke->text();
    
    if (str.isEmpty()) {
        return QDialog::eventFilter(object, event);
    }
    
    // C++20: Structured detection logic
    const QChar ch = str[0];
    if (!ch.isLetter()) {
        return QDialog::eventFilter(object, event);
    }
    
    const bool hasShift = (ke->modifiers() & Qt::ShiftModifier) != 0;
    const bool isLowerCase = ch >= 'a' && ch <= 'z';
    const bool isUpperCase = ch >= 'A' && ch <= 'Z';
    
    // Detect caps lock mismatch
    const bool capsLockDetected = (hasShift && isLowerCase) || (!hasShift && isUpperCase);
    
    if (fCapsLock != capsLockDetected) {
        fCapsLock = capsLockDetected;
        ui->capsLabel->setText(fCapsLock 
            ? tr("Warning: The Caps Lock key is on!") 
            : QString());
    }
    
    return QDialog::eventFilter(object, event);
}

// C++20: Use inline for header-compatible function
inline void SecureClearQLineEdit(QLineEdit* edit)
{
    // Attempt to overwrite text so that they do not linger around in memory
    const auto textSize = edit->text().size();
    edit->setText(QString(" ").repeated(textSize));
    edit->clear();
}

void AskPassphraseDialog::secureClearPassFields()
{
    // C++20: Use array for iteration
    const std::array<QLineEdit*, 3> fields = {
        ui->passEdit1, ui->passEdit2, ui->passEdit3
    };
    
    for (auto* field : fields) {
        SecureClearQLineEdit(field);
    }
}
