#pragma once

#include <QDialog>

#include "ui_NetworkConnectionsDialog.h"

class NetworkConnectionsDialog: public QDialog, private Ui::NetworkConnectionsDialog
{
    Q_OBJECT

public:
    NetworkConnectionsDialog(QWidget * parent = 0, Qt::WindowFlags f = Qt::WindowFlags());
    ~NetworkConnectionsDialog();

    bool IsProxyEnabled() const;
    bool IsSystemProxyEnabled() const;
    QString GetHostName() const;
    int GetPort() const;

    void SetProxyDisabled();
    void SetManualProxyConfiguration();
    void SetProxyHttpHost(QString name);
    void SetProxyHttpPort(int port);
    void SetSystemProxyConfiguration();

public slots:
    void accept();

};
