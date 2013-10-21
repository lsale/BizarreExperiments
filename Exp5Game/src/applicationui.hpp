#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QObject>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>


namespace bb
{
    namespace cascades
    {
        class Application;
    }
}

class ApplicationUI : public QObject
{
    Q_OBJECT
public:

    //Public member variables
    int _donScore;
	int _lucaScore;
	int _maxScore;
	bool _hasGameStarted;

	//Public functions
    ApplicationUI(bb::cascades::Application *app);
    virtual ~ApplicationUI();
    void ConnectionEstablished(QString requestType, QString player);
    void setWinnerFlag(bool flagStatus);
    Q_INVOKABLE QString getIPAddress();
    Q_INVOKABLE bool startGame();
    Q_INVOKABLE int getDisplayWidth();
    Q_INVOKABLE int getDisplayHeight();


signals:
	void scoreUpdated(const QVariant score, const QVariant player);
	void bestPlayerUpdated(const QVariant player);

private:
    bb::cascades::QmlDocument *_qml;
    bool _hasPlayerWon;
    int _displayWidth, _displayHeight;
};

#endif /* ApplicationUI_HPP_ */
