/********************************************************************************
** Form generated from reading UI file 'openuridialog.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPENURIDIALOG_H
#define UI_OPENURIDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include "qvalidatedlineedit.h"

QT_BEGIN_NAMESPACE

class Ui_OpenURIDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QValidatedLineEdit *uriEdit;
    QPushButton *selectFileButton;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *OpenURIDialog)
    {
        if (OpenURIDialog->objectName().isEmpty())
            OpenURIDialog->setObjectName("OpenURIDialog");
        OpenURIDialog->resize(564, 109);
        verticalLayout = new QVBoxLayout(OpenURIDialog);
        verticalLayout->setObjectName("verticalLayout");
        label_2 = new QLabel(OpenURIDialog);
        label_2->setObjectName("label_2");

        verticalLayout->addWidget(label_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(OpenURIDialog);
        label->setObjectName("label");

        horizontalLayout->addWidget(label);

        uriEdit = new QValidatedLineEdit(OpenURIDialog);
        uriEdit->setObjectName("uriEdit");

        horizontalLayout->addWidget(uriEdit);

        selectFileButton = new QPushButton(OpenURIDialog);
        selectFileButton->setObjectName("selectFileButton");
        selectFileButton->setText(QString::fromUtf8("\342\200\246"));
        selectFileButton->setAutoDefault(false);

        horizontalLayout->addWidget(selectFileButton);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        buttonBox = new QDialogButtonBox(OpenURIDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(OpenURIDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, OpenURIDialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, OpenURIDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(OpenURIDialog);
    } // setupUi

    void retranslateUi(QDialog *OpenURIDialog)
    {
        OpenURIDialog->setWindowTitle(QCoreApplication::translate("OpenURIDialog", "Open URI", nullptr));
        label_2->setText(QCoreApplication::translate("OpenURIDialog", "Open payment request from URI or file", nullptr));
        label->setText(QCoreApplication::translate("OpenURIDialog", "URI:", nullptr));
#if QT_CONFIG(tooltip)
        selectFileButton->setToolTip(QCoreApplication::translate("OpenURIDialog", "Select payment request file", nullptr));
#endif // QT_CONFIG(tooltip)
    } // retranslateUi

};

namespace Ui {
    class OpenURIDialog: public Ui_OpenURIDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPENURIDIALOG_H
