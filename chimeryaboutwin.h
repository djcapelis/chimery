#ifndef CHIMERYABOUTWIN_H
#define CHIMERYABOUTWIN_H

#include<QDialog>
#include<QGridLayout>
#include<QTextEdit>
#include<QPushButton>
#include<QResource>
#include<QLocale>

class ChimeryAboutWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ChimeryAboutWindow(QWidget *parent = 0);
    ~ChimeryAboutWindow();

public slots:

private:
    QTextEdit * text;
    QResource * about;
    QPushButton * close;
};

#endif // CHIMERYABOUTWIN_H
