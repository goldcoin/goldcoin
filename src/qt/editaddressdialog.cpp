// Copyright (c) 2011-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "editaddressdialog.h"
#include "ui_editaddressdialog.h"

#include "addresstablemodel.h"
#include "guiutil.h"

#include <QDataWidgetMapper>
#include <QMessageBox>

#include <optional>
#include <string_view>

// C++20: Concept for address validation
template<typename T>
concept AddressModel = requires(T* model) {
    { model->addRow(std::declval<AddressTableModel::Mode>(), 
                    std::declval<QString>(), 
                    std::declval<QString>()) } -> std::convertible_to<QString>;
    { model->getEditStatus() } -> std::convertible_to<AddressTableModel::EditStatus>;
};

EditAddressDialog::EditAddressDialog(Mode _mode, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditAddressDialog),
    mapper(nullptr),
    mode(_mode),
    model(nullptr)
{
    ui->setupUi(this);

    GUIUtil::setupAddressWidget(ui->addressEdit, this);

    // C++20: Structured dialog setup
    const auto setupDialog = [this](std::string_view title, bool enableAddress) {
        setWindowTitle(tr(title.data()));
        ui->addressEdit->setEnabled(enableAddress);
    };

    switch(mode) {
    case NewReceivingAddress:
        setupDialog("New receiving address", false);
        break;
    case NewSendingAddress:
        setupDialog("New sending address", true);
        break;
    case EditReceivingAddress:
        setupDialog("Edit receiving address", false);
        break;
    case EditSendingAddress:
        setupDialog("Edit sending address", true);
        break;
    }

    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
}

EditAddressDialog::~EditAddressDialog()
{
    delete ui;
}

void EditAddressDialog::setModel(AddressTableModel *_model)
{
    this->model = _model;
    if (!_model) return;

    // Configure data mapper
    mapper->setModel(_model);
    mapper->addMapping(ui->labelEdit, AddressTableModel::Label);
    mapper->addMapping(ui->addressEdit, AddressTableModel::Address);
}

void EditAddressDialog::loadRow(int row)
{
    mapper->setCurrentIndex(row);
}

bool EditAddressDialog::saveCurrentRow()
{
    if (!model) return false;

    // C++20: Use optional for cleaner result handling
    std::optional<QString> result;

    switch(mode) {
    case NewReceivingAddress:
    case NewSendingAddress: {
        // C++20: Ternary for mode selection
        const auto addressMode = (mode == NewSendingAddress) 
            ? AddressTableModel::Send 
            : AddressTableModel::Receive;
        
        result = model->addRow(addressMode, 
                              ui->labelEdit->text(), 
                              ui->addressEdit->text());
        break;
    }
    case EditReceivingAddress:
    case EditSendingAddress:
        if (mapper->submit()) {
            result = ui->addressEdit->text();
        }
        break;
    }

    if (result.has_value()) {
        address = result.value();
    }
    return !address.isEmpty();
}

void EditAddressDialog::accept()
{
    if (!model) return;

    if (saveCurrentRow()) {
        QDialog::accept();
        return;
    }

    // C++20: Lambda for error display
    const auto showError = [this](const QString& message, bool critical = false) {
        if (critical) {
            QMessageBox::critical(this, windowTitle(), message, 
                                QMessageBox::Ok, QMessageBox::Ok);
        } else {
            QMessageBox::warning(this, windowTitle(), message, 
                               QMessageBox::Ok, QMessageBox::Ok);
        }
    };

    // C++20: Use structured bindings for cleaner code
    const auto address = ui->addressEdit->text();
    
    switch(model->getEditStatus()) {
    case AddressTableModel::OK:
    case AddressTableModel::NO_CHANGES:
        // Silent failure - just reject
        break;
        
    case AddressTableModel::INVALID_ADDRESS:
        showError(tr("The entered address \"%1\" is not a valid Goldcoin address.")
                 .arg(address));
        break;
        
    case AddressTableModel::DUPLICATE_ADDRESS:
        showError(tr("The entered address \"%1\" is already in the address book.")
                 .arg(address));
        break;
        
    case AddressTableModel::WALLET_UNLOCK_FAILURE:
        showError(tr("Could not unlock wallet."), true);
        break;
        
    case AddressTableModel::KEY_GENERATION_FAILURE:
        showError(tr("New key generation failed."), true);
        break;
    }
}

QString EditAddressDialog::getAddress() const
{
    return address;
}

void EditAddressDialog::setAddress(const QString &_address)
{
    this->address = _address;
    ui->addressEdit->setText(_address);
}
