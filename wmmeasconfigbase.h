/****************************************************************************
** Form interface generated from reading ui file 'wmmeasconfigbase.ui'
**
** Created: Mo Mai 6 13:06:55 2013
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef WMMEASCONFIGBASE_H
#define WMMEASCONFIGBASE_H

#include <qvariant.h>
#include <qdialog.h>
#include "formatinfo.h"
#include "eparameter.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QGroupBox;
class QLabel;
class QSpinBox;
class QComboBox;
class QRadioButton;
class QPushButton;

class WMMeasConfigBase : public QDialog
{
    Q_OBJECT

public:
    WMMeasConfigBase( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~WMMeasConfigBase();

    QGroupBox* groupBox40;
    QLabel* textLabel1;
    QLabel* textLabel2;
    QLabel* textLabel3;
    QGroupBox* LPgroupBox;
    QSpinBox* LPspinBoxFW;
    QSpinBox* LPspinBoxRS;
    QComboBox* LPcomboBox;
    QRadioButton* TotalRMSradioButton;
    QRadioButton* H1radioButton;
    QGroupBox* ErrgroupBox;
    QSpinBox* ErrspinBoxFW;
    QSpinBox* ErrspinBoxRS;
    QComboBox* ErrcomboBox;
    QRadioButton* IECradioButton;
    QRadioButton* ANSIradioButton;
    QGroupBox* WgroupBox;
    QSpinBox* WspinBoxFW;
    QSpinBox* WspinBoxRS;
    QComboBox* WcomboBox;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;

public slots:
    virtual void ReceiveFormatInfoSlot( int m, int m2, int n, cFormatInfo * fi );
    virtual void TotalRMScheckedSlot();
    virtual void H1RMScheckedSlot();

signals:
    void SendFormatInfoSignal(int, int, int, cFormatInfo*);

protected:
    QVBoxLayout* WMMeasConfigBaseLayout;
    QHBoxLayout* layout4;
    QVBoxLayout* groupBox40Layout;
    QSpacerItem* spacer54;
    QSpacerItem* spacer55;
    QSpacerItem* spacer56;
    QSpacerItem* spacer57;
    QVBoxLayout* LPgroupBoxLayout;
    QVBoxLayout* ErrgroupBoxLayout;
    QVBoxLayout* WgroupBoxLayout;
    QSpacerItem* spacer53;
    QHBoxLayout* layout48;
    QSpacerItem* Horizontal_Spacing2;

protected slots:
    virtual void languageChange();

private:
    int m_nLPDisplayMode;
    int m_nDisplayMode;
    cFormatInfo m_Format[3];
    eUnit* m_AngleUnit[6];
    eUnit* m_ErrUnit[2];
    eUnit* m_LPUnit[3];

    void init();
    virtual void Actualize();

private slots:
    virtual void LPComboActivatedSlot( int index );
    virtual void ErrComboActivatedSlot( int index );
    virtual void WComboActivatedSlot( int index );
    virtual void IECCheckedSlot();
    virtual void ANSICheckedSlot();
    virtual void SendData2AppSlot();

};

#endif // WMMEASCONFIGBASE_H
