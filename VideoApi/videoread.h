#ifndef VIDEOREAD_H
#define VIDEOREAD_H

#include <QObject>
#include<QImage>
#include"common.h"
#include<QTimer>
#define IMAGE_WIDTH (320)
#define IMAGE_HEIGHT (240)

class VideoRead : public QObject
{
    Q_OBJECT
    ~VideoRead();
public:
    explicit VideoRead(QObject *parent = nullptr);

signals:
    void SIG_sendVideoFrame(QImage img);
public slots:
    void slot_getVideoFrame();
    void slot_openVideo();
    void slot_closeVideo();
private:
    QTimer *m_timer;

    //opencv获取图片的对象
    cv::VideoCapture cap;
};

#endif // VIDEOREAD_H
