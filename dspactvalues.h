#ifndef DSPACTVALUES_H
#define DSPACTVALUES_H

class cDspActValues  // raw data, wie vom dsp geliefert
{
public:
    cDspActValues();

    cDspActValues operator = (cDspActValues dspVals);

    float kfkorrf; // kreisfrequenz  korrektur koeffizient
    float rmsnf, ampl1nf;
    float rmsxf, ampl1xf;
    float dphif; // phix-phin  gefiltert im bogenmaß -pi....+pi
    float tdsync; // zeit zwischen pps und 1'st sample auflösung 10nS
    float phin, phix; // bogenmaß
};


class cDspFastRMSValues  // raw data, wie vom dsp geliefert
{
public:
    cDspFastRMSValues(){}

    float fastRMSN;
    float fastRMSN1;
    float fastRMSX;
    float fastRMSX1;
};

#endif // DSPACTVALUES_H
