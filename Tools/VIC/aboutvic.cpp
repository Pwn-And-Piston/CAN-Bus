#include "aboutvic.h"
#include "ui_aboutvic.h"

AboutVIC::AboutVIC(QWidget *parent) :QDialog(parent),ui(new Ui::AboutVIC){
    ui->setupUi(this);
}

AboutVIC::~AboutVIC(){
    delete ui;
}


