// definition wmtimer

#ifndef WMTIMER_H
#define WMTIMER_H

#include <qobject.h>
#include <qtimer.h>

#include "wmglobal.h"

class cWMTimer: public QTimer
{
    Q_OBJECT
    
public:
    cWMTimer(QObject *parent = 0, const char *name = 0);
    bool timeOut();
    int start (int msec,bool sshot = FALSE ); 
    
private:
    bool timeout;
    
private slots:
    void timerDone();
};
    
#endif

   
