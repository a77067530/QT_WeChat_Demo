#ifndef CKERNEL_H
#define CKERNEL_H

#include <QObject>
#include"wechatdialog.h"
#include"TcpClientMediator.h"
#include"packdef.h"
#include"logindialog.h"
#include"roomdialog.h"
#include"audioread.h"
#include"audiowrite.h"
//协议映射表使用的类型
class Ckernel;
typedef void (Ckernel::*PFUN)(uint sock,char* buf,int nlen);

class Ckernel : public QObject
{
    Q_OBJECT
public:
    explicit Ckernel(QObject *parent = nullptr);
    //单例
    static Ckernel * GetInstance()
    {
        static Ckernel kernel;
        return &kernel;
    }

signals:

public slots:
    void setNetPackMap();
    void slot_destroy();
    //发送登录信息
    void slot_loginCommit(QString tel ,QString pass);
    //发送注册信息
    void slot_registerCommit(QString tel,QString pass,QString name);

    void slot_createRoom();
    void slot_joinRoom();

    void slot_quitRoom();

    //网络信息 处理
    void slot_dealData(uint sock,char* buf,int nlen);
    //登陆回复处理
    void slot_dealLoginRs(uint sock,char* buf,int nlen);
    //注册回复处理
    void slot_dealRegisterRs(uint sock,char* buf,int nlen);

    //创建房间回复处理
    void slot_dealCreateRoomRs(uint sock,char* buf,int nlen);
    //加入房间回复处理
    void slot_dealJoinRoomRs(uint sock,char *buf,int nlen);
    //房间成员请求处理
    void slot_dealRoomMemberRq(uint sock,char *buf,int nlen);
    //离开房间的请求处理
    void slot_dealLeaveRoomRq(uint sock,char *buf,int nlen);
    //音频帧处理
    void slot_dealAudioFrameRq(uint sock,char *buf,int nlen);


    void slot_startAudio();
    void slot_pauseAudio();
    //发送音频帧
    void slot_audioFrame(QByteArray ba);

private:
    WeChatDialog *m_pWeChatDlg;
    LoginDialog *m_pLoginDlg;
    INetMediator *m_pClient;
    RoomDialog *m_pRoomdialog;
    //协议映射表
    PFUN m_netPackMap[DEF_PACK_COUNT];


    int m_id;
    int m_roomid;
    QString m_name;

    //////////////////////////////////////////
    ///音频 1个采集 多个播放 每一个成员 1:1 map 映射
    ///
    AudioRead * m_pAudioRead;
    std::map<int,AudioWrite*> m_mapIDToAudioWrite;

};

#endif // CKERNEL_H
