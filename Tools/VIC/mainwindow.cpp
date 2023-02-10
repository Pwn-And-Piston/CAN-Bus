#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent), ui(new Ui::MainWindow){

    ui->setupUi(this);

    timerOne = new QTimer();
    connect(timerOne, SIGNAL(timeout()), this, SLOT(AnimateDataOne()));
    timerOne->start(1200);

    timerTwo = new QTimer();
    connect(timerTwo, SIGNAL(timeout()), this, SLOT(AnimateDataTwo()));
    timerTwo->start(1000);

    timerThree = new QTimer();
    connect(timerThree, SIGNAL(timeout()), this, SLOT(AnimateDataThree()));

    timerFour = new QTimer();
    connect(timerFour, SIGNAL(timeout()), this, SLOT(AnimateDataFour()));

    timerFive = new QTimer();
    connect(timerFive, SIGNAL(timeout()), this, SLOT(AnimateDataFive()));

    instrumentClusterScene = new QGraphicsScene();
    instrumentClusterScene->setSceneRect(0,0,1200,700);
    ui->graphicsView->setScene(instrumentClusterScene);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    lTurn = new BackgroundItem(QPixmap(":images/turn_left.png"));
    lTurn->setPos(585,130);
    instrumentClusterScene->addItem(lTurn);

    rTurn = new BackgroundItem(QPixmap(":images/turn_right.png"));
    rTurn->setPos(735,130);
    instrumentClusterScene->addItem(rTurn);

    engine = new BackgroundItem(QPixmap(":images/engine.png"));
    engine->setPos(1012,346);
    instrumentClusterScene->addItem(engine);

    speed = new BackgroundItem(QPixmap(":images/speed.png"));
    speed->setPos(1022,140);
    instrumentClusterScene->addItem(speed);

    hBeam = new BackgroundItem(QPixmap(":images/high_beam.png"));
    hBeam->setPos(595,340);
    instrumentClusterScene->addItem(hBeam);

    abs = new BackgroundItem(QPixmap(":images/abs.png"));
    abs->setPos(725,346);
    instrumentClusterScene->addItem(abs);

    tpms = new BackgroundItem(QPixmap(":images/tpms.png"));
    tpms->setPos(877,345);
    instrumentClusterScene->addItem(tpms);

    tcontrol = new BackgroundItem(QPixmap(":images/t_control.png"));
    tcontrol->setPos(870,135);
    instrumentClusterScene->addItem(tcontrol);

    infoText = new DataText();
    infoText->InfoText();
    infoText->setPos(43,145);
    instrumentClusterScene->addItem(infoText);

    dataTextOne = new DataText();
    dataTextOne->DataTextOne();
    dataTextOne->setPos(45,170);
    instrumentClusterScene->addItem(dataTextOne);

    dataTextOneC = new DataText();
    dataTextOneC->DataTextOneC();
    dataTextOneC->setPos(45,170);

    dataTextOneWhite = new DataText();
    dataTextOneWhite->DataTextOneWhite();
    dataTextOneWhite->setPos(385,170);
    instrumentClusterScene->addItem(dataTextOneWhite);

    dataTextOneRed = new DataText();
    dataTextOneRed->DataTextOneRed();
    dataTextOneRed->setPos(385,170);

    endRedOne = new DataText();
    endRedOne->EndRedOne();
    endRedOne->setPos(463,170);

    dataTextTwo = new DataText();
    dataTextTwo->DataTextTwo();
    dataTextTwo->setPos(45,200);
    instrumentClusterScene->addItem(dataTextTwo);

    dataTextTwoC = new DataText();
    dataTextTwoC->DataTextTwoC();
    dataTextTwoC->setPos(45,200);

    dataTextTwoWhite = new DataText();
    dataTextTwoWhite->DataTextTwoWhite();
    dataTextTwoWhite->setPos(325,200);
    instrumentClusterScene->addItem(dataTextTwoWhite);

    dataTextTwoRed = new DataText();
    dataTextTwoRed->DataTextTwoRed();
    dataTextTwoRed->setPos(325,200);

    endRedTwo = new DataText();
    endRedTwo->EndRedTwo();
    endRedTwo->setPos(495,200);

    dataTextThree = new DataText();
    dataTextThree->DataTextThree();

    dataTextThreeC = new DataText();
    dataTextThreeC->DataTextThreeC();

    dataTextThreeWhite = new DataText();
    dataTextThreeWhite->DataTextThreeWhite();

    dataTextThreeRed = new DataText();
    dataTextThreeRed->DataTextThreeRed();

    endRedThree = new DataText();
    endRedThree->EndRedThree();

    dataTextFour = new DataText();
    dataTextFour->DataTextFour();

    dataTextFourC = new DataText();
    dataTextFourC->DataTextFourC();

    dataTextFourWhite = new DataText();
    dataTextFourWhite->DataTextFourWhite();

    dataTextFourRed = new DataText();
    dataTextFourRed->DataTextFourRed();

    endRedFour = new DataText();
    endRedFour->EndRedFour();

    dataTextFive = new DataText();
    dataTextFive->DataTextFive();

    dataTextFiveC = new DataText();
    dataTextFiveC->DataTextFiveC();

    dataTextFiveWhite = new DataText();
    dataTextFiveWhite->DataTextFiveWhite();

    dataTextFiveRed = new DataText();
    dataTextFiveRed->DataTextFiveRed();

    endRedFive = new DataText();
    endRedFive->EndRedFive();

    //Left Turn signal animation
    leftBlinkerAnimation = new LeftBlinkerAnimation();
    leftBlinkerAnimation->setPos(597,145);

    //Left Turn signal animation
    rightBlinkerAnimation = new RightBlinkerAnimation();
    rightBlinkerAnimation->setPos(746,145);

    speedAnimation = new SpeedAnimation();
    speedAnimation->setPos(1022,140);

    toggleOn = false;
    toggleOnTwo = false;
    toggleOnThree = false;
    toggleOnFour = false;

    isWhite = true;
    isRed = false;

    isWhiteTwo = true;
    isRedTwo = false;

    dataThreeActive = false;
    isWhiteThree = false;
    isRedThree = false;

    dataFourActive = false;
    isWhiteFour= false;
    isRedFour = false;

    dataFiveActive = false;
    isWhiteFive= false;
    isRedFive = false;

    itemCount = 0;
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::AnimateDataOne(){

    if(isWhite){
        instrumentClusterScene->removeItem(dataTextOne);
        instrumentClusterScene->removeItem(dataTextOneWhite);
        instrumentClusterScene->addItem(dataTextOneC);
        instrumentClusterScene->addItem(dataTextOneRed);
        instrumentClusterScene->addItem(endRedOne);
        isWhite = false;
        isRed = true;
    }
    else if(isRed){
        instrumentClusterScene->removeItem(dataTextOneC);
        instrumentClusterScene->removeItem(dataTextOneRed);
        instrumentClusterScene->removeItem(endRedOne);
        instrumentClusterScene->addItem(dataTextOne);
        instrumentClusterScene->addItem(dataTextOneWhite);
        isWhite = true;
        isRed = false;
    }else{
        return;
    }
}

void MainWindow::AnimateDataTwo(){

    if(isWhiteTwo){
        instrumentClusterScene->removeItem(dataTextTwo);
        instrumentClusterScene->removeItem(dataTextTwoWhite);
        instrumentClusterScene->addItem(dataTextTwoC);
        instrumentClusterScene->addItem(dataTextTwoRed);
        instrumentClusterScene->addItem(endRedTwo);
        isWhiteTwo = false;
        isRedTwo = true;
    }
    else if(isRedTwo){
        instrumentClusterScene->removeItem(dataTextTwoC);
        instrumentClusterScene->removeItem(dataTextTwoRed);
        instrumentClusterScene->removeItem(endRedTwo);
        instrumentClusterScene->addItem(dataTextTwo);
        instrumentClusterScene->addItem(dataTextTwoWhite);
        isWhiteTwo = true;
        isRedTwo = false;
    }else{
        return;
    }
}

void MainWindow::AnimateDataThree(){

    if(itemCount == 1){
        dataTextThree->setPos(45,230);
        dataTextThreeC->setPos(45,230);
        dataTextThreeWhite->setPos(384,230);
        dataTextThreeRed->setPos(384,230);
        endRedThree->setPos(463,230);

    }
    else if(itemCount == 2 && dataFourActive){
        dataTextFour->setPos(45,260);
        dataTextFourC->setPos(45,260);
        dataTextFourWhite->setPos(384,260);
        dataTextFourRed->setPos(384,260);
        endRedFour->setPos(463,260);

        dataTextThree->setPos(45,230);
        dataTextThreeC->setPos(45,230);
        dataTextThreeWhite->setPos(384,230);
        dataTextThreeRed->setPos(384,230);
        endRedThree->setPos(463,230);
    }
    else if(itemCount == 2 && dataFiveActive){
        dataTextFive->setPos(45,260);
        dataTextFiveC->setPos(45,260);
        dataTextFiveWhite->setPos(258,260);
        dataTextFiveRed->setPos(258,260);
        endRedFive->setPos(463,260);

        dataTextThree->setPos(45,230);
        dataTextThreeC->setPos(45,230);
        dataTextThreeWhite->setPos(384,230);
        dataTextThreeRed->setPos(384,230);
        endRedThree->setPos(463,230);
    }
    else if(itemCount == 3 && dataFourActive && dataFiveActive){
        dataTextFour->setPos(45,260);
        dataTextFourC->setPos(45,260);
        dataTextFourWhite->setPos(384,260);
        dataTextFourRed->setPos(384,260);
        endRedFour->setPos(463,260);

        dataTextFive->setPos(45,290);
        dataTextFiveC->setPos(45,290);
        dataTextFiveWhite->setPos(258,290);
        dataTextFiveRed->setPos(258,290);
        endRedFive->setPos(463,290);

        dataTextThree->setPos(45,230);
        dataTextThreeC->setPos(45,230);
        dataTextThreeWhite->setPos(384,230);
        dataTextThreeRed->setPos(384,230);
        endRedThree->setPos(463,230);
    }
    if(!dataThreeActive){
        instrumentClusterScene->addItem(dataTextThree);
        instrumentClusterScene->addItem(dataTextThreeWhite);
        dataThreeActive = true;
        isWhiteThree = true;
    }
    else if(isWhiteThree){
        instrumentClusterScene->removeItem(dataTextThree);
        instrumentClusterScene->removeItem(dataTextThreeWhite);
        instrumentClusterScene->addItem(dataTextThreeC);
        instrumentClusterScene->addItem(dataTextThreeRed);
        instrumentClusterScene->addItem(endRedThree);
        isWhiteThree = false;
        isRedThree = true;
    }
    else if(isRedThree){
        instrumentClusterScene->removeItem(dataTextThreeC);
        instrumentClusterScene->removeItem(dataTextThreeRed);
        instrumentClusterScene->removeItem(endRedThree);
        instrumentClusterScene->addItem(dataTextThree);
        instrumentClusterScene->addItem(dataTextThreeWhite);
        isWhiteThree = true;
        isRedThree = false;
    }else{
        return;
    }
}

void MainWindow::AnimateDataFour(){

    if(itemCount == 1){
        dataTextFour->setPos(45,230);
        dataTextFourC->setPos(45,230);
        dataTextFourWhite->setPos(384,230);
        dataTextFourRed->setPos(384,230);
        endRedFour->setPos(463,230);
    }
    else if(itemCount == 2 && dataThreeActive){
        dataTextFour->setPos(45,260);
        dataTextFourC->setPos(45,260);
        dataTextFourWhite->setPos(384,260);
        dataTextFourRed->setPos(384,260);
        endRedFour->setPos(463,260);
    }
    else if(itemCount == 2 && dataFiveActive){
        dataTextFive->setPos(45,260);
        dataTextFiveC->setPos(45,260);
        dataTextFiveWhite->setPos(258,260);
        dataTextFiveRed->setPos(258,260);
        endRedFive->setPos(463,260);

        dataTextFour->setPos(45,230);
        dataTextFourC->setPos(45,230);
        dataTextFourWhite->setPos(384,230);
        dataTextFourRed->setPos(384,230);
        endRedFour->setPos(463,230);
    }
    else if(itemCount == 3 && dataThreeActive && dataFiveActive){

        dataTextFive->setPos(45,290);
        dataTextFiveC->setPos(45,290);
        dataTextFiveWhite->setPos(258,290);
        dataTextFiveRed->setPos(258,290);
        endRedFive->setPos(463,290);

        dataTextFour->setPos(45,260);
        dataTextFourC->setPos(45,260);
        dataTextFourWhite->setPos(384,260);
        dataTextFourRed->setPos(384,260);
        endRedFour->setPos(463,260);

        dataTextThree->setPos(45,230);
        dataTextThreeC->setPos(45,230);
        dataTextThreeWhite->setPos(384,230);
        dataTextThreeRed->setPos(384,230);
        endRedThree->setPos(463,230);
    }

    if(!dataFourActive){
        instrumentClusterScene->addItem(dataTextFour);
        instrumentClusterScene->addItem(dataTextFourWhite);
        dataFourActive = true;
        isWhiteFour = true;
    }
    else if(isWhiteFour){
        instrumentClusterScene->removeItem(dataTextFour);
        instrumentClusterScene->removeItem(dataTextFourWhite);
        instrumentClusterScene->addItem(dataTextFourC);
        instrumentClusterScene->addItem(dataTextFourRed);
        instrumentClusterScene->addItem(endRedFour);
        isWhiteFour = false;
        isRedFour = true;
    }
    else if(isRedFour){
        instrumentClusterScene->removeItem(dataTextFourC);
        instrumentClusterScene->removeItem(dataTextFourRed);
        instrumentClusterScene->removeItem(endRedFour);
        instrumentClusterScene->addItem(dataTextFour);
        instrumentClusterScene->addItem(dataTextFourWhite);
        isWhiteFour = true;
        isRedFour = false;
    }else{
        return;
    }
}

void MainWindow::AnimateDataFive(){

    if(itemCount == 1){
        dataTextFive->setPos(45,230);
        dataTextFiveC->setPos(45,230);
        dataTextFiveWhite->setPos(258,230);
        dataTextFiveRed->setPos(258,230);
        endRedFive->setPos(463,230);
    }
    else if(itemCount == 2 && dataThreeActive){
        dataTextFive->setPos(45,260);
        dataTextFiveC->setPos(45,260);
        dataTextFiveWhite->setPos(258,260);
        dataTextFiveRed->setPos(258,260);
        endRedFive->setPos(463,260);

    }
    else if(itemCount == 2 && dataFourActive){  
        dataTextFive->setPos(45,260);
        dataTextFiveC->setPos(45,260);
        dataTextFiveWhite->setPos(258,260);
        dataTextFiveRed->setPos(258,260);
        endRedFive->setPos(463,260);
    }
    else if(itemCount == 3 && dataThreeActive && dataFourActive){
        dataTextFive->setPos(45,290);
        dataTextFiveC->setPos(45,290);
        dataTextFiveWhite->setPos(258,290);
        dataTextFiveRed->setPos(258,290);
        endRedFive->setPos(463,290);
    }

    if(!dataFiveActive){
        instrumentClusterScene->addItem(dataTextFive);
        instrumentClusterScene->addItem(dataTextFiveWhite);
        dataFiveActive = true;
        isWhiteFive = true;
    }
    else if(isWhiteFive){
        instrumentClusterScene->removeItem(dataTextFive);
        instrumentClusterScene->removeItem(dataTextFiveWhite);
        instrumentClusterScene->addItem(dataTextFiveC);
        instrumentClusterScene->addItem(dataTextFiveRed);
        instrumentClusterScene->addItem(endRedFive);
        isWhiteFive = false;
        isRedFive= true;
    }
    else if(isRedFive){
        instrumentClusterScene->removeItem(dataTextFiveC);
        instrumentClusterScene->removeItem(dataTextFiveRed);
        instrumentClusterScene->removeItem(endRedFive);
        instrumentClusterScene->addItem(dataTextFive);
        instrumentClusterScene->addItem(dataTextFiveWhite);
        isWhiteFive = true;
        isRedFive = false;
    }else{
        return;
    }
}

void MainWindow::on_On_Off_One_clicked(){

    toggleOn = !toggleOn;

    if (toggleOn){
        itemCount++;
        timerThree->start(500);
        qDebug()<< itemCount;
        instrumentClusterScene->removeItem(lTurn);
        instrumentClusterScene->addItem(leftBlinkerAnimation);
    }
    else if(!toggleOn){
        timerThree->stop();
        itemCount--;
        qDebug()<< itemCount;
        instrumentClusterScene->removeItem(leftBlinkerAnimation);
        instrumentClusterScene->addItem(lTurn);

        if(isWhiteThree){
            instrumentClusterScene->removeItem(dataTextThree);
            instrumentClusterScene->removeItem(dataTextThreeWhite);
            dataThreeActive = false;
            isRedThree = false;
            isWhiteThree =false;
        }
        else if(isRedThree){
            instrumentClusterScene->removeItem(dataTextThreeC);
            instrumentClusterScene->removeItem(dataTextThreeRed);
            instrumentClusterScene->removeItem(endRedThree);
            dataThreeActive = false;
            isRedThree = false;
            isWhiteThree =false;
        }
    }
}

void MainWindow::on_On_Off_Two_clicked(){

    toggleOnTwo = !toggleOnTwo;

    if (toggleOnTwo){
    instrumentClusterScene->removeItem(rTurn);
    instrumentClusterScene->addItem(rightBlinkerAnimation);
    }
    else if (!toggleOnTwo){
        instrumentClusterScene->removeItem(rightBlinkerAnimation);
        instrumentClusterScene->addItem(rTurn);
    }
}

void MainWindow::on_On_Off_Three_clicked(){

    toggleOnThree = !toggleOnThree;

    if (toggleOnThree){
        itemCount++;
        timerFive->start(800);
         qDebug()<< itemCount;

    }
    else if (!toggleOnThree){
        timerFive->stop();
        itemCount--;
         qDebug()<< itemCount;

         if(isWhiteFive){
             instrumentClusterScene->removeItem(dataTextFive);
             instrumentClusterScene->removeItem(dataTextFiveWhite);
             dataFiveActive = false;
             isRedFive = false;
             isWhiteFive =false;
         }
         else if(isRedFive){
             instrumentClusterScene->removeItem(dataTextFiveC);
             instrumentClusterScene->removeItem(dataTextFiveRed);
             instrumentClusterScene->removeItem(endRedFive);
             dataFiveActive = false;
             isRedFive = false;
             isWhiteFive =false;
         }
    }
}

void MainWindow::on_On_Off_Four_clicked(){

    toggleOnFour = !toggleOnFour;

    if (toggleOnFour){

        itemCount++;
        timerFour->start(700);

        qDebug()<< itemCount;
        instrumentClusterScene->removeItem(speed);
        instrumentClusterScene->addItem(speedAnimation);
    }
    else if(!toggleOnFour){
        timerFour->stop();
        itemCount--;
        qDebug()<< itemCount;
        instrumentClusterScene->removeItem(speedAnimation);
        instrumentClusterScene->addItem(speed);

        if(isWhiteFour){
            instrumentClusterScene->removeItem(dataTextFour);
            instrumentClusterScene->removeItem(dataTextFourWhite);
            dataFourActive = false;
            isRedFour = false;
            isWhiteFour =false;
        }
        else if(isRedFour){
            instrumentClusterScene->removeItem(dataTextFourC);
            instrumentClusterScene->removeItem(dataTextFourRed);
            instrumentClusterScene->removeItem(endRedFour);
            dataFourActive = false;
            isRedFour = false;
            isWhiteFour =false;
        }
    }
}

void MainWindow::on_send_Button_clicked(){

    QString canFrameData = ui->lineEdit->text();
    QString ArbitrationId = ui->lineEdit_2->text();
    QString dataTextFour = "AB FG HE XA YB FS F4 FF";
    QString dataTextFourArbId = "215";

    if(canFrameData == dataTextFour && dataTextFourArbId == ArbitrationId){
        on_On_Off_Four_clicked();
    }
}

void MainWindow::on_actionVirtual_IC_triggered(){

    aboutVic = new AboutVIC();
    aboutVic->show();
}

void MainWindow::on_actionTerminal_triggered(){

    QProcess *openTerminal = new QProcess();
    QString programPath = "/usr/bin/gnome-terminal";
    QStringList arguments;
    arguments << "--working-directory=/Home";
    openTerminal->start(programPath,arguments);
}
