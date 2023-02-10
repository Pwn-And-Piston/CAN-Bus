#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QDesktopWidget>
#include <QTimer>

int main(int argc, char *argv[]){

    QApplication a(argc, argv);

    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":images/coyote_crop.png"));
    splash->show();

    MainWindow w;
    w.setFixedSize(1200,700);
    w.setWindowIcon(QIcon(":images/coyote_crop.ico"));
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int x = (screenGeometry.width() - w.width()) / 2;
    int y = (screenGeometry.height() - w.height()) / 2;
    w.move(x, y);

    QTimer::singleShot(4000,splash,SLOT(close()));
    QTimer::singleShot(4000,&w,SLOT(show()));
    //w.show();

    return a.exec();
}
