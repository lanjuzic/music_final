#ifndef SECONDARYWINDOW_H
#define SECONDARYWINDOW_H

#include <QDialog>
#include<QPainter>


namespace Ui {
class Secondarywindow;
}

class Secondarywindow : public QDialog
{
    Q_OBJECT

public:
    explicit Secondarywindow(QWidget *parent = nullptr);
    ~Secondarywindow();

private:
    Ui::Secondarywindow *ui;

public:
    void paintEvent(QPaintEvent* event) override;
private slots:
    void on_buttonBox_accepted();
    void on_pushButton_clicked();
};

#endif // SECONDARYWINDOW_H
