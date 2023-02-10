#ifndef DATATEXT_H
#define DATATEXT_H
#include <QGraphicsTextItem>
#include <QFont>

class DataText: public QGraphicsTextItem{

public:
    DataText(QGraphicsItem * parent=nullptr);

    void DataTextOne();
    void DataTextOneC();
    void DataTextOneWhite();
    void DataTextOneRed();
    void EndRedOne();

    void DataTextTwo();
    void DataTextTwoC();
    void DataTextTwoWhite();
    void DataTextTwoRed();
    void EndRedTwo();

    void DataTextThree();
    void DataTextThreeC();
    void DataTextThreeWhite();
    void DataTextThreeRed();
    void EndRedThree();

    void DataTextFour();
    void DataTextFourC();
    void DataTextFourWhite();
    void DataTextFourRed();
    void EndRedFour();

    void DataTextFive();
    void DataTextFiveC();
    void DataTextFiveWhite();
    void DataTextFiveRed();
    void EndRedFive();

    void InfoText();

public slots:


private:

};

#endif // DATATEXT_H
