/*
* Copyright (C) 2012 J-P Nurmi <jpnurmi@gmail.com>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*/
//#define QT_NO_DEBUG_OUTPUT

#include "account.h"
#include <QtDebug>

Account::Account(QObject* parent) : QObject(parent), m_account(0)
{
    connect(&m_manager, SIGNAL(accountCreated(Accounts::AccountId)),
                  this, SLOT(onAccountCreated(Accounts::AccountId)));
    connect(&m_manager, SIGNAL(accountRemoved(Accounts::AccountId)),
                  this, SLOT(onAccountRemoved(Accounts::AccountId)));
    connect(&m_manager, SIGNAL(accountUpdated(Accounts::AccountId)),
                  this, SLOT(onAccountUpdated(Accounts::AccountId)));
}

Account::~Account()
{
}

int Account::id() const
{
    return m_account ? m_account->id() : -1;
}

bool Account::init()
{
    foreach (Accounts::AccountId id, m_manager.accountList())
    {
        Accounts::Account* account = m_manager.account(id);
        if (account->providerName() == "evernote") {
            m_account = account;
            break;
        }
    }
    return m_account;
}

void Account::create()
{
    // TODO
    if (!m_account)
        m_account = m_manager.createAccount("evernote");
}

void Account::onAccountCreated(Accounts::AccountId id)
{
    // TODO
    qDebug() << Q_FUNC_INFO << id;
}

void Account::onAccountRemoved(Accounts::AccountId id)
{
    // TODO
    qDebug() << Q_FUNC_INFO << id;
}

void Account::onAccountUpdated(Accounts::AccountId id)
{
    // TODO
    qDebug() << Q_FUNC_INFO << id;
}
