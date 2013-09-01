
#ifndef WEBSERVER_HPP_
#define WEBSERVER_HPP_

#include <QObject>
#include <QMap>
#include <HttpServer.h>
#include <HttpHandler.h>
#include <HttpConnection.h>

#include "applicationui.hpp"

class WebServer : public QObject {
	Q_OBJECT
public:
	WebServer(ApplicationUI* parent);
	virtual ~WebServer();
	void AppendAddress(QByteArray address);

	QMap<QByteArray, int>* m_pPlayers;

private:
	Pillow::HttpServer* m_pServer;
	qint64 n;
	ApplicationUI* m_pMyApp;
	QByteArray helloWorldToken;
	int _maxScore;

public slots:
	bool handleRequest(Pillow::HttpConnection *connection);
};


#endif /* WEBSERVER_HPP_ */
