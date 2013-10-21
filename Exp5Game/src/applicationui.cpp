#include <QDebug>

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/LocaleHandler>

#include <bb/device/DisplayInfo>

#include "applicationui.hpp"

using namespace bb::cascades;

ApplicationUI::ApplicationUI(bb::cascades::Application *app) :
		QObject(app)
{
	_donScore = 0, _lucaScore = 0, _maxScore = 100, _hasPlayerWon = false, _hasGameStarted = false;

	bb::device::DisplayInfo display;
	_displayWidth = display.pixelSize().width();
	_displayHeight = display.pixelSize().height();

	// Create scene document from main.qml asset, the parent is set
	// to ensure the document gets destroyed properly at shut down.
	_qml = QmlDocument::create("asset:///main.qml").parent(this);

	_qml->setContextProperty("mainApp", this);

	// Create root object for the UI
	AbstractPane *root = _qml->createRootObject<AbstractPane>();

	//Connect the mainContainer to an object in the native code
	QObject *obj = root->findChild<QObject*>((const QString) "mainContainer");
	QObject::connect( this, SIGNAL(scoreUpdated(const QVariant &, const QVariant &)),
			obj, SLOT(scoreUpdate(const QVariant &, const QVariant &)));
	QObject::connect( this, SIGNAL(bestPlayerUpdated(const QVariant &)),
			obj, SLOT(updatePlayer(const QVariant &)));

	// Set created root object as the application scene
	app->setScene(root);
}

ApplicationUI::~ApplicationUI()
{

}

void ApplicationUI::ConnectionEstablished(QString requestType, QString player)
{
	QString request = requestType.mid(1);
	if (request.contains("don", Qt::CaseInsensitive))
	{
		_donScore++;
		emit scoreUpdated(_donScore, 0);
	}
	else if (request.contains("luca", Qt::CaseInsensitive))
	{
		_lucaScore++;
		emit scoreUpdated(_lucaScore, 1);
	}
	if (_donScore > _maxScore - 1 || _lucaScore > _maxScore - 1)
	{
		if (player.size() > 1 && !_hasPlayerWon)
		{
			_hasPlayerWon = true;
			emit bestPlayerUpdated(player);
		}
	}
}

QString ApplicationUI::getIPAddress()
{
	QString _IPv4 = "none";
	foreach (const QNetworkInterface &interface, QNetworkInterface::allInterfaces())
	{
		qDebug() << "humanReadableName: " << interface.humanReadableName();
		if(QString::compare(interface.humanReadableName(), "tiw_sta0") == 0)
		{
			foreach (const QNetworkAddressEntry &entry, interface.addressEntries())
			{
				if (entry.prefixLength() <= 32)
				{
					qDebug() << "IPv4: " << entry.ip().toString();
					return entry.ip().toString();
				}
			}
		}
	}
	return _IPv4;
}

bool ApplicationUI::startGame()
{
	_hasGameStarted = true;
	return _hasGameStarted;
}

int ApplicationUI::getDisplayWidth()
{
    qDebug() << "[ApplicationUI] getDisplayWidth returns " << _displayWidth;
    return _displayWidth;
}

int ApplicationUI::getDisplayHeight()
{
    qDebug() << "[ApplicationUI] getDisplayHeight returns " << _displayHeight;
    return _displayHeight;
}

