// headerdatei für class cReanimate

#ifndef REANIMATE_H
#define REANIMATE_H

#include <qobject.h>
#include <qvaluelist.h>


class cReanimate: public QObject
{
    Q_OBJECT
	    
public:
    cReanimate(){};
    ~cReanimate(){};
    int PushAction(int); // eine action ins fifo rückgabe anzal der einträge
    
public slots:
    void ReanimateSlot(void);
    
signals:
    void SendActionSignal(int); // sendet eine action wenn eine da ist und timer 0 durchgang 
    
private:
    QValueList<int> mActionList;
};

#endif
