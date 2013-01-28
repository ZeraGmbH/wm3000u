#include <QDir>
#include <QFile>
#include <QString>
#include <QWidget>
#include "csessionhelper.h"
#include "widgeom.h"
#include "wmglobal.h"
cSessionHelper::cSessionHelper()
{
}

void cSessionHelper::writeSession(QWidget *widget, cWidgetGeometry geometry, QString session)
{
  QFileInfo fi(session);
  if(!QDir(QString("%1/.wm3000u/").arg(wm3000uHome)).exists())
  {
    //create temporary object that gets deleted when leaving the control block
    QDir().mkdir(QString("%1/.wm3000u/").arg(wm3000uHome));
  }

  QString ls = QString("%1/.wm3000u/%2%3").arg(wm3000uHome).arg(widget->name()).arg(fi.fileName());
  QFile file(ls);
  //    file.remove();
  if(file.open(QIODevice::Unbuffered | QIODevice::WriteOnly))
  {
    file.at(0);

    int vi;

    vi = (widget->isVisible()) ? 1 : 0;
    if (vi)
      geometry.SetGeometry(widget->pos(),widget->size());
    geometry.SetVisible(vi);

    QDataStream stream( &file );
    stream << geometry;
    file.close();
  }
}

cWidgetGeometry cSessionHelper::readSession(QWidget *widget, QString session)
{
  cWidgetGeometry geometry;
  QFileInfo fi(session);
  QString ls = QString("%1/.wm3000u/%2%3").arg(wm3000uHome).arg(widget->name()).arg(fi.fileName());
  QFile file(ls);
  if ( file.open( QIODevice::ReadOnly ) ) {
      QDataStream stream( &file );
      stream >> geometry;
      file.close();
      widget->hide();
      widget->resize(geometry.m_Size);
      widget->move(geometry.m_Point);
      if (geometry.vi)
      {
          widget->show();
      }
// FVWM und Gnome verhalten sich anders
#ifndef FVWM
  widget->move(geometry.m_Point);
#endif
  }
  return geometry;
}
