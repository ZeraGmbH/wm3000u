//  headerdatei confdata.h
// definition der konfiguarationsdaten ...durch zusammenfassung 
// besser als signal verschickbar und übersichtlicher

#ifndef CONFDATA_H
#define CONFDATA_H

#include <qptrlist.h>
#include <qstring.h>

#include "ethadress.h"

const int ConfVersion = 8;

class cConfData
{
public:
    cConfData(){};
    
    void serialize(QDataStream&);
    bool deserialize(QDataStream&);
    
    int m_nVersion; // die Version steht hier
    bool m_bSimulation; // ist simulation oder nicht
    bool m_bRunning; // läuft oder läuft nicht
    bool m_bOECorrection; // eigenfehlerkorrektur ein/aus
    bool m_bOENewLoad; // true wenn eigenfehler geladen oder editiert wurden
    int m_nMeasMode; // mess modus
    int m_nMeasPeriod; // wieviele perioden eine messung dauert
    int m_nIntegrationTime; // nach ablauf dieser zeit wird filter im dsp ausgekoppelt
    int m_nSyncSource; // synchronisations quellen
    int m_nTSync; // T [ms] für internes sync.-timing
    int m_nSFreq; // abtastfrequenz 16 2/3, 50, 60 Hz enum
    float m_fSFreq; // signal frequenzwert float
    int m_nSRate; // abtastrate 80 bzw 256 samples /periode 
    int m_nTMode; // der aktuelle testmode normal 0 !!! wird nicht gespeichert !!!!!!
    int FirstASDU, LastASDU; // logical node (61850-9-2)
    int DataSet; // data set in ASDU
    int Language;
    long m_nLogFileMax; // max. logfile grösse
    double m_fxPhaseShift; // phasenkorrekturwert für prüfling
    double m_fxTimeShift; // totzeitkorrekturwert für prüfling 
    cETHAdress m_MacSourceAdr, m_MacDestAdr; // mac adressen für kommunikation mit mergingunit
    ulong m_nPriorityTagged;
    ulong m_nEthTypeHeader;
    bool m_bStrongEthSynchronisation;
    QString m_sOETFile; // filename der eigenfehlertabelle
    QString m_sResultFile; // filename der ergebnisdatei
    // bereiche für übersetzungsverhältnisse
    QString m_NPrimary,m_NSecondary;  // normal wandler primär, sekundär werte
    QString m_XPrimary,m_XSecondary; // x wandler (prüfling)
    QString m_EVTPrimary,m_EVTSecondary; // ect (rogowsky)
    // anwählbare bereiche 
    QString m_sRangeN,m_sRangeNSoll,m_sRangeNVorgabe; // bereichN gesetzt, soll, gewählt
    QString m_sRangeX,m_sRangeXSoll,m_sRangeXVorgabe; // bereichDUT gesetzt, soll, gewählt
    QString m_sRangeEVT,m_sRangeEVTSoll,m_sRangeEVTVorgabe;  // bereichECT  gesetzt, soll, gewählt
    QString m_sADSPFile;
};

#endif
