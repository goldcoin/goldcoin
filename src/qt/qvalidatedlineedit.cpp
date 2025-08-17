// Copyright (c) 2011-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "qvalidatedlineedit.h"

#include "bitcoinaddressvalidator.h"
#include "guiconstants.h"

#include <concepts>
#include <type_traits>

// C++20: Concept for validator types
template<typename T>
concept ValidatorType = requires(T* v, QString& str, int& pos) {
    { v->validate(str, pos) } -> std::convertible_to<QValidator::State>;
};

QValidatedLineEdit::QValidatedLineEdit(QWidget *parent) :
    QLineEdit(parent),
    valid(true),
    checkValidator(nullptr)
{
    // Qt 6.9: Modern signal connection
    connect(this, &QValidatedLineEdit::textChanged, this, &QValidatedLineEdit::markValid);
}

void QValidatedLineEdit::setValid(bool _valid)
{
    if(_valid == this->valid)
    {
        return;
    }

    if(_valid)
    {
        setStyleSheet("");
    }
    else
    {
        setStyleSheet(STYLE_INVALID);
    }
    this->valid = _valid;
}

void QValidatedLineEdit::focusInEvent(QFocusEvent *evt)
{
    // Clear invalid flag on focus
    setValid(true);

    QLineEdit::focusInEvent(evt);
}

void QValidatedLineEdit::focusOutEvent(QFocusEvent *evt)
{
    checkValidity();

    QLineEdit::focusOutEvent(evt);
}

void QValidatedLineEdit::markValid()
{
    // As long as a user is typing ensure we display state as valid
    setValid(true);
}

void QValidatedLineEdit::clear()
{
    setValid(true);
    QLineEdit::clear();
}

void QValidatedLineEdit::setEnabled(bool enabled)
{
    if (!enabled)
    {
        // A disabled QValidatedLineEdit should be marked valid
        setValid(true);
    }
    else
    {
        // Recheck validity when QValidatedLineEdit gets enabled
        checkValidity();
    }

    QLineEdit::setEnabled(enabled);
}

void QValidatedLineEdit::checkValidity()
{
    // C++20: Use structured binding and constexpr if
    auto validateText = [this]() -> bool {
        if (text().isEmpty())
            return true;
            
        if (!hasAcceptableInput())
            return false;
            
        // C++20: Constexpr if for compile-time branching
        if constexpr (requires { checkValidator; }) {
            if (checkValidator) {
                QString address = text();
                int pos = 0;
                return checkValidator->validate(address, pos) == QValidator::Acceptable;
            }
        }
        return true;
    };
    
    setValid(validateText());
    Q_EMIT validationDidChange(this);
}

void QValidatedLineEdit::setCheckValidator(const QValidator *v)
{
    checkValidator = v;
}

bool QValidatedLineEdit::isValid()
{
    // use checkValidator in case the QValidatedLineEdit is disabled
    if (checkValidator)
    {
        QString address = text();
        int pos = 0;
        if (checkValidator->validate(address, pos) == QValidator::Acceptable)
            return true;
    }

    return valid;
}
