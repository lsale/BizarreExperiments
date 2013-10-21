
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
	void updateScore(QByteArray address);
	QString getHighestScorer();

	QMap<QByteArray, int> mPlayers;

private:
	Pillow::HttpServer* m_pServer;
	qint64 n;
	ApplicationUI* m_pMyApp;
	QByteArray playerIpAddress;
	int _maxScore;

public slots:
	bool handleRequest(Pillow::HttpConnection *connection);
};


#endif /* WEBSERVER_HPP_ */
