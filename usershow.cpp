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

    //画视频帧
}
