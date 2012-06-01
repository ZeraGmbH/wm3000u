/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/


void RangeDialogBase::ActualizeDialog() // aktualisiert dialoganzeige
{
    if ( m_ConfData.m_sRangeNVorgabe == "Auto" )
	RangeNComboBox->setCurrentItem(m_sNXItemList.findIndex(m_ConfData.m_sRangeNVorgabe));
    else
	RangeNComboBox->setCurrentItem(m_sNXItemList.findIndex(m_ConfData.m_sRangeN));
    if ( m_ConfData.m_sRangeXVorgabe == "Auto" )
	RangeXComboBox->setCurrentItem(m_sNXItemList.findIndex(m_ConfData.m_sRangeXVorgabe));
    else
	RangeXComboBox->setCurrentItem(m_sNXItemList.findIndex(m_ConfData.m_sRangeX));
    if ( m_ConfData.m_sRangeEVTVorgabe == "Auto")
	RangeEVTComboBox->setCurrentItem(m_sEVTItemList.findIndex(m_ConfData.m_sRangeEVTVorgabe));
    else
	RangeEVTComboBox->setCurrentItem(m_sEVTItemList.findIndex(m_ConfData.m_sRangeEVT));
    switch (m_ConfData.m_nMeasMode) {
	case Un_UxAbs:
	    RangeXComboBox->setEnabled(true);
	    RangeEVTComboBox->setEnabled(false);
	    break;
	case Un_EVT:
	    RangeXComboBox->setEnabled(false);
	    RangeEVTComboBox->setEnabled(true);
	    break;
	case Un_nConvent:
	    RangeXComboBox->setEnabled(false);
	    RangeEVTComboBox->setEnabled(false);
	    break;
	}
}


void RangeDialogBase::SetRangeListSlot( cWMRangeList& nx,  cWMRangeList& evt)
{
    CWMRange *Range;
    QPtrListIterator<CWMRange> it(nx);
   
   while (!m_sNXItemList.isEmpty()) m_sNXItemList.pop_back(); // liste erst mal leeren
   for(Range=it.toFirst();Range;Range=++it) m_sNXItemList.append(Range->Name());
   RangeNComboBox->clear();
   RangeNComboBox->insertStringList(m_sNXItemList);
   RangeXComboBox->clear();
   RangeXComboBox->insertStringList(m_sNXItemList);
      
   it=evt;
   while (!m_sEVTItemList.isEmpty()) m_sEVTItemList.pop_back(); // liste erst mal leeren   
   for(Range=it.toFirst();Range;Range=++it) m_sEVTItemList.append(Range->Name());
   RangeEVTComboBox->clear();
   RangeEVTComboBox->insertStringList(m_sEVTItemList);
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
