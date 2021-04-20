#ifndef MOVINGWINDOWFILTER_H
#define MOVINGWINDOWFILTER_H

#include <QList>
#include "wmglobal.h"

class cMovingWindowFilter
{
public:
    cMovingWindowFilter();

    void setFilterLength(int len);
    void append(cDspActValues dspactualvalues);
    cDspActValues getOutput();

private:
    int m_nLen;
    QList<cDspActValues> m_actvalFifo;
    cDspActValues m_FilterResult;
};


#endif // MOVINGWINDOWFILTER_H
