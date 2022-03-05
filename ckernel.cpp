#include "ckernel.h"
#include"qDebug"
#include"md5.h"
#include<QMessageBox>

#define NetPackMap(a) m_netPackMap[a-DEF_PACK_BASE]

//设置添加协议映射关系
void Ckernel::setNetPackMap()
{
    memset(m_netPackMap,0,sizeof(m_netPackMap));

    //m_netPackMap[DEF_PACK_LOGIN_RS-DEF_PACK_BASE] = &Ckernel::slot_dealLoginRs;

    NetPackMap(DEF_PACK_LOGIN_RS) = &Ckernel::slot_dealLoginRs;

    NetPackMap(DEF_PACK_REGISTER_RS) = &Ckernel::slot_dealRegisterRs;

    NetPackMap(DEF_PACK_CREATEROOM_RS) = &Ckernel::slot_dealCreateRoomRs;

    NetPackMap(DEF_PACK_JOINROOM_RS) = &Ckernel::slot_dealJoinRoomRs;

    NetPackMap(DEF_PACK_ROOM_MEMBER) = &Ckernel::slot_dealRoomMemberRq;

    NetPackMap(DEF_PACK_LEAVEROOM_RQ) = &Ckernel::slot_dealLeaveRoomRq;
}


Ckernel::Ckernel(QObject *parent) : QObject(parent),m_id(0),m_roomid(0)
{
    setNetPackMap();
    m_pWeChatDlg  = new WeChatDialog;
    connect(m_pWeChatDlg,SIGNAL(SIG_close()),this,SLOT(slot_destroy()));

    connect(m_pWeChatDlg,SIGNAL(SIG_createRoom()),this,SLOT(slot_createRoom()));

    connect(m_pWeChatDlg,SIGNAL(SIG_joinRoom()),this,SLOT(slot_joinRoom()));
    //     m_pWeChatDlg->show();

    m_pLoginDlg = new LoginDialog;

    connect(m_pLoginDlg,SIGNAL(SIG_loginCommit(QString,QString)),this,SLOT(slot_loginCommit(QString,QString)));

    connect(m_pLoginDlg,SIGNAL(SIG_close()),this,SLOT(slot_destroy()));

    connect(m_pLoginDlg,SIGNAL(SIG_registerCommit(QString,QString,QString)),this,SLOT(slot_registerCommit(QString,QString,QString)));

    m_pLoginDlg->show();

    m_pRoomdialog = new RoomDialog;

    connect(m_pRoomdialog,SIGNAL(SIG_close()),this,SLOT(slot_quitRoom()));

    //添加网络
    m_pClient = new TcpClientMediator;
    m_pClient->OpenNet(_DEF_SERVERIP,_DEF_PORT);

    connect(m_pClient,SIGNAL(SIG_ReadyData(uint,char*,int)),this,SLOT(slot_dealData(uint,char*,int)));

}


//回收
void Ckernel::slot_destroy()
{
    qDebug()<<__func__;
    if(m_pWeChatDlg)
    {
        m_pWeChatDlg->hide();
        delete m_pWeChatDlg;
        m_pWeChatDlg = NULL;
    }
    if(m_pLoginDlg)
    {
        m_pLoginDlg->hide();
        delete m_pLoginDlg;
        m_pLoginDlg = NULL;
    }
    if(m_pClient)
    {
        m_pClient->CloseNet();
        delete m_pClient;
        m_pClient = NULL;
    }

    exit(0);
}

#define MD5_KEY (1234)
static std::string GetMD5(QString value)
{
    QString str = QString("%1_%2").arg(value).arg(MD5_KEY);
    std::string strSrc = str.toStdString();
    MD5 md5(strSrc);
    return md5.toString();
}

//提交登陆信息
void Ckernel::slot_loginCommit(QString tel, QString pass)
{
    std::string strTel = tel.toStdString();
    //std::string strPass = tel.toStdString();

    STRU_LOGIN_RQ rq;
    strcpy(rq.m_tel,strTel.c_str());

    std::string strPassMD5 = GetMD5(pass);
    qDebug() << strPassMD5.c_str();

    strcpy(rq.m_password,strPassMD5.c_str());

    m_pClient->SendData(0,(char*)&rq,sizeof(rq));
}

//发送注册信息
void Ckernel::slot_registerCommit(QString tel, QString pass, QString name)
{
    std::string strTel = tel.toStdString();
    //std::string strPass = tel.toStdString();
    std:string strName = name.toStdString();//格式utf-8

    STRU_REGISTER_RQ rq;
    strcpy(rq.m_tel,strTel.c_str());

    std::string strPassMD5 = GetMD5(pass);
    qDebug() << strPassMD5.c_str();

    //兼容中文 utf8 QString -> std::string -->char *
    strcpy(rq.m_name,strName.c_str());

    strcpy(rq.m_password,strPassMD5.c_str());

    m_pClient->SendData(0,(char*)&rq,sizeof(rq));
}

//创建房间
void Ckernel::slot_createRoom()
{
    //判断是否在房间内 m_roomid
    if(m_roomid != 0)
    {
        QMessageBox::about(m_pWeChatDlg,"提示","在房间内，无法创建，先退出");
        return ;
    }
    //发命令 创建房间
    STRU_CREATEROOM_RQ rq;
    rq.m_UserID = m_id;

    m_pClient->SendData(0,(char*)&rq,sizeof(rq));
}

#include<QInputDialog>
#include"QRegExp"
//加入房间
void Ckernel::slot_joinRoom()
{
    //判断是否在房间内 m_roomid
    if(m_roomid != 0)
    {
        QMessageBox::about(m_pWeChatDlg,"提示","在房间内，无法加入，先退出");
        return ;
    }
    //弹出房间 填房间号
    QString strRoom = QInputDialog::getText(m_pWeChatDlg,"加入房间","请输入房间号");
    //合理化判断
    QRegExp exp("^[0-9]\{1,8\}$");
    if(!exp.exactMatch(strRoom))
    {
        QMessageBox::about(m_pWeChatDlg,"提示","房间号不合法,长度不超过8的数字");
        return;
    }
    qDebug()<<strRoom;
    //发命令 加入房间
    STRU_JOINROOM_RQ rq;
    rq.m_UserID = m_id;
    rq.m_RoomID = strRoom.toInt();
    m_pClient->SendData(0,(char*)&rq,sizeof(rq));
}

//退出房间
void Ckernel::slot_quitRoom()
{
    //发退出包
    STRU_LEAVEROOM_RQ rq;
    rq.m_nUserId = m_id;
    rq.m_RoomId = m_roomid;
    std::string name = m_name.toStdString();

    strcpy(rq.szUserName,name.c_str());

    m_pClient->SendData(0,(char*)&rq,sizeof(rq));
    //关闭 音频 视频 todo

    //回收资源
    m_pRoomdialog->slot_clearUserShow();
    m_roomid = 0;

}


//网络处理
void Ckernel::slot_dealData(uint sock, char *buf, int nlen)
{
    int type = *(int*)buf;
    if(type >= DEF_PACK_BASE && type < DEF_PACK_BASE +DEF_PACK_COUNT)
    {
        //取得协议头，根据协议映射关系，找到函数指针
        PFUN pf = NetPackMap(type);
        if(pf)
        {
            (this->*pf)(sock,buf,nlen);
        }
    }
    delete[] buf;
}
//登陆回复处理
void Ckernel::slot_dealLoginRs(uint sock, char *buf, int nlen)
{
    qDebug()<<__func__;

    //拆包
    STRU_LOGIN_RS * rs = (STRU_LOGIN_RS*)buf;
    //根据返回结果，得到不同信息
    switch (rs->m_lResult)
    {
    case user_not_exist:
        QMessageBox::about(m_pLoginDlg,"提示","用户不存在，登录失败");
        break;
    case password_error:
        QMessageBox::about(m_pLoginDlg,"提示","密码错误，登陆失败");
        break;
    case login_success:
        {
            //QString strName =QString("用户[%1]登录成功").arg( rs->m_name );
            //QMessageBox::about(m_pLoginDlg,"提示",strName);
            //id记录
            m_id = rs->m_userid;

            m_name = QString::fromStdString(rs->m_name);
            m_pWeChatDlg->setInfo(m_name);
            //ui跳转
            m_pLoginDlg->hide();
            m_pWeChatDlg->showNormal();
        }
        break;
    }
}
//注册回复处理
void Ckernel::slot_dealRegisterRs(uint sock, char *buf, int nlen)
{
    //拆包
    STRU_REGISTER_RS *rs = (STRU_REGISTER_RS *)buf;
    //根据不同的结果 弹出不同的提示窗
    switch (rs->m_lResult)
    {
    case tel_is_exist:
        QMessageBox::about(m_pLoginDlg,"提示","手机号已存在，注册失败");
        break;
    case register_success:
        QMessageBox::about(m_pLoginDlg,"提示","注册成功");
        break;
    case name_is_exist:
        QMessageBox::about(m_pLoginDlg,"提示","昵称已存在，注册失败");
        break;
    default:
        break;
    }
}

//创建房间回复处理
void Ckernel::slot_dealCreateRoomRs(uint sock, char *buf, int nlen)
{
    //拆包
    STRU_CREATEROOM_RS *rs = (STRU_CREATEROOM_RS*) buf;
    //房间号 显示到界面 跳转
    m_pRoomdialog->slot_setInfo(QString::number(rs->m_RoomId));
    //服务器没有把个人信息发给你，作为第一个进入房间的

    //把自己的信息放到房间里 作显示 todo
    UserShow * user = new UserShow;
    user->slot_setInfo(m_id,m_name);
    m_pRoomdialog->slot_addUserShow(user);

    m_roomid= rs->m_RoomId;
    m_pRoomdialog->showNormal();
}
//加入房间回复处理
void Ckernel::slot_dealJoinRoomRs(uint sock, char *buf, int nlen)
{
    //拆包
    STRU_JOINROOM_RS * rs = (STRU_JOINROOM_RS*)buf;
    //根据结果 失败 提示
    if(rs->m_lResult == 0)
    {
        QMessageBox::about(m_pWeChatDlg,"提示","房间id不存在，加入失败");
        return ;
    }
    //成功
    //房间号 显示到界面 跳转
    m_pRoomdialog->slot_setInfo(QString::number(rs->m_RoomID));
    //跳转 roomid设置
    m_roomid = rs->m_RoomID;
    m_pRoomdialog->showNormal();
}
//房间成员请求处理
void Ckernel::slot_dealRoomMemberRq(uint sock, char *buf, int nlen)
{
    //拆包
    STRU_ROOM_MEMBER_RQ *rq = (STRU_ROOM_MEMBER_RQ*)buf;
    //创建用户对应控件
    UserShow * user = new UserShow;
    user->slot_setInfo(rq->m_UserID,QString::fromStdString(rq->m_szUser));
    m_pRoomdialog->slot_addUserShow(user);


}

//离开房间的请求处理
void Ckernel::slot_dealLeaveRoomRq(uint sock, char *buf, int nlen)
{
    //拆包
    STRU_LEAVEROOM_RQ *rq = (STRU_LEAVEROOM_RQ*)buf;
    //把这个人 从ui去掉
    if(rq->m_RoomId == m_roomid)
    {
        m_pRoomdialog->slot_removeUserShow(rq->m_nUserId);
    }
}
