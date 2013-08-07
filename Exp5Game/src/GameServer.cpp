#include "GameServer.hpp"

#include <QtNetwork/QTcpSocket>

GameServer::GameServer(QObject *parent, int listenPort) : QTcpServer(parent), listenPort(listenPort) {

}

void GameServer::start() {

	qDebug() << "Starting the Game Server on port " << listenPort;

	bool isListening = listen(QHostAddress::Any, (quint16) listenPort);

	if (isListening) {
		qDebug() << "Started listening";
	} else {
		qDebug() << "Could not start listening" << errorString();
	}
}

void GameServer::incomingConnection(int socketDescriptor) {

	qDebug() << "Got incoming connection";

	QTcpSocket *tcpSocket = new QTcpSocket(this);
	tcpSocket->setSocketDescriptor(socketDescriptor);
	//close the socket immediately
	tcpSocket->abort();
	delete tcpSocket;

	emit tugReceived();

	/**
	 * Note: the readyRead() (inherited from QIODevice) signal is emitted every
	 * time a character is passed to the socket, this is sub optimal because
	 * we're actually waiting for a full line (the request) before we can
	 * do anything. We could use waitForBytesWritten instead, however, we don't
	 * know ahead of time how long the request string will be so this could be the
	 * only way
	 */
	//connect(s, SIGNAL(readyRead()), this, SLOT(handleRequest()));

	//clean up the socket after it disconnects
	//connect(s, SIGNAL(disconnected()), this, SLOT(discardClient()));
	//
}

void GameServer::handleRequest() {

	QTcpSocket *tcpSocket = (QTcpSocket*) sender();

	QString request;
	QByteArray uri;
	QList<QByteArray> headers;

	//process the request, for reading the headers see lines 2514 and 2533 of mongoose.c
	//The request is comprises strings separated by '\r\n'.
	//The first string is the resource request (e.g. GET /index.html HTTP/1.1)
	//The following strings are headers which give information about the client
	//(e.g. Host: www.ilovehorses.com:9969)

	if (tcpSocket->canReadLine()) {
		request.append(tcpSocket->readLine());
		qDebug() << "Got request: " << request;
	}

	tcpSocket->close();

	if (tcpSocket->state() == QTcpSocket::UnconnectedState) {
		delete tcpSocket;
		qDebug() << "Connection closed";
	}

	emit tugReceived();
}

GameServer::~GameServer() {

}

