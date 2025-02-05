#include "secondarywindow.h"
#include "ui_secondarywindow.h"

Secondarywindow::Secondarywindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Secondarywindow)
{
    ui->setupUi(this);

    //去掉标题栏
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setFixedSize(this->width(),this->height());


}

Secondarywindow::~Secondarywindow()
{
    delete ui;
}

void Secondarywindow::paintEvent(QPaintEvent* event){
    QPainter qPainter(this);
    qPainter.drawPixmap(0,0,width(),height(),QPixmap(":/new/prefix1/ResImages/MBGMusicplayer_ui2.jpg"));
}

void Secondarywindow::on_buttonBox_accepted()
{

}

void Secondarywindow::on_pushButton_clicked()
{
    close();
}
