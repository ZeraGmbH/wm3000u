// implementation cETHAdress

#include "ethadress.h"

bool cETHAdress::operator == (const cETHAdress& eadr) 
{
    bool equal = true;
    for (int i =0; i < 6; i++) equal &= (eadr.MacAdrByte[i] == this->MacAdrByte[i]);
    return equal;
}



bool cETHAdress::operator != (const cETHAdress& eadr) 
{
    bool ne = false;
    for (int i =0; i < 6; i++) ne |= (eadr.MacAdrByte[i] != this->MacAdrByte[i]);
    return ne;
}


QString cETHAdress::GetMacAdress()
{
    int i;
    QString s="";
    
    for (i = 0; i < 5; i++)
	s += QString("%1-").arg(MacAdrByte[i],0,16);
    s += QString("%1").arg(MacAdrByte[i],0,16);
    
    return s;
}


bool cETHAdress::SetMacAdress(QString& s)
{
    int i;
    QString ls;
    ushort us;
    cETHAdress leth;
    for (i = 0; i < 6 ; i++)
    {
	ls = s.section('-',i,i);
	if (ls.isEmpty() ) // wenn 1 byte fehlt -> fehler
	    break;
	us = ls.toUShort(0,16);
	if ( us >255 ) // werte über ff -> fehler
	    break;
	leth.MacAdrByte[i] = us;
    }
    
    if (i < 6)
	return false;
    
    for (i = 0; i < 6 ; i++)
	this->MacAdrByte[i] = leth.MacAdrByte[i];
    
    return true;
}


QDataStream& operator << (QDataStream& qds, const cETHAdress& eadr)
{
    for (int i = 0; i < 6; i++) qds << (Q_UINT8&) (eadr.MacAdrByte[i]);
    return qds;
}


QDataStream& operator >> (QDataStream& qds, const cETHAdress& eadr)
{
    for (int i = 0; i < 6; i++) qds >> (Q_UINT8&) (eadr.MacAdrByte[i]);
    return qds;
}
