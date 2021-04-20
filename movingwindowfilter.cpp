#include "movingwindowfilter.h"


cMovingWindowFilter::cMovingWindowFilter()
{
    m_nLen = 1; // default 1 eintrag
}


void cMovingWindowFilter::setFilterLength(int len)
{
    m_nLen = len;
    m_actvalFifo.clear();
}


void cMovingWindowFilter::append(cDspActValues dspactualvalues)
{
    if (m_actvalFifo.size() == m_nLen)
        m_actvalFifo.removeFirst();
    m_actvalFifo.append(dspactualvalues);
}


cDspActValues cMovingWindowFilter::getOutput()
{
    int actSize;

    actSize = m_actvalFifo.size();

    m_FilterResult.rmsnf = 0.0;
    m_FilterResult.ampl1nf = 0.0;
    m_FilterResult.rmsxf = 0.0;
    m_FilterResult.ampl1xf = 0.0;
    m_FilterResult.dphif = 0.0;

    for (int i = 0; i < actSize; i++)
    {
        m_FilterResult.rmsnf += m_actvalFifo.at(i).rmsnf;
        m_FilterResult.ampl1nf += m_actvalFifo.at(i).ampl1nf;
        m_FilterResult.rmsxf += m_actvalFifo.at(i).rmsxf;
        m_FilterResult.ampl1xf += m_actvalFifo.at(i).ampl1xf;
        m_FilterResult.dphif += m_actvalFifo.at(i).dphif;
    }

    m_FilterResult.rmsnf /= actSize;
    m_FilterResult.ampl1nf /= actSize;
    m_FilterResult.rmsxf /= actSize;
    m_FilterResult.ampl1xf /= actSize;
    m_FilterResult.dphif /= actSize;

    // die werte sind nicht gefiltert
    m_FilterResult.kfkorrf = m_actvalFifo.at(actSize-1).kfkorrf;
    m_FilterResult.tdsync = m_actvalFifo.at(actSize-1).tdsync;
    m_FilterResult.phin = m_actvalFifo.at(actSize-1).phin;
    m_FilterResult.phix = m_actvalFifo.at(actSize-1).phix;

    return m_FilterResult;
}

