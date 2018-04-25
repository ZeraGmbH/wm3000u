// headerdatei eparameter.h

#ifndef EPARAMETER_H
#define EPARAMETER_H

#include <qstring.h>

enum ScaleFactors { d3, dw3, AnzScaleFactors};
enum ErrorUnits {ErrProzent, ErrPromill, Errppm, AnzErrorUnits};
enum VoltageUnits {VoltMV, VoltkV3, VoltkVw3, VoltkV, VoltV3, VoltVw3, VoltmV3, VoltmVw3,VoltmV, VoltuV, VoltV, AnzVoltageUnits};
enum CurrentUnits {CurrkA, CurrmA, CurruA, CurrA, AnzCurrentUnits};
enum LoadpointUnits {LPProzent, LPPromill, AnzLoadPointUnits};
enum AngleUnits { Anglegrad, Anglemin , Anglecrad, Anglemrad, Angleurad, Anglegrad2, Anglerad , AnzAngleUnits};
enum RCFUnits { nix, AnzRCFUnits};

class eUnit { 
public:    
    eUnit(){};
    eUnit(QString s, double d)
	:Name(s), fak(d){};
    friend QDataStream& operator << (QDataStream& ds, eUnit& eu);
    friend QDataStream& operator >> (QDataStream& ds, eUnit& eu);
    QString Name;
    double fak;
};


class eParameter	{
public:
    eParameter(QString s) {eP = s;Unit=0;}; 
    eParameter(){Unit=0;};
    ~eParameter(){};
    
    void operator = (QString);
    bool isCurrent();
    bool isVoltage();
    bool isLoadPoint();
    bool isError();
    bool isAngle();
    bool withoutUnit();

    double toDouble(bool *ok = 0);
private:
    bool isScaleFactor();
    eUnit* Unit;
    QString eP;
    bool Test(eUnit*, int);
};


#endif
