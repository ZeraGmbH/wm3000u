// globaleWM definitionen

#ifndef WMGLOBAL_H
#define WMGLOBAL_H

#include <q3ptrlist.h>
#include <qstring.h>
//Added by qt3to4:
#include <Q3MemArray>

#include <QDir>

#include "confdata.h"
#include "complex.h"
#include "range.h"

//#define FVWM 1

#define TheDevice "127.0.0.1"
//#define TheDevice "10.0.2.16"
//#define TheDevice "192.168.6.129"

// V1.01 erste lauffähige version
// V1.02 wm3000scpiface geändert -> konfiguration abfrage, setzen besser synchronisiert
// V1.03 wm3000u schrittkette umgebaut, so dass sk messperiode selbst verwaltet und im dsp messperiode = signalperiode ist -> befehle werden schneller bearbeitet. das bedeutet aber mehr speicherbedarf im userworkspace deshalb muss die anzahl der messperioden auf 18 begrenzt werden wenn 256samples/per.
// V1.04 arbeitet mit der neuen version des leiterkartenservers V1.02 zusammen. Das justage handling wurde hier grundlegend geändert. bei der abfrage des korrekturwerte wird der hierzu erforderliche parameter mitgesendet, vorher wurde der parameter mit einem separaten befehl gesetzt. Die berechnung der justage koeffizienten erfolgt jetzt direkt im server und nicht mehr im device. das heisst dass auch im justageteil des devices einiges geändert werden musste. 
// V1.05 cethadress operator != verbessert,  
//            zusätzliche anzeige lastpunkt relativ zu X kanal eingebaut
//            andere kette an dsp übertragen um die benötigten werte zu erhalten
//            den html browser für die online doku entfernt
// V1.06 in confmenu alle /3 /w3 buttons verlinkt und den safety range für evt entfernt da separater eingang für evt geschaffen wurde. den validator (regexp) verändert für die ratio eingaben verbessert, und den winkelfehler in der xml ergebnisdatei mit rad ausgegeben. berechnung der lastpunkte auf primärgrössen umgestellt, für X wird der Lastpunkt über die über N abgeleitete primärgrösse bestimmt.
// V1.07 durch wegfall des safety range wurde der evt 15v bereich nicht mehr phasenjustiert. 
// dies wurde korrigiert
// V1.08 19.12.2011 complex klasse geändert. fehler in der mult. und div. routine
// V1.09 17.01.2012 	accelerator keys geändert weil  f4 von mindowmanager beeinflusst wurde
//	        	bei der dateiname eingabe für session management wird ein name vorbesetzt
//		das gleiche gilt für die xml datei auswahl für messwegebnisse speichern
// 		und ebenfalls für die eigenfehlertabelle
// leere eigefehlertabellen waren nicht editierbar , d.h. man konnte aus dem wm-programm heraus keine eigenfehler
// tabellen editieren. dies ist jetzt möglich und es wird eine beispiel eingabezeile eingefügt.
// RegExp für transformer ratio geändert -> es werden jetzt mV,V,kV und KEINE einheit akzeptiert, wichtig für checkcon 
// vektoranzeige contextmenu ergänzt umschaltung primär/sekundär , hierzu mussten auch die istwerte entsprechend berechnet werden, bei nConvent bleibt für x das wandlerverhältnis eingabefeld aktiv , weil wir jetzt hier die sekundär grössen berechnen müssen
// V1.10 30.01.2012 bereich handling komplett überarbeitet incl. schutzfunktion bei spannungen > 120% * 480V
// es wurde die hauptstate machine wm3000 dahingehend geändert, dass keine gleichartigen events kumulieren. etwaige performanceprobleme (die beim xscale tatsächlich vorhanden sind) fallen damit nicht mehr auf
// die ansi fehlerberechnung wurde korrigiert
// die max. anzahl messperiode für 256 samples musste auf 14 perioden reduziert werden (speichermangel im dsp)
// information ob justiert oder nicht in statuszeile, in xml bei ergebnisexport, in status system questionable bit 11
// V1.11 26.06.2012 winkel auflösung in xml datei auf 5 stellen erhöht
// V1.12 06.09.2012 bei der istwertabfrage über scpi lastpunkte auf kanal x umgestellt
// V1.13 17.09.2012 kontextmenu für istwertanzeige um mrad erweitert, bei start des programms immer bereichautomatik ein,
//                  bei ergebnisdatei, eigenfehlerdatei solte open anstatt save beim öffnen verwendet werden, nicht machbar unter qt3 wenn man default selection verwenden möchte.
// V1.14 20.09.2012 in eparameter scalefactors eingeführt sonst fehler wenn keine einheit und /3 oder /w3 button
// V1.15 19.11.2012 dsp loader file location changed
// V2.15 14.12.2012 version number changed for difference old/new device
// V2.16 11.04.2013 en61850-9-2 decoder: ethernetframe sendet 8 samples/frame wenn 256samples/periode. 2 eingabefelder erforderlich
//                  für asdu nummern. first asdu, last asdu. es wird versucht von der 1. bis zur letzten asdu alles zuzuordnen.
//                  sync. überwachung läuft auch mit. es ist dsp version ab 3.09 erforderlich für diese funktion.
// v2.17 11.12.2013 nach jeder "umkonfiguration" wird der fehlermesswertund der phasenwinkel
//                  "inaktiv" und erst wieder aktiviert wenn ein neuer messwert da ist
//                  wenn |samplefrequenz - signalfrequenz| > 1.0 dann hinweis in statuszeile
//                  nicht justiert rot in statuszeile
//                  jede änderung in der konfiguration oder der anzeigefenster wird sofort
//                  gespeichert, damit die letzte konf. auch nach abschalten erhalten wird
// v2.18 19.12.2013 programm erweitert für wm1000u (nur conventional und ect)
// v2.19 28.05.2014 fehler in eigenfehlerkorrektur beseitigt. die einträge in der tabelle wurden falsch soriert
//                  wodurch die interpolation im anschluss falsche werte lieferte

#define WMVersion "V2.19"

#define wm3000uHome QDir::homePath()
#define ServerCommLogFilePath "/usr/share/wm3000u/log/ServerComm.log"
#define SelftestLogFilePath "/usr/share/wm3000u/log/Selftest.log"
#define PhaseJustLogFilePath "/usr/share/wm3000u/log/PhaseJust.log"

const int nmaxRecentOEFiles = 5;
const int nmaxRecentMVFiles = 5;
const int nmaxRecentSESFiles = 5;
const float TDBase = 100.0e6; // 100 mhz auflösung für td messung (pps->1. sample)

enum DisplayModes {ANSI,IEC,MaxCompMode}; // compmodi
enum LPDisplayModes {totalRms, H1Rms}; // lastpunkt anzeige modi
enum VekAmplModes {x1, x1_SQRT2}; // x 1, x 1/sqrt(2)
enum VekPrimSekModes {prim, sek};
enum VekWinkelModes {mathpos, techpos}; // winkel anzeige math. pos. bzw. techn. pos.
enum SyncSources {Intern,Extern,MaxSSource}; // sync sources
enum SignalFreqs {F16,F50,F60,MaxFreq}; // -> feste abtastfrequenzen
enum SampleRates {S80,S256,MaxSRate}; // abtastraten
enum tsmode {sensNsensX, adcNadcX, sensNadcX, sensXadcN, adcXadcN = 5}; // testmodi innerhalb der hardware 
enum MeasModes {Un_UxAbs,Un_EVT,Un_nConvent,maxMMode}; // messmodi
enum UserDecisions {AbortProgram,Stop,Retry,SimulationMode}; // benutzer entscheidungen
enum Languages {de,gb}; 


class cTCPConfig 
{
    public:
    cTCPConfig(){};
    
    QString pcbHost, dspHost;
    uint pcbPort, dspPort;
};


class cDspVar; // forward

typedef Q3MemArray<float> cDspVarMemArray;
typedef Q3PtrList<cDspVar> cDspVarPtrList;


struct cDspActValues { // raw data, wie vom dsp geliefert
    float kfkorrf; // kreisfrequenz  korrektur koeffizient
    float rmsnf, ampl1nf;
    float rmsxf, ampl1xf;
    float dphif; // phix-phin gefiltert im bogenmaß -pi....+pi
    float tdsync; // zeit zwischen pps und 1'st sample auflösung 10nS 
    float phin, phix; // bogenmaß
};


struct cDspMaxValues { // raw data, die maxima
    float maxn, maxx, maxRdy;
};


struct cwmActValues {  // wird an andere objekte gesendet
    cDspActValues dspActValues;
    double TDSync;
    double Frequenz;
    double RMSN, RMSNSek;  // hier die je nach mode berechneten werte in SI einheiten primär, sekundär
    double RMSX, RMSXSek; // dito
    double RMSN1, RMSN1Sek;
    double RMSX1, RMSX1Sek;
    complex UInCorr;   // der zugehörige korrekturvektor für un od. in
    complex VekN, VekNSek;
    complex VekX, VekXSek;
    double LoadPoint, LoadPoint1; // Lastpunkt v. rms total, bzw. rms grundwelle
    double LoadPointX, LoadPoint1X; // Lastpunkt v. rms total, bzw. rms grundwelle kanal x
    double AmplErrorIEC; 
    double AmplErrorANSI;
    double AngleError;
    double PHIN;
    double PHIX;
    bool bvalid;
};
    

struct tVersSerial
{
    QString DeviceName;
    QString DeviceVersion;
    QString PCBSerialNr;
    QString PCBVersion;
    QString PCBServer;
    QString DSPVersion;
    QString DSPServer;
    QString JDataChksum;
};


class cPhaseCalcInfo
{
public:
    cPhaseCalcInfo(const QString chn, const QString rng)
	:m_sChannel(chn), m_sRange(rng){};
    QString m_sChannel;
    QString m_sRange;
};



class cPhaseNodeMeasInfo
{
public:
    cPhaseNodeMeasInfo(const QString rng0, const QString rng1, tsmode tm, MeasModes mm, int nS, int nIgn, int nMeas )
	:m_srng0(rng0), m_srng1(rng1), m_nTMode(tm), m_nmMode(mm), m_nnS(nS), m_nIgnore(nIgn), m_nnMeas(nMeas){};
    QString m_srng0; // bereich kanal n
    QString m_srng1; // bereich kanal x
    tsmode m_nTMode; // test mode (was zu testen bzw. justieren ist)
    MeasModes m_nmMode; // in welchem messmodus
    int m_nnS; // samples pro periode
    int m_nIgnore; // anzahl messungen zum einschwingen
    int m_nnMeas; // anzahl messungen zur messwertbestimmung
};

typedef Q3PtrList<CWMRange> cWMRangeList;
typedef Q3PtrList<cPhaseCalcInfo> cPhaseCalcInfoList;
typedef Q3PtrList<cPhaseNodeMeasInfo> cPhaseNodeMeasInfoList;
#endif

