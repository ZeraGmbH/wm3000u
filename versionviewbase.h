/****************************************************************************
** Form interface generated from reading ui file 'versionviewbase.ui'
**
** Created: Di Aug 19 11:06:40 2014
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef VERSIONSVIEWBASE_H
#define VERSIONSVIEWBASE_H

#include <qvariant.h>
#include <qdialog.h>
#include "wmglobal.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;

class VersionsViewBase : public QDialog
{
    Q_OBJECT

public:
    VersionsViewBase( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~VersionsViewBase();

    QLabel* DeviceLabel;
    QLabel* DevVersionLabel;
    QLabel* SerialNrLabel;
    QLabel* AdjDataLabel;
    QLabel* PCBVersionLabel;
    QLabel* PCBServerLabel;
    QLabel* DSPVersionLabel;
    QLabel* DSPServerLabel;
    QLabel* DeviceText;
    QLabel* DeviceVersionText;
    QLabel* SerialNrText;
    QLabel* AdjDataText;
    QLabel* PCBVersionText;
    QLabel* PCBServerText;
    QLabel* DSPVersionText;
    QLabel* DSPServerText;

public slots:
    virtual void ReceiveVersionData( tVersSerial * p );
    virtual void ShowVersionSlot();

protected:
    QHBoxLayout* VersionsViewBaseLayout;
    QSpacerItem* spacer4;
    QVBoxLayout* layout7;
    QVBoxLayout* layout8;

protected slots:
    virtual void languageChange();

private:
    tVersSerial* pVersion;

};

#endif // VERSIONSVIEWBASE_H
