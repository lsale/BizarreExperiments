

#include <QDebug>
#include "webserver.hpp"

static const QByteArray someHeaderToken("some-header");
static const QString someParam("Some-Param");

WebServer::WebServer(ApplicationUI* parent) : QObject(){
	m_pMyApp = parent;
	_maxScore = m_pMyApp->_maxScore;
	m_pServer = new Pillow::HttpServer(QHostAddress(QHostAddress::Any), 1337);
	if (!m_pServer->isListening())
		exit(1);
	qDebug() << "Ready";
	n = 0;
	QMap<QByteArray, int>(mPlayers);
	QObject::connect(m_pServer, SIGNAL(requestReady(Pillow::HttpConnection*)), this, SLOT(handleRequest(Pillow::HttpConnection*)));
}

WebServer::~WebServer(){
	delete m_pServer;
}

bool WebServer::handleRequest(Pillow::HttpConnection *connection){
	qDebug() << "[WebServer] ServerListener - Got Connection";
	m_pMyApp->ConnectionEstablished(connection->requestPathDecoded(), "");
	n += connection->requestContent().size();
	n += connection->requestFragment().size();
	n += connection->requestHeaderValue(someHeaderToken).size();
	n += connection->requestMethod().size();
	n += connection->requestPath().size();
	n += connection->requestQueryString().size();
	n += connection->requestUri().size();
	n += connection->requestUriDecoded().size();
	n += connection->requestFragmentDecoded().size();
	n += connection->requestPathDecoded().size();
	n += connection->requestQueryStringDecoded().size();
	n += connection->requestParamValue(someParam).size();
	helloWorldToken = connection->remoteAddress().toString().toLatin1();
	QByteArray _response = "<html><head></head><body><font size='14'> Player: "+helloWorldToken+"<br><a href='javascript:location.reload()'>Play again</a></font></body></html>";
	AppendAddress(helloWorldToken);
	connection->writeResponse(200, Pillow::HttpHeaderCollection() << Pillow::HttpHeader("Content-Type", "text/html"), _response);
	return true;
}

void WebServer::AppendAddress(QByteArray address){
	if(m_pMyApp->_hasGameStarted){
		qDebug() << "Adding to memory";
		int _tempNum = 1;
		if(!mPlayers.contains(address)){
			mPlayers.insert(address, _tempNum);
		}else{
			_tempNum = mPlayers.value(address);
			if(_tempNum < _maxScore-1){
				_tempNum++;
				mPlayers.insert(address, _tempNum);
			}else{
				qDebug()<< "Sending player name";
				m_pMyApp->ConnectionEstablished("", getHighestScorer());
			}
		}
	}
}

QString WebServer::getHighestScorer(){
	QString _player;
	int _highestScore = 0;
	QMap<QByteArray, int>::const_iterator i = mPlayers.constBegin();
	while (i != mPlayers.constEnd()) {
		if(i.value() > _highestScore){
			_highestScore = i.value();
			_player = i.key();
		}
	    ++i;
	}
	return _player;
}
