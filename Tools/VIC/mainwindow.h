#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>
#include <QDebug>
#include <QProcess>

#include "datatext.h"
#include "leftblinkeranimation.h"
#include "rightblinkeranimation.h"
#include "speedanimation.h"
#include "backgrounditem.h"
#include "aboutvic.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void AnimateDataOne();
    void AnimateDataTwo();
    void AnimateDataThree();
    void AnimateDataFour();
    void AnimateDataFive();

    void on_On_Off_One_clicked();
    void on_On_Off_Two_clicked();
    void on_On_Off_Four_clicked();
    void on_On_Off_Three_clicked();
    void on_send_Button_clicked();

    void on_actionVirtual_IC_triggered();

    void on_actionTerminal_triggered();

private:
    Ui::MainWindow *ui;

    QTimer *timerOne;
    QTimer *timerTwo;
    QTimer *timerThree;
    QTimer *timerFour;
    QTimer *timerFive;

    QGraphicsScene *instrumentClusterScene;

    DataText *dataTextOne;
    DataText *dataTextOneC;
    DataText *dataTextOneWhite;
    DataText *dataTextOneRed;
    DataText *endRedOne;

    DataText *dataTextTwo;
    DataText *dataTextTwoC;
    DataText *dataTextTwoWhite;
    DataText *dataTextTwoRed;
    DataText *endRedTwo;

    DataText *dataTextThree;
    DataText *dataTextThreeC;
    DataText *dataTextThreeWhite;
    DataText *dataTextThreeRed;
    DataText *endRedThree;

    DataText *dataTextFour;
    DataText *dataTextFourC;
    DataText *dataTextFourWhite;
    DataText *dataTextFourRed;
    DataText *endRedFour;

    DataText *dataTextFive;
    DataText *dataTextFiveC;
    DataText *dataTextFiveWhite;
    DataText *dataTextFiveRed;
    DataText *endRedFive;

    DataText *infoText;

    LeftBlinkerAnimation *leftBlinkerAnimation;
    RightBlinkerAnimation *rightBlinkerAnimation;
    SpeedAnimation *speedAnimation;

    BackgroundItem *lTurn;
    BackgroundItem *rTurn;
    BackgroundItem *speed;
    BackgroundItem *engine;
    BackgroundItem *tpms;
    BackgroundItem *tcontrol;
    BackgroundItem *hBeam;
    BackgroundItem *abs;

    AboutVIC *aboutVic;


    bool isWhite;
    bool isRed;

    bool isRedTwo;
    bool isWhiteTwo;

    bool isWhiteThree;
    bool isRedThree;

    bool isWhiteFour;
    bool isRedFour;

    bool isWhiteFive;
    bool isRedFive;

    bool toggleOn;
    bool toggleOnTwo;
    bool toggleOnThree;
    bool toggleOnFour;

    bool dataThreeActive;
    bool dataFourActive;
    bool dataFiveActive;

    int itemCount;
};

#endif // MAINWINDOW_H
