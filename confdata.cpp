// implementierung cConfData

#include <qdatastream.h>
#include "wmglobal.h"
#include "confdata.h"
 

void cConfData::setConfVersion()
{
    m_nVersion = ConfVersion10;
}

void cConfData::serialize(QDataStream& ts)
{
    ts << m_nVersion << (int)m_bSimulation << (int)m_bRunning << (int)m_bOECorrection << (int)m_bOENewLoad;
    ts << m_nMeasMode << m_nMeasPeriod << m_nIntegrationTime<< m_nSyncSource;
    ts << m_nTSync << m_nSFreq << m_nSRate << FirstASDU << LastASDU << DataSet;
    ts << m_nLogFileMax << m_fxPhaseShift << m_fxTimeShift;
    ts << m_MacSourceAdr << m_MacDestAdr;
    ts << m_sOETFile << m_sResultFile; 
    ts << m_NPrimary << m_NSecondary << m_XPrimary << m_XSecondary;
    ts << m_EVTPrimary << m_EVTSecondary;
    ts << m_sRangeN << m_sRangeNSoll << m_sRangeNVorgabe;
    ts << m_sRangeX << m_sRangeXSoll << m_sRangeXVorgabe;
    ts << m_sRangeEVT << m_sRangeEVTSoll << m_sRangeEVTVorgabe;
    ts << m_sADSPFile;
    ts << m_nPriorityTagged;
    ts << m_nEthTypeHeader;
    ts << (int) m_bStrongEthSynchronisation;
    ts << Language;
    ts << (int)m_bDCmeasurement;
    ts << (int)m_bOffsetCorrectionN;
    ts << (int)m_bOffsetCorrectionX;
}


bool cConfData::deserialize(QDataStream& ts)
{
    int tmp;
    bool ret;
    
    ts >> tmp;
    if ((ret = (tmp >= ConfVersion8)))
    {
        int s,r,o,ol;
        ts >> s >> r >> o >> ol;
        m_bSimulation = s; m_bRunning = r; m_bOECorrection = o; m_bOENewLoad = ol;
        ts >> m_nMeasMode >> m_nMeasPeriod >> m_nIntegrationTime>> m_nSyncSource;
        ts >> m_nTSync >> m_nSFreq >> m_nSRate >> FirstASDU >> LastASDU >> DataSet;
        ts >> m_nLogFileMax >> m_fxPhaseShift >> m_fxTimeShift;
        ts >> m_MacSourceAdr >> m_MacDestAdr;
        ts >> m_sOETFile >> m_sResultFile;
        ts >> m_NPrimary >> m_NSecondary >> m_XPrimary >> m_XSecondary;
        ts >> m_EVTPrimary >> m_EVTSecondary;
        ts >> m_sRangeN >> m_sRangeNSoll >> m_sRangeNVorgabe;
        ts >> m_sRangeX >> m_sRangeXSoll >> m_sRangeXVorgabe;
        ts >> m_sRangeEVT >> m_sRangeEVTSoll >> m_sRangeEVTVorgabe;
        ts >> m_sADSPFile;
        ts >> m_nPriorityTagged;
        ts >> m_nEthTypeHeader;
        int t;
        ts >> t;
        m_bStrongEthSynchronisation = (bool) t;
        ts >> Language;
        m_bOENewLoad = (m_sOETFile != "") ; // falls schon eine datei geladen war -> mitteilen
    }

    if (tmp == ConfVersion9 || tmp == ConfVersion10)
    {
        int dc;
        ts >> dc;
        m_bDCmeasurement = dc;
    }

    if (tmp == ConfVersion10)
    {
        int yes;
        ts >> yes;
        m_bOffsetCorrectionN = yes;
        ts >> yes;
        m_bOffsetCorrectionX = yes;
    }

    return ret;
}
