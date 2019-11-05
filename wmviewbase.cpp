#include <Q3MainWindow>
#include <qlabel.h>
#include <QFileInfo>
#include <Q3FileDialog>
#include <QStatusBar>
#include <QDebug>
#include <QTextEdit>

#include "wmglobal.h"
#include "wmeditor.h"
#include "widgeom.h"
#include "wmviewbase.h"
#include "ui_wmviewbase.h"

WMViewBase::WMViewBase(QWidget *parent) :
    Q3MainWindow(parent),
    ui(new Ui::WMViewBase)
{
    ui->setupUi((Q3MainWindow*) this);
    init();
}

WMViewBase::~WMViewBase()
{
    destroy();
    delete ui;
}

extern QApplication *g_app;

void WMViewBase::removeJustageItem()
{
    ui->MenuBarEditor->removeAction(ui->Justage->menuAction());
}


void WMViewBase::configureWM1000Items()
{
    ui->Ansicht->removeAction(ui->ansichtEN61850Action);
    QString s;
    s = QObject::tr("Wandlermesseinrichtung WM1000U");
    setWindowTitle(s);
}


void WMViewBase::configureWMwoDC()
{
    ui->Messung->removeAction(ui->messungOffsetabgleichWM3000Action);
    ui->Messung->removeAction(ui->messungOffsetKanalNAction);
    ui->Messung->removeAction(ui->messungOffsetKanalXAction);
    ui->Ansicht->removeAction(ui->ansichtOffsetAction);
}


void WMViewBase::init()
{
    m_bJustified = false;
    m_bFreqQuestionable = false;
    for (int i =0 ; i < nmaxRecentOEFiles; i++) m_nrecentOEFileIds[i] = -1;
    for (int i =0 ; i < nmaxRecentMVFiles; i++) m_nrecentMVFileIds[i] = -1;
    for (int i =0 ; i < nmaxRecentSESFiles; i++) m_nrecentSESFileIds[i] = -1;
    CreateStatusBar();
    wmEdit = new wmEditor();
    wmEdit2 = new wmEditor();
    connect(wmEdit,SIGNAL(textMessage(QString)),this,SLOT(ReceiveOETFileSlot(QString)));
    connect(wmEdit2,SIGNAL(textMessage(QString)),this,SLOT(ReceiveResultFileSlot(QString)));
    ui->ansichtFehlerMessungAction->setChecked(false);
    ui->ansichtIstwerteAction->setChecked(false);
    ui->ansichtOffsetAction->setChecked(false);
    ui->ansichtDialogAction->setChecked(false);
    ui->ansichtEigenfehlerAction->setChecked(false);
    ui->ansichtEN61850Action->setChecked(false);

    LoadSession(".ses");

    connect(ui->ansichtFehlerMessungAction,SIGNAL(toggled(bool)),this,SIGNAL(UIansichtFehlerMessungActionToggled(bool))); // öffnen der fehlermesswert anzeige
    connect(this,SIGNAL(UIansichtFehlerMessungActionSet(bool)),ui->ansichtFehlerMessungAction,SLOT(setChecked(bool)));
    connect(this,SIGNAL(UIansichtFehlerMessungActionSet(bool)),this,SLOT(SaveDefaultSessionSlot(bool)));

    connect(ui->ansichtEigenfehlerAction,SIGNAL(toggled(bool)),this,SIGNAL(UIansichtEigenfehlerActionToggled(bool))); // öffnen der eigenfehler anzeige
    connect(this,SIGNAL(UIansichtEigenfehlerActionSet(bool)),ui->ansichtEigenfehlerAction,SLOT(setChecked(bool)));
    connect(this,SIGNAL(UIansichtEigenfehlerActionSet(bool)),this,SLOT(SaveDefaultSessionSlot(bool)));

    connect(ui->ansichtIstwerteAction,SIGNAL(toggled(bool)),this,SIGNAL(UIansichtIstwerteActionToggled(bool))); // öffnen der eigenfehler anzeige
    connect(this,SIGNAL(UIansichtIstwerteActionSet(bool)),ui->ansichtIstwerteAction,SLOT(setChecked(bool)));
    connect(this,SIGNAL(UIansichtIstwerteActionSet(bool)),this,SLOT(SaveDefaultSessionSlot(bool)));

    connect(ui->ansichtOffsetAction,SIGNAL(toggled(bool)),this,SIGNAL(UIansichtOffsetActionToggled(bool)));
    connect(this,SIGNAL(UIansichtOffsetActionSet(bool)),ui->ansichtOffsetAction,SLOT(setChecked(bool)));
    connect(this,SIGNAL(UIansichtOffsetActionSet(bool)),this,SLOT(SaveDefaultSessionSlot(bool)));

    connect(ui->ansichtDialogAction,SIGNAL(toggled(bool)),this,SIGNAL(UIansichtDialogActionToggled(bool))); // öffnen der eigenfehler anzeige
    connect(this,SIGNAL(UIansichtDialogActionSet(bool)),ui->ansichtDialogAction,SLOT(setChecked(bool)));
    connect(this,SIGNAL(UIansichtDialogActionSet(bool)),this,SLOT(SaveDefaultSessionSlot(bool)));

    connect(ui->ansichtEN61850Action,SIGNAL(toggled(bool)),this,SIGNAL(UIansichtEN61850ActionToggled(bool))); // öffnen der eigenfehler anzeige
    connect(this,SIGNAL(UIansichtEN61850ActionSet(bool)),ui->ansichtEN61850Action,SLOT(setChecked(bool)));
    connect(this,SIGNAL(UIansichtEN61850ActionSet(bool)),this,SLOT(SaveDefaultSessionSlot(bool)));

    connect(ui->hilfeManualAction,SIGNAL(activated()),this,SIGNAL(UIhilfeManualActionActivated()));
    connect(ui->einstellungenConfAction,SIGNAL(activated()),this,SIGNAL(UIeinstellungenConfActionActivated()));
    connect(ui->einstellungenBereichAction,SIGNAL(activated()),this,SIGNAL(UIeinstellungenBereichActionActivated()));
    connect(ui->JustageAmplitudeAction,SIGNAL(activated()),this,SIGNAL(UIJustageAmplitudeActionActivated()));
    connect(ui->JustagePhaseAction,SIGNAL(activated()),this,SIGNAL(UIJustagePhaseActionActivated()));
    connect(ui->JustageKoeffBerechnungAction,SIGNAL(activated()),this,SIGNAL(UIJustageKoeffBerechnungActionActivated()));
    connect(ui->JustageOffsetAction,SIGNAL(activated()),this,SIGNAL(UIJustageOffsetActionActivated()));
    connect(ui->JustageOffsetBerechnungAction,SIGNAL(activated()),this,SIGNAL(UIJustageOffsetBerechnungActionActivated()));
    connect(ui->messungOffsetabgleichWM3000Action,SIGNAL(activated()),this,SIGNAL(UIJustageOffsetVarActionActivated()));
    connect(ui->messungOffsetKanalNAction,SIGNAL(activated()),this,SIGNAL(UIMessungOffsetKanalNActivated()));
    connect(ui->messungOffsetKanalXAction,SIGNAL(activated()),this,SIGNAL(UIMessungOffsetKanalXActivated()));
    connect(ui->hilfeInfo_ber_QtAction,SIGNAL(activated()),this,SIGNAL(UIhilfeInfo_ber_QtActionActivated()));
    connect(ui->hilfeInfo_ber_ZeraAction,SIGNAL(activated()),this,SIGNAL(UIhilfeInfo_ber_ZeraActionActivated()));
    connect(ui->hilfeInfoAction,SIGNAL(activated()),this,SIGNAL(UIhilfeInfoActionActivated()));
    connect(ui->hilfeSelbsttestAction,SIGNAL(activated()),this,SIGNAL(UIhilfeSelbsttestActionActivated()));
    connect(ui->dateiBeendenAction,SIGNAL(activated()),this,SIGNAL(UIdateiBeendenActionActivated()));
    connect(ui->hilfeVersionAction,SIGNAL(activated()),this,SIGNAL(UIhilfeVersionActionActivated()));
    connect(ui->hilfeReleaseInfoAction,SIGNAL(activated()),this,SIGNAL(UIhilfeReleaseInfoActionActivated()));

}


void WMViewBase::destroy()
{
    SaveSession(".ses");
}


void WMViewBase::SetViewConfDataInfoSlot( cConfData *cd )
{
    m_ConfData=*cd;
    ActualizeStates();
}


void WMViewBase::ActualizeStates()
{
    if (m_ConfData.m_bDCmeasurement)
    {
        ui->messungOffsetKanalNAction->setEnabled(true);
        ui->messungOffsetKanalXAction->setEnabled(true);
    }
    else
    {
        ui->messungOffsetKanalNAction->setEnabled(false);
        ui->messungOffsetKanalXAction->setEnabled(false);
    }

    if (m_ConfData.m_bRunning)
    {
        ui->messungStartAction->setMenuText(tr("S&top"));
        ui->messungStartAction->setStatusTip(tr("Messung anhalten"));
    }
    else
    {
        ui->messungStartAction->setMenuText(tr("S&tart"));
        ui->messungStartAction->setStatusTip(tr("Messung starten"));
    }
    
    ui->messungSimulationAction->setOn(m_ConfData.m_bSimulation);
    ui->hilfeSelbsttestAction->setDisabled(m_ConfData.m_bSimulation); // selbsttest disabled wenn simulation
    
    m_pRunningLabel->setText( QString (( m_ConfData.m_bRunning) ? tr("Gestartet") : tr("Gestoppt")));
    m_pSimulationLabel->setText( QString (( m_ConfData.m_bSimulation) ? tr("Simulation") : tr("Reale Messung")));
    
    m_pStatusLabel->setText( m_bJustified ? tr("Justiert") : tr("Nicht justiert"));

    m_pFreqLabel->setText( m_bFreqQuestionable ? tr("!!SignalFrequenz!!") : tr(""));
    
    QFileInfo fi (m_ConfData.m_sOETFile);
    m_pOETLabel->setText("EFT="+((m_ConfData.m_sOETFile=="") ? tr("Keine") : fi.baseName())); // statuszeile eigenfehlertabelle eintragen
    UpdateRecentFileList(recentOETFiles,m_ConfData.m_sOETFile);
    
    fi.setFile(m_ConfData.m_sResultFile);
    m_pResultLabel->setText("MED="+((m_ConfData.m_sResultFile=="") ? tr("Keine") : fi.baseName())); // statuszeile ergebnisdatei  eintragen
    UpdateRecentFileList(recentResultFiles,m_ConfData.m_sResultFile);
    
    m_pRangeNLabel->setText(QString("ChN=%1").arg(m_ConfData.m_sRangeN)); // statuszeile bereich N eintragen
    
    switch (m_ConfData.m_nMeasMode) { // statuszeile bereich x,diff,ect eintragen
	case Un_UxAbs: 
	    m_pRangeXLabel->setText(QString("ChX=%1").arg(m_ConfData.m_sRangeX));
	    m_pRangeXLabel->show();
	    break;
	case Un_EVT:
	    m_pRangeXLabel->setText(QString("ChEVT=%1").arg(m_ConfData.m_sRangeEVT));
	    m_pRangeXLabel->show();
	    break;
	case Un_nConvent:
	    m_pRangeXLabel->hide();
	    break;
	}
}


void WMViewBase::OpenOETFileSlot() // zum laden einer eigenfehlertabelle
{
   Q3FileDialog *OETFileDialog=new Q3FileDialog ( wm3000uHome,tr("Eigenfehlertabellen (*.oet)"),this);
   OETFileDialog->setCaption(tr("Eigenfehlertabellen"));
   OETFileDialog->setMode( Q3FileDialog::AnyFile);
   OETFileDialog-> setSelection("./Transformer.oet");
   if ( OETFileDialog->exec() == QDialog::Accepted ) {
       QString OETFile = OETFileDialog->selectedFile();
       UpdateRecentFileList(recentOETFiles,OETFile);
       m_ConfData.m_sOETFile = OETFile;
       m_ConfData.m_bOENewLoad = true; // 1x analysieren
       emit SendConfDataSignal(&m_ConfData);
   }
    delete OETFileDialog;
}
	

void WMViewBase::UpdateRecentFileList(QStringList& sl, QString f)
{
    if (f != "") {
	sl.remove(f); // wenn es den eintrag schon gibt -> löschen
	sl.push_front(f); // dann neuen an den anfang der liste setzen
    }
    
    while ((int) recentOETFiles.size()>nmaxRecentOEFiles)
	recentOETFiles.pop_back();
    int n = (int) recentOETFiles.size();
    
    for (int i=0; i<n; i++)
    {
	QString text = QString ("&%1 %2").arg(i+1).arg(strippedName(recentOETFiles[i]));
	if (m_nrecentOEFileIds[i]==-1)
	{
            m_nrecentOEFileIds[i]=ui->Datei->insertItem(text,this,SLOT(OpenRecentOETFileSlot(int)),0,i,1+i);
            ui->Datei->setItemParameter(m_nrecentOEFileIds[i],i);
	}
	else
	{
            ui->Datei->changeItem(m_nrecentOEFileIds[i],text);
	}
    }
    
    while ((int) recentResultFiles.size()>nmaxRecentMVFiles)
	recentResultFiles.pop_back();
        
    for (int i=0; i< (int) recentResultFiles.size(); i++)
    {
	QString text = QString ("&%1 %2").arg(i+n+1).arg(strippedName(recentResultFiles[i]));
	if (m_nrecentMVFileIds[i]==-1)
	{
            m_nrecentMVFileIds[i]=ui->Datei->insertItem(text,this,SLOT(OpenRecentResultFileSlot(int)),0,nmaxRecentOEFiles+i,ui->Datei->count()-2); // wir nehmen eine id > nmaxRecentOEFiles -1 ---> damit eindeutigkeit herrscht
            ui->Datei->setItemParameter(m_nrecentMVFileIds[i],i);
	}
	else
	{
            ui->Datei->changeItem(m_nrecentMVFileIds[i],text);
	}
    }
}


QString WMViewBase::strippedName(QString s)
{
   QFileInfo fi(s); 
   return(fi.fileName());
}


void WMViewBase::OpenRecentOETFileSlot(int index)
{
    QString s = recentOETFiles[index];
    UpdateRecentFileList(recentOETFiles,s); // liste event. kürzen und einträge ins menu
    m_ConfData.m_sOETFile = s;
    m_ConfData.m_bOENewLoad = true;
    emit SendConfDataSignal(&m_ConfData);
}


void WMViewBase::SimulationSlot()
{
    m_ConfData.m_bSimulation = !m_ConfData.m_bSimulation;
    emit SendConfDataSignal(&m_ConfData);
    ActualizeStates();
}


void WMViewBase::StartSlot()
{
    m_ConfData.m_bRunning = !m_ConfData.m_bRunning;
    emit SendConfDataSignal(&m_ConfData); 
    ActualizeStates();
}


void WMViewBase::CreateStatusBar()
{
    m_pOETLabel=new QLabel("",this); // eigenfehlertabelle in statuszeile
    statusBar()->addPermanentWidget(m_pOETLabel,0);
    m_pResultLabel=new QLabel("",this); // ergebnisdatei in statuszeile
    statusBar()->addPermanentWidget(m_pResultLabel,0);
    m_pRangeNLabel=new QLabel("",this); // bereich N in statuszeile
    statusBar()->addPermanentWidget(m_pRangeNLabel,0);
    m_pRangeXLabel=new QLabel("",this); // bereich X in statuszeile
    statusBar()->addPermanentWidget(m_pRangeXLabel,0);
    m_pRunningLabel=new QLabel("",this); // gestartet bzw. gestoppt in statuszeile
    statusBar()->addPermanentWidget(m_pRunningLabel,0);
    m_pSimulationLabel=new QLabel("",this); // simulation oder reale messung in statuszeile
    statusBar()->addPermanentWidget(m_pSimulationLabel,0);
    m_pStatusLabel=new QLabel("",this); // normaler ablauf oder fehlerausgaben in statuszeile
    statusBar()->addPermanentWidget(m_pStatusLabel,0);
    m_pStatusLabel->setStyleSheet("QLabel {color:red;}");
    m_pFreqLabel=new QLabel("",this); // erstmal kein fehler
    m_pFreqLabel->setStyleSheet("QLabel {color:red;}");
    statusBar()->addPermanentWidget(m_pFreqLabel,0);
    m_pDummyLabel=new QLabel("",this); // letztes feld ist bereich N in statuszeile
    statusBar()->addPermanentWidget(m_pDummyLabel,1);
}


void WMViewBase::CloseOETFileSlot()
{
    m_ConfData.m_sOETFile = "";
    m_ConfData.m_bOENewLoad = true;
    emit SendConfDataSignal(&m_ConfData);
}


void WMViewBase::EditOETFileSlot()
{
    if (m_ConfData.m_sOETFile=="")  // wir haben noch keine eigenfehlertabelle geöffnet
	OpenOETFileSlot();
    if (m_ConfData.m_sOETFile=="") return; // nichts ausgewählt -> fertig

    wmEdit->setCaption(strippedName(m_ConfData.m_sOETFile));
    QFile file(m_ConfData.m_sOETFile); // text einlesen 
    if ( file.open( IO_ReadWrite ) ) {
	QTextStream stream( &file );
        wmEdit->setTextFormat(Qt::PlainText);
	QString text = stream.read();
	if (text.isEmpty())
	    text = "100V;100V;100%;0.01%;0.0grad";
	wmEdit->setText( text );
	file.close();
	wmEdit->show();
    } // wenn der editor geschlossen wird -> signal mit neuem text (geht auf den slot eins tiefer)
}


void WMViewBase::ReceiveOETFileSlot(QString nText)
{
    wmEdit->hide();
    if (wmEdit->isModified()) { // der text hat sich geändert
        m_ConfData.m_sOETFile = Q3FileDialog::getSaveFileName( m_ConfData.m_sOETFile,
							      tr("Eigenfehlertabelle (*oet)"),
							      this,
                                  "",
                                  tr("Speichern unter"));
    m_ConfData.m_bOENewLoad = true;
    }
        
    QFile file(m_ConfData.m_sOETFile); 
    file.remove();
    if ( file.open( IO_WriteOnly ) ) {
	QTextStream stream( &file );
	stream << nText;
	file.close();
	emit SendConfDataSignal(&m_ConfData);
    }
}


void WMViewBase::OpenResultFileSlot()
{
    Q3FileDialog *ResultFileDialog=new Q3FileDialog ( ".",tr("Ergebnisdateien (*.xml)"),this);
    ResultFileDialog->setDir(QDir(QString("%1%2").arg(wm3000uHome).arg("results")));
    ResultFileDialog->setCaption(tr("Ergebnisdateien"));
    ResultFileDialog->setMode( Q3FileDialog::AnyFile);
    ResultFileDialog->setSelection("results.xml");
    if ( ResultFileDialog->exec() == QDialog::Accepted ) {
    QString rFile;
    rFile=ResultFileDialog->selectedFile();
    UpdateRecentFileList(recentResultFiles,rFile); // liste event. kürzen und einträge ins menu
    m_ConfData.m_sResultFile = rFile;
    emit SendConfDataSignal(&m_ConfData);
    }
    delete ResultFileDialog;
}


void WMViewBase::EditResultFileSlot()
{
   if (m_ConfData.m_sResultFile=="")  // wir haben noch keine ergebnisdatei geöffnet
	OpenResultFileSlot();
    if (m_ConfData.m_sResultFile=="") return; // nichts ausgewählt -> fertig

    wmEdit2->setCaption(strippedName(m_ConfData.m_sResultFile));
    QFile file(m_ConfData.m_sResultFile); // text einlesen 
    if ( file.open( IO_ReadOnly ) ) {
	QTextStream stream( &file );
        wmEdit2->setTextFormat(Qt::PlainText);
	wmEdit2->setText( stream.read() );
	file.close();
	wmEdit2->show();
    } // wenn der editor geschlossen wird -> signal mit neuem text (geht auf den slot eins tiefer)
}


void WMViewBase::ReceiveResultFileSlot(QString nText)
{
    wmEdit2->hide();
    QFile file(m_ConfData.m_sResultFile); 
    file.remove();
    if ( file.open( IO_WriteOnly ) ) {
	QTextStream stream( &file );
	stream << nText;
	file.close();
    }
}


void WMViewBase::CloseResultFileSlot()
{
    m_ConfData.m_sResultFile = "";
    emit SendConfDataSignal(&m_ConfData);
}


void WMViewBase::StoreResultSlot()
{
    if (m_ConfData.m_sResultFile=="") { // wir haben noch keine ergebnisdatei geöffnet
        QString s = Q3FileDialog::getSaveFileName(
        QString("%1%2").arg(wm3000uHome).arg("results/results.xml"),
		"Ergebnisdateien (*.xml)",
		this,
		"",
		"Ergebnisdateien" );
	if (s == "") return; // nichts ausgewählt -> fertig
	m_ConfData.m_sResultFile = s;
	emit SendConfDataSignal(&m_ConfData);
    }
    emit StoreResultSignal(); // signal an wm3000 messergebnisse zu speichern
}


void WMViewBase::OpenRecentResultFileSlot(int index)
{
    QString s = recentResultFiles[index];
    UpdateRecentFileList(recentResultFiles,s); // liste event. kürzen und einträge ins menu
    m_ConfData.m_sResultFile = s;
    emit SendConfDataSignal(&m_ConfData);
}


bool WMViewBase::LoadSession(QString session)
{
    QFileInfo fi(session);
    QString ls = QString("%1/.wm3000u/%2%3").arg(wm3000uHome).arg(name()).arg(fi.fileName());
    QFile file(ls); 
    if ( file.open( IO_ReadOnly ) ) {
	QDataStream stream( &file );
    int mA, iA , oA, dA, eA, enA;
    stream >> mA >> iA >> oA >> dA >> eA >> enA;
	stream >> m_widGeometry;
	file.close();
	
        ui->ansichtFehlerMessungAction->setChecked(mA);
        ui->ansichtIstwerteAction->setChecked(iA);
        ui->ansichtOffsetAction->setChecked(oA);
        ui->ansichtDialogAction->setChecked(dA);
        ui->ansichtEigenfehlerAction->setChecked(eA);
        ui->ansichtEN61850Action->setChecked(enA);

	hide();
	resize(m_widGeometry.m_Size);
	move(m_widGeometry.m_Point);
	show();
// FVWM und Gnome verhalten sich anders
#ifndef FVWM 
    move(m_widGeometry.m_Point);
#endif   

	return true;
    }
    return false;    
}   
    
    
void WMViewBase::SaveSession(QString session)
{
    QFileInfo fi(session);
    QString ls = QString("%1/.wm3000u/%2%3").arg(wm3000uHome).arg(name()).arg(fi.fileName());
    QFile file(ls); 
//    file.remove();
    if ( file.open( QIODevice::Unbuffered | QIODevice::WriteOnly ) ) {
	file.at(0);
	
	int vi;
	
	vi = (isVisible()) ? 1 : 0;
	if (vi)
	    m_widGeometry.SetGeometry(pos(),size());
	m_widGeometry.SetVisible(vi);
	
	QDataStream stream( &file );
        stream << (int)ui->ansichtFehlerMessungAction->isChecked()
                   << (int)ui->ansichtIstwerteAction->isChecked()
                   << (int)ui->ansichtOffsetAction->isChecked()
                   << (int)ui->ansichtDialogAction->isChecked()
                   << (int)ui->ansichtEigenfehlerAction->isChecked()
                   << (int)ui->ansichtEN61850Action->isChecked();

	stream << m_widGeometry;
	file.close();
    }
}    

void WMViewBase::StoreSessionSlot()
{
    SessionName = Q3FileDialog::getSaveFileName( QString("%1%2").arg(wm3000uHome).arg("Session.ses"),
		  			  tr("Sitzung Name (*.ses)"),
					  this,
					  "",
					  tr("Sitzung speichern"));
    
    QFile file(SessionName); 
    if ( file.open( IO_WriteOnly ) ) {
	file.close();
	SaveSession(SessionName); // eigene session speichern
	emit SaveSessionSignal(SessionName); // die anderen
    }
}


void WMViewBase::LoadSessionSlot()
{
    Q3FileDialog *SessionFileDialog=new Q3FileDialog (  wm3000uHome,tr("Sitzung Name (*.ses)"),this);
    SessionFileDialog->setCaption(tr("Sitzung laden"));
    SessionFileDialog->setMode( Q3FileDialog::ExistingFile);
    if ( SessionFileDialog->exec() == QDialog::Accepted ) {
	SessionName = SessionFileDialog->selectedFile();
	UpdateRecentSESList(SessionName);
	LoadSession(SessionName); // eigene session laden
	emit LoadSessionSignal(SessionName); // jetzt die anderen
    }
    delete SessionFileDialog;
}



void WMViewBase::closeEvent(QCloseEvent* ce)
{
    m_widGeometry.SetGeometry(pos(),size());
    m_widGeometry.SetVisible(0);
    SaveSession(".ses");
    ce->accept();
}


void WMViewBase::resizeEvent ( QResizeEvent *)
{
    SaveSession(".ses");
}


void WMViewBase::moveEvent( QMoveEvent *)
{
    SaveSession(".ses");
}


void WMViewBase::UpdateRecentSESList( QString ses )
{
    recentSESFiles.remove(ses); // wenn es den eintrag schon gibt -> löschen
    recentSESFiles.push_front(ses); // dann neuen an den anfang der liste setzen
    
    while ((int) recentSESFiles.size()>nmaxRecentSESFiles)
	recentSESFiles.pop_back(); 
    int n = (int) recentSESFiles.size();
    
    for (int i=0; i<n; i++)
    {
	QString text = QString ("&%1 %2").arg(i+1).arg(strippedName(recentSESFiles[i]));
	if (m_nrecentSESFileIds[i]==-1)
	{
            m_nrecentSESFileIds[i]=ui->Messung->insertItem(text,this,SLOT(OpenRecentSESFileSlot(int)),0,i,3+i);
            ui->Messung->setItemParameter(m_nrecentSESFileIds[i],i);
	}
	else
	{
            ui->Messung->changeItem(m_nrecentSESFileIds[i],text);
	}
    }
}


void WMViewBase::OpenRecentSESFileSlot(int index)
{
    QString s = recentSESFiles[index];
    UpdateRecentSESList(s); // liste event. kürzen und einträge ins menu
    LoadSession(s); // eigene session laden
    emit LoadSessionSignal(s); // jetzt die anderen
}


void WMViewBase::SetGermanSlot()
{
    m_ConfData.Language = de;
    emit SendConfDataSignal(&m_ConfData);
    g_app->exit(10);
}


void WMViewBase::SetEnglishSlot()
{
    m_ConfData.Language = gb;
    emit SendConfDataSignal(&m_ConfData);
    g_app->exit(10);
}


void WMViewBase::SetPolishSlot()
{
    m_ConfData.Language = pl;
    emit SendConfDataSignal(&m_ConfData);
    g_app->exit(10);
}


void WMViewBase::JustFlashProgSlot()
{
    emit JustFlashProgSignal(); // signal an wm3000 das flash zu prograsmmieren
}


void WMViewBase::JustFlashExportSlot()
{
    QString File = Q3FileDialog::getSaveFileName(QString("%1%2").arg(wm3000uHome).arg("JData.xml"),
					tr("Datei Name (*.xml)"),
					 this,
					"",
					tr("Justagedaten exportieren"));

    if (File != "") // wenn ""  -> es war cancel
	emit JustFlashExportSignal(File);
}


void WMViewBase::JustFlashImportSlot()
{
    QString File = Q3FileDialog::getOpenFileName(QString("%1%2").arg(wm3000uHome).arg("JData.xml"),
					 tr("Datei Name (*.xml)"),
					 this,
					 "",
					 tr("Justagedaten importieren"));
    
    if (File != "") // wenn ""  -> es war cancel
	emit JustFlashImportSignal(File);
}


void WMViewBase::SaveDefaultSessionSlot(bool)
{
    SaveSession(".ses");
}


void WMViewBase::RemoteCtrlInfoSlot(bool remote )
{
    uint i;
    
    for (i = 0; i < ui->Datei->count(); i++) // wenn remote betrieb alle items disablen
        ui->Datei->setItemEnabled (ui->Datei->idAt(i) , !remote) ;
    for (i = 0; i < ui->Messung->count(); i++) // wenn remote betrieb alle items disablen
        ui->Messung->setItemEnabled (ui->Messung->idAt(i) , !remote) ;
    for (i = 0; i < ui->Sprache->count(); i++) // wenn remote betrieb alle items disablen
        ui->Sprache->setItemEnabled (ui->Sprache->idAt(i) , !remote) ;
    ui->Hilfe->setItemEnabled(ui->Hilfe->idAt(7), !remote); // kein selbsttest wenn remote
}


void WMViewBase::SetJustifiedSlot( bool b )
{
    m_bJustified = b;
    if (m_bJustified)
        m_pStatusLabel->setStyleSheet("QLabel {color:black;}");
    else
        m_pStatusLabel->setStyleSheet("QLabel {color:red;}");
    ActualizeStates();
}


void WMViewBase::SetFreqStatSlot(bool b)
{
    m_bFreqQuestionable = b;
    ActualizeStates();
}

