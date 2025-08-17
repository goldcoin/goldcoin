// Copyright (c) 2011-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "openuridialog.h"
#include "ui_openuridialog.h"

#include "guiutil.h"
#include "walletmodel.h"

#include <QUrl>
#include <string_view>
#include <optional>

OpenURIDialog::OpenURIDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenURIDialog)
{
    ui->setupUi(this);
    // Qt 6.9: Placeholder text is always supported
    ui->uriEdit->setPlaceholderText("goldcoin:");
}

OpenURIDialog::~OpenURIDialog()
{
    delete ui;
}

QString OpenURIDialog::getURI()
{
    return ui->uriEdit->text();
}

void OpenURIDialog::accept()
{
    // C++20: Early validation pattern
    SendCoinsRecipient rcp;
    
    if (GUIUtil::parseBitcoinURI(getURI(), &rcp)) {
        // Only accept valid URIs
        QDialog::accept();
    } else {
        ui->uriEdit->setValid(false);
    }
}

void OpenURIDialog::on_selectFileButton_clicked()
{
    // C++20: Use nullptr instead of NULL
    const auto filename = GUIUtil::getOpenFileName(
        this, 
        tr("Select payment request file to open"), 
        "", "", nullptr);
    
    if (filename.isEmpty()) return;
    
    // Build URI from selected file
    const QUrl fileUri = QUrl::fromLocalFile(filename);
    ui->uriEdit->setText("goldcoin:?r=" + QUrl::toPercentEncoding(fileUri.toString()));
}
