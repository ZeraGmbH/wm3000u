// implementierung cWidgetGeometry

#include "widgeom.h"


QDataStream& operator << (QDataStream& ds, cWidgetGeometry& wg)
{
    ds << wg.vi << wg.m_Point.x() << wg.m_Point.y() << wg.m_Size.width() << wg.m_Size.height();
    return ds;
}


QDataStream& operator >> (QDataStream& ds, cWidgetGeometry& wg)
{
    int x,y,w,h;
    ds >> wg.vi >> x >> y >> w >> h;
    wg.m_Point = QPoint(x,y);
    wg.m_Size = QSize(w,h);
    return ds;
}

void cWidgetGeometry::SetGeometry(QPoint p,QSize s)
{
    m_Point = p;
    m_Size = s;
}

void cWidgetGeometry::SetVisible(int v)
{
    vi = v;
}
