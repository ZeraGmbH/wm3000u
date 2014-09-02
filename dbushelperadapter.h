/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -c WM3000UDBusHelperAdapter -a dbushelperadapter.h:dbushelperadapter.cpp de.zera.wm3000u.xml
 *
 * qdbusxml2cpp is Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#ifndef DBUSHELPERADAPTER_H_1359378273
#define DBUSHELPERADAPTER_H_1359378273

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
class QByteArray;
template<class T> class QList;
template<class Key, class Value> class QMap;
class QString;
class QStringList;
class QVariant;

/*
 * Adaptor class for interface de.zera.WM3000UDBusHelper
 */
class WM3000UDBusHelperAdapter: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "de.zera.WM3000UDBusHelper")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"de.zera.WM3000UDBusHelper\">\n"
"    <method name=\"setVisible\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"widgetName\"/>\n"
"      <arg direction=\"in\" type=\"b\" name=\"visibility\"/>\n"
"    </method>\n"
"    <method name=\"isVisible\">\n"
"      <arg direction=\"out\" type=\"b\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"widgetName\"/>\n"
"    </method>\n"
"  </interface>\n"
        "")
public:
    WM3000UDBusHelperAdapter(QObject *parent);
    virtual ~WM3000UDBusHelperAdapter();

public: // PROPERTIES
public Q_SLOTS: // METHODS
    bool isVisible(const QString &widgetName);
    void setVisible(const QString &widgetName, bool visibility);
Q_SIGNALS: // SIGNALS
};

#endif