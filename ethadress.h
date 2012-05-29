// headerdatei für class cethadress
// hält eine ethernetadresse und kann adressen vergleichen

#include <qdatastream.h>
#include <qstring.h>

#ifndef ETHADRESS_H
#define ETHADRESS_H

class cETHAdress
{
public:
    cETHAdress(){};
    bool operator ==(const cETHAdress&);
    bool operator !=(const cETHAdress&);
    uchar MacAdrByte[6];
    friend QDataStream& operator <<(QDataStream&,const cETHAdress&);
    friend QDataStream& operator >>(QDataStream&,const cETHAdress&);
    QString GetMacAdress();
    bool SetMacAdress(QString&);
};    

QDataStream& operator <<(QDataStream&,const cETHAdress&);
QDataStream& operator >>(QDataStream&,const cETHAdress&);

#endif
