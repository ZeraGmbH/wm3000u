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

extern eUnit LoadpointUnit[];
extern eUnit VoltageUnit[];
extern eUnit ErrorUnit[];
extern eUnit AngleUnit[];

void WMMeasConfigBase::init()
{
   int i;
   
   m_LPUnit[0] = LoadpointUnit + LPProzent;
   m_LPUnit[1] = VoltageUnit + VoltV;
   m_LPUnit[2] = VoltageUnit + VoltkV;
    
   for (i = 0; i < 3; i++)
	LPcomboBox->insertItem(m_LPUnit[i]->Name);
       
    m_ErrUnit[0] = ErrorUnit + ErrProzent;
    m_ErrUnit[1] = ErrorUnit + Errppm;
    // FcomboBox-	>insertItem(ErrorUnit[ErrPromill]->Name);
    
    for (i = 0; i < 2; i++)
	ErrcomboBox->insertItem(m_ErrUnit[i]->Name);

    m_AngleUnit[0] = AngleUnit + Anglegrad;
    m_AngleUnit[1] = AngleUnit + Anglemin;
    m_AngleUnit[2] = AngleUnit + Anglecrad;
    m_AngleUnit[3] = AngleUnit + Angleurad;
    m_AngleUnit[4] = AngleUnit + Anglerad;
    
    for (i = 0; i < 5; i++)    
	WcomboBox->insertItem(m_AngleUnit[i]->Name);
    
    m_nDisplayMode = IEC; // wmglobal
    m_nLPDisplayMode = totalRms;
}


void WMMeasConfigBase::ReceiveFormatInfoSlot(int m, int m2, int n, cFormatInfo* fi )
{
    int i;
    for(i = 0; i < n; i++, fi++)
	m_Format[i] = *fi;
    
    m_nDisplayMode = m;
    m_nLPDisplayMode = m2;
    
    LPspinBoxFW->setValue(m_Format[0].FieldWidth);
    LPspinBoxRS->setValue(m_Format[0].Resolution);
    LPcomboBox->setCurrentText(m_Format[0].UnitInfo.Name);
		
    ErrspinBoxFW->setValue(m_Format[1].FieldWidth);
    ErrspinBoxRS->setValue(m_Format[1].Resolution);
    ErrcomboBox->setCurrentText(m_Format[1].UnitInfo.Name);
    
    WspinBoxFW->setValue(m_Format[2].FieldWidth);
    WspinBoxRS->setValue(m_Format[2].Resolution);
    WcomboBox->setCurrentText(m_Format[2].UnitInfo.Name);
 
    Actualize();
}



void WMMeasConfigBase::LPComboActivatedSlot(int index)
{
    m_Format[0].UnitInfo = *m_LPUnit[index];
}


void WMMeasConfigBase::ErrComboActivatedSlot(int index)
{
    m_Format[1].UnitInfo = *m_ErrUnit[index];	
}


void WMMeasConfigBase::WComboActivatedSlot(int index)
{
    m_Format[2].UnitInfo = *m_AngleUnit[index];
}


void WMMeasConfigBase::Actualize()
{
    bool isIEC = (m_nDisplayMode == IEC);
    
    WgroupBox->setEnabled(isIEC); // winkel combox nur wenn iec anzeige
    
    IECradioButton->setChecked(isIEC);
    ANSIradioButton->setChecked(!isIEC);
    
    bool isTotalRMS = (m_nLPDisplayMode == totalRms);
    
    TotalRMSradioButton->setChecked(isTotalRMS);
    H1radioButton->setChecked(!isTotalRMS);
}


void WMMeasConfigBase::IECCheckedSlot()
{
    m_nDisplayMode = IEC;
    Actualize();
}


void WMMeasConfigBase::ANSICheckedSlot()
{
    m_nDisplayMode = ANSI;
    Actualize();
}


void WMMeasConfigBase::TotalRMScheckedSlot()
{
    m_nLPDisplayMode = totalRms;
    Actualize();
}


void WMMeasConfigBase::H1RMScheckedSlot()
{
    m_nLPDisplayMode = H1Rms;
    Actualize();
}


void WMMeasConfigBase::SendData2AppSlot()
{
    m_Format[0].FieldWidth = LPspinBoxFW->value();
    m_Format[0].Resolution = LPspinBoxRS->value();
  		
    m_Format[1].FieldWidth = ErrspinBoxFW->value();
    m_Format[1].Resolution = ErrspinBoxRS->value();
    
    m_Format[2].FieldWidth = WspinBoxFW->value();
    m_Format[2].Resolution = WspinBoxRS->value();
    
    emit SendFormatInfoSignal(m_nDisplayMode, m_nLPDisplayMode, 3, m_Format);
}

