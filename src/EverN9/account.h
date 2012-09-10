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
#ifndef _ACCOUNT_H_
#define _ACCOUNT_H_

#include <QObject>
#include <Accounts/Account>
#include <Accounts/Manager>
#include <AccountsUI/ProviderPluginProxy>

class Account : public QObject
{
    Q_OBJECT

public:
    explicit Account(QObject* parent = 0);
    virtual ~Account();

    int credentialsId() const;

public slots:
    bool init();
    void create();

signals:
    void failed();
    void cancelled();
    void created(int credentialsId);

private slots:
    void onAccountCreated(int accountId);

private:
    Accounts::Manager m_manager;
    Accounts::Account* m_account;
    AccountsUI::ProviderPluginProxy* m_proxy;
};

#endif // _ACCOUNT_H_
