// definition cOwnError

#include <qfile.h>
#include <qmessagebox.h>
#include <qfileinfo.h>
//Added by qt3to4:
#include <Q3TextStream>
#include <Q3ValueList>
#include <math.h>

#include "ownerror.h"
#include "wmoeviewbase.h"
#include "eparameter.h"
#include "complex.h"

const double PI_180 = 1.74532925e-2;

cOwnError::cOwnError(QObject* parent)
{
    pa = parent;
    m_pViewData = new cOwnErrorViewData();
    m_pViewData->m_bValid = false;
    m_sOEFileName = "";
}
		

cOwnError::~cOwnError()
{
    delete m_pViewData;
}

struct tepOEEntry {
    eParameter Prim;
    eParameter Sek;
    eParameter LP;
    eParameter BetragF;
    eParameter WinkelF;
};

struct tOEKoefficient {
    double OELP;
    double OEBetragF;
    double OEWinkelF;
};


complex cOwnError::GetOECorrVector() 
{
    return OEVector;
}

complex cOwnError::CmpOECorrVector()
{
    complex k(1.0,0.0); // keine drehung
    if ((m_pViewData->m_bValid = m_ConfData.m_bOECorrection)) {
	double betrag, winkel;
	QStringList relEntriesList;
        if ((m_pViewData->m_bValid = !m_sOEEntriesList.empty())) { // es gibt gültige einträge in der verwendeten datei
	    QString searchEntry = m_ConfData.m_NPrimary+";"+m_ConfData.m_NSecondary+";";
	    for ( QStringList::Iterator it = m_sOEEntriesList.begin(); it != m_sOEEntriesList.end(); ++it ) {
		QString s = *it;
		if (s.contains(searchEntry)) relEntriesList.push_back(s);
	    }
            if ((m_pViewData->m_bValid = (relEntriesList.count() > 0))) { // gab es einträge für das gewählte ü-verhältnis ?
		Q3ValueList<tOEKoefficient> OEKoefficientList; // eine liste für koeffizienten
		tOEKoefficient OEKoefficient; 
		tepOEEntry epEntries;
		for ( QStringList::Iterator it = relEntriesList.begin(); it != relEntriesList.end(); ++it ) {
		    eParameter *ep = &epEntries.Prim;
		    for (int i=0; i<5; i++,ep++)  *ep = (*it).section(';',i,i);
		    OEKoefficient.OELP = epEntries.Sek.toDouble() * epEntries.LP.toDouble() ;
		    OEKoefficient.OEBetragF = epEntries.BetragF.toDouble();
		    OEKoefficient.OEWinkelF = epEntries.WinkelF.toDouble();
		    OEKoefficientList.append(OEKoefficient);
		}
		int n = OEKoefficientList.count();
		if (n > 1) { // wenn mehr als 1 eintrag -> sortieren wir die liste nach lastpunkt
		    for (int i=0; i < (n-1); i++)
            for (int j=i+1; j<n; j++)
			    if (OEKoefficientList[j].OELP < OEKoefficientList[i].OELP ) {
			             OEKoefficient = OEKoefficientList[j];
			             OEKoefficientList[j] = OEKoefficientList[i];
		                           OEKoefficientList[i] = OEKoefficient;
				       }
		}
		
		
		
		tOEKoefficient oek = {0.0,0.0,0.0};
		OEKoefficientList.push_front(oek); // einen startkoeffizienten falls es nur 1 eintrag gibt
		int lower = 0;
		int upper = 1;
		n = OEKoefficientList.size();
		if (n > 2 ) {
		    for (int i=1; i<n; i++) {
			if (m_ActValues.RMSNSek > OEKoefficientList[i].OELP) {
			    lower++;
			    upper++;
			}
			else break;
		    }
		}
		
		if ( upper > (n-1) ) { // der lastpunkt liegt oberhalb des letzten eintrags
		    upper--;
		    lower--;
		}
		
		double LPdiff = OEKoefficientList[upper].OELP - OEKoefficientList[lower].OELP;
		betrag = ( (OEKoefficientList[upper].OEBetragF - OEKoefficientList[lower].OEBetragF) / LPdiff) * 
		                (m_ActValues.RMSNSek - OEKoefficientList[lower].OELP) +  OEKoefficientList[lower].OEBetragF;
		
		winkel = ( (OEKoefficientList[upper].OEWinkelF - OEKoefficientList[lower].OEWinkelF) / LPdiff) * 
		               (m_ActValues.RMSNSek- OEKoefficientList[lower].OELP) +  OEKoefficientList[lower].OEWinkelF;
		
		double betragk = 1.0/(1.0+betrag);
		k = complex (cos(-winkel*PI_180)*betragk,sin(-winkel*PI_180)*betragk);
	    }
	}
	
	if (m_pViewData->m_bValid) { // jetzt wenn gültig noch die anzeigenwerte besetzen
	    m_pViewData->m_sAmpl = QString("%1%").arg(100.0*betrag,8,'f',4);
                  m_pViewData->m_sPhase = QString("%1%2").arg(winkel,8,'f',4).arg( trUtf8("°"));
	}
    }
    
    m_pViewData->m_sLoad = QString("%1%").arg(m_ActValues.LoadPoint,7,'f',3);
    
    emit SendOEViewData(m_pViewData);
    return k;
}


QStringList& cOwnError::GetNPrimList()
{
    return m_sNPrimList;
}


QStringList& cOwnError::GetNSekList()
{
    return m_sNSekList;
}


void cOwnError::SetConfInfoSlot(cConfData *cd)
{
    m_ConfData=*cd; // daten jetzt übernehmen
    
    m_pViewData->m_sPrim = m_ConfData.m_NPrimary; // wandler stufen übernehmen
    m_pViewData->m_sSec  = m_ConfData.m_NSecondary;
    
    // kann sein dass neue datei geladen wurde, bzw. alte datei editiert wurde
    if (m_ConfData.m_bOENewLoad) {
	m_sOEFileName = m_ConfData.m_sOETFile;
	m_pViewData->m_sTrName = strippedName(m_sOEFileName).section('.',0,0);
	m_sOEEntriesList.clear(); // alle einträge löschen
	m_sNPrimList.clear();
	m_sNSekList.clear();
	QFile file(m_ConfData.m_sOETFile);
	if ((m_ConfData.m_sOETFile != "") && file.open(QIODevice::ReadOnly)) {
	    Q3TextStream stream(&file);
	    QString line;
	    while (!stream.atEnd()) {
		line=stream.readLine(); // liesst eine zeile
		if (!line.isEmpty()) { // es steht was in der zeile 
		    if ( isValidEntry(line) ) { // die zeile ist vollständig und ok
			m_sOEEntriesList.push_back(line); // zeile in die liste
			QStringList words = QStringList::split(";",line); // zeile in teilworte zerlegen
			line = words.first();
			if (m_sNPrimList.find(line) == m_sNPrimList.end())
			    m_sNPrimList.push_back(line);
			words.pop_front(); // 1. eintrag entfernen
			line = words.first();
			if (m_sNSekList.find(line) == m_sNSekList.end())
			    m_sNSekList.push_back(line);
		    }
		    else
		    {
			// fehlerbehandlung (nur benutzer hinweis)
		    
			QMessageBox::warning( 0, tr("Eigenfehlertabelle"),
					            tr("Fehler in %1\n"
					            "Zeile : %2\n").arg(strippedName(m_ConfData.m_sOETFile)).arg(line),
					           QMessageBox::Ok,
					           Qt::NoButton,
					           Qt::NoButton);
			break;
		    }
		}
	    }
	}
    } // neue eigenfehlertabelle gelesen
    emit SendAnalizeDone();
}


QString cOwnError::strippedName(QString s)
{
   QFileInfo fi(s); 
   return(fi.fileName());
}


void cOwnError::SetActualValuesSlot(cwmActValues *av)
{
    m_ActValues=*av;
    OEVector = CmpOECorrVector();
}


bool cOwnError::isValidEntry(QString s) 
{
    QStringList words;
    eParameter eP;
    words=QStringList::split(";",s); // zeile in teilworte zerlegen
    if (words.size() != 5) return false; // 5 einträge erforderlich
    eP = words.first();
    if ( !(eP.isVoltage() || eP.withoutUnit()) ) return false; // der 1. eintrag muss eine spannung sein
    words.pop_front();
    
    eP = words.first();
    
    if ( !(eP.isVoltage() || eP.withoutUnit()) ) return false; // der 2. auch
    words.pop_front();
    
    eP = words.first();
    if ( !(eP.isLoadPoint() || eP.withoutUnit()) ) return false; // %
    words.pop_front();
    
    eP = words.first();
    if ( !(eP.isError() || eP.withoutUnit())) return false; // %, ppm, ....
    words.pop_front();
    
    eP = words.first();
    if ( !(eP.isAngle() || eP.withoutUnit()) ) return false; // °,' ....
    words.pop_front();
    
    return true;
}
