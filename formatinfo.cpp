// implementierung cFormatInfo

#include "formatinfo.h"

cFormatInfo::cFormatInfo(uchar fb, uchar al, eUnit ei)
{
    FieldWidth = fb;
    Resolution = al;
    UnitInfo = ei;
}
    

QDataStream& operator << (QDataStream& ds, cFormatInfo& fi)
{
    ds << fi.FieldWidth << fi.Resolution << fi.UnitInfo;
    return ds;
}


QDataStream& operator >> (QDataStream& ds, cFormatInfo& fi)
{
    ds >> fi.FieldWidth >> fi.Resolution >> fi.UnitInfo;
    return ds;
}
