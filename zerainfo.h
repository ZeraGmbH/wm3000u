// headerfile für zera info dienste 

#ifndef ZERAINFO_H
#define ZERAINFO_H

#include <qobject.h>

class cZeraInfo:public QObject
{
    Q_OBJECT
    
public slots:
    void AboutZeraSlot();
    void AboutWM3000Slot();
};

#endif	
