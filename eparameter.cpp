// implementierung eParameter
// zur überprüfung von einheiten behafteter größen
// und zum wandeln derselben


#include <qdatastream.h>
#include "eparameter.h"


eUnit ScaleFactor[AnzScaleFactors] = { eUnit("/3", 0.33333333),
                                       eUnit("/w3", 0.57735026)};


eUnit CurrentUnit[AnzCurrentUnits] ={ eUnit("kA", 1.0e3),
		                            eUnit("mA", 1.0e-3),
		                            eUnit("uA", 1.0e-6),
		                            eUnit("A", 1.0) };

eUnit VoltageUnit[AnzVoltageUnits] = { eUnit("MV", 1.0e6),
				  eUnit("kV/3", 333.33333),
				  eUnit("kV/w3", 577.3502692),
				  eUnit("kV", 1.0e3),
				  eUnit("V/3", 0.333333333),
		                              eUnit("V/w3", 0.57735027),
                                                          eUnit("mV/3", 0.333333333e-3),      
				  eUnit("mV/w3", .57735027e-3),
		                              eUnit("mV", 1.0e-3),
		                              eUnit("uV", 1.0e-6),
		                              eUnit("V", 1.0) };
	
eUnit LoadpointUnit[AnzLoadPointUnits] = { eUnit("%", 1.0e-2),
    		                                    eUnit(QString(QChar(8240)), 1.0e-3) }; // promille

eUnit ErrorUnit[AnzErrorUnits] = { eUnit("%", 1.0e-2),
		                      eUnit(QString(QChar(8240)), 1.0e-3), // promille
		                      eUnit("ppm", 1.0e-6) }; 

eUnit AngleUnit[AnzAngleUnits] = { eUnit(QString(QChar(176)), 1.0),
		                        eUnit("'", 0.01666666),
		                        eUnit("crad", 0.572957795),
                                eUnit("mrad",0.0572957795),
                                eUnit("urad", 0.0000572957795),
		                        eUnit("grad", 1.0),
			          eUnit("rad",57.2957795),};

eUnit RCFUnit[AnzRCFUnits] = { eUnit("", 1.0) };

QDataStream& operator << (QDataStream& ds, eUnit& eu)
{
    ds << eu.Name << eu.fak;
    return ds;    
}
 

QDataStream& operator >> (QDataStream& ds, eUnit& eu)
{
    ds >> eu.Name >> eu.fak;
    return ds;
}


void eParameter::operator =(QString s) {
  eP = s;   
  Unit = 0;
}

bool eParameter::Test(eUnit* eu, int n)
{
    for (int i=0; i<n; i++,eu++)
    {
        if (eP.contains(eu->Name))
        {
            bool ok;
            QString teststr = eP;
            teststr.remove(eu->Name);
            teststr.toDouble(&ok);
            if (ok)
            {
                Unit = eu;
                return true;
            }
        }
    }
    return false;
}

bool eParameter::isCurrent() {
    Unit = 0;
    return Test(CurrentUnit, sizeof(CurrentUnit)/sizeof(eUnit));
}
 

bool eParameter::isVoltage() {
    Unit = 0;
    return Test(VoltageUnit, sizeof(VoltageUnit)/sizeof(eUnit));
}


bool eParameter::isLoadPoint() {
    Unit = 0;
    return Test(LoadpointUnit, sizeof(LoadpointUnit)/sizeof(eUnit));
}


bool eParameter::isError() {
    Unit = 0;
    return Test(ErrorUnit, sizeof(ErrorUnit)/sizeof(eUnit));
}
 

bool eParameter::isAngle() {
    Unit = 0;
    return Test(AngleUnit, sizeof(AngleUnit)/sizeof(eUnit));
}
    

bool eParameter::withoutUnit() {
    bool t;
    eP.toDouble(&t);
    return t;
}


double eParameter::toDouble(bool* ok) {
    QString s = eP;
    if (Unit != 0) {
	s.remove(Unit->Name);
	s.stripWhiteSpace();
	return (s.toDouble(ok)*(Unit->fak));
    }
    else
    {
	if (isCurrent()) return toDouble(ok);
	if (isVoltage()) return toDouble(ok);
	if (isLoadPoint()) return toDouble(ok);
	if (isError()) return toDouble(ok);
	if (isAngle()) return toDouble(ok);
    if (isScaleFactor()) return toDouble(ok);
	return (s.toDouble(ok)); // im zweifelsfalle für den angeklagten
    }
}


bool eParameter::isScaleFactor()
{
    Unit = 0;
    return Test(ScaleFactor, sizeof(ScaleFactor)/sizeof(eUnit));
}

