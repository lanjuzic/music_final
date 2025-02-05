#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QPainter>
#include<QMessageBox>

#include "secondarywindow.h"//调用副窗口
#include<QRect>

#include<QNetworkAccessManager>//使用该类获取网络请求
#include<QNetworkReply>//请求访问后返回的响应类

#include<QMediaPlayer>//播放器头文件
#include<QMediaPlaylist>//播放器列表

#include<QTextBlock>
#include<QTimer>
#include<QFileDialog>

#include<QJsonParseError>//错误处理
#include<QJsonObject>
#include<QJsonArray>


#include<QLCDNumber>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);//构造函数
    ~MainWindow();//析构函数

public:
    void paintEvent(QPaintEvent* event) override;//重写背景绘制函数

    /*鼠标事件*/
    void mousePressEvent(QMouseEvent* event) override;//鼠标拖动窗口
    void mouseMoveEvent(QMouseEvent* event) override; //鼠标移动事件
    void mouseReleaseEvent(QMouseEvent* event) override;//鼠标是否释放
private:
    bool mousebool;//鼠标是否按下
    QPoint window;//窗口起始位置
    QPoint mouse;//鼠标起始位置*/
    /*鼠标事件*/

private slots:
    void on_pushButton_close_clicked();//关闭按钮

    void on_pushButton_about_clicked();//关于

    void on_pushButton_search_clicked();//搜索按钮

    void on_pushButton_closesound_clicked();

    void on_pushButton_file_clicked();//文件选择

    void on_pushButton_last_clicked();//上一首

    void on_pushButton_pause_clicked();//暂停和播放

    void on_pushButton_next_clicked();//下一首

    //void on_pushButton_play_clicked();

    void on_pushButton_end_clicked();//结束

    void on_horizontalSlider_voice_valueChanged(int value);

    void on_horizontalSlider_program_valueChanged(int value);

    void titlemove();

private:
    Ui::MainWindow *ui;

protected slots:
    void infoBack(QNetworkReply* reply);//处理返回信息
    void Lcdshow(qint64 duration);//处理LCD时间
    void jindutiao(qint64 duration);//处理进度条
    void  jindutiaoweizhi(qint64 position);//进度条位置
    void palysong();//播放歌曲


private://添加成员如下
    QMediaPlayer* player;//播放器对象
    QMediaPlaylist* playlist;//播放列表对象

    QByteArray byteinfo;//存储网上数据信息
    QString localmusic;//本地音乐

    int music_id;
    QString music_name,music_singer;
    int ipos; //状态标记

    QTextDocument* dotxt;//处理文本内容
    QTextBlock* dofilblockp;//处理文本块指针

    QNetworkAccessManager*  network;//网络对象

    int currentPosition =0;
    int thenPosition =0;

    QString strtitle;
    int ititle;


};
#endif // MAINWINDOW_H
