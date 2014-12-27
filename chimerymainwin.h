#ifndef CHIMERYMAINWIN_H
#define CHIMERYMAINWIN_H

#include<QMainWindow>
#include<QGridLayout>
#include<QTimeEdit>
#include<QSpinBox>
#include<QLabel>
#include<QTime>
#include<QDateTime>
#include<QDate>
#include<QLinkedList>
#include<QPushButton>
#include<QTimer>
#include<QSound>
#include<QApplication>
#include<QGuiApplication>
#include<QIcon>
#include<QSystemTrayIcon>

#include<chimeryaboutwin.h>
#include<chimerymenus.h>

class ChimeryMenus;

class ChimeryMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChimeryMainWindow(QWidget *parent = 0);
    ~ChimeryMainWindow();

public slots:
    void refresh();
    void cyc();
    void cycSubmit();
    void chime();
    void reinit();

private:
    QTimeEdit * cycStart;
    QSpinBox * cycNum;
    QSpinBox * cycDur;
    QLabel * cycStartL;
    QLabel * cycNumL;
    QLabel * cycDurL;
    QLabel * gapL;
    QPushButton * gogogo;
    int cycs;
    QLinkedList<QPushButton *> * cycButtons;
    QLinkedList<QSpinBox *> * cycGaps;
    QLinkedList<QDateTime *> * times;
    QSound * begin;
    QSound * end;
    QIcon * active_icon;
    QIcon * passive_icon;
    QSystemTrayIcon * systray;
    ChimeryAboutWindow * about;
    ChimeryMenus * menus;
    QTimer * timer;
};

#endif // CHIMERYMAINWIN_H
