#include "QMessageBox"
#include "confdialogbase.h"
#include "ui_confdialogbase.h"

ConfDialogBase::ConfDialogBase( QWidget* parent):
    QDialog(parent),
    ui(new Ui::ConfDialogBase)
{
    ui->setupUi(this);
    init();
}

ConfDialogBase::~ConfDialogBase()
{
    destroy();
    delete ui;
}

void ConfDialogBase::init()
{
    QRegExp rx( "^[0-9,A-F]{2,2}$" );
    QValidator* MacValidator = new QRegExpValidator( rx, this );
    
    ui->MacSLineEdit6->setInputMask("NN");
    ui->MacSLineEdit6->setValidator(MacValidator);
    ui->MacSLineEdit5->setInputMask("NN");
    ui->MacSLineEdit5->setValidator(MacValidator);
    ui->MacSLineEdit4->setInputMask("NN");
    ui->MacSLineEdit4->setValidator(MacValidator);
    ui->MacSLineEdit3->setInputMask("NN");
    ui->MacSLineEdit3->setValidator(MacValidator);
    ui->MacSLineEdit2->setInputMask("NN");
    ui->MacSLineEdit2->setValidator(MacValidator);
    ui->MacSLineEdit1->setInputMask("NN");
    ui->MacSLineEdit1->setValidator(MacValidator);
    
    ui->MacDLineEdit6->setInputMask("NN");
    ui->MacDLineEdit6->setValidator(MacValidator);
    ui->MacDLineEdit5->setInputMask("NN");
    ui->MacDLineEdit5->setValidator(MacValidator);
    ui->MacDLineEdit4->setInputMask("NN");
    ui->MacDLineEdit4->setValidator(MacValidator);
    ui->MacDLineEdit3->setInputMask("NN");
    ui->MacDLineEdit3->setValidator(MacValidator);
    ui->MacDLineEdit2->setInputMask("NN");
    ui->MacDLineEdit2->setValidator(MacValidator);
    ui->MacDLineEdit1->setInputMask("NN");
    ui->MacDLineEdit1->setValidator(MacValidator);
    
    QRegExp rx2( "^[0-7]{1,1}$" );
    QValidator* UPriorityValidator = new QRegExpValidator( rx2, this );
    ui->UPrioritylineEdit->setInputMask("N");
    ui->UPrioritylineEdit->setValidator(UPriorityValidator);
    
    QRegExp rx3( "^[0-1]{1,1}$" );
    QValidator* CFIValidator = new QRegExpValidator( rx3, this );
    ui->CFIlineEdit->setInputMask("N");
    ui->CFIlineEdit->setValidator(CFIValidator);
    
    QRegExp rx4( "^[0-9,A-F]{3,3}$" );
    QValidator* VIDValidator = new QRegExpValidator( rx4, this );
    ui->VIDlineEdit->setInputMask("NNN");
    ui->VIDlineEdit->setValidator(VIDValidator);
    
    QRegExp rx5( "^[0-9,A-F]{4,4}$" );
    QValidator* APPIDValidator = new QRegExpValidator( rx5, this );
    ui->APPIDlineEdit->setInputMask("NNNN");
    ui->APPIDlineEdit->setValidator(APPIDValidator);
    
    QRegExp rx6 ("^([0-9]+)(|\\.([0-9]+))(|mV|kV|V)$" );
    QValidator* RatioValidator = new QRegExpValidator( rx6, this );
    ui->RatioPrimNLineEdit->setValidator(RatioValidator);
    ui->RatioSekNLineEdit->setValidator(RatioValidator);
    ui->RatioPrimXLineEdit->setValidator(RatioValidator);
    ui->RatioSekXLineEdit->setValidator(RatioValidator);
    ui->RatioPrimEVTLineEdit->setValidator(RatioValidator);
    ui->RatioSekEVTLineEdit->setValidator(RatioValidator);
    
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
        ui->RatioNPrimComboBox->clear();
        ui->RatioNPrimComboBox->insertStringList(NPItems); // normalwandler primär stufen
        ui->RatioNPrimComboBox->setCurrentItem(NPItems.findIndex(m_ConfData.m_NPrimary));
        ui->RatioNSekComboBox->clear();
        ui->RatioNSekComboBox->insertStringList(NSItems); // normalwandler sekundär stufen
        ui->RatioNSekComboBox->setCurrentItem(NSItems.findIndex(m_ConfData.m_NSecondary));    	}
}


void ConfDialogBase::Actualize()
{
   switch (m_ConfData.m_nMeasMode) {
	case Un_UxAbs:
	case Un_nConvent:
            ui->XTRatioGroupBox->setEnabled(true);
            ui->EVTratioGroupBox->setEnabled(false);
	    break;
	case Un_EVT:
            ui->XTRatioGroupBox->setEnabled(false);
            ui->EVTratioGroupBox->setEnabled(true);
	    break;
/*	case Un_nConvent:
            ui->XTRatioGroupBox->setEnabled(false);
            ui->EVTratioGroupBox->setEnabled(false);
	    break;*/
	}

   if (ui->RatioNPrimComboBox->count()==0  || m_bRemoteCtrl ) // es existiert keine eigenfehlertabelle
   { // bzw. wir sind unter rechner kontrolle
       ui->NTRatioGroupBox->setEnabled(true);
       ui->NTRatioGroupBox2->setEnabled(false);
    }
    else
    {
        ui->NTRatioGroupBox2->setEnabled(true);
        ui->NTRatioGroupBox->setEnabled(false);
    }
}


void ConfDialogBase::accept()
{
    bool ratioInputOK = true;
    
    ratioInputOK = ratioInputOK &&  ui->RatioPrimNLineEdit->hasAcceptableInput()
                                 && ui->RatioSekNLineEdit->hasAcceptableInput()
                                 && ui->RatioPrimXLineEdit->hasAcceptableInput()
                           && ui->RatioSekXLineEdit->hasAcceptableInput()
                           && ui->RatioPrimEVTLineEdit->hasAcceptableInput()
                           && ui->RatioSekEVTLineEdit->hasAcceptableInput();
    
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
            ui->Mode0RadioButton->setChecked(true);
	    break;
	case Un_EVT:
            ui->Mode2RadioButton->setChecked(true);
	    break;
	case Un_nConvent:
            ui->Mode3RadioButton->setChecked(true);
	    break;
	}
}


void ConfDialogBase::SetCmpMenu()
{
    ui->CmpKorrLineEdit1->setText(QString( "%1" ).arg(m_ConfData.m_fxPhaseShift,8,'f',4));
    ui->CmpKorrLineEdit2->setText(QString( "%1" ).arg(m_ConfData.m_fxTimeShift,5,'f',3));
    ui->CmpCorrCheckBox->setChecked(m_ConfData.m_bOECorrection);
}


void ConfDialogBase::SetSyncMenu()
{
    switch (m_ConfData.m_nSyncSource) {
	case Extern:
            ui->SyncRadioButton1->setChecked(true);
	    break;
	case Intern:
            ui->SyncRadioButton2->setChecked(true);
	    break;
	}
    
    ui->TSyncSpinBox->setValue(m_ConfData.m_nTSync);
    ui->SSynccheckBox->setChecked(m_ConfData.m_bStrongEthSynchronisation);
}


void ConfDialogBase::SetRatioMenu()
{
    // alle edit felder  und radiobuttons vorbesetzen
    ui->RatioPrimNLineEdit->setText(baseUnitText(m_ConfData.m_NPrimary));
    ui->RatioSekNLineEdit->setText(baseUnitText(m_ConfData.m_NSecondary));
    ui->nPrim_3radioButton->setChecked(is_3(m_ConfData.m_NPrimary));
    ui->nPrim_w3radioButton->setChecked(is_w3(m_ConfData.m_NPrimary));
    ui->nSek_3radioButton->setChecked(is_3(m_ConfData.m_NSecondary));
    ui->nSek_w3radioButton->setChecked(is_w3(m_ConfData.m_NSecondary));
    
    
    ui->RatioPrimXLineEdit->setText(baseUnitText(m_ConfData.m_XPrimary));
    ui->RatioSekXLineEdit->setText(baseUnitText(m_ConfData.m_XSecondary));
    ui->xPrim_3radioButton->setChecked(is_3(m_ConfData.m_XPrimary));
    ui->xPrim_w3radioButton->setChecked(is_w3(m_ConfData.m_XPrimary));
    ui->xSek_3radioButton->setChecked(is_3(m_ConfData.m_XSecondary));
    ui->xSek_w3radioButton->setChecked(is_w3(m_ConfData.m_XSecondary));
    
    
    ui->RatioPrimEVTLineEdit->setText(baseUnitText(m_ConfData.m_EVTPrimary));
    ui->RatioSekEVTLineEdit->setText(baseUnitText(m_ConfData.m_EVTSecondary));
    ui->evtPrim_3radioButton->setChecked(is_3(m_ConfData.m_EVTPrimary));
    ui->evtPrim_w3radioButton->setChecked(is_w3(m_ConfData.m_EVTPrimary));
    ui->evtSek_3radioButton->setChecked(is_3(m_ConfData.m_EVTSecondary));
    ui->evtSek_w3radioButton->setChecked(is_w3(m_ConfData.m_EVTSecondary));
}



void ConfDialogBase::SetLogMenu()
{
   ui->LogfileSizeSpinbox->setValue(m_ConfData.m_nLogFileMax >>16);
}


void ConfDialogBase::ApplyDataSlot() // einstellungen werden intern übernommen, die menus aktualisiert
{
    switch (m_ConfData.m_nMeasMode) {
	case Un_UxAbs:
	case Un_nConvent:
            m_ConfData.m_XPrimary = genRatioText( ui->RatioPrimXLineEdit->text(), ui->xPrim_3radioButton, ui->xPrim_w3radioButton);
            m_ConfData.m_XSecondary = genRatioText( ui->RatioSekXLineEdit->text(), ui->xSek_3radioButton, ui->xSek_w3radioButton);
	    break;
	case Un_EVT:
            m_ConfData.m_EVTPrimary = genRatioText( ui->RatioPrimEVTLineEdit->text(), ui->evtPrim_3radioButton, ui->evtPrim_w3radioButton);
            m_ConfData.m_EVTSecondary = genRatioText( ui->RatioSekEVTLineEdit->text(), ui->evtSek_3radioButton, ui->evtSek_w3radioButton);
	    break;
	}
 
    if (ui->Mode0RadioButton->isChecked()) m_ConfData.m_nMeasMode=Un_UxAbs;
    if (ui->Mode2RadioButton->isChecked()) m_ConfData.m_nMeasMode=Un_EVT;
    if (ui->Mode3RadioButton->isChecked()) m_ConfData.m_nMeasMode=Un_nConvent;
    
    // ModeMenu gescannt
    
    m_ConfData.m_fxPhaseShift=(ui->CmpKorrLineEdit1->text()).toDouble();
    m_ConfData.m_fxTimeShift=(ui->CmpKorrLineEdit2->text()).toDouble();
    m_ConfData.m_bOECorrection=ui->CmpCorrCheckBox->isChecked();
    // CmpMenu gescannt
    
    if (ui->SyncRadioButton1->isChecked()) m_ConfData.m_nSyncSource=Extern;
    if (ui->SyncRadioButton2->isChecked()) m_ConfData.m_nSyncSource=Intern;
    m_ConfData.m_nTSync=ui->TSyncSpinBox->value();
    m_ConfData.m_bStrongEthSynchronisation=ui->SSynccheckBox->isChecked();
    // SyncMenu gescannt
    
    m_ConfData.m_nLogFileMax=(ui->LogfileSizeSpinbox->value())<<16;
    // LogMenu gescannt
    
     if (ui->RatioNPrimComboBox->count()==0)   // es existiert keine eigenfehlertabelle, bzw. korrektur aus
    {
         m_ConfData.m_NPrimary = genRatioText( ui->RatioPrimNLineEdit->text(), ui->nPrim_3radioButton, ui->nPrim_w3radioButton);
         m_ConfData.m_NSecondary = genRatioText( ui->RatioSekNLineEdit->text(), ui->nSek_3radioButton, ui->nSek_w3radioButton);
    }
    else
    {	
        m_ConfData.m_NPrimary = ui->RatioNPrimComboBox->currentText();
        m_ConfData.m_NSecondary = ui->RatioNSekComboBox->currentText();
    }
    
    m_ConfData.m_MacSourceAdr.MacAdrByte[5]=ui->MacSLineEdit6->text().toUShort(0,16);
    m_ConfData.m_MacSourceAdr.MacAdrByte[4]=ui->MacSLineEdit5->text().toUShort(0,16);
    m_ConfData.m_MacSourceAdr.MacAdrByte[3]=ui->MacSLineEdit4->text().toUShort(0,16);
    m_ConfData.m_MacSourceAdr.MacAdrByte[2]=ui->MacSLineEdit3->text().toUShort(0,16);
    m_ConfData.m_MacSourceAdr.MacAdrByte[1]=ui->MacSLineEdit2->text().toUShort(0,16);
    m_ConfData.m_MacSourceAdr.MacAdrByte[0]=ui->MacSLineEdit1->text().toUShort(0,16);
    
    m_ConfData.m_MacDestAdr.MacAdrByte[5]=ui->MacDLineEdit6->text().toUShort(0,16);
    m_ConfData.m_MacDestAdr.MacAdrByte[4]=ui->MacDLineEdit5->text().toUShort(0,16);
    m_ConfData.m_MacDestAdr.MacAdrByte[3]=ui->MacDLineEdit4->text().toUShort(0,16);
    m_ConfData.m_MacDestAdr.MacAdrByte[2]=ui->MacDLineEdit3->text().toUShort(0,16);
    m_ConfData.m_MacDestAdr.MacAdrByte[1]=ui->MacDLineEdit2->text().toUShort(0,16);
    m_ConfData.m_MacDestAdr.MacAdrByte[0]=ui->MacDLineEdit1->text().toUShort(0,16);
    
    m_ConfData.ASDU = ui->ASDUSpinBox->value();
    m_ConfData.DataSet = ui->SetSpinBox->value();
    
    m_ConfData.m_nPriorityTagged = (ui->TPIDlineEdit->text().toUShort(0,16) << 16);
    m_ConfData.m_nPriorityTagged +=(ui->UPrioritylineEdit->text().toUShort(0,10) << 13);
    m_ConfData.m_nPriorityTagged +=(ui->CFIlineEdit->text().toUShort(0,10) << 12);
    m_ConfData.m_nPriorityTagged +=(ui->VIDlineEdit->text().toUShort(0,16));
    
    m_ConfData.m_nEthTypeHeader = (ui->EthTypelineEdit->text().toUShort(0,16) << 16);
    m_ConfData.m_nEthTypeHeader += ui->APPIDlineEdit->text().toUShort(0,16);
    
    // nConventMenu gescannt
    
    if (ui->F16RadioButton->isChecked()) m_ConfData.m_nSFreq=F16;
    if (ui->F50RadioButton->isChecked()) m_ConfData.m_nSFreq=F50;
    if (ui->F60RadioButton->isChecked()) m_ConfData.m_nSFreq=F60;
    
    float f;
    switch (m_ConfData.m_nSFreq) {  // wir setzen den realen frequenzwert
      case F16: f = 50.0/3;break;
      case F50: f = 50.0;break;
      case F60: f = 60.0;
      }
    
    m_ConfData.m_fSFreq = f;
    
    m_ConfData.m_nMeasPeriod=ui->CmpIntervallSpinBox->value();
    
    if (ui->S80RadioButton->isChecked())
    {
	m_ConfData.m_nSRate=S80;
        ui->CmpIntervallSpinBox->setMaxValue(40);
    }
	    
    if (ui->S256RadioButton->isChecked())
    {
	m_ConfData.m_nSRate=S256;
        ui->CmpIntervallSpinBox->setMaxValue(14);
	if (m_ConfData.m_nMeasPeriod > 14) 
	{
	    m_ConfData.m_nMeasPeriod = 14;
            ui->CmpIntervallSpinBox->setValue(14);
	}
    }
    
    m_ConfData.m_nIntegrationTime=ui->TIntegrationSpinBox->value();
    //  MessungMenu gescannt
    
    Actualize();
}

void ConfDialogBase::SetnConventMenu()
{
    ui->MacSLineEdit6->setText( QString("%1").arg(m_ConfData.m_MacSourceAdr.MacAdrByte[5],2,16).replace(' ','0').upper() );
    ui->MacSLineEdit5->setText( QString("%1").arg(m_ConfData.m_MacSourceAdr.MacAdrByte[4],2,16).replace(' ','0').upper() );
    ui->MacSLineEdit4->setText( QString("%1").arg(m_ConfData.m_MacSourceAdr.MacAdrByte[3],2,16).replace(' ','0').upper());
    ui->MacSLineEdit3->setText( QString("%1").arg(m_ConfData.m_MacSourceAdr.MacAdrByte[2],2,16).replace(' ','0').upper() );
    ui->MacSLineEdit2->setText( QString("%1").arg(m_ConfData.m_MacSourceAdr.MacAdrByte[1],2,16).replace(' ','0').upper() );
    ui->MacSLineEdit1->setText( QString("%1").arg(m_ConfData.m_MacSourceAdr.MacAdrByte[0],2,16).replace(' ','0').upper() );
    
    ui->MacDLineEdit6->setText( QString("%1").arg(m_ConfData.m_MacDestAdr.MacAdrByte[5],2,16).replace(' ','0').upper() );
    ui->MacDLineEdit5->setText( QString("%1").arg(m_ConfData.m_MacDestAdr.MacAdrByte[4],2,16).replace(' ','0').upper() );
    ui->MacDLineEdit4->setText( QString("%1").arg(m_ConfData.m_MacDestAdr.MacAdrByte[3],2,16).replace(' ','0').upper() );
    ui->MacDLineEdit3->setText( QString("%1").arg(m_ConfData.m_MacDestAdr.MacAdrByte[2],2,16).replace(' ','0').upper() );
    ui->MacDLineEdit2->setText( QString("%1").arg(m_ConfData.m_MacDestAdr.MacAdrByte[1],2,16).replace(' ','0').upper() );
    ui->MacDLineEdit1->setText( QString("%1").arg(m_ConfData.m_MacDestAdr.MacAdrByte[0],2,16).replace(' ','0').upper() );
    
    ui->ASDUSpinBox->setValue(m_ConfData.ASDU);
    ui->SetSpinBox->setValue(m_ConfData.DataSet);
    
    ui->TPIDlineEdit->setText(QString("%1").arg((m_ConfData.m_nPriorityTagged >> 16) & 0xFFFF,4,16).replace(' ','0').upper() );
    ui->UPrioritylineEdit->setText(QString("%1").arg((m_ConfData.m_nPriorityTagged >> 13) & 7,1,10));
    ui->CFIlineEdit->setText(QString("%1").arg((m_ConfData.m_nPriorityTagged >> 12) & 1,1,10));
    ui->VIDlineEdit->setText(QString("%1").arg(m_ConfData.m_nPriorityTagged & 0xFFF,3,16).replace(' ','0').upper() );
    
    ui->EthTypelineEdit->setText(QString("%1").arg((m_ConfData.m_nEthTypeHeader >> 16) & 0xFFFF,4,16).replace(' ','0').upper() );
    ui->APPIDlineEdit->setText(QString("%1").arg(m_ConfData.m_nEthTypeHeader & 0xFFFF,4,16).replace(' ','0').upper() );
    
}


void ConfDialogBase::SetMessungMenu()
{
    switch (m_ConfData.m_nSFreq) {
    case F16:
        ui->F16RadioButton->setChecked(true);
	break;
    case F50:
        ui->F50RadioButton->setChecked(true);
	break;
    case F60:
        ui->F60RadioButton->setChecked(true);
	break;
    }
    
    switch (m_ConfData.m_nSRate) {
    case S80:
        ui->S80RadioButton->setChecked(true);
        ui->CmpIntervallSpinBox->setMaxValue(40);
	break;
    case S256:
        ui->S256RadioButton->setChecked(true);
        ui->CmpIntervallSpinBox->setMaxValue(14);
	break;
    }
    
    ui->CmpIntervallSpinBox->setValue(m_ConfData.m_nMeasPeriod);
    ui->TIntegrationSpinBox->setValue(m_ConfData.m_nIntegrationTime);
}


void ConfDialogBase::RemoteCtrlInfoSlot(bool remote)
{
    m_bRemoteCtrl = remote;
    ui->buttonOk->setEnabled(!remote);
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
    if (ui->nPrim_3radioButton->isChecked())
        ui->nPrim_w3radioButton->setChecked(false);
}


void ConfDialogBase::nPrim_w3radioButtonChecked()
{
    if (ui->nPrim_w3radioButton->isChecked())
        ui->nPrim_3radioButton->setChecked(false);
}


void ConfDialogBase::nSek_3radioButtonChecked()
{
    if (ui->nSek_3radioButton->isChecked())
        ui->nSek_w3radioButton->setChecked(false);
}


void ConfDialogBase::nSek_w3radioButtonChecked()
{
    if (ui->nSek_w3radioButton->isChecked())
        ui->nSek_3radioButton->setChecked(false);
}


void ConfDialogBase::xPrim_3radioButtonChecked()
{
    if (ui->xPrim_3radioButton->isChecked())
        ui->xPrim_w3radioButton->setChecked(false);
}



void ConfDialogBase::xPrim_w3radioButtonChecked()
{
        if (ui->xPrim_w3radioButton->isChecked())
        ui->xPrim_3radioButton->setChecked(false);

}


void ConfDialogBase::xSek_3radioButtonChecked()
{
    if (ui->xSek_3radioButton->isChecked())
        ui->xSek_w3radioButton->setChecked(false);
}


void ConfDialogBase::xSek_w3radioButtonChecked()
{
    if (ui->xSek_w3radioButton->isChecked())
        ui->xSek_3radioButton->setChecked(false);
}


void ConfDialogBase::evtPrim_3radioButtonChecked()
{
    if (ui->evtPrim_3radioButton->isChecked())
        ui->evtPrim_w3radioButton->setChecked(false);
}


void ConfDialogBase::evtPrim_w3radioButtonChecked()
{
    if (ui->evtPrim_w3radioButton->isChecked())
        ui->evtPrim_3radioButton->setChecked(false);
}


void ConfDialogBase::evtSek_3radioButtonChecked()
{
    if (ui->evtSek_3radioButton->isChecked())
        ui->evtSek_w3radioButton->setChecked(false);
}


void ConfDialogBase::evtSek_w3radioButtonChecked()
{
    if (ui->evtSek_w3radioButton->isChecked())
        ui->evtSek_3radioButton->setChecked(false);
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

