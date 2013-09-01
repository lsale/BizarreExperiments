
#include <QDebug>

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/LocaleHandler>

#include "applicationui.hpp"

using namespace bb::cascades;

ApplicationUI::ApplicationUI(bb::cascades::Application *app) :
        QObject(app)
{
	_donScore = 0, _lucaScore = 0, _maxScore = 30;
    // Create scene document from main.qml asset, the parent is set
    // to ensure the document gets destroyed properly at shut down.
	_qml = QmlDocument::create("asset:///main.qml").parent(this);

	_qml->setContextProperty("mainApp", this);

    // Create root object for the UI
    AbstractPane *root = _qml->createRootObject<AbstractPane>();

    //Connect the mainContainer to an object in the native code
    QObject *obj = root->findChild<QObject*>((const QString) "mainContainer");
    QObject::connect(  this,   SIGNAL(scoreUpdated(const QVariant &, const QVariant &)),
    		obj,     SLOT(scoreUpdate(const QVariant &, const QVariant &)));
    QObject::connect(  this,   SIGNAL(bestPlayerUpdated(const QVariant &)),
        		obj,     SLOT(updatePlayer(const QVariant &)));

    // Set created root object as the application scene
     app->setScene(root);
}

ApplicationUI::~ApplicationUI(){

}

void ApplicationUI::ConnectionEstablished(QString requestType, QString player){
	QString request = requestType.mid(1);
	if(request.contains("left", Qt::CaseInsensitive)){
		_donScore++;
		emit scoreUpdated(_donScore, 0);
	}else if(request.contains("right", Qt::CaseInsensitive)){
		_lucaScore++;
		emit scoreUpdated(_lucaScore, 1);
	}
	if(_donScore > _maxScore || _lucaScore > _maxScore){
		if(player.size() > 1)
			emit bestPlayerUpdated(player);
	}
}
