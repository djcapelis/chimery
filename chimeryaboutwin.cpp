#include"chimeryaboutwin.h"
#include<QDebug>

ChimeryAboutWindow::ChimeryAboutWindow(QWidget *parent) :
    QDialog(parent)
{
    QGridLayout * lay = new QGridLayout(this);
    setLayout(lay);
    about = new QResource(":about.html", QLocale::system());
    text = new QTextEdit(QString((char *)about->data()), this);
    text->setReadOnly(true);
    text->setMinimumWidth(500);
    text->setMinimumHeight(600);
    lay->addWidget(text, 0, 0);
    adjustSize();
}

ChimeryAboutWindow::~ChimeryAboutWindow()
{
}
