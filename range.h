//  headerdatei range.h

#ifndef RANGE_H
#define RANGE_H

#include <qstring.h>

class CRange;

class CRange // ein bereich hat immer einer namen, einen selektor und einen wert
{
public:
    CRange(){};
    CRange(const QString rn, const QString rs,double rv) :m_sRangeName(rn), m_sRangeSelector(rs), m_fRangeValue(rv) {}
    
    QString& Name(){return(m_sRangeName);}
    QString& Selector(){return(m_sRangeSelector);}
    double Value(){return(m_fRangeValue);}
    
    CRange operator=(const QString);
    CRange operator=(const char*);
    
private:
    QString m_sRangeName; // so wird er im programm verwendet
    QString m_sRangeSelector; // dies dient zur auswahl beim server
    double m_fRangeValue;

    double str2double(const QString);
};    
	

class CWMRange: public CRange // ein wm bereich hat darüber hinaus eine aussteuerung für 100%
{ // und ein linearitätslimit für die bereichautomatik
public:     
    CWMRange(const QString,const QString,double,double,double, const QString);
    double Rejection();
    double LinearityLimit(); 
    QString getOffsKorrKey();

private:    
    double m_fRejection;
    double m_fLineartiyLimit;
    QString m_sOffsetKorrKey;
} ;   


#endif    
