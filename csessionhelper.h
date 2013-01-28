#ifndef CSESSIONHELPER_H
#define CSESSIONHELPER_H

class QWidget;
class QString;
class cWidgetGeometry;

class CSessionHelper
{
protected:
  CSessionHelper();

public:
  static void writeSession(QWidget *widget, cWidgetGeometry geometry, QString session);
  static cWidgetGeometry readSession(QWidget *widget, QString session);
};

#endif // CSESSIONHELPER_H
