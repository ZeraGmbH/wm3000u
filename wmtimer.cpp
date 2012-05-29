// implementierung cWMTimer


#include "wmtimer.h"


cWMTimer::cWMTimer(QObject *parent, const char *name)
    :QTimer(parent,name)
{
    QObject::connect(this,SIGNAL(timeout()),this,SLOT(timerDone()));
}


bool cWMTimer::timeOut()
{
    return(timeout);
}


int cWMTimer::start (int msec,bool sshot)
{
    this->QTimer::stop();
    timeout = false;
    return (this->QTimer::start(msec,sshot));
}
    
void cWMTimer::timerDone()
{
    timeout = true;
}
