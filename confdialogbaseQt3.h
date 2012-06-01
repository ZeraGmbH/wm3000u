/****************************************************************************
** Form interface generated from reading ui file 'confdialogbase.ui'
**
** Created: Mi Feb 15 08:22:05 2012
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CONFDIALOGBASE_H
#define CONFDIALOGBASE_H

#include <qvariant.h>
#include <qdialog.h>
#include "qvalidator.h"
#include "wmglobal.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QTabWidget;
class QWidget;
class QButtonGroup;
class QRadioButton;
class QGroupBox;
class QLabel;
class QLineEdit;
class QCheckBox;
class QSpinBox;
class QComboBox;
class QPushButton;

class ConfDialogBase : public QDialog
{
    Q_OBJECT

public:
    ConfDialogBase( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~ConfDialogBase();

    QTabWidget* ConfTabWidget;
    QWidget* Widget8;
    QButtonGroup* ModeButtonGroup;
    QRadioButton* Mode0RadioButton;
    QRadioButton* Mode2RadioButton;
    QRadioButton* Mode3RadioButton;
    QWidget* Widget9;
    QGroupBox* CmpKorrGroupBox;
    QLabel* CmpKorrLabel2;
    QLabel* CmpKorrLabel1;
    QLineEdit* CmpKorrLineEdit1;
    QLineEdit* CmpKorrLineEdit2;
    QGroupBox* CmpCorrGroupBox;
    QCheckBox* CmpCorrCheckBox;
    QWidget* TabPage;
    QGroupBox* TIntegrationGroupBox;
    QLabel* TIntegrationLabel;
    QSpinBox* TIntegrationSpinBox;
    QButtonGroup* FreqButtonGroup;
    QRadioButton* F16RadioButton;
    QRadioButton* F50RadioButton;
    QRadioButton* F60RadioButton;
    QButtonGroup* SPeriodeButtonGroup;
    QRadioButton* S80RadioButton;
    QRadioButton* S256RadioButton;
    QGroupBox* CmpIntervallGroupBox;
    QLabel* CmpIntervallLabel;
    QSpinBox* CmpIntervallSpinBox;
    QWidget* TabPage_2;
    QButtonGroup* SyncButtonGroup;
    QRadioButton* SyncRadioButton1;
    QRadioButton* SyncRadioButton2;
    QLabel* TSyncLabel;
    QSpinBox* TSyncSpinBox;
    QGroupBox* groupBox;
    QCheckBox* SSynccheckBox;
    QWidget* TabPage_3;
    QGroupBox* EVTratioGroupBox;
    QLineEdit* RatioSekEVTLineEdit;
    QLabel* RatioSekXLabel_3;
    QLineEdit* RatioPrimEVTLineEdit;
    QLabel* RatioPrimXLabel_3;
    QRadioButton* evtPrim_3radioButton;
    QRadioButton* evtSek_3radioButton;
    QRadioButton* evtPrim_w3radioButton;
    QRadioButton* evtSek_w3radioButton;
    QGroupBox* NTRatioGroupBox;
    QLineEdit* RatioSekNLineEdit;
    QLineEdit* RatioPrimNLineEdit;
    QLabel* RatioPrimXLabel_2;
    QLabel* RatioSekXLabel_2;
    QRadioButton* nPrim_3radioButton;
    QRadioButton* nSek_3radioButton;
    QRadioButton* nPrim_w3radioButton;
    QRadioButton* nSek_w3radioButton;
    QGroupBox* NTRatioGroupBox2;
    QComboBox* RatioNSekComboBox;
    QLabel* RatioSekNLabel;
    QComboBox* RatioNPrimComboBox;
    QLabel* RatioPrimNLabel;
    QGroupBox* XTRatioGroupBox;
    QLineEdit* RatioSekXLineEdit;
    QLabel* RatioSekXLabel;
    QLineEdit* RatioPrimXLineEdit;
    QLabel* RatioPrimXLabel;
    QRadioButton* xPrim_3radioButton;
    QRadioButton* xSek_3radioButton;
    QRadioButton* xPrim_w3radioButton;
    QRadioButton* xSek_w3radioButton;
    QWidget* TabPage_4;
    QGroupBox* groupBox7;
    QLabel* MacMULabel;
    QLabel* MacWMLabel;
    QLineEdit* MacSLineEdit1;
    QLabel* textLabel4;
    QLineEdit* MacSLineEdit2;
    QLabel* textLabel5;
    QLineEdit* MacSLineEdit3;
    QLabel* textLabel6;
    QLineEdit* MacSLineEdit4;
    QLabel* textLabel7;
    QLineEdit* MacSLineEdit5;
    QLabel* textLabel8;
    QLineEdit* MacSLineEdit6;
    QLineEdit* MacDLineEdit1;
    QLabel* textLabel4_2;
    QLineEdit* MacDLineEdit2;
    QLabel* textLabel5_2;
    QLineEdit* MacDLineEdit3;
    QLabel* textLabel6_2;
    QLineEdit* MacDLineEdit4;
    QLabel* textLabel7_2;
    QLineEdit* MacDLineEdit5;
    QLabel* textLabel8_2;
    QLineEdit* MacDLineEdit6;
    QGroupBox* groupBox11;
    QLabel* TPIDtextLabel;
    QLabel* textLabel4_3;
    QLabel* textLabel2;
    QLabel* textLabel3;
    QLineEdit* UPrioritylineEdit;
    QLineEdit* CFIlineEdit;
    QLineEdit* TPIDlineEdit;
    QLineEdit* VIDlineEdit;
    QGroupBox* groupBox12;
    QLabel* textLabel5_3;
    QLabel* textLabel6_3;
    QLineEdit* EthTypelineEdit;
    QLineEdit* APPIDlineEdit;
    QGroupBox* groupBox9;
    QLabel* ASDUTextLabel;
    QLabel* DatasetTextLabel;
    QSpinBox* ASDUSpinBox;
    QSpinBox* SetSpinBox;
    QWidget* LogPage;
    QGroupBox* LogfileSizeGroupBox;
    QLabel* LogfileSizeTextLabel;
    QSpinBox* LogfileSizeSpinbox;
    QLabel* LogfileSizeTextLabel2;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;

    virtual bool is_3( const QString & s );
    virtual bool is_w3( const QString & s );

public slots:
    virtual void SetConfInfoSlot( cConfData * cd );
    virtual void SetConfListSlot( QStringList & NPItems, QStringList & NSItems );
    virtual void accept();
    virtual void SetnConventMenu();
    virtual void RemoteCtrlInfoSlot( bool remote );

signals:
    void SendConfDataSignal(cConfData*);

protected:
    virtual void SetModeMenu();
    virtual void SetCmpMenu();
    virtual void SetSyncMenu();
    virtual void SetRatioMenu();
    virtual void SetLogMenu();
    virtual void SetMessungMenu();

    QVBoxLayout* ConfDialogBaseLayout;
    QSpacerItem* spacer45;
    QVBoxLayout* Widget9Layout;
    QSpacerItem* spacer13_3;
    QHBoxLayout* layout46;
    QSpacerItem* spacer14_2;
    QVBoxLayout* layout45;
    QGridLayout* CmpKorrGroupBoxLayout;
    QVBoxLayout* layout16;
    QVBoxLayout* CmpCorrGroupBoxLayout;
    QGridLayout* TabPageLayout;
    QSpacerItem* spacer58;
    QSpacerItem* spacer59;
    QHBoxLayout* TIntegrationGroupBoxLayout;
    QSpacerItem* spacer57;
    QVBoxLayout* FreqButtonGroupLayout;
    QVBoxLayout* SPeriodeButtonGroupLayout;
    QSpacerItem* spacer27;
    QHBoxLayout* CmpIntervallGroupBoxLayout;
    QSpacerItem* spacer9_2;
    QVBoxLayout* TabPageLayout_2;
    QSpacerItem* spacer86;
    QHBoxLayout* layout37;
    QSpacerItem* spacer85;
    QVBoxLayout* layout36;
    QHBoxLayout* SyncButtonGroupLayout;
    QVBoxLayout* layout101;
    QVBoxLayout* layout102;
    QSpacerItem* spacer77;
    QHBoxLayout* layout95;
    QSpacerItem* spacer12;
    QGridLayout* TabPageLayout_3;
    QSpacerItem* spacer44;
    QSpacerItem* spacer43;
    QGridLayout* EVTratioGroupBoxLayout;
    QGridLayout* NTRatioGroupBoxLayout;
    QGridLayout* NTRatioGroupBox2Layout;
    QSpacerItem* spacer22;
    QSpacerItem* spacer21;
    QGridLayout* XTRatioGroupBoxLayout;
    QHBoxLayout* TabPageLayout_4;
    QVBoxLayout* layout23;
    QHBoxLayout* groupBox7Layout;
    QSpacerItem* spacer51;
    QVBoxLayout* layout69;
    QVBoxLayout* layout78;
    QHBoxLayout* layout67;
    QSpacerItem* spacer47;
    QHBoxLayout* layout66;
    QSpacerItem* spacer46;
    QHBoxLayout* layout47;
    QGridLayout* groupBox11Layout;
    QGridLayout* groupBox12Layout;
    QGridLayout* groupBox9Layout;
    QVBoxLayout* LogPageLayout;
    QSpacerItem* spacer50;
    QHBoxLayout* layout24;
    QSpacerItem* spacer49;
    QHBoxLayout* LogfileSizeGroupBoxLayout;
    QHBoxLayout* layout19;
    QSpacerItem* Horizontal_Spacing2;

protected slots:
    virtual void languageChange();

private:
    cConfData m_ConfData;
    bool m_bRemoteCtrl;
    QString m_sText;

    void init();
    void Actualize();
    virtual const QString & baseUnitText( const QString & s );
    virtual const QString & genRatioText( QString s, QRadioButton * qrb_3, QRadioButton * qrb_w3 );

private slots:
    void ApplyDataSlot();
    virtual void nPrim_3radioButtonChecked();
    virtual void nPrim_w3radioButtonChecked();
    virtual void nSek_3radioButtonChecked();
    virtual void nSek_w3radioButtonChecked();
    virtual void xPrim_3radioButtonChecked();
    virtual void xPrim_w3radioButtonChecked();
    virtual void xSek_3radioButtonChecked();
    virtual void xSek_w3radioButtonChecked();
    virtual void evtPrim_3radioButtonChecked();
    virtual void evtPrim_w3radioButtonChecked();
    virtual void evtSek_3radioButtonChecked();
    virtual void evtSek_w3radioButtonChecked();

};

#endif // CONFDIALOGBASE_H
