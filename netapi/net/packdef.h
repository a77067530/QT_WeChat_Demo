#pragma once


#define _DEF_BUFFER  ( 4096 )
#define _DEF_CONTENT_SIZE	(1024)
#define _MAX_SIZE	(40)

//自定义协议   先写协议头 再写协议结构
//登录 注册 获取好友信息 添加好友 聊天 发文件 下线请求

#define _DEF_PORT 8000
#define _DEF_SERVERIP "192.168.120.128"

#define DEF_PACK_BASE  (10000)
#define DEF_PACK_COUNT  (100)

//注册
#define  DEF_PACK_REGISTER_RQ    (DEF_PACK_BASE + 0)
#define  DEF_PACK_REGISTER_RS    (DEF_PACK_BASE + 1)
//登录
#define  DEF_PACK_LOGIN_RQ    (DEF_PACK_BASE + 2)
#define  DEF_PACK_LOGIN_RS    (DEF_PACK_BASE + 3)
//创建房间
#define DEF_PACK_CREATEROOM_RQ  (DEF_PACK_BASE + 4)
#define DEF_PACK_CREATEROOM_RS  (DEF_PACK_BASE + 5)
//加入房间
#define DEF_PACK_JOINROOM_RQ  (DEF_PACK_BASE + 6)
#define DEF_PACK_JOINROOM_RS  (DEF_PACK_BASE + 7)
//房间列表请求
#define DEF_PACK_ROOM_MEMBER    (DEF_PACK_BASE + 8)
//音频数据
#define DEF_PACK_AUDIO_FRAME    (DEF_PACK_BASE + 9)
//视频数据
#define DEF_PACK_VIDEO_FRAME    (DEF_PACK_BASE + 10)
//退出房间请求
#define DEF_PACK_LEAVEROOM_RQ   (DEF_PACK_BASE + 11)
//退出房间回复
#define DEF_PACK_LEAVEROOM_RS   (DEF_PACK_BASE + 12)


//返回的结果
//注册请求的结果
#define tel_is_exist		(0)
#define register_success	(1)
#define name_is_exist       (2)
//登录请求的结果
#define user_not_exist		(0)
#define password_error		(1)
#define login_success		(2)
//添加好友的结果
#define no_this_user		(0)
#define user_refuse			(1)
#define user_offline		(2)
#define add_success			(3)

typedef int PackType;

#define MAX_SIZE            (60  )
//协议结构
//登录请求
typedef struct STRU_LOGIN_RQ
{
    STRU_LOGIN_RQ()
    {
        m_nType = DEF_PACK_LOGIN_RQ;
        memset(m_tel,0,MAX_SIZE);
        memset(m_password,0,MAX_SIZE);
    }

    PackType m_nType;   //包类型
    char     m_tel[MAX_SIZE] ; //用户ID
    char     m_password[MAX_SIZE];  //密码
} STRU_LOGIN_RQ;


//登录回复
typedef struct STRU_LOGIN_RS
{
    STRU_LOGIN_RS()
    {
        m_nType= DEF_PACK_LOGIN_RS;
        m_userid = 0;
        m_lResult = 0;
        memset(m_name,0,MAX_SIZE);
    }
    PackType m_nType;   //包类型
    int  m_userid;
    int  m_lResult ; //注册结果
    char m_name[MAX_SIZE];
} STRU_LOGIN_RS;

//注册请求
typedef struct STRU_REGISTER_RQ
{
    STRU_REGISTER_RQ()
    {
        m_nType = DEF_PACK_REGISTER_RQ;
        memset(m_tel,0,MAX_SIZE);
        memset(m_password,0,MAX_SIZE);
        memset(m_name,0,MAX_SIZE);
    }

    PackType m_nType;   //包类型
    char     m_tel[MAX_SIZE] ; //手机号
    char     m_password[MAX_SIZE];  //密码
    char     m_name[MAX_SIZE];  //昵称
} STRU_REGISTER_RQ;

//注册回复
typedef struct STRU_REGISTER_RS
{
    STRU_REGISTER_RS()
    {
        m_nType= DEF_PACK_REGISTER_RS;
        m_lResult = 0;
    }
    PackType m_nType;   //包类型
    int  m_lResult ; //注册结果

} STRU_REGISTER_RS;

//创建房间请求
typedef struct STRU_CREATEROOM_RQ
{
    STRU_CREATEROOM_RQ()
    {
        m_nType = DEF_PACK_CREATEROOM_RQ;
        m_UserID = 0;
    }

    PackType m_nType;   //包类型
    int m_UserID;

}STRU_CREATEROOM_RQ;

//创建房间回复
typedef struct STRU_CREATEROOM_RS
{
    STRU_CREATEROOM_RS()
    {
        m_nType= DEF_PACK_CREATEROOM_RS;
        m_lResult = 0;
        m_RoomId = 0;
    }
    PackType m_nType;   //包类型
    int  m_lResult ;    //创建结果
    int  m_RoomId;

}STRU_CREATEROOM_RS;

//加入房间请求
typedef struct STRU_JOINROOM_RQ
{
    STRU_JOINROOM_RQ()
    {
        m_nType = DEF_PACK_JOINROOM_RQ;
        m_UserID = 0;
        m_RoomID = 0;
    }

    PackType m_nType;   //包类型
    int m_UserID;
    int m_RoomID;

}STRU_JOINROOM_RQ;

//加入房间回复
typedef struct STRU_JOINROOM_RS
{
    STRU_JOINROOM_RS()
    {
        m_nType= DEF_PACK_JOINROOM_RS;
        m_lResult = 0;
        m_RoomID = 0;
    }
    PackType m_nType;   //包类型
    int  m_lResult ;    //注册结果
    int m_RoomID;
}STRU_JOINROOM_RS;

//房间成员请求
typedef struct STRU_ROOM_MEMBER_RQ
{
    STRU_ROOM_MEMBER_RQ()
    {
        m_nType= DEF_PACK_ROOM_MEMBER;
        m_UserID =0;
        memset(m_szUser,0,MAX_SIZE);
    }
    PackType m_nType;   //包类型
    int m_UserID;
    char m_szUser[MAX_SIZE];

}STRU_ROOM_MEMBER_RQ;


//离开房间请求
typedef struct STRU_LEAVEROOM_RQ
{
    STRU_LEAVEROOM_RQ()
    {
        m_nType = DEF_PACK_LEAVEROOM_RQ;
        m_nUserId = 0;
        m_RoomId = 0;
        memset(szUserName,0,MAX_SIZE);
    }
    PackType   m_nType;   //包类型
    int    m_nUserId; //用户ID
    int    m_RoomId;
    char   szUserName[MAX_SIZE];
}STRU_LEAVEROOM_RQ;

////好友信息
//typedef struct STRU_FRIEND_INFO
//{
//    STRU_FRIEND_INFO():type(DEF_PACK_FRIEND_INFO) , userid(0) , iconid(0) , state(0)
//	{
//		memset( name ,  0 , sizeof(name) );
//		memset( feeling , 0 , sizeof(feeling));
//	}
//	//需要 用户id 头像id 昵称 签名 状态--是否在线
//	PackType type;
//	int userid;
//	int iconid;
//	int state;
//	char name[_MAX_SIZE];
//	char feeling[_MAX_SIZE];

//}STRU_FRIEND_INFO;

////添加好友
//typedef struct STRU_ADD_FRIEND_RQ
//{
//	// 如果用户1 添加用户2 为好友 需要 用户1 id 用户1 名字 ,用户2的名字
//    STRU_ADD_FRIEND_RQ():type(DEF_PACK_ADDFRIEND_RQ) , userid(0)
//	{
//		memset( userName  , 0 , sizeof(userName));
//		memset( friendName , 0 , sizeof(friendName));
//	}
//	PackType type;
//	int userid;
//	char userName[_MAX_SIZE];
//	char friendName[_MAX_SIZE];

//}STRU_ADD_FRIEND_RQ;

//typedef struct STRU_ADD_FRIEND_RS
//{
//    STRU_ADD_FRIEND_RS():type(DEF_PACK_ADDFRIEND_RS) , userid(0), friendid(0),result(add_success)
//	{
//		memset( friendName  , 0 , sizeof(friendName));
//	}
//	//如果用户1 添加用户2 需要 用户1id  用户2 id 用户2的名字 结果
//	PackType type;
//	int userid; // 用户1
//	int friendid; // 用户2
//	int result;
//	char friendName[_MAX_SIZE];
	
//}STRU_ADD_FRIEND_RS;

////聊天
//typedef struct STRU_CHAT_RQ
//{
//    STRU_CHAT_RQ():type(DEF_PACK_CHAT_RQ) , userid(0) , friendid(0)
//	{
//		memset( content , 0 , _DEF_CONTENT_SIZE );
//	}
//	// 谁发给谁 服务器转发  用户1 id 用户2 id 发的内容
//	PackType type;
//	int userid;
//	int friendid;
//	char content[_DEF_CONTENT_SIZE];

//}STRU_CHAT_RQ;

//typedef struct STRU_CHAT_RS
//{
//    STRU_CHAT_RS():type(DEF_PACK_CHAT_RS) , userid(0) , friendid(0),result(0)
//	{
//	}
//	PackType type;
//	int userid;
//	int friendid; //方便找是哪个人不在线
//	int result;

//}STRU_CHAT_RS;

////发给服务器说明离线
//typedef struct STRU_OFFLINE
//{
//    STRU_OFFLINE():type(DEF_PACK_OFFLINE_RQ) , userid(0)
//    {
//    }
//    PackType type;
//    int userid;

//}STRU_OFFLINE;

// 数据库
/*
	表1 用户表 id->唯一标识 tel  password  name  iconid   feeling 也可以写成两张表 登录和用户信息
	列:  id  tel password  name  iconid  feeling

	表2 好友  idA  idB
	列 idA  idB  
	操作: 好友关系是双向关系  关注是单向关系  , 添加好友时 , 需要写入两行 即 A->B B->A 方便查询
	(添加好友 查找一般比插入多)

//	表3 离线消息
	
	用户数据表t_user
	create table t_user ( id bigint unsigned  AUTO_INCREMENT primary key, tel varchar (260) , 
	password varchar(260)  , name varchar (260) , icon int , feeling varchar(260));

	可以添加测试数据 
	insert into t_user ( tel,password , name , icon , feeling  ) 
	values ('12345','test', '12345' ,1,'比较懒,什么也没写')

	好友表 t_friend
	create table t_friend ( idA bigint unsigned  , idB bigint unsigned);

*/

///音频数据帧
/// 成员描述
/// int type;
/// int userId;
/// int roomId;
/// int min
/// int sec;
/// int msec;
/// QByteArray audioFrame;

///视频数据帧
/// 成员描述
/// int type;
/// int userId;
/// int roomId;
/// int min;
/// int sec;
/// int msec;
/// QByteArray videoFrame;
///





