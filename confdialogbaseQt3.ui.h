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


void ConfDialogBase::init()
{
    QRegExp rx( "^[0-9,A-F]{2,2}$" );
    QValidator* MacValidator = new QRegExpValidator( rx, this );
    
    MacSLineEdit6->setInputMask("NN");
    MacSLineEdit6->setValidator(MacValidator);
    MacSLineEdit5->setInputMask("NN");
    MacSLineEdit5->setValidator(MacValidator);
    MacSLineEdit4->setInputMask("NN");
    MacSLineEdit4->setValidator(MacValidator);
    MacSLineEdit3->setInputMask("NN");
    MacSLineEdit3->setValidator(MacValidator);
    MacSLineEdit2->setInputMask("NN");    
    MacSLineEdit2->setValidator(MacValidator);
    MacSLineEdit1->setInputMask("NN");
    MacSLineEdit1->setValidator(MacValidator);
    
    MacDLineEdit6->setInputMask("NN");
    MacDLineEdit6->setValidator(MacValidator);
    MacDLineEdit5->setInputMask("NN");
    MacDLineEdit5->setValidator(MacValidator);
    MacDLineEdit4->setInputMask("NN");
    MacDLineEdit4->setValidator(MacValidator);
    MacDLineEdit3->setInputMask("NN");    
    MacDLineEdit3->setValidator(MacValidator);
    MacDLineEdit2->setInputMask("NN");    
    MacDLineEdit2->setValidator(MacValidator);
    MacDLineEdit1->setInputMask("NN");
    MacDLineEdit1->setValidator(MacValidator);
    
    QRegExp rx2( "^[0-7]{1,1}$" );
    QValidator* UPriorityValidator = new QRegExpValidator( rx2, this );
    UPrioritylineEdit->setInputMask("N");  
    UPrioritylineEdit->setValidator(UPriorityValidator);
    
    QRegExp rx3( "^[0-1]{1,1}$" );
    QValidator* CFIValidator = new QRegExpValidator( rx3, this );
    CFIlineEdit->setInputMask("N");
    CFIlineEdit->setValidator(CFIValidator);
    
    QRegExp rx4( "^[0-9,A-F]{3,3}$" );
    QValidator* VIDValidator = new QRegExpValidator( rx4, this );
    VIDlineEdit->setInputMask("NNN");
    VIDlineEdit->setValidator(VIDValidator);
    
    QRegExp rx5( "^[0-9,A-F]{4,4}$" );
    QValidator* APPIDValidator = new QRegExpValidator( rx5, this );
    APPIDlineEdit->setInputMask("NNNN");
    APPIDlineEdit->setValidator(APPIDValidator);
    
    QRegExp rx6 ("^([0-9]+)(|\\.([0-9]+))(|mV|kV|V)$" );
    QValidator* RatioValidator = new QRegExpValidator( rx6, this );
    RatioPrimNLineEdit->setValidator(RatioValidator);
    RatioSekNLineEdit->setValidator(RatioValidator);
    RatioPrimXLineEdit->setValidator(RatioValidator);
    RatioSekXLineEdit->setValidator(RatioValidator);
    RatioPrimEVTLineEdit->setValidator(RatioValidator);
    RatioSekEVTLineEdit->setValidator(RatioValidator);
    
    m_bRemoteCtrl = false;
}

void ConfDialogBase::SetConfInfoSlot(cConfData *cd )
{
    if ( true /*! isVisible()*/ ) {
	m_ConfData=*cd;
	SetModeMenu(); // modus menu einrichten 
	SetCmpMenu(); // berechnung menu einrichten
	SetSyncMenu(); // sync menu einrichten
	SetRatioMenu(); // teiler menu einrichten  
	SetLogMenu(); // logfile menu einrichten
	SetMessungMenu(); // messung menu einrichten
	SetnConventMenu(); // nconvent menu einrichten 
	Actualize();
    }
}


void ConfDialogBase::SetConfListSlot( QStringList & NPItems, QStringList & NSItems)
{
    if (! isVisible() ) {
	RatioNPrimComboBox->clear();
	RatioNPrimComboBox->insertStringList(NPItems); // normalwandler primär stufen 
	RatioNPrimComboBox->setCurrentItem(NPItems.findIndex(m_ConfData.m_NPrimary));
	RatioNSekComboBox->clear();
	RatioNSekComboBox->insertStringList(NSItems); // normalwandler sekundär stufen
	RatioNSekComboBox->setCurrentItem(NSItems.findIndex(m_ConfData.m_NSecondary));    	}
}


void ConfDialogBase::Actualize()
{
   switch (m_ConfData.m_nMeasMode) {
	case Un_UxAbs:
	case Un_nConvent:
	    XTRatioGroupBox->setEnabled(true);
	    EVTratioGroupBox->setEnabled(false);
	    break;
	case Un_EVT:
	    XTRatioGroupBox->setEnabled(false);
	    EVTratioGroupBox->setEnabled(true);
	    break;
/*	case Un_nConvent:
	    XTRatioGroupBox->setEnabled(false);
	    EVTratioGroupBox->setEnabled(false);
	    break;*/
	}

   if (RatioNPrimComboBox->count()==0  || m_bRemoteCtrl ) // es existiert keine eigenfehlertabelle
   { // bzw. wir sind unter rechner kontrolle
       NTRatioGroupBox->setEnabled(true);
       NTRatioGroupBox2->setEnabled(false);
    }
    else
    {
	NTRatioGroupBox2->setEnabled(true);
	NTRatioGroupBox->setEnabled(false);
    }
}


void ConfDialogBase::accept()
{
    bool ratioInputOK = true;
    
    ratioInputOK = ratioInputOK &&  RatioPrimNLineEdit->hasAcceptableInput() 
		                 && RatioSekNLineEdit->hasAcceptableInput() 
		                 && RatioPrimXLineEdit->hasAcceptableInput()
			   && RatioSekXLineEdit->hasAcceptableInput()
			   && RatioPrimEVTLineEdit->hasAcceptableInput()
			   && RatioSekEVTLineEdit->hasAcceptableInput();
    
    if ( !ratioInputOK )
    {
	QMessageBox::information( this, trUtf8("Teilerverhältnisse"),  trUtf8("Bitte überprüfen! \nErlaubt mV,V,kV"));
    }
    else
    {
	ApplyDataSlot();
	emit SendConfDataSignal(&m_ConfData);
	close();
    }
}


void ConfDialogBase::SetModeMenu()
{
    switch (m_ConfData.m_nMeasMode) {
	case Un_UxAbs:
	    Mode0RadioButton->setChecked(true);
	    break;
	case Un_EVT:
	    Mode2RadioButton->setChecked(true);
	    break;
	case Un_nConvent:
	    Mode3RadioButton->setChecked(true);
	    break;
	}
}


void ConfDialogBase::SetCmpMenu()
{
    CmpKorrLineEdit1->setText(QString( "%1" ).arg(m_ConfData.m_fxPhaseShift,8,'f',4));
    CmpKorrLineEdit2->setText(QString( "%1" ).arg(m_ConfData.m_fxTimeShift,5,'f',3));
    CmpCorrCheckBox->setChecked(m_ConfData.m_bOECorrection);
}


void ConfDialogBase::SetSyncMenu()
{
    switch (m_ConfData.m_nSyncSource) {
	case Extern:
	    SyncRadioButton1->setChecked(true);
	    break;
	case Intern:
	    SyncRadioButton2->setChecked(true);
	    break;
	}
    
    TSyncSpinBox->setValue(m_ConfData.m_nTSync);
    SSynccheckBox->setChecked(m_ConfData.m_bStrongEthSynchronisation);
}


void ConfDialogBase::SetRatioMenu()
{
    // alle edit felder  und radiobuttons vorbesetzen
    RatioPrimNLineEdit->setText(baseUnitText(m_ConfData.m_NPrimary));
    RatioSekNLineEdit->setText(baseUnitText(m_ConfData.m_NSecondary));
    nPrim_3radioButton->setChecked(is_3(m_ConfData.m_NPrimary));
    nPrim_w3radioButton->setChecked(is_w3(m_ConfData.m_NPrimary));
    nSek_3radioButton->setChecked(is_3(m_ConfData.m_NSecondary));
    nSek_w3radioButton->setChecked(is_w3(m_ConfData.m_NSecondary));
    
    
    RatioPrimXLineEdit->setText(baseUnitText(m_ConfData.m_XPrimary));
    RatioSekXLineEdit->setText(baseUnitText(m_ConfData.m_XSecondary));
    xPrim_3radioButton->setChecked(is_3(m_ConfData.m_XPrimary));
    xPrim_w3radioButton->setChecked(is_w3(m_ConfData.m_XPrimary));
    xSek_3radioButton->setChecked(is_3(m_ConfData.m_XSecondary));
    xSek_w3radioButton->setChecked(is_w3(m_ConfData.m_XSecondary));
    
    
    RatioPrimEVTLineEdit->setText(baseUnitText(m_ConfData.m_EVTPrimary));
    RatioSekEVTLineEdit->setText(baseUnitText(m_ConfData.m_EVTSecondary));
    evtPrim_3radioButton->setChecked(is_3(m_ConfData.m_EVTPrimary));
    evtPrim_w3radioButton->setChecked(is_w3(m_ConfData.m_EVTPrimary));
    evtSek_3radioButton->setChecked(is_3(m_ConfData.m_EVTSecondary));
    evtSek_w3radioButton->setChecked(is_w3(m_ConfData.m_EVTSecondary));
}



void ConfDialogBase::SetLogMenu()
{
   LogfileSizeSpinbox->setValue(m_ConfData.m_nLogFileMax >>16);
}


void ConfDialogBase::ApplyDataSlot() // einstellungen werden intern übernommen, die menus aktualisiert
{
    switch (m_ConfData.m_nMeasMode) {
	case Un_UxAbs:
	case Un_nConvent:
	    m_ConfData.m_XPrimary = genRatioText( RatioPrimXLineEdit->text(), xPrim_3radioButton, xPrim_w3radioButton);
	    m_ConfData.m_XSecondary = genRatioText( RatioSekXLineEdit->text(), xSek_3radioButton, xSek_w3radioButton);
	    break;
	case Un_EVT:
	    m_ConfData.m_EVTPrimary = genRatioText( RatioPrimEVTLineEdit->text(), evtPrim_3radioButton, evtPrim_w3radioButton);
	    m_ConfData.m_EVTSecondary = genRatioText( RatioSekEVTLineEdit->text(), evtSek_3radioButton, evtSek_w3radioButton);
	    break;
	}
 
    if (Mode0RadioButton->isChecked()) m_ConfData.m_nMeasMode=Un_UxAbs;
    if (Mode2RadioButton->isChecked()) m_ConfData.m_nMeasMode=Un_EVT;
    if (Mode3RadioButton->isChecked()) m_ConfData.m_nMeasMode=Un_nConvent;
    
    // ModeMenu gescannt
    
    m_ConfData.m_fxPhaseShift=(CmpKorrLineEdit1->text()).toDouble();
    m_ConfData.m_fxTimeShift=(CmpKorrLineEdit2->text()).toDouble();
    m_ConfData.m_bOECorrection=CmpCorrCheckBox->isChecked();
    // CmpMenu gescannt
    
    if (SyncRadioButton1->isChecked()) m_ConfData.m_nSyncSource=Extern;
    if (SyncRadioButton2->isChecked()) m_ConfData.m_nSyncSource=Intern;
    m_ConfData.m_nTSync=TSyncSpinBox->value();
    m_ConfData.m_bStrongEthSynchronisation=SSynccheckBox->isChecked();
    // SyncMenu gescannt
    
    m_ConfData.m_nLogFileMax=(LogfileSizeSpinbox->value())<<16;
    // LogMenu gescannt
    
     if (RatioNPrimComboBox->count()==0)   // es existiert keine eigenfehlertabelle, bzw. korrektur aus
    {
	 m_ConfData.m_NPrimary = genRatioText( RatioPrimNLineEdit->text(), nPrim_3radioButton, nPrim_w3radioButton);
	 m_ConfData.m_NSecondary = genRatioText( RatioSekNLineEdit->text(), nSek_3radioButton, nSek_w3radioButton);
    }
    else
    {	
	m_ConfData.m_NPrimary = RatioNPrimComboBox->currentText();
	m_ConfData.m_NSecondary = RatioNSekComboBox->currentText();
    }
    
    m_ConfData.m_MacSourceAdr.MacAdrByte[5]=MacSLineEdit6->text().toUShort(0,16);
    m_ConfData.m_MacSourceAdr.MacAdrByte[4]=MacSLineEdit5->text().toUShort(0,16);
    m_ConfData.m_MacSourceAdr.MacAdrByte[3]=MacSLineEdit4->text().toUShort(0,16);
    m_ConfData.m_MacSourceAdr.MacAdrByte[2]=MacSLineEdit3->text().toUShort(0,16);
    m_ConfData.m_MacSourceAdr.MacAdrByte[1]=MacSLineEdit2->text().toUShort(0,16);
    m_ConfData.m_MacSourceAdr.MacAdrByte[0]=MacSLineEdit1->text().toUShort(0,16);
    
    m_ConfData.m_MacDestAdr.MacAdrByte[5]=MacDLineEdit6->text().toUShort(0,16);
    m_ConfData.m_MacDestAdr.MacAdrByte[4]=MacDLineEdit5->text().toUShort(0,16);
    m_ConfData.m_MacDestAdr.MacAdrByte[3]=MacDLineEdit4->text().toUShort(0,16);
    m_ConfData.m_MacDestAdr.MacAdrByte[2]=MacDLineEdit3->text().toUShort(0,16);
    m_ConfData.m_MacDestAdr.MacAdrByte[1]=MacDLineEdit2->text().toUShort(0,16);
    m_ConfData.m_MacDestAdr.MacAdrByte[0]=MacDLineEdit1->text().toUShort(0,16);
    
    m_ConfData.ASDU = ASDUSpinBox->value();
    m_ConfData.DataSet = SetSpinBox->value();
    
    m_ConfData.m_nPriorityTagged = (TPIDlineEdit->text().toUShort(0,16) << 16);
    m_ConfData.m_nPriorityTagged +=(UPrioritylineEdit->text().toUShort(0,10) << 13);
    m_ConfData.m_nPriorityTagged +=(CFIlineEdit->text().toUShort(0,10) << 12);
    m_ConfData.m_nPriorityTagged +=(VIDlineEdit->text().toUShort(0,16));
    
    m_ConfData.m_nEthTypeHeader = (EthTypelineEdit->text().toUShort(0,16) << 16);
    m_ConfData.m_nEthTypeHeader += APPIDlineEdit->text().toUShort(0,16);
    
    // nConventMenu gescannt
    
    if (F16RadioButton->isChecked()) m_ConfData.m_nSFreq=F16;
    if (F50RadioButton->isChecked()) m_ConfData.m_nSFreq=F50;
    if (F60RadioButton->isChecked()) m_ConfData.m_nSFreq=F60;
    
    float f;
    switch (m_ConfData.m_nSFreq) {  // wir setzen den realen frequenzwert
      case F16: f = 50.0/3;break;
      case F50: f = 50.0;break;
      case F60: f = 60.0;
      }
    
    m_ConfData.m_fSFreq = f;
    
    m_ConfData.m_nMeasPeriod=CmpIntervallSpinBox->value();
    
    if (S80RadioButton->isChecked())
    {
	m_ConfData.m_nSRate=S80;
	CmpIntervallSpinBox->setMaxValue(40);
    }
	    
    if (S256RadioButton->isChecked())
    {
	m_ConfData.m_nSRate=S256;
	CmpIntervallSpinBox->setMaxValue(14);
	if (m_ConfData.m_nMeasPeriod > 14) 
	{
	    m_ConfData.m_nMeasPeriod = 14;
	    CmpIntervallSpinBox->setValue(14);
	}
    }
    
    m_ConfData.m_nIntegrationTime=TIntegrationSpinBox->value();
    //  MessungMenu gescannt
    
    Actualize();
}

void ConfDialogBase::SetnConventMenu()
{
    MacSLineEdit6->setText( QString("%1").arg(m_ConfData.m_MacSourceAdr.MacAdrByte[5],2,16).replace(' ','0').upper() );
    MacSLineEdit5->setText( QString("%1").arg(m_ConfData.m_MacSourceAdr.MacAdrByte[4],2,16).replace(' ','0').upper() );
    MacSLineEdit4->setText( QString("%1").arg(m_ConfData.m_MacSourceAdr.MacAdrByte[3],2,16).replace(' ','0').upper());
    MacSLineEdit3->setText( QString("%1").arg(m_ConfData.m_MacSourceAdr.MacAdrByte[2],2,16).replace(' ','0').upper() );
    MacSLineEdit2->setText( QString("%1").arg(m_ConfData.m_MacSourceAdr.MacAdrByte[1],2,16).replace(' ','0').upper() );
    MacSLineEdit1->setText( QString("%1").arg(m_ConfData.m_MacSourceAdr.MacAdrByte[0],2,16).replace(' ','0').upper() );
    
    MacDLineEdit6->setText( QString("%1").arg(m_ConfData.m_MacDestAdr.MacAdrByte[5],2,16).replace(' ','0').upper() );
    MacDLineEdit5->setText( QString("%1").arg(m_ConfData.m_MacDestAdr.MacAdrByte[4],2,16).replace(' ','0').upper() );
    MacDLineEdit4->setText( QString("%1").arg(m_ConfData.m_MacDestAdr.MacAdrByte[3],2,16).replace(' ','0').upper() );
    MacDLineEdit3->setText( QString("%1").arg(m_ConfData.m_MacDestAdr.MacAdrByte[2],2,16).replace(' ','0').upper() );
    MacDLineEdit2->setText( QString("%1").arg(m_ConfData.m_MacDestAdr.MacAdrByte[1],2,16).replace(' ','0').upper() );
    MacDLineEdit1->setText( QString("%1").arg(m_ConfData.m_MacDestAdr.MacAdrByte[0],2,16).replace(' ','0').upper() );
    
    ASDUSpinBox->setValue(m_ConfData.ASDU);
    SetSpinBox->setValue(m_ConfData.DataSet);
    
    TPIDlineEdit->setText(QString("%1").arg((m_ConfData.m_nPriorityTagged >> 16) & 0xFFFF,4,16).replace(' ','0').upper() );
    UPrioritylineEdit->setText(QString("%1").arg((m_ConfData.m_nPriorityTagged >> 13) & 7,1,10));
    CFIlineEdit->setText(QString("%1").arg((m_ConfData.m_nPriorityTagged >> 12) & 1,1,10));
    VIDlineEdit->setText(QString("%1").arg(m_ConfData.m_nPriorityTagged & 0xFFF,3,16).replace(' ','0').upper() );
    
    EthTypelineEdit->setText(QString("%1").arg((m_ConfData.m_nEthTypeHeader >> 16) & 0xFFFF,4,16).replace(' ','0').upper() );
    APPIDlineEdit->setText(QString("%1").arg(m_ConfData.m_nEthTypeHeader & 0xFFFF,4,16).replace(' ','0').upper() );
    
}


void ConfDialogBase::SetMessungMenu()
{
    switch (m_ConfData.m_nSFreq) {
    case F16:
	F16RadioButton->setChecked(true);
	break;
    case F50:
	F50RadioButton->setChecked(true);
	break;
    case F60:
	F60RadioButton->setChecked(true);
	break;
    }
    
    switch (m_ConfData.m_nSRate) {
    case S80:
	S80RadioButton->setChecked(true);
	CmpIntervallSpinBox->setMaxValue(40);
	break;
    case S256:
	S256RadioButton->setChecked(true);
	CmpIntervallSpinBox->setMaxValue(14);
	break;
    }
    
    CmpIntervallSpinBox->setValue(m_ConfData.m_nMeasPeriod);
    TIntegrationSpinBox->setValue(m_ConfData.m_nIntegrationTime);
}


void ConfDialogBase::RemoteCtrlInfoSlot(bool remote)
{
    m_bRemoteCtrl = remote;
    buttonOk->setEnabled(!remote);
    Actualize();
}


const QString& ConfDialogBase::baseUnitText(const QString& s )
{
    m_sText = s;
    m_sText = m_sText.replace("/w3", "");
    m_sText = m_sText.replace("/3", "");
    return m_sText;
}


void ConfDialogBase::nPrim_3radioButtonChecked()
{
    if (nPrim_3radioButton->isChecked())
	nPrim_w3radioButton->setChecked(false);
}


void ConfDialogBase::nPrim_w3radioButtonChecked()
{
    if (nPrim_w3radioButton->isChecked())
	nPrim_3radioButton->setChecked(false);
}


void ConfDialogBase::nSek_3radioButtonChecked()
{
    if (nSek_3radioButton->isChecked())
	nSek_w3radioButton->setChecked(false);
}


void ConfDialogBase::nSek_w3radioButtonChecked()
{
    if (nSek_w3radioButton->isChecked())
	nSek_3radioButton->setChecked(false);
}


void ConfDialogBase::xPrim_3radioButtonChecked()
{
    if (xPrim_3radioButton->isChecked())
	xPrim_w3radioButton->setChecked(false);
}



void ConfDialogBase::xPrim_w3radioButtonChecked()
{
        if (xPrim_w3radioButton->isChecked())
	xPrim_3radioButton->setChecked(false);

}


void ConfDialogBase::xSek_3radioButtonChecked()
{
    if (xSek_3radioButton->isChecked())
	xSek_w3radioButton->setChecked(false);
}


void ConfDialogBase::xSek_w3radioButtonChecked()
{
    if (xSek_w3radioButton->isChecked())
	xSek_3radioButton->setChecked(false);
}


void ConfDialogBase::evtPrim_3radioButtonChecked()
{
    if (evtPrim_3radioButton->isChecked())
	evtPrim_w3radioButton->setChecked(false);
}


void ConfDialogBase::evtPrim_w3radioButtonChecked()
{
    if (evtPrim_w3radioButton->isChecked())
	evtPrim_3radioButton->setChecked(false);
}


void ConfDialogBase::evtSek_3radioButtonChecked()
{
    if (evtSek_3radioButton->isChecked())
	evtSek_w3radioButton->setChecked(false);
}


void ConfDialogBase::evtSek_w3radioButtonChecked()
{
    if (evtSek_w3radioButton->isChecked())
	evtSek_3radioButton->setChecked(false);
}


bool ConfDialogBase::is_3( const QString &s )
{
    return (s.contains("/3") > 0);
}


bool ConfDialogBase::is_w3( const QString &s )
{
    return (s.contains("/w3") > 0);
}



const QString& ConfDialogBase::genRatioText(QString s, QRadioButton *qrb_3, QRadioButton *qrb_w3)
{
    m_sText = s;
    if (qrb_3->isChecked())
	m_sText += "/3";
    if (qrb_w3->isChecked())
	m_sText += "/w3";
    return m_sText;
}

