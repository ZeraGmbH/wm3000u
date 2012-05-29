// implementation tools
#include <stdlib.h>
#include <qobject.h>
#include <math.h>
#include "tools.h"

const double PI2 = 6.283185307;
const double PI = 3.141592654;
const double PI_2 = 1.570796327;


double normWinkelrad02PI(double w)
{
   
    double quo;
    double rem;
    
    
    quo=w / PI2;
    rem=quo - (int)quo;
    w = rem * PI2;
    if (w < 0.0)
	w += PI2;
    
/*    while ( w > PI2 )
	w -=PI2;
    while ( w < 0 )
	w +=PI2;
*/	
    return w;
}


double normWinkelrad_PIPI(double w)
{
    double w2 = normWinkelrad02PI(w);
    if (w2 > PI)
	w2 -= PI2;
    return w2;
}


double UserAtan(double im, double re)
{
    double w;
    if (re == 0.0) w = PI_2; else w = atan(im/re);
    if (re < 0.0) w += PI;
    if (w < 0.0) w += PI2;
    return w;
}


double UserAtan(complex c)
{
    return UserAtan(c.im(),c.re());
}


double sign(double arg)
{
    return (arg < 0.0) ? -1.0 : 1.0;
}

	
char* sAlloc(QString& s) // allokiert speicher für inhalt von qstring	
{
    char* r = (char*) malloc (s.length()+1); 
    strcpy(r, s.latin1());
    return r;
}


cSMTimer::cSMTimer()
{
//    m_lTimer = new QTimer();
    connect(this,SIGNAL(timeout()),this,SLOT(TimeExpiredSlot()));
}


cSMTimer::~cSMTimer()
{
//    delete m_lTimer;
}


void cSMTimer::start (int ms, int act)
{
    SMTimerData td; // wir setzen den context in die liste 
    td.t  = ms;
    td.Action = act;
    m_TimerDataList.push_back(td);
    
    if (!isActive())   // wenn der timer nicht mehr aktiv ist starten wir ihn direkt
    {
	td = m_TimerDataList.first();
	m_TimerDataList.pop_front();
	m_TimerEventList.push_back(td); // was gestartet ist kommt in die eventliste
	QTimer::start(td.t,true);
    }
}   


void cSMTimer::TimeExpiredSlot()
{
    SMTimerData td;
    td = m_TimerEventList.first();
    m_TimerEventList.pop_front();
    emit timeout(td.Action);
    
    if (m_TimerDataList.count() > 0) // wir haben noch was in der liste
    {
	if (!isActive()) 
	{
	    td = m_TimerDataList.first();
	    m_TimerDataList.pop_front();
	    m_TimerEventList.push_back(td); // was gestartet ist kommt in die eventliste
	    QTimer::start(td.t,true);
	}
    }
}

