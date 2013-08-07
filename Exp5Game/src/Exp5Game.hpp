#ifndef Exp5Game_HPP_
#define Exp5Game_HPP_

#include <QObject>
#include <bb/cascades/ImageView>
#include <bb/cascades/Label>

namespace bb {
namespace cascades {
class Application;
}
}

/*!
 * @brief Application pane object
 *
 *Use this object to create and init app UI, to create context objects, to register the new meta types etc.
 */
class Exp5Game: public QObject {
Q_OBJECT
public:
	Exp5Game(bb::cascades::Application *app);
	virtual ~Exp5Game() {
	}

Q_SIGNALS:

	void ropePositionUpdated(int newPosition);

public slots:

	void tugA();
	void tugB();

private:

	int ropePosition, teamAPullCount, teamBPullCount;
	bb::cascades::ImageView *ropeImage;
	bb::cascades::Label *teamAPullCountLabel, *teamBPullCountLabel;

	void tug(bool isFromTeamA);
};

#endif /* Exp5Game_HPP_ */
