#include "dspactvalues.h"


cDspActValues::cDspActValues()
{
}


cDspActValues cDspActValues::operator = (cDspActValues dspVals)
{
    kfkorrf = dspVals.kfkorrf;
    rmsnf = dspVals.rmsnf;
    ampl1nf = dspVals.ampl1nf;
    rmsxf = dspVals.rmsxf;
    ampl1xf = dspVals.ampl1xf;
    dphif = dspVals.dphif;
    tdsync = dspVals.tdsync;
    phin = dspVals.phin;
    phix =dspVals.phix;

    return *this;
}
