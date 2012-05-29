// implementation cZHServerIFace

#include <qtextstream.h>

#include "zhserveriface.h"


cZHServerIFace::cZHServerIFace(QString hn, int hp, int t)
{
    iFaceSock = new cZHClientSocket(t);
    m_sHost = hn;
    m_nPort = hp;
}


cZHServerIFace::~cZHServerIFace()
{
    delete iFaceSock;
}



