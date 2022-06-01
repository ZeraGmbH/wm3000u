#include "QMessageBox"
#include "confdialogbase.h"
#include "ui_confdialogbase.h"
#include "wm3000u.h"

extern cWM3000U* g_WMDevice;

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

    QRegExp rx7( "^[1-8]{1,1}$" );
    QValidator* ASDUSetValidator = new QRegExpValidator( rx7, this );
    ui->FirstASDUlineEdit->setInputMask("N");
    ui->FirstASDUlineEdit->setValidator(ASDUSetValidator);
    ui->LastASDUlineEdit->setInputMask("N");
    ui->LastASDUlineEdit->setValidator(ASDUSetValidator);
    ui->SetlineEdit->setInputMask("N");
    ui->SetlineEdit->setValidator(ASDUSetValidator);

    connect(ui->buttonOk,SIGNAL(clicked()),this,SLOT(accept()));
    connect(ui->buttonCancel,SIGNAL(clicked()),this,SLOT(abortSlot()));
    connect(ui->Mode0RadioButton,SIGNAL(clicked()),this,SLOT(ApplyDataSlot()));
    connect(ui->Mode2RadioButton,SIGNAL(clicked()),this,SLOT(ApplyDataSlot()));
    connect(ui->Mode3RadioButton,SIGNAL(clicked()),this,SLOT(ApplyDataSlot()));
    connect(ui->DCRadioButton,SIGNAL(clicked()),this,SLOT(ApplyDataSlot()));
    connect(ui->ACRadioButton,SIGNAL(clicked()),this,SLOT(ApplyDataSlot()));
    connect(ui->CmpCorrCheckBox,SIGNAL(clicked()),this,SLOT(ApplyDataSlot()));
    connect(ui->nSek_w3radioButton,SIGNAL(clicked()),this,SLOT(nSek_w3radioButtonChecked()));
    connect(ui->nSek_3radioButton,SIGNAL(clicked()),this,SLOT(nSek_3radioButtonChecked()));
    connect(ui->nPrim_w3radioButton,SIGNAL(clicked()),this,SLOT(nPrim_w3radioButtonChecked()));
    connect(ui->nPrim_3radioButton,SIGNAL(clicked()),this,SLOT(nPrim_3radioButtonChecked()));
    connect(ui->evtPrim_w3radioButton,SIGNAL(clicked()),this,SLOT(evtPrim_w3radioButtonChecked()));
    connect(ui->evtPrim_3radioButton,SIGNAL(clicked()),this,SLOT(evtPrim_3radioButtonChecked()));
    connect(ui->evtSek_w3radioButton,SIGNAL(clicked()),this,SLOT(evtSek_w3radioButtonChecked()));
    connect(ui->evtSek_3radioButton,SIGNAL(clicked()),this,SLOT(evtSek_3radioButtonChecked()));
    connect(ui->xPrim_w3radioButton,SIGNAL(clicked()),this,SLOT(xPrim_w3radioButtonChecked()));
    connect(ui->xPrim_3radioButton,SIGNAL(clicked()),this,SLOT(xPrim_3radioButtonChecked()));
    connect(ui->xSek_w3radioButton,SIGNAL(clicked()),this,SLOT(xSek_w3radioButtonChecked()));
    connect(ui->xSek_3radioButton,SIGNAL(clicked()),this,SLOT(xSek_3radioButtonChecked()));
    connect(ui->S80RadioButton,SIGNAL(clicked()),this,SLOT(S80RadioButtonChecked()));
    connect(ui->S256RadioButton,SIGNAL(clicked()),this,SLOT(S256RadioButtonChecked()));

    m_bRemoteCtrl = false;

    if (g_WMDevice->isConventional())
    {
        ui->ConfTabWidget->removeTab(ui->ConfTabWidget->indexOf(ui->nConventTabPage));
        ui->ConfTabWidget->removeTab(ui->ConfTabWidget->indexOf(ui->SyncTabPage));
        ui->ModeButtonGroup->removeChild(ui->Mode3RadioButton);
        ui->ModeButtonGroup->removeChild(ui->Mode2RadioButton);
        ui->RatioTabPage->removeChild((ui->EVTratioGroupBox));
    }

    if (!g_WMDevice->isDC())
    {
        ui->ConfTabWidget->removeChild(ui->DCRadioButton);
        ui->ACRadioButton->setEnabled(false);
    }
}


void ConfDialogBase::SetConfInfoSlot(cConfData *cd )
{
    if ( true /*! isVisible()*/ ) {
    m_ConfDataTemp = m_ConfData = *cd;
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
        ui->RatioNPrimComboBox->setCurrentItem(NPItems.findIndex(m_ConfDataTemp.m_NPrimary));
        ui->RatioNSekComboBox->clear();
        ui->RatioNSekComboBox->insertStringList(NSItems); // normalwandler sekundär stufen
        ui->RatioNSekComboBox->setCurrentItem(NSItems.findIndex(m_ConfDataTemp.m_NSecondary));    	}
}


void ConfDialogBase::Actualize()
{
   switch (m_ConfDataTemp.m_nMeasMode) {
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
        m_ConfData = m_ConfDataTemp;

        emit SendConfDataSignal(&m_ConfData);
        close();
    }
}


void ConfDialogBase::abortSlot()
{
    m_ConfDataTemp = m_ConfData;
    SetConfInfoSlot(&m_ConfData);
    close();
}


void ConfDialogBase::SetModeMenu()
{
    switch (m_ConfDataTemp.m_nMeasMode) {
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

    ui->DCRadioButton->setChecked(m_ConfDataTemp.m_bDCmeasurement);
    ui->ACRadioButton->setChecked(!m_ConfDataTemp.m_bDCmeasurement);
}


void ConfDialogBase::SetCmpMenu()
{
    ui->CmpKorrLineEdit1->setText(QString( "%1" ).arg(m_ConfDataTemp.m_fxPhaseShift,8,'f',4));
    ui->CmpKorrLineEdit2->setText(QString( "%1" ).arg(m_ConfDataTemp.m_fxTimeShift,5,'f',3));
    ui->CmpCorrCheckBox->setChecked(m_ConfDataTemp.m_bOECorrection);
    ui->OffsetCorrectionNCheckbox->setChecked(m_ConfData.m_bOffsetCorrectionN);
    ui->OffsetCorrectionXCheckbox->setChecked(m_ConfData.m_bOffsetCorrectionX);
}


void ConfDialogBase::SetSyncMenu()
{
    switch (m_ConfDataTemp.m_nSyncSource) {
	case Extern:
            ui->SyncRadioButton1->setChecked(true);
	    break;
	case Intern:
            ui->SyncRadioButton2->setChecked(true);
	    break;
	}
    
    ui->TSyncSpinBox->setValue(m_ConfDataTemp.m_nTSync);
    ui->SSynccheckBox->setChecked(m_ConfDataTemp.m_bStrongEthSynchronisation);
}


void ConfDialogBase::SetRatioMenu()
{
    // alle edit felder  und radiobuttons vorbesetzen
    ui->RatioPrimNLineEdit->setText(baseUnitText(m_ConfDataTemp.m_NPrimary));
    ui->RatioSekNLineEdit->setText(baseUnitText(m_ConfDataTemp.m_NSecondary));
    ui->nPrim_3radioButton->setChecked(is_3(m_ConfDataTemp.m_NPrimary));
    ui->nPrim_w3radioButton->setChecked(is_w3(m_ConfDataTemp.m_NPrimary));
    ui->nSek_3radioButton->setChecked(is_3(m_ConfDataTemp.m_NSecondary));
    ui->nSek_w3radioButton->setChecked(is_w3(m_ConfDataTemp.m_NSecondary));
    
    
    ui->RatioPrimXLineEdit->setText(baseUnitText(m_ConfDataTemp.m_XPrimary));
    ui->RatioSekXLineEdit->setText(baseUnitText(m_ConfDataTemp.m_XSecondary));
    ui->xPrim_3radioButton->setChecked(is_3(m_ConfDataTemp.m_XPrimary));
    ui->xPrim_w3radioButton->setChecked(is_w3(m_ConfDataTemp.m_XPrimary));
    ui->xSek_3radioButton->setChecked(is_3(m_ConfDataTemp.m_XSecondary));
    ui->xSek_w3radioButton->setChecked(is_w3(m_ConfDataTemp.m_XSecondary));
    
    
    ui->RatioPrimEVTLineEdit->setText(baseUnitText(m_ConfDataTemp.m_EVTPrimary));
    ui->RatioSekEVTLineEdit->setText(baseUnitText(m_ConfDataTemp.m_EVTSecondary));
    ui->evtPrim_3radioButton->setChecked(is_3(m_ConfDataTemp.m_EVTPrimary));
    ui->evtPrim_w3radioButton->setChecked(is_w3(m_ConfDataTemp.m_EVTPrimary));
    ui->evtSek_3radioButton->setChecked(is_3(m_ConfDataTemp.m_EVTSecondary));
    ui->evtSek_w3radioButton->setChecked(is_w3(m_ConfDataTemp.m_EVTSecondary));
}



void ConfDialogBase::SetLogMenu()
{
   ui->LogfileSizeSpinbox->setValue(m_ConfDataTemp.m_nLogFileMax >>16);
}


void ConfDialogBase::ApplyDataSlot() // einstellungen werden intern übernommen, die menus aktualisiert
{
    switch (m_ConfDataTemp.m_nMeasMode) {
	case Un_UxAbs:
	case Un_nConvent:
            m_ConfDataTemp.m_XPrimary = genRatioText( ui->RatioPrimXLineEdit->text(), ui->xPrim_3radioButton, ui->xPrim_w3radioButton);
            m_ConfDataTemp.m_XSecondary = genRatioText( ui->RatioSekXLineEdit->text(), ui->xSek_3radioButton, ui->xSek_w3radioButton);
	    break;
	case Un_EVT:
            m_ConfDataTemp.m_EVTPrimary = genRatioText( ui->RatioPrimEVTLineEdit->text(), ui->evtPrim_3radioButton, ui->evtPrim_w3radioButton);
            m_ConfDataTemp.m_EVTSecondary = genRatioText( ui->RatioSekEVTLineEdit->text(), ui->evtSek_3radioButton, ui->evtSek_w3radioButton);
	    break;
	}
 
    if (ui->Mode0RadioButton->isChecked()) m_ConfDataTemp.m_nMeasMode=Un_UxAbs;
    if (ui->Mode2RadioButton->isChecked()) m_ConfDataTemp.m_nMeasMode=Un_EVT;
    if (ui->Mode3RadioButton->isChecked()) m_ConfDataTemp.m_nMeasMode=Un_nConvent;
    if (ui->DCRadioButton->isChecked())
        m_ConfDataTemp.m_bDCmeasurement = true;
    if (ui->ACRadioButton->isChecked())
        m_ConfDataTemp.m_bDCmeasurement = false;
    // ModeMenu gescannt
    
    m_ConfDataTemp.m_fxPhaseShift=(ui->CmpKorrLineEdit1->text()).toDouble();
    m_ConfDataTemp.m_fxTimeShift=(ui->CmpKorrLineEdit2->text()).toDouble();
    m_ConfDataTemp.m_bOECorrection=ui->CmpCorrCheckBox->isChecked();
    m_ConfDataTemp.m_bOffsetCorrectionN=ui->OffsetCorrectionNCheckbox->isChecked();
    m_ConfDataTemp.m_bOffsetCorrectionX=ui->OffsetCorrectionXCheckbox->isChecked();
    // CmpMenu gescannt
    
    if (ui->SyncRadioButton1->isChecked()) m_ConfDataTemp.m_nSyncSource=Extern;
    if (ui->SyncRadioButton2->isChecked()) m_ConfDataTemp.m_nSyncSource=Intern;
    m_ConfDataTemp.m_nTSync=ui->TSyncSpinBox->value();
    m_ConfDataTemp.m_bStrongEthSynchronisation=ui->SSynccheckBox->isChecked();
    // SyncMenu gescannt
    
    m_ConfDataTemp.m_nLogFileMax=(ui->LogfileSizeSpinbox->value())<<16;
    // LogMenu gescannt
    
     if (ui->RatioNPrimComboBox->count()==0)   // es existiert keine eigenfehlertabelle, bzw. korrektur aus
    {
         m_ConfDataTemp.m_NPrimary = genRatioText( ui->RatioPrimNLineEdit->text(), ui->nPrim_3radioButton, ui->nPrim_w3radioButton);
         m_ConfDataTemp.m_NSecondary = genRatioText( ui->RatioSekNLineEdit->text(), ui->nSek_3radioButton, ui->nSek_w3radioButton);
    }
    else
    {	
        m_ConfDataTemp.m_NPrimary = ui->RatioNPrimComboBox->currentText();
        m_ConfDataTemp.m_NSecondary = ui->RatioNSekComboBox->currentText();
    }
    
    m_ConfDataTemp.m_MacSourceAdr.MacAdrByte[5]=ui->MacSLineEdit6->text().toUShort(0,16);
    m_ConfDataTemp.m_MacSourceAdr.MacAdrByte[4]=ui->MacSLineEdit5->text().toUShort(0,16);
    m_ConfDataTemp.m_MacSourceAdr.MacAdrByte[3]=ui->MacSLineEdit4->text().toUShort(0,16);
    m_ConfDataTemp.m_MacSourceAdr.MacAdrByte[2]=ui->MacSLineEdit3->text().toUShort(0,16);
    m_ConfDataTemp.m_MacSourceAdr.MacAdrByte[1]=ui->MacSLineEdit2->text().toUShort(0,16);
    m_ConfDataTemp.m_MacSourceAdr.MacAdrByte[0]=ui->MacSLineEdit1->text().toUShort(0,16);
    
    m_ConfDataTemp.m_MacDestAdr.MacAdrByte[5]=ui->MacDLineEdit6->text().toUShort(0,16);
    m_ConfDataTemp.m_MacDestAdr.MacAdrByte[4]=ui->MacDLineEdit5->text().toUShort(0,16);
    m_ConfDataTemp.m_MacDestAdr.MacAdrByte[3]=ui->MacDLineEdit4->text().toUShort(0,16);
    m_ConfDataTemp.m_MacDestAdr.MacAdrByte[2]=ui->MacDLineEdit3->text().toUShort(0,16);
    m_ConfDataTemp.m_MacDestAdr.MacAdrByte[1]=ui->MacDLineEdit2->text().toUShort(0,16);
    m_ConfDataTemp.m_MacDestAdr.MacAdrByte[0]=ui->MacDLineEdit1->text().toUShort(0,16);
    
    m_ConfDataTemp.FirstASDU = ui->FirstASDUlineEdit->text().toUShort(0,10);
    m_ConfDataTemp.LastASDU = ui->LastASDUlineEdit->text().toUShort(0,10);
    m_ConfDataTemp.DataSet = ui->SetlineEdit->text().toUShort(0,10);
    
    m_ConfDataTemp.m_nPriorityTagged = (ui->TPIDlineEdit->text().toUShort(0,16) << 16);
    m_ConfDataTemp.m_nPriorityTagged +=(ui->UPrioritylineEdit->text().toUShort(0,10) << 13);
    m_ConfDataTemp.m_nPriorityTagged +=(ui->CFIlineEdit->text().toUShort(0,10) << 12);
    m_ConfDataTemp.m_nPriorityTagged +=(ui->VIDlineEdit->text().toUShort(0,16));
    
    m_ConfDataTemp.m_nEthTypeHeader = (ui->EthTypelineEdit->text().toUShort(0,16) << 16);
    m_ConfDataTemp.m_nEthTypeHeader += ui->APPIDlineEdit->text().toUShort(0,16);
    
    // nConventMenu gescannt
    
    if (ui->F16RadioButton->isChecked()) m_ConfDataTemp.m_nSFreq=F16;
    if (ui->F50RadioButton->isChecked()) m_ConfDataTemp.m_nSFreq=F50;
    if (ui->F60RadioButton->isChecked()) m_ConfDataTemp.m_nSFreq=F60;
    
    float f;
    switch (m_ConfDataTemp.m_nSFreq) {  // wir setzen den realen frequenzwert
      case F16: f = 50.0/3;break;
      case F50: f = 50.0;break;
      case F60: f = 60.0;
      }
    
    m_ConfDataTemp.m_fSFreq = f;
    
    m_ConfDataTemp.m_nMeasPeriod=ui->CmpIntervallSpinBox->value();
    
    if (ui->S80RadioButton->isChecked())
    {
        m_ConfDataTemp.m_nSRate=S80;
        ui->CmpIntervallSpinBox->setMaxValue(nmaxS80MeasPeriod);
    }
	    
    if (ui->S256RadioButton->isChecked())
    {
        m_ConfDataTemp.m_nSRate=S256;
        ui->CmpIntervallSpinBox->setMaxValue(nmaxS256MeasPeriod);
        /*
        if (m_ConfDataTemp.m_nMeasPeriod > 14)
        {
            m_ConfDataTemp.m_nMeasPeriod = 14;
                ui->CmpIntervallSpinBox->setValue(14);
        }
        */
    }

    m_ConfDataTemp.m_nIntegrationTime=ui->TIntegrationSpinBox->value();
    //  MessungMenu gescannt
    
    Actualize();
}

void ConfDialogBase::SetnConventMenu()
{
    ui->MacSLineEdit6->setText( QString("%1").arg(m_ConfDataTemp.m_MacSourceAdr.MacAdrByte[5],2,16).replace(' ','0').upper() );
    ui->MacSLineEdit5->setText( QString("%1").arg(m_ConfDataTemp.m_MacSourceAdr.MacAdrByte[4],2,16).replace(' ','0').upper() );
    ui->MacSLineEdit4->setText( QString("%1").arg(m_ConfDataTemp.m_MacSourceAdr.MacAdrByte[3],2,16).replace(' ','0').upper());
    ui->MacSLineEdit3->setText( QString("%1").arg(m_ConfDataTemp.m_MacSourceAdr.MacAdrByte[2],2,16).replace(' ','0').upper() );
    ui->MacSLineEdit2->setText( QString("%1").arg(m_ConfDataTemp.m_MacSourceAdr.MacAdrByte[1],2,16).replace(' ','0').upper() );
    ui->MacSLineEdit1->setText( QString("%1").arg(m_ConfDataTemp.m_MacSourceAdr.MacAdrByte[0],2,16).replace(' ','0').upper() );
    
    ui->MacDLineEdit6->setText( QString("%1").arg(m_ConfDataTemp.m_MacDestAdr.MacAdrByte[5],2,16).replace(' ','0').upper() );
    ui->MacDLineEdit5->setText( QString("%1").arg(m_ConfDataTemp.m_MacDestAdr.MacAdrByte[4],2,16).replace(' ','0').upper() );
    ui->MacDLineEdit4->setText( QString("%1").arg(m_ConfDataTemp.m_MacDestAdr.MacAdrByte[3],2,16).replace(' ','0').upper() );
    ui->MacDLineEdit3->setText( QString("%1").arg(m_ConfDataTemp.m_MacDestAdr.MacAdrByte[2],2,16).replace(' ','0').upper() );
    ui->MacDLineEdit2->setText( QString("%1").arg(m_ConfDataTemp.m_MacDestAdr.MacAdrByte[1],2,16).replace(' ','0').upper() );
    ui->MacDLineEdit1->setText( QString("%1").arg(m_ConfDataTemp.m_MacDestAdr.MacAdrByte[0],2,16).replace(' ','0').upper() );
    
    ui->FirstASDUlineEdit->setText(QString("%1").arg(m_ConfDataTemp.FirstASDU) );
    ui->LastASDUlineEdit->setText(QString("%1").arg(m_ConfDataTemp.LastASDU) );
    ui->SetlineEdit->setText(QString("%1").arg(m_ConfDataTemp.DataSet) );

    ui->TPIDlineEdit->setText(QString("%1").arg((m_ConfDataTemp.m_nPriorityTagged >> 16) & 0xFFFF,4,16).replace(' ','0').upper() );
    ui->UPrioritylineEdit->setText(QString("%1").arg((m_ConfDataTemp.m_nPriorityTagged >> 13) & 7,1,10));
    ui->CFIlineEdit->setText(QString("%1").arg((m_ConfDataTemp.m_nPriorityTagged >> 12) & 1,1,10));
    ui->VIDlineEdit->setText(QString("%1").arg(m_ConfDataTemp.m_nPriorityTagged & 0xFFF,3,16).replace(' ','0').upper() );
    
    ui->EthTypelineEdit->setText(QString("%1").arg((m_ConfDataTemp.m_nEthTypeHeader >> 16) & 0xFFFF,4,16).replace(' ','0').upper() );
    ui->APPIDlineEdit->setText(QString("%1").arg(m_ConfDataTemp.m_nEthTypeHeader & 0xFFFF,4,16).replace(' ','0').upper() );
    
}


void ConfDialogBase::SetMessungMenu()
{
    switch (m_ConfDataTemp.m_nSFreq) {
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
    
    switch (m_ConfDataTemp.m_nSRate) {
    case S80:
        ui->S80RadioButton->setChecked(true);
        ui->CmpIntervallSpinBox->setMaxValue(nmaxS80MeasPeriod);
	break;
    case S256:
        ui->S256RadioButton->setChecked(true);
        ui->CmpIntervallSpinBox->setMaxValue(nmaxS256MeasPeriod);
	break;
    }
    
    ui->CmpIntervallSpinBox->setValue(m_ConfDataTemp.m_nMeasPeriod);
    ui->TIntegrationSpinBox->setValue(m_ConfDataTemp.m_nIntegrationTime);
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


void ConfDialogBase::S80RadioButtonChecked()
{
    if (ui->S80RadioButton->isChecked())
    {
        m_ConfDataTemp.FirstASDU = 1;
        m_ConfDataTemp.LastASDU = 1;
        SetnConventMenu();
        ApplyDataSlot();
    }
}


void ConfDialogBase::S256RadioButtonChecked()
{
    if (ui->S256RadioButton->isChecked())
    {
        m_ConfDataTemp.FirstASDU = 1;
        m_ConfDataTemp.LastASDU = 8;
        SetnConventMenu();
        ApplyDataSlot();
    }
}


bool ConfDialogBase::is_3( const QString &s )
{
    return (s.contains("/3"));
}


bool ConfDialogBase::is_w3( const QString &s )
{
    return (s.contains("/w3"));
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




