#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QObject>

#include "DroneController.h"
#include "GamePadListener.h"

namespace bb
{
    namespace cascades
    {
        class Application;

    }
}

/*!
 * @brief Application object
 *
 *
 */

class ApplicationUI : public QObject
{
    Q_OBJECT
public:
    ApplicationUI(bb::cascades::Application *app);
    virtual ~ApplicationUI();
private slots:
private:

	DroneController* m_pDroneController;
	GamePadListener* m_pGamePadListener;
};

#endif /* ApplicationUI_HPP_ */
