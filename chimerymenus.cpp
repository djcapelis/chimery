#include"chimerymenus.h"
#include<QDebug>

ChimeryMenus::ChimeryMenus(ChimeryMainWindow * mwin, ChimeryAboutWindow * awin) :
    QMenuBar(NULL)
{
    mainwin = mwin;
    aboutwin = awin;
    menu = new QMenu("&Chimes", this);
    sched = new QAction("&Stop Chimes", this);
    about = new QAction("&About Chimery", this);
    sched->setEnabled(false);
    sched->setShortcuts(QKeySequence::Undo);
    menu->addAction(sched);
    menu->addAction(about);
    this->addMenu(menu);
#ifdef Q_OS_MAC
    menu->setAsDockMenu();
#endif
    QObject::connect(sched, SIGNAL(triggered()), mainwin, SLOT(reinit()), Qt::QueuedConnection);
    QObject::connect(about, SIGNAL(triggered()), aboutwin, SLOT(show()), Qt::QueuedConnection);
}

void ChimeryMenus::sched_start()
{
    sched->setEnabled(true);
}

void ChimeryMenus::sched_stop()
{
    sched->setEnabled(false);
}

ChimeryMenus::~ChimeryMenus()
{
}
