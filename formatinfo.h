// headerdatei formatinfo.h

#ifndef FORMATINFO_H
#define FORMATINFO_H

#include <qdatastream.h>
#include "eparameter.h"

class cFormatInfo
{
public:
    cFormatInfo(){};
    cFormatInfo(uchar, uchar, eUnit);
    friend QDataStream& operator << (QDataStream&, cFormatInfo&);
    friend QDataStream& operator >> (QDataStream&, cFormatInfo&);
    uchar FieldWidth;
    uchar Resolution;
    eUnit UnitInfo;
};

#endif
