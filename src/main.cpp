
#include <QApplication>
#include <QSplashScreen>

#include <Cg/cg.h>

#include "Scene.h"

#include <ImfThreading.h>
#include <QThread>
#include <QDir>

QSplashScreen* splash = NULL;

void cg_error_handler (CGcontext, CGerror error, void* appdata)
{
    qFatal("%s", cgGetErrorString(error));
}

int main (int argc, char** argv)
{
    QApplication app (argc, argv);

    QDir::addSearchPath("media", "../trunk/media");

    Imf::setGlobalThreadCount(QThread::idealThreadCount());

    cgSetErrorHandler(cg_error_handler, NULL);

    QPixmap splash_pixmap (":media/textures/flame.jpg");
    splash = new QSplashScreen(splash_pixmap);

    splash->show();
    app.processEvents();

    Scene scene;
    scene.show();

    splash->finish(&scene);

    return app.exec();
}
