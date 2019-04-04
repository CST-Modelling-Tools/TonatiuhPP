/*
 * NetworkConnectionsDialog.cpp
 *
 *  Created on: 02/01/2013
 *      Author: amutuberria
 */

#include <QMessageBox>
#include <QProcessEnvironment>
#include <QUrl>

#include "NetworkConnectionsDialog.h"

/*!
 * Creates a dialog to define proxy settings.
 */
NetworkConnectionsDialog::NetworkConnectionsDialog( QWidget * parent, Qt::WindowFlags f )
:QDialog( parent, f)
{
	setupUi( this );
}

/*!
 * Destroys dialog.
 */
NetworkConnectionsDialog::~NetworkConnectionsDialog()
{

}

/*!
 * Returns true if the proxy must be defined to network connections.
 * Otherwise returns false.
 */
bool NetworkConnectionsDialog::IsProxyEnabled() const
{
	if( notProxyRadio->isChecked() )	return false;
	return true;
}

/*!
 * Returns true if the user defines to use system proxy settings.
 * Otherwise, returns false.
 */
bool NetworkConnectionsDialog::IsSystemProxyEnabled() const
{
	return systemProxyRadio->isChecked();
}

/*!
 * Returns the host name for the http proxy.
 * If the proxy is not enabled returns empty name.
 */
QString NetworkConnectionsDialog::GetHostName() const
{
	if( systemProxyRadio->isChecked() )
	{
	    QString urlEnv = QProcessEnvironment::systemEnvironment().value("http_proxy");
	     if (!urlEnv.isEmpty() )
	     {
	          QUrl url = QUrl(urlEnv, QUrl::TolerantMode);
	          return url.host();
	     }
	 	void SetProxyDisabled();
	 	void SetManualProxyConfiguration();
	 	void SetProxyHttpHost( QString name );
	 	void SetProxyHttpPort( int port );
	 	void SetSystemProxyConfiguration();
	}
	else if( manualProxyRadio->isChecked() )
	{
		return httpLine->text();
	}
	return QString();

}

/*!
 * Returns the port number for the http proxy.
 * Returns -1 if the port is unspecified.
 */
int NetworkConnectionsDialog::GetPort() const
{
	if( systemProxyRadio->isChecked() )
	{
		QString urlEnv = QProcessEnvironment::systemEnvironment().value("http_proxy");
		if (!urlEnv.isEmpty())
		{
			QUrl url = QUrl(urlEnv, QUrl::TolerantMode);
			return url.port();
		}
	}
	else if( manualProxyRadio->isChecked() )
	{
		return portSpin->value();
	}
	return -1;
}

/*!
 * Sets the proxy disabled for network connections.
 */
void NetworkConnectionsDialog::SetProxyDisabled()
{
	notProxyRadio->setChecked( true );
}

/*!
 * Sets the manual proxy configuration enabled for network connections.
 */
void NetworkConnectionsDialog::SetManualProxyConfiguration()
{

	manualProxyRadio->setChecked( true );
}

/*
 * Sets the host name for the proxy to \a name.
 */
void NetworkConnectionsDialog::SetProxyHttpHost( QString name )
{
	httpLine->setText( name );
}

/*!
 * Sets the port value for the proxy to \a port.
 */
void NetworkConnectionsDialog::SetProxyHttpPort( int port )
{
	portSpin->setValue( port );
}

/*!
 * Sets the system proxy configuration enabled for network connections.
 */
void NetworkConnectionsDialog::SetSystemProxyConfiguration()
{
	systemProxyRadio->setChecked( true );
}

/*!
 * Verifies that the defined configuration is a valid configuration closes the dialog.
 */
void NetworkConnectionsDialog::accept()
{
	if( manualProxyRadio->isChecked() )
	{
		QUrl httpHostURL( httpLine->text() );
		if( !httpHostURL.isValid() || portSpin->value() < 0 )
		{
			QString message( tr( "Defined proxy configuration is not valid." ) );
			QMessageBox::warning( this, QLatin1String( "Tonatiuh" ),
					message );
			return;
		}

	}
	QDialog::accept();
}
