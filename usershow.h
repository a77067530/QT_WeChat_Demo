#ifndef USERSHOW_H
#define USERSHOW_H

#include <QWidget>
#include<QPaintEvent>
#include<QImage>
namespace Ui {
class UserShow;
}

class UserShow : public QWidget
{
    Q_OBJECT

public:
    explicit UserShow(QWidget *parent = nullptr);
    ~UserShow();

public slots:
    void slot_setInfo(int id,QString name);
    void paintEvent(QPaintEvent *event);
private:
    Ui::UserShow *ui;

    int m_id;
    QString m_userName;
    QImage m_img;
    friend class RoomDialog;
};

#endif // USERSHOW_H
