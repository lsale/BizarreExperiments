// Default empty project template
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>

#include <QLocale>
#include <QTranslator>
#include <Qt/qdeclarativedebug.h>
#include "Exp5Game.hpp"

using namespace bb::cascades;

void myMessageOutput(QtMsgType type, const char* msg) {

	Q_UNUSED(type);
	fprintf(stdout, "%s\n", msg);
	fflush(stdout);
}

Q_DECL_EXPORT int main(int argc, char **argv)
{

	qInstallMsgHandler(myMessageOutput);
	qDebug() << "Starting Experiment 5 - Let the games commence!!";

	// this is where the server is started etc
    Application app(argc, argv);

    new Exp5Game(&app);

    // we complete the transaction started in the app constructor and start the client event loop here
    return Application::exec();
    // when loop is exited the Application deletes the scene which deletes all its children (per qt rules for children)
}
