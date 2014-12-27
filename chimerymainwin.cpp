#include"chimerymainwin.h"
#include<QDebug>

ChimeryMainWindow::ChimeryMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    QTime currentT = QTime::currentTime();
    cycButtons = new QLinkedList<QPushButton *>();
    cycGaps = new QLinkedList<QSpinBox *>();
    times = new QLinkedList<QDateTime *>();
    QWidget * central = new QWidget(this);
    setCentralWidget(central);
    QGridLayout * lay = new QGridLayout(central);
    central->setLayout(lay);
    cycStart = new QTimeEdit(QTime(currentT.hour(), currentT.minute() - (currentT.minute() % 5) + 5), central);
    cycNum = new QSpinBox(central);
    cycNum->setRange(1, 12);
    cycNum->setValue(8);
    cycDur = new QSpinBox(central);
    cycDur->setRange(5, 90);
    cycDur->setValue(50);
    cycDur->setSingleStep(5);
    cycStartL = new QLabel("Start Time", central);
    cycNumL = new QLabel("Cycles", central);
    cycDurL = new QLabel("Duration", central);
    gapL = NULL; 
    gogogo = NULL;
    begin = new QSound(":art/begin.wav", this);
    end = new QSound(":art/end.wav", this);
    lay->addWidget(cycStartL, 0, 0, 1, 1);
    lay->addWidget(cycNumL, 0, 1, 1, 1);
    lay->addWidget(cycDurL, 0, 2, 1, 1);
    lay->addWidget(cycStart, 1, 0, 1, 1);
    lay->addWidget(cycNum, 1, 1, 1, 1);
    lay->addWidget(cycDur, 1, 2, 1, 1);
    adjustSize();
    about = new ChimeryAboutWindow(this);
    menus = new ChimeryMenus(this, about);
    systray = new QSystemTrayIcon(this);
    systray->setContextMenu(menus->menu);
    active_icon = new QIcon(":art/chimeryactivelogo.svg");
    passive_icon = new QIcon(":art/chimerylogo.svg");
    QObject::connect(cycStart, SIGNAL(timeChanged(QTime)), this, SLOT(cyc()), Qt::QueuedConnection);
    QObject::connect(cycNum, SIGNAL(valueChanged(int)), this, SLOT(cyc()), Qt::QueuedConnection); 
    QObject::connect(cycDur, SIGNAL(valueChanged(int)), this, SLOT(cyc()), Qt::QueuedConnection); 
    timer = new QTimer(this);
    timer->setTimerType(Qt::VeryCoarseTimer);
    timer->setSingleShot(true);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(chime()), Qt::QueuedConnection);
    cyc();
}

void ChimeryMainWindow::reinit()
{
    // Reinit widgets
    QTime rcur = QTime::currentTime();
    rcur.setHMS(rcur.hour(), rcur.minute() - (rcur.minute() % 5) + 5, 0);
    cycStart->setTime(rcur);
    cycNum->setValue(8);
    cycDur->setValue(50);

    // Reinit state
    timer->stop();
    cyc();
    refresh();
    systray->hide();
    menus->sched_stop();
    this->show();
    QGuiApplication::setQuitOnLastWindowClosed(true);
}

void ChimeryMainWindow::cyc()
{
    // Get info
    cycs = cycNum->value(); 
    QGridLayout * lay = (QGridLayout *) centralWidget()->layout();

    // Delete all times in list (we recompute them in this loop)
    while(!times->isEmpty())
        delete times->takeFirst();

    // Init stuff
    if(gogogo)
    {
        delete gogogo;
        gogogo = NULL;
    }
    if(gapL)
    {
        delete gapL;
        gapL = NULL;
    }
    int cycle;
    QDateTime Tstart = QDateTime(QDate::currentDate(), cycStart->time());
    QDateTime Tend = QDateTime(Tstart);
    QDateTime * insert;
    QLinkedList<QPushButton *> * cycButtons_old = cycButtons;
    QLinkedList<QPushButton *> * cycButtons_new = new QLinkedList<QPushButton *>();
    QLinkedList<QSpinBox *> * cycGaps_old = cycGaps;
    QLinkedList<QSpinBox *> * cycGaps_new = new QLinkedList<QSpinBox *>();
    QPushButton * cycB;
    QSpinBox * cycGap;

    // Build widgets for cycles
    for(cycle = 0; cycle < cycs; cycle++)
    {
        if(!cycGaps_old->isEmpty() && cycle < (cycs - 1))
        {
            cycGap = cycGaps_old->takeFirst();
            cycGaps_new->append(cycGap);
        }
        else if(cycle < (cycs - 1))
        {
            cycGap = new QSpinBox(centralWidget());
            cycGap->setRange(5, 90);
            cycGap->setValue(20);
            cycGap->setSingleStep(5);
            lay->addWidget(cycGap, 2+cycle, 2);
            QObject::connect(cycGap, SIGNAL(valueChanged(int)), this, SLOT(cyc()), Qt::QueuedConnection); 
            cycGaps_new->append(cycGap);
        }
        Tend = Tstart.addSecs(cycDur->value() * 60);
        if(!cycButtons_old->isEmpty())
        {
            cycB = cycButtons_old->takeFirst();
            cycB->setText(Tstart.toString("hh:mm") + "-" + Tend.toString("hh:mm"));
            cycButtons_new->append(cycB);
        }
        else
        {
            cycB = new QPushButton(Tstart.toString("hh:mm") + "-" + Tend.toString("hh:mm"), centralWidget());
            cycB->setCheckable(true);
            cycB->setChecked(true);
            lay->addWidget(cycB, 2+cycle, 0);
            QObject::connect(cycB, SIGNAL(clicked()), this, SLOT(cyc()), Qt::QueuedConnection); 
            cycButtons_new->append(cycB);
        }
        if(cycle == 0 && cycs > 1)
        {
            gapL = new QLabel("Gap:", centralWidget());
            lay->addWidget(gapL, 2, 1, Qt::AlignRight);
        }
        if(cycB->isChecked())
        {
            insert = new QDateTime(Tstart);
            times->append(insert);
            insert = new QDateTime(Tend);
            times->append(insert);
        }
        if(cycle < cycs - 1)
            Tstart = Tstart.addSecs((cycDur->value() + cycGap->value()) * 60);
    }

    // Cleanup and swap lists
    while(!cycButtons_old->isEmpty())
        delete cycButtons_old->takeFirst();
    delete cycButtons_old;
    while(!cycGaps_old->isEmpty())
        delete cycGaps_old->takeFirst();
    delete cycGaps_old;
    cycButtons = cycButtons_new;
    cycGaps = cycGaps_new;

    // Layout new widgets
    gogogo = new QPushButton("Go!", centralWidget());
    lay->addWidget(gogogo, 2+cycle+1, 0, 1, 3);
    QObject::connect(gogogo, SIGNAL(clicked()), this, SLOT(cycSubmit()), Qt::QueuedConnection);

    // Unfortunate Qt has a problem where adjustSize() won't do the right thing until after we resume the EventLoop, so schedule to do that then
    QTimer::singleShot(0, this, SLOT(refresh()));
}

void ChimeryMainWindow::refresh()
{
    adjustSize();
}

void ChimeryMainWindow::cycSubmit()
{
    cycs = -1;
    
    // Cleanup lists
    while(!cycButtons->isEmpty())
        delete cycButtons->takeFirst();
    cycButtons = new QLinkedList<QPushButton *>();
    while(!cycGaps->isEmpty())
        delete cycGaps->takeFirst();
    cycGaps = new QLinkedList<QSpinBox *>();

    chime();
}

void ChimeryMainWindow::chime()
{
    if(cycs == -1)
    {
        QGuiApplication::setQuitOnLastWindowClosed(false);
        this->hide();
        systray->show();
        menus->sched_start();
    }
    if(cycs != -1)
    {
        if(cycs % 2 == 0)
        {
            qDebug() << "START CHIME";
            begin->play();
            systray->setIcon(*active_icon);
        }
        else
        {
            qDebug() << "END CHIME";
            end->play();
            systray->setIcon(*passive_icon);
        }
    }
    if(times->isEmpty())
    {
        reinit();
        return;
    }
    cycs++;
    QDateTime * next = times->takeFirst();
    qint64 next_ms;
    next_ms = QDateTime::currentDateTime().msecsTo(*next);
    delete next;
    next = NULL;
    if(next_ms < 1)
    {
        qDebug() << "Time to chime is less than zero: " << next_ms << "ms";
        chime();
        return;
    }
    if(cycs == 0 && next_ms < 300000) /* If the first chime is within the next five minutes, just trigger the start chime now. */
    {
        qDebug() << "Start immed";
        chime();
        return;
    }
    qDebug() << "Cyc" << cycs << "- Chime in" << next_ms << "ms";
    timer->start(next_ms);
}

ChimeryMainWindow::~ChimeryMainWindow()
{
    delete cycNum;
    delete cycStart;
    delete cycDur;
    delete cycNumL;
    delete cycStartL;
    delete cycDurL;
    delete gapL;
    while(!cycButtons->isEmpty())
        delete cycButtons->takeFirst();
    delete cycButtons;
    while(!cycGaps->isEmpty())
        delete cycGaps->takeFirst();
    delete cycGaps;
    while(!times->isEmpty())
        delete times->takeFirst();
    delete times;
    delete timer;
    delete about;
    delete menus;
}
