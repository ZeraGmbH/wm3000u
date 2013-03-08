#include <QWidget>

#include "dbushelper.h"
#include "dbushelperadapter.h"


WM3000UDBusHelper::WM3000UDBusHelper(QObject *parent) :
  QObject(parent)
{
  new WM3000UDBusHelperAdapter(this);

  QDBusConnection dbusConn = QDBusConnection::sessionBus();

  dbusConn.registerService("de.zera.WM3000UDBusHelper");
  dbusConn.registerObject("/", this);
}

void WM3000UDBusHelper::registerWidget(QWidget *wid)
{
  if(!registeredWidgets.contains(wid))
  {
    registeredWidgets.append(wid);
  }
}

void WM3000UDBusHelper::setVisible(QString widgetName, bool visibility)
{
  QWidget* target=0;

  foreach(QWidget* wid, registeredWidgets)
  {
    if(wid->name()==widgetName)
    {
      target=wid;
      break;
    }
  }
  if (target!=0)
  {
    if(visibility==true)
    {
      target->show();
    }
    else
    {
      target->close();
    }
  }
}

bool WM3000UDBusHelper::isVisible(QString widgetName)
{
  QWidget* target=0;

  foreach(QWidget* wid, registeredWidgets)
  {
    if(wid->name()==widgetName)
    {
      target=wid;
      break;
    }
  }
  if (target!=0)
  {
    return target->isVisible();
  }
  else
  {
    return 0;
  }
}
