#include "mainwindow.h"
#include "ui_mainwindow.h"

#pragma execution_character_set("utf-8")//中文显示乱码

MainWindow::MainWindow(QWidget *parent)//mainwindow的构造函数
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setFixedSize(this->width(),this->height());
    //去掉标题栏
    this->setWindowFlag(Qt::FramelessWindowHint);

    //将光标焦点定位到搜索栏
    ui->lineEdit_input->setFocus();

    //初始化网络对象
    network = new QNetworkAccessManager(this);

    ipos=0;//初始话标志

    dotxt=ui->plainTextEdit->document();//初始化文本对象

    ui->plainTextEdit->setReadOnly(true);

    player = new QMediaPlayer(this);
    playlist = new QMediaPlaylist(this);

    player->setMedia(playlist);//设置播放列表
    playlist->setPlaybackMode(QMediaPlaylist::Loop);




    connect(player,SIGNAL(positionChanged(qint64)),this,SLOT(Lcdshow(qint64)));//这里歌曲的播放进度变化与Lcd显示函数相连接
    // 连接positionChanged信号到jindutiaoweizhi槽
    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(jindutiaoweizhi(qint64)));//更新进度条位置

    // 连接durationChanged信号到jindutiao槽
    connect(player, SIGNAL(durationChanged(qint64)), this, SLOT(jindutiao(qint64)));//设置进度条最大值

    connect(network,SIGNAL(finished(QNetworkReply*)),this,SLOT(infoBack(QNetworkReply*)));
    //当发送请求完成后就执行接收回复的操作
    //connect(player, &QMediaPlayer::positionChanged, this, SLOT(Lcdshow(qint64)));

    ui->label_top->setFont((QFont("宋体",10,QFont::Normal)));
    strtitle = ui->label_top->text();
    QTimer* p = new QTimer(this);
    ititle = 0;
     connect(p,SIGNAL(timeout()),this,SLOT(titlemove()));
    p->start(100);
}

MainWindow::~MainWindow()
{
    delete ui;
}
//定义窗口重绘函数
void MainWindow::paintEvent(QPaintEvent* event){
    QPainter qPainter(this);
    qPainter.drawPixmap(0,0,width(),height(),QPixmap(":/new/prefix1/ResImages/view.png"));//MBGMusicplayer_ui2.jpg
}
/*鼠标拖动实现*/
void MainWindow::mousePressEvent(QMouseEvent* event){
    if(event->button()==Qt::LeftButton){
        mousebool = true;
        mouse = event->globalPos();
        window = this->frameGeometry().topLeft();//窗口右上顶点位置
    }
}//鼠标拖动窗口
void MainWindow::mouseMoveEvent(QMouseEvent* event){
    if(mousebool==true){
        QPoint nowpoint = event->globalPos()-mouse;//移动位置
        this->move(window+nowpoint);//移动到后续位置
    }
} //鼠标移动事件
void MainWindow::mouseReleaseEvent(QMouseEvent* event){
    if(event->button()==Qt::LeftButton){
        mousebool = false;
    }
}//鼠标是否释放*/
/*鼠标拖动实现*/







void MainWindow::on_pushButton_close_clicked()
{
    close();
}

void MainWindow::on_pushButton_about_clicked()//负责窗口跳转
{
    //QMessageBox::information(this,"提示","对话框",QMessageBox::Yes,QMessageBox::No);//用messagebox显示信息
    Secondarywindow* secondwindow = new Secondarywindow(this);


    QRect qrect = geometry();
    int fx = qrect.x()+0;
    int fy = qrect.y()+0;
    secondwindow->setGeometry(fx,fy,400,300);//渲染的起始位置，以主窗口为坐标系

    //secondwindow->show();
    secondwindow->exec();//阻塞主窗口保证一次只能操作一个窗口
}

void MainWindow::infoBack(QNetworkReply* reply){
    byteinfo = reply->readAll();

    QJsonParseError jsonerror;//定义错误信息对象

    QJsonDocument jsondocu = QJsonDocument::fromJson(byteinfo,&jsonerror);//获取json文本

    if(jsonerror.error != QJsonParseError::NoError){//如果有错误的话
         QMessageBox::information(this,"ooo","出错啦！！！！！！！！");
        return;
    }else{

    /*1.读取字节流
    2.字节流转换为JSON文档
    3.检查解析错误
    4.从JSON文档获取JSON对象或数组
    5.进一步处理JSON数据*/
       // QMessageBox::information(this,"ooo","没出错啦！！！！！！！！");

        QJsonObject totaljsonobject = jsondocu.object();// 将QJsonDocument对象转换为QJsonObject对象，以便进一步访问JSON数据
        //获取json对象的键值对;

        QStringList strkeys = totaljsonobject.keys();// 获取QJsonObject对象中所有的键（key），存储到QStringList对象strkeys中

        // 这里可以进一步处理获取到的键值对数据
        if(strkeys.contains("result")){//是否包含指定字符串

            QJsonObject resultjson = totaljsonobject["result"].toObject();//将result的数据信息转换成json对象
            QStringList strkey = resultjson.keys();
            if(strkey.contains("songs")){
                QJsonArray array = resultjson["songs"].toArray();//将result的数据信息转换成json对象


                for(auto isong:array){//在for循环中查询歌曲的字段信息
                    QJsonObject song = isong.toObject();
                    music_id = song["id"].toInt();
                    music_name = song["name"].toString();

                    QStringList strkey = song.keys();
                    if(strkey.contains("artists")){

                        QJsonArray jsonobarry1 =  song["artists"].toArray();
                        for(auto ja: jsonobarry1){
                            QJsonObject a = ja.toObject();
                            music_singer = a["name"].toString();
                        }
                    }

                }
            }
        }
        QString songurl;//QString文本可以转换为url
        songurl = QString("https://music.163.com/song/media/outer/url?id=%0").arg(music_id);
        playlist->addMedia(QUrl(songurl));
        ui->plainTextEdit->appendPlainText(music_name+"-"+music_singer);

    }


}//处理返回信息

void MainWindow::Lcdshow(qint64 duration){
    int int_miao = duration/1000;
    int int_minute = int_miao/60;
    int_miao = int_miao%60;
    QString songtime = QString::asprintf("%d:%d",int_minute,int_miao);
    ui->lcdNumber_time->display(songtime);
}//处理LCD时间

void MainWindow::jindutiao(qint64 duration)
{
    // 设置进度条的最大值
    ui->horizontalSlider_program->setRange(0, duration);
    int cvalue = playlist->currentIndex();
    QTextBlock txt = dotxt->findBlockByNumber(cvalue);
    QString qtxt = txt.text();
    ui->label_top->setText(qtxt);
}

void MainWindow::jindutiaoweizhi(qint64 position)
{
    // 如果用户正在拖动滑块，不更新
    if(ui->horizontalSlider_program->isSliderDown()){
        thenPosition = ui->horizontalSlider_program->value();


        //return;

    }else{
        ui->horizontalSlider_program->setSliderPosition(position);
    }
    if(thenPosition != currentPosition){
        currentPosition = thenPosition;
        player->setPosition(currentPosition);
    }
    // 更新进度条的位置

}
void MainWindow::palysong(){

}//播放歌曲

void MainWindow::titlemove(){
    strtitle = ui->label_top->text();
    if(ititle<strtitle.length()){
       QString strtemp = strtitle.mid(ititle)+strtitle.mid(0,ititle);
       ui->label_top->setText(strtemp);
      ititle++;
    }else{
        ititle =0;
    }
}



void MainWindow::on_pushButton_search_clicked()
{
    QString str1,str2;
    str1=ui->lineEdit_input->text();


    str2 = "http://music.163.com/api/search/get/web?csrf_token=hlpretag=&hlposttag=&s={"+str1+"}&type=1&offset=0&total=true&limit=1";
    QNetworkRequest networkrequest;//定义请求对象
    networkrequest.setUrl(str2);//请求格式设置给请求对象

    network->get(networkrequest);
    //QMessageBox::information(this,"ooo",str2);
}

void MainWindow::on_pushButton_closesound_clicked()
{
    if(player->isMuted()){
        ui->pushButton_closesound->setIcon(QIcon(":/new/prefix1/ResImages/MSound.png"));
    }else{
        ui->pushButton_closesound->setIcon(QIcon(":/new/prefix1/ResImages/MNoSound.png"));
    }

       player->setMuted(!player->isMuted());

}

void MainWindow::on_pushButton_file_clicked()
{
    QString projrctpath = QDir::homePath();
    QString title = "请选择音乐文件";
    QString setfile = "所有文件(*.*);;音频文件(*.mp3)";

    QStringList strmp3filelist = QFileDialog::getOpenFileNames(this,title,projrctpath,setfile);//这里会打开文件选择器 获取文件完整路径的列表

    if(strmp3filelist.count()<1){
        return;
    }
    for(int i =0; i<strmp3filelist.count();i++){
        QString strfile = strmp3filelist.at(i);//获取文件完整路径
        playlist->addMedia(QUrl::fromLocalFile(strfile));
        QFileInfo qfile(strfile);//通过文件路径来解析文件
        ui->plainTextEdit->appendPlainText(qfile.fileName());//打印出文件信息
    }
    if(player->state()!= QMediaPlayer::PlayingState){
        playlist->setCurrentIndex(0);
    }
    player->play();
}

void MainWindow::on_pushButton_last_clicked()
{
    if(playlist->currentIndex()>0){
    if(player->state()== QMediaPlayer::PlayingState){
        player->stop();
     }
    playlist->setCurrentIndex(playlist->currentIndex()-1);
    player->play();
    }
}

void MainWindow::on_pushButton_pause_clicked()
{

    if(playlist->mediaCount()==0)return;
    if(player->state()!= QMediaPlayer::PlayingState){
        player->play();

            ui->pushButton_pause->setIcon(QIcon(":/new/prefix1/ResImages/MPause.png"));


    }else if(player->state()== QMediaPlayer::PlayingState){
        player->pause();
        ui->pushButton_pause->setIcon(QIcon(":/new/prefix1/ResImages/MPlay.png"));
    }

}

void MainWindow::on_pushButton_next_clicked()
{
    if(playlist->currentIndex() < playlist->mediaCount()-1){
        if(player->state()== QMediaPlayer::PlayingState){
            player->stop();
         }
        playlist->setCurrentIndex(playlist->currentIndex()+1);
        player->play();
    }
}

//void MainWindow::on_pushButton_play_clicked()
//{

//}

void MainWindow::on_pushButton_end_clicked()
{
    if(player->state()!=QMediaPlayer::StoppedState){
        ui->pushButton_pause->setIcon(QIcon(":/new/prefix1/ResImages/MPlay.png"));
        player->stop();
    }
}

void MainWindow::on_horizontalSlider_voice_valueChanged(int value)
{
    player->setVolume(value);
}//音量控制

void MainWindow::on_horizontalSlider_program_valueChanged(int value)
{
    //value = 100;
    //player->setPosition(value);
}
