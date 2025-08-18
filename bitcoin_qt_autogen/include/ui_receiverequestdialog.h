/********************************************************************************
** Form generated from reading UI file 'receiverequestdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RECEIVEREQUESTDIALOG_H
#define UI_RECEIVEREQUESTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include "receiverequestdialog.h"

QT_BEGIN_NAMESPACE

class Ui_ReceiveRequestDialog
{
public:
    QVBoxLayout *verticalLayout_3;
    QRImageWidget *lblQRCode;
    QTextEdit *outUri;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnCopyURI;
    QPushButton *btnCopyAddress;
    QPushButton *btnSaveAs;
    QSpacerItem *horizontalSpacer_2;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ReceiveRequestDialog)
    {
        if (ReceiveRequestDialog->objectName().isEmpty())
            ReceiveRequestDialog->setObjectName("ReceiveRequestDialog");
        ReceiveRequestDialog->resize(487, 597);
        verticalLayout_3 = new QVBoxLayout(ReceiveRequestDialog);
        verticalLayout_3->setObjectName("verticalLayout_3");
        lblQRCode = new QRImageWidget(ReceiveRequestDialog);
        lblQRCode->setObjectName("lblQRCode");
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lblQRCode->sizePolicy().hasHeightForWidth());
        lblQRCode->setSizePolicy(sizePolicy);
        lblQRCode->setMinimumSize(QSize(300, 320));
        lblQRCode->setTextFormat(Qt::PlainText);
        lblQRCode->setAlignment(Qt::AlignCenter);
        lblQRCode->setWordWrap(true);

        verticalLayout_3->addWidget(lblQRCode);

        outUri = new QTextEdit(ReceiveRequestDialog);
        outUri->setObjectName("outUri");
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(outUri->sizePolicy().hasHeightForWidth());
        outUri->setSizePolicy(sizePolicy1);
        outUri->setMinimumSize(QSize(0, 50));
        outUri->setFrameShape(QFrame::NoFrame);
        outUri->setFrameShadow(QFrame::Plain);
        outUri->setTabChangesFocus(true);
        outUri->setTextInteractionFlags(Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);

        verticalLayout_3->addWidget(outUri);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        btnCopyURI = new QPushButton(ReceiveRequestDialog);
        btnCopyURI->setObjectName("btnCopyURI");
        btnCopyURI->setAutoDefault(false);

        horizontalLayout->addWidget(btnCopyURI);

        btnCopyAddress = new QPushButton(ReceiveRequestDialog);
        btnCopyAddress->setObjectName("btnCopyAddress");
        btnCopyAddress->setAutoDefault(false);

        horizontalLayout->addWidget(btnCopyAddress);

        btnSaveAs = new QPushButton(ReceiveRequestDialog);
        btnSaveAs->setObjectName("btnSaveAs");
        btnSaveAs->setAutoDefault(false);

        horizontalLayout->addWidget(btnSaveAs);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        buttonBox = new QDialogButtonBox(ReceiveRequestDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setStandardButtons(QDialogButtonBox::Close);

        horizontalLayout->addWidget(buttonBox);


        verticalLayout_3->addLayout(horizontalLayout);


        retranslateUi(ReceiveRequestDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, ReceiveRequestDialog, qOverload<>(&QDialog::reject));
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, ReceiveRequestDialog, qOverload<>(&QDialog::accept));

        QMetaObject::connectSlotsByName(ReceiveRequestDialog);
    } // setupUi

    void retranslateUi(QDialog *ReceiveRequestDialog)
    {
#if QT_CONFIG(tooltip)
        lblQRCode->setToolTip(QCoreApplication::translate("ReceiveRequestDialog", "QR Code", nullptr));
#endif // QT_CONFIG(tooltip)
        btnCopyURI->setText(QCoreApplication::translate("ReceiveRequestDialog", "Copy &URI", nullptr));
        btnCopyAddress->setText(QCoreApplication::translate("ReceiveRequestDialog", "Copy &Address", nullptr));
        btnSaveAs->setText(QCoreApplication::translate("ReceiveRequestDialog", "&Save Image...", nullptr));
        (void)ReceiveRequestDialog;
    } // retranslateUi

};

namespace Ui {
    class ReceiveRequestDialog: public Ui_ReceiveRequestDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RECEIVEREQUESTDIALOG_H
