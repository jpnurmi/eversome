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
#include <QTimer>

Account::Account(QObject* parent) : QObject(parent), m_account(0), m_proxy(0)
{
}

Account::~Account()
{
}

int Account::credentialsId() const
{
    return m_account ? m_account->credentialsId() : -1;
}

bool Account::init()
{
    foreach (Accounts::AccountId id, m_manager.accountListEnabled())
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
    if (!m_proxy) {
        m_proxy = new AccountsUI::ProviderPluginProxy(this);
        connect(m_proxy, SIGNAL(failed()), this, SIGNAL(failed()));
        connect(m_proxy, SIGNAL(cancelled()), this, SIGNAL(cancelled()));
        connect(m_proxy, SIGNAL(created(int)), this, SLOT(onAccountCreated(int)));

        // ProviderPluginProxy does not emit any signal if and when
        // the accounts UI is closed without creating an account...
        QTimer* timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
        timer->start(2000);
    }
    m_proxy->createAccount(m_manager.provider("evernote"), QString());
}

void Account::onTimeout()
{
    if (!m_proxy)
        sender()->deleteLater();
    else if (!m_proxy->isPluginRunning())
        emit cancelled();
}

void Account::onAccountCreated(int accountId)
{
    m_account = m_manager.account(accountId);
    if (m_account)
        emit created(m_account->credentialsId());
    m_proxy->deleteLater();
    m_proxy = 0;
}
