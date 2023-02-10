#ifndef ABOUTVIC_H
#define ABOUTVIC_H

#include <QDialog>

namespace Ui {
class AboutVIC;
}

class AboutVIC : public QDialog
{
    Q_OBJECT

public:
    explicit AboutVIC(QWidget *parent = nullptr);
    ~AboutVIC();

private slots:


private:
    Ui::AboutVIC *ui;
};

#endif // ABOUTVIC_H
