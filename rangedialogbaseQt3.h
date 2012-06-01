/****************************************************************************
** Form interface generated from reading ui file 'rangedialogbase.ui'
**
** Created: Mi Feb 15 08:22:05 2012
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef RANGEDIALOGBASE_H
#define RANGEDIALOGBASE_H

#include <qvariant.h>
#include <qdialog.h>
#include "wmglobal.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QComboBox;
class QLabel;

class RangeDialogBase : public QDialog
{
    Q_OBJECT

public:
    RangeDialogBase( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~RangeDialogBase();

    QComboBox* RangeNComboBox;
    QComboBox* RangeEVTComboBox;
    QLabel* RangeXTextLabel;
    QComboBox* RangeXComboBox;
    QLabel* RangeEVTTextLabel;
    QLabel* RangeNTextLabel;

public slots:
    virtual void SetRangeListSlot( cWMRangeList & nx, cWMRangeList & evt );
    void SetConfInfoSlot( cConfData * cd );
    void SetNRangeSlot( const QString & nr );
    void SetXRangeSlot( const QString & xr );
    void SetEVTRangeSlot( const QString & er );
    virtual void RemoteCtrlInfoSlot( bool );

signals:
    void SendRange(cConfData*);
    void isVisible(bool);
    void SendRangeInfoReqSignal();

protected:
    virtual void showEvent( QShowEvent * );

    QGridLayout* RangeDialogBaseLayout;

protected slots:
    virtual void languageChange();

private:
    cConfData m_ConfData;
    QStringList m_sNXItemList;
    QStringList m_sEVTItemList;

    void ActualizeDialog();

};

#endif // RANGEDIALOGBASE_H
