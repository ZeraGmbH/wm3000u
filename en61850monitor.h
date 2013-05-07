/****************************************************************************
** Form interface generated from reading ui file 'en61850monitor.ui'
**
** Created: Mi Sep 19 07:33:29 2012
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef EN61850MONBASE_H
#define EN61850MONBASE_H

#include <qvariant.h>
#include <qdialog.h>
#include "en61850.h"
#include "widgeom.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QButtonGroup;
class QLabel;
class QCheckBox;
class QPushButton;

class EN61850monbase : public QDialog
{
    Q_OBJECT

public:
    EN61850monbase( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~EN61850monbase();

    QButtonGroup* MonitorCountbuttonGroup;
    QLabel* ByteCountLabel;
    QLabel* ByteCountValLabel;
    QLabel* LostSyncCountLabel;
    QLabel* LostSyncValLabel;
    QButtonGroup* MACErrorsbuttonGroup;
    QCheckBox* RUNTFramecheckBox;
    QCheckBox* LateCollisioncheckBox;
    QCheckBox* RWTOcheckBox;
    QCheckBox* PHysLayercheckBox;
    QCheckBox* AlignErrorcheckBox;
    QCheckBox* CRCErrorcheckBox;
    QCheckBox* FIFOOvfcheckBox;
    QCheckBox* MACSyncLostcheckBox;
    QButtonGroup* DecoderErrorbuttonGroup;
    QCheckBox* savPducheckBox;
    QCheckBox* ASDUcheckBox;
    QCheckBox* seqASDUcheckBox;
    QCheckBox* seqASDUncheckBox;
    QCheckBox* serviceIdentcheckBox;
    QCheckBox* smpCountcheckBox;
    QCheckBox* confRevcheckBox;
    QCheckBox* smpSynchcheckBox;
    QCheckBox* seqDatacheckBox;
    QCheckBox* ETHMacAdrcheckBox;
    QCheckBox* ETHHeadercheckBox;
    QCheckBox* PriorityTaggedcheckBox;
    QCheckBox* nASDUcheckBox;
    QCheckBox* DataSyncLostcheckBox;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;

public slots:
    virtual void ShowHideSlot( bool b );
    virtual void SetETHStatusSlot( cEN61850Info * ethInfo );
    bool LoadSession( QString session );
    void SaveSession( QString session );

signals:
    void InformationRequest();
    void isVisibleSignal(bool);
    void ResetETHStatus();

protected:
    int newVariable;

    virtual void closeEvent( QCloseEvent * ce );

    QVBoxLayout* EN61850monbaseLayout;
    QVBoxLayout* MonitorCountbuttonGroupLayout;
    QHBoxLayout* layout11;
    QSpacerItem* spacer36;
    QHBoxLayout* layout8;
    QSpacerItem* spacer37;
    QHBoxLayout* layout12;
    QVBoxLayout* MACErrorsbuttonGroupLayout;
    QSpacerItem* spacer5;
    QHBoxLayout* layout7;
    QVBoxLayout* layout5;
    QVBoxLayout* layout6;
    QHBoxLayout* DecoderErrorbuttonGroupLayout;
    QVBoxLayout* layout8_2;
    QVBoxLayout* layout11_2;
    QHBoxLayout* Layout1;
    QSpacerItem* Horizontal_Spacing2;

protected slots:
    virtual void languageChange();

    virtual void accept();
    virtual void reject();


private:
    QTimer *m_pTimer;
    cWidgetGeometry m_widGeometry;
    cEN61850Info ETHStatus;

    virtual void init();
    virtual void destroy();

private slots:
    void TimerSlot();

};

#endif // EN61850MONBASE_H
