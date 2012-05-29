// header datei für cWidgetGeometry 
// zum speichern von fenster position und grösse und ob sichtbar oder nicht

#ifndef WIDGETGEOMETRY_H
#define WIDGETGEOMETRY_H

#include <qpoint.h>
#include <qsize.h>
#include <qdatastream.h>

class cWidgetGeometry
{
public:    
    cWidgetGeometry(){vi = 0;};
    friend QDataStream& operator << (QDataStream&, cWidgetGeometry&);
    friend QDataStream& operator >> (QDataStream&, cWidgetGeometry&);
    void SetGeometry(QPoint,QSize);
    void SetVisible(int);
    QPoint m_Point;
    QSize m_Size;
    int vi;
};

    
    
    
#endif    
