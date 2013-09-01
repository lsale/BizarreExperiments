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
    ApplicationUI(bb::cascades::Application *app);
    virtual ~ApplicationUI();
    void ConnectionEstablished(QString requestType, QString player);
    int _donScore;
    int _lucaScore;
    int _maxScore;

signals:
	void scoreUpdated(const QVariant score, const QVariant player);
	void bestPlayerUpdated(const QVariant player);

private:
    bb::cascades::QmlDocument *_qml;
};

#endif /* ApplicationUI_HPP_ */
