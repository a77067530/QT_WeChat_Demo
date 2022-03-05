#ifndef ROOMDIALOG_H
#define ROOMDIALOG_H

#include <QDialog>

#include<QVBoxLayout>
#include"usershow.h"
namespace Ui {
class RoomDialog;
}

class RoomDialog : public QDialog
{
    Q_OBJECT
signals:
    void SIG_close();

public:
    explicit RoomDialog(QWidget *parent = nullptr);
    ~RoomDialog();

public slots:
    void slot_setInfo(QString roomid);

    void slot_addUserShow(UserShow *user);

    void slot_removeUserShow(UserShow *user);

    void slot_removeUserShow(int id);

    void slot_clearUserShow();
private slots:
    void on_pb_close_clicked();

    void on_pb_quit_clicked();

    void closeEvent(QCloseEvent *event);
private:
    Ui::RoomDialog *ui;
    QVBoxLayout * m_mainLayout;
    std::map<int,UserShow*> m_mapIDToUserShow;

};

#endif // ROOMDIALOG_H
