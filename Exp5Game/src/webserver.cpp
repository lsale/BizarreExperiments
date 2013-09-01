

#include <QDebug>
#include "webserver.hpp"

static const QByteArray someHeaderToken("Some-Header");
static const QString someParam("Some-Param");

WebServer::WebServer(ApplicationUI* parent) : QObject(){
	m_pMyApp = parent;
	_maxScore = m_pMyApp->_maxScore;
	m_pServer = new Pillow::HttpServer(QHostAddress(QHostAddress::Any), 4567);
	if (!m_pServer->isListening())
		exit(1);
	qDebug() << "Ready";
	n = 0;
	m_pPlayers = new QMap<QByteArray, int>;
	QObject::connect(m_pServer, SIGNAL(requestReady(Pillow::HttpConnection*)), this, SLOT(handleRequest(Pillow::HttpConnection*)));
}

WebServer::~WebServer(){
	delete m_pServer;
	delete m_pPlayers;
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
	AppendAddress(helloWorldToken);
	connection->writeResponse(200, Pillow::HttpHeaderCollection(), helloWorldToken);
	return true;
}

void WebServer::AppendAddress(QByteArray address){
	int _tempNum = 1;
	if(!m_pPlayers->contains(address)){
		m_pPlayers->insert(address, _tempNum);
	}else{
		_tempNum = m_pPlayers->value(address);
		if(_tempNum < _maxScore){
			_tempNum++;
			m_pPlayers->insert(address, _tempNum);
		}else{
			m_pMyApp->ConnectionEstablished("", address);
		}
	}
}
