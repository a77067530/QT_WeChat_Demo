#include "usershow.h"
#include "ui_usershow.h"
#include<QPainter>
UserShow::UserShow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserShow)
{
    ui->setupUi(this);
}

UserShow::~UserShow()
{
    delete ui;
}

//设置
void UserShow::slot_setInfo(int id, QString name)
{
       m_id = id;
       m_userName = name;

       QString text = QString("用户：%1").arg(m_userName);
       ui->lb_name->setText(text);
}

//重绘事件
void UserShow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setBrush(Qt::black);
    painter.drawRect(-1,0,this->width()+1,this->height());

    if(m_img.size().height() <= 0) return;
    //画视频帧
    //等比例缩放
    m_img = m_img.scaled(this->width(),this->height()-ui->lb_name->height(),Qt::KeepAspectRatio);
    QPixmap pix = QPixmap::fromImage(m_img);
    int x = this->width() - pix.width();
    x = x/2;
    int y = this->height() - pix.height()-ui->lb_name->height();
    y = y/2 + ui->lb_name->height();
    painter.drawPixmap(QPoint(x,y),pix);
    painter.end();
}

void UserShow::slot_setImage(QImage &img)
{
    m_img = img;
    this->update();//触发重绘
}
