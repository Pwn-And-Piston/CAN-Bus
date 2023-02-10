#include "datatext.h"

DataText::DataText(QGraphicsItem *parent): QGraphicsTextItem(parent){


}

void DataText::DataTextOne(){

    setPlainText(QString("0.978841     215     AB FG HE XA YB FS      FF      ....#..."));
    setDefaultTextColor(Qt::white);
    setFont(QFont("times",15));

}

void DataText::DataTextOneC(){

    setPlainText(QString("0.889952     215     AB FG HE XA YB FS      FF      .  .... ."));
    setDefaultTextColor(Qt::white);
    setFont(QFont("times",15));
}

void DataText::DataTextOneWhite(){

    setPlainText(QString("F5"));
    setDefaultTextColor(Qt::white);
    setFont(QFont("times",15));
}

void DataText::DataTextOneRed(){

    setPlainText(QString("F4"));
    setDefaultTextColor(Qt::red);
    setFont(QFont("times",15));
}

void DataText::EndRedOne(){

    setPlainText(QString(" #    , "));
    setDefaultTextColor(Qt::red);
    setFont(QFont("times",15));
}

void DataText::DataTextTwo(){

    setPlainText(QString("0.572632     313     AA EF XL HG       S7 LH P5      .)......"));
    setDefaultTextColor(Qt::white);
    setFont(QFont("times",15));
}

void DataText::DataTextTwoC(){

    setPlainText(QString("0.325810     313     AA EF XL HG       S7 LH P5      ......  ."));
    setDefaultTextColor(Qt::white);
    setFont(QFont("times",15));
}

void DataText::DataTextTwoWhite(){

    setPlainText(QString("Z3"));
    setDefaultTextColor(Qt::white);
    setFont(QFont("times",15));
}

void DataText::DataTextTwoRed(){

    setPlainText(QString("6A"));
    setDefaultTextColor(Qt::red);
    setFont(QFont("times",15));
}

void DataText::EndRedTwo(){

    setPlainText(QString(")"));
    setDefaultTextColor(Qt::red);
    setFont(QFont("times",15));
}

void DataText::DataTextThree(){

    setPlainText(QString("0.978841     215     BB FG HE XA YB FS      FF      ....#..."));
    setDefaultTextColor(Qt::white);
    setFont(QFont("times",15));
}

void DataText::DataTextThreeC(){

    setPlainText(QString("0.889952     215     BB FG HE XA YB FS      FF      .  .... ."));
    setDefaultTextColor(Qt::white);
    setFont(QFont("times",15));
}

void DataText::DataTextThreeWhite(){

    setPlainText(QString("D6"));
    setDefaultTextColor(Qt::white);
    setFont(QFont("times",15));
}

void DataText::DataTextThreeRed(){

    setPlainText(QString("D7"));
    setDefaultTextColor(Qt::red);
    setFont(QFont("times",15));
}

void DataText::EndRedThree(){

    setPlainText(QString(" #    , "));
    setDefaultTextColor(Qt::red);
    setFont(QFont("times",15));
}

void DataText::DataTextFour(){

    setPlainText(QString("0.978841     215     AB FG HE XA YB FS      FF      ....!..."));
    setDefaultTextColor(Qt::white);
    setFont(QFont("times",15));
}

void DataText::DataTextFourC(){

    setPlainText(QString("0.889952     215     AB FG HE XA YB FS      FF      .  .... ."));
    setDefaultTextColor(Qt::white);
    setFont(QFont("times",15));
}

void DataText::DataTextFourWhite(){

    setPlainText(QString("F4"));
    setDefaultTextColor(Qt::red);
    setFont(QFont("times",15));
}

void DataText::DataTextFourRed(){

    setPlainText(QString("X7"));
    setDefaultTextColor(Qt::red);
    setFont(QFont("times",15));
}

void DataText::EndRedFour(){

    setPlainText(QString(" *    , "));
    setDefaultTextColor(Qt::red);
    setFont(QFont("times",15));
}

void DataText::DataTextFive(){

    setPlainText(QString("0.231765     212    AB FG       XA YB FS ER FF      ....#..."));
    setDefaultTextColor(Qt::white);
    setFont(QFont("times",15));
}

void DataText::DataTextFiveC(){

    setPlainText(QString("0.183387     212    AB FG       XA YB FS ER FF      .  .... ."));
    setDefaultTextColor(Qt::white);
    setFont(QFont("times",15));
}

void DataText::DataTextFiveWhite(){

    setPlainText(QString("X3"));
    setDefaultTextColor(Qt::red);
    setFont(QFont("times",15));

}

void DataText::DataTextFiveRed(){
    setPlainText(QString("X8"));
    setDefaultTextColor(Qt::red);
    setFont(QFont("times",15));
}

void DataText::EndRedFive(){

    setPlainText(QString(" $    , "));
    setDefaultTextColor(Qt::red);
    setFont(QFont("times",15));
}

void DataText::InfoText(){

    setPlainText(QString("    Delta         ID                         Data"));
    setDefaultTextColor(Qt::white);
    setFont(QFont("times",15));
}



