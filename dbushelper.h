#ifndef DBUSHELPER_H
#define DBUSHELPER_H

#include <QObject>
#include <QList>

QT_BEGIN_NAMESPACE
class QWidget;
QT_END_NAMESPACE

class WM3000UDBusHelper : public QObject
{
  Q_OBJECT
public:
  explicit WM3000UDBusHelper(QObject *parent = 0);

  void registerWidget(QWidget* wid);
  
signals:
  
public slots:
  void setVisible(QString widgetName, bool visibility);
  bool isVisible(QString widgetName);

private:

  QList<QWidget *> registeredWidgets;
};

#endif // DBUSHELPER_H
