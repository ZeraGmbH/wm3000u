// definition CRange

#include "range.h"


CRange CRange::operator=(const QString rn)
{  
    m_sRangeName=rn;
    m_fRangeValue=str2double(rn);
    return (*this);
}

CRange CRange::operator=(const char* rn)
{ 
    m_sRangeName=rn;
    m_fRangeValue=str2double(rn);
    return (*this);
}


double CRange::str2double(const QString s)
{
    QString ls=s;
    ls.replace ('V',"",false) ; // einheiten abschneiden
    ls.replace ('A',"",false) ;
    return(ls.toDouble());
}


CWMRange::CWMRange(QString rn, QString rs, double rv, double rj, double ll, const QString key)
    :CRange(rn,rs,rv)
{
    m_fRejection = rj;
    m_fLineartiyLimit = ll;
    m_sOffsetKorrKey = key;
}    


double CWMRange::Rejection()
{
    return(m_fRejection);
}


double CWMRange::LinearityLimit()
{
    return(m_fLineartiyLimit);
}


QString CWMRange::getOffsKorrKey()
{
    return m_sOffsetKorrKey;
}
