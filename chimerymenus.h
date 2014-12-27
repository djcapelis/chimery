#ifndef CHIMERYMENUS_H
#define CHIMERYMENUS_H

#include<QMenuBar>
#include<QMenu>
#include<QAction>
#include<QKeySequence>

#include"chimerymainwin.h"
#include"chimeryaboutwin.h"

// Forward decls
class ChimeryMainWindow;
class ChimeryAboutWindow;
class ChimeryMenus;

class ChimeryMenus : public QMenuBar
{
    Q_OBJECT

public:
    explicit ChimeryMenus(ChimeryMainWindow * mainwin, ChimeryAboutWindow * aboutwin);
    QMenu * menu;
    ~ChimeryMenus();

public slots:
    void sched_stop();
    void sched_start();

private:
    QAction * sched;
    QAction * about;
    ChimeryMainWindow * mainwin;
    ChimeryAboutWindow * aboutwin;
};

#endif // CHIMERYMENUS_H
