#ifndef GAMESERVER_HPP_
#define GAMESERVER_HPP_

#include <QObject>
#include <QtNetwork/QTcpServer>

class GameServer: public QTcpServer {
Q_OBJECT
public:
	GameServer(QObject *parent, int port);
	virtual ~GameServer();
	void start();

Q_SIGNALS:
	void tugReceived();

protected:
	void incomingConnection(int socketDescriptor);

private:
	int listenPort;

private slots:
	void handleRequest();

	//void discardClient();

};

#endif /* GAMESERVER_HPP_ */
