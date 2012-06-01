//Added by qt3to4:
#include <QShowEvent>
#include "rangedialogbase.h"
#include "ui_rangedialogbase.h"

RangeDialogBase::RangeDialogBase( QWidget* parent ):
    QDialog(parent),
    ui(new Ui::RangeDialogBase)
{
    ui->setupUi(this);
}

RangeDialogBase::~RangeDialogBase()
{
    delete ui;
}



void RangeDialogBase::ActualizeDialog() // aktualisiert dialoganzeige
{
    if ( m_ConfData.m_sRangeNVorgabe == "Auto" )
        ui->RangeNComboBox->setCurrentItem(m_sNXItemList.findIndex(m_ConfData.m_sRangeNVorgabe));
    else
        ui->RangeNComboBox->setCurrentItem(m_sNXItemList.findIndex(m_ConfData.m_sRangeN));
    if ( m_ConfData.m_sRangeXVorgabe == "Auto" )
        ui->RangeXComboBox->setCurrentItem(m_sNXItemList.findIndex(m_ConfData.m_sRangeXVorgabe));
    else
        ui->RangeXComboBox->setCurrentItem(m_sNXItemList.findIndex(m_ConfData.m_sRangeX));
    if ( m_ConfData.m_sRangeEVTVorgabe == "Auto")
        ui->RangeEVTComboBox->setCurrentItem(m_sEVTItemList.findIndex(m_ConfData.m_sRangeEVTVorgabe));
    else
        ui->RangeEVTComboBox->setCurrentItem(m_sEVTItemList.findIndex(m_ConfData.m_sRangeEVT));
    switch (m_ConfData.m_nMeasMode) {
	case Un_UxAbs:
            ui->RangeXComboBox->setEnabled(true);
            ui->RangeEVTComboBox->setEnabled(false);
	    break;
	case Un_EVT:
            ui->RangeXComboBox->setEnabled(false);
            ui->RangeEVTComboBox->setEnabled(true);
	    break;
	case Un_nConvent:
            ui->RangeXComboBox->setEnabled(false);
            ui->RangeEVTComboBox->setEnabled(false);
	    break;
	}
}


void RangeDialogBase::SetRangeListSlot( cWMRangeList& nx,  cWMRangeList& evt)
{
    CWMRange *Range;
    Q3PtrListIterator<CWMRange> it(nx);
   
   while (!m_sNXItemList.isEmpty()) m_sNXItemList.pop_back(); // liste erst mal leeren
   for(Range=it.toFirst();Range;Range=++it) m_sNXItemList.append(Range->Name());
   ui->RangeNComboBox->clear();
   ui->RangeNComboBox->insertStringList(m_sNXItemList);
   ui->RangeXComboBox->clear();
   ui->RangeXComboBox->insertStringList(m_sNXItemList);
      
   it=evt;
   while (!m_sEVTItemList.isEmpty()) m_sEVTItemList.pop_back(); // liste erst mal leeren   
   for(Range=it.toFirst();Range;Range=++it) m_sEVTItemList.append(Range->Name());
   ui->RangeEVTComboBox->clear();
   ui->RangeEVTComboBox->insertStringList(m_sEVTItemList);
   ActualizeDialog();
}


void RangeDialogBase::SetConfInfoSlot(cConfData *cd)
{
    m_ConfData=*cd;
    ActualizeDialog();
}


void RangeDialogBase::SetNRangeSlot( const QString & nr )
{
    m_ConfData.m_sRangeNVorgabe=nr;
    if ( nr != QString("Auto") ) m_ConfData.m_sRangeNSoll=nr; // damit wird sofort geschaltet  
    emit SendRange(&m_ConfData);
}


void RangeDialogBase::SetXRangeSlot( const QString & xr)
{
    m_ConfData.m_sRangeXVorgabe=xr;
    if ( xr != QString("Auto") ) m_ConfData.m_sRangeXSoll=xr;
    emit SendRange(&m_ConfData);
}



void RangeDialogBase::SetEVTRangeSlot( const QString & er)
{
    m_ConfData.m_sRangeEVTVorgabe=er; 
    if ( er != QString("Auto") ) m_ConfData.m_sRangeEVTSoll=er;
    emit SendRange(&m_ConfData);
}


void RangeDialogBase::showEvent( QShowEvent * )
{
    emit SendRangeInfoReqSignal(); // aufforderung an device bereichliste und configuration zu senden
}


void RangeDialogBase::RemoteCtrlInfoSlot(bool remote)
{
    setEnabled(!remote);
}
