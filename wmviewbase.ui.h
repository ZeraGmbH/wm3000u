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

extern QApplication *g_app;

void WMViewBase::init()
{
    m_bJustified = false;
    for (int i =0 ; i < nmaxRecentOEFiles; i++) m_nrecentOEFileIds[i] = -1;
    for (int i =0 ; i < nmaxRecentMVFiles; i++) m_nrecentMVFileIds[i] = -1;
    for (int i =0 ; i < nmaxRecentSESFiles; i++) m_nrecentSESFileIds[i] = -1;
    CreateStatusBar();
    wmEdit = new wmEditor();
    wmEdit2 = new wmEditor();
    connect(wmEdit,SIGNAL(textMessage(QString)),this,SLOT(ReceiveOETFileSlot(QString)));
    connect(wmEdit2,SIGNAL(textMessage(QString)),this,SLOT(ReceiveResultFileSlot(QString)));
    ansichtFehlerMessungAction->setOn(false);
    ansichtIstwerteAction->setOn(false);
    ansichtDialogAction->setOn(false);
    ansichtEigenfehlerAction->setOn(false);
    LoadSession(".ses");
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
    if (m_ConfData.m_bRunning)
    {
	messungStartAction->setMenuText(tr("S&top"));
	messungStartAction->setStatusTip(tr("Messung anhalten"));
    }
    else
    {
	messungStartAction->setMenuText(tr("S&tart"));
	messungStartAction->setStatusTip(tr("Messung starten"));
    }
    
    messungSimulationAction->setOn(m_ConfData.m_bSimulation); 
    
    m_pRunningLabel->setText( QString (( m_ConfData.m_bRunning) ? tr("Gestartet") : tr("Gestoppt")));
    m_pSimulationLabel->setText( QString (( m_ConfData.m_bSimulation) ? tr("Simulation") : tr("Reale Messung")));
    
    m_pStatusLabel->setText( m_bJustified ? tr("Justiert") : tr("Nicht justiert"));
    
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
   QFileDialog *OETFileDialog=new QFileDialog ( ".",tr("Eigenfehlertabellen (*.oet)"),this); 
   OETFileDialog->setCaption(tr("Eigenfehlertabellen"));
   OETFileDialog->setMode( QFileDialog::AnyFile);
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
	    m_nrecentOEFileIds[i]=Datei->insertItem(text,this,SLOT(OpenRecentOETFileSlot(int)),0,i,1+i);
	    Datei->setItemParameter(m_nrecentOEFileIds[i],i);
	}
	else
	{
	    Datei->changeItem(m_nrecentOEFileIds[i],text);
	}
    }
    
    while ((int) recentResultFiles.size()>nmaxRecentMVFiles)
	recentResultFiles.pop_back();
        
    for (int i=0; i< (int) recentResultFiles.size(); i++)
    {
	QString text = QString ("&%1 %2").arg(i+n+1).arg(strippedName(recentResultFiles[i]));
	if (m_nrecentMVFileIds[i]==-1)
	{
	    m_nrecentMVFileIds[i]=Datei->insertItem(text,this,SLOT(OpenRecentResultFileSlot(int)),0,nmaxRecentOEFiles+i,Datei->count()-2); // wir nehmen eine id > nmaxRecentOEFiles -1 ---> damit eindeutigkeit herrscht
	    Datei->setItemParameter(m_nrecentMVFileIds[i],i);
	}
	else
	{
	    Datei->changeItem(m_nrecentMVFileIds[i],text);
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
    statusBar()->addWidget(m_pOETLabel,0);
    m_pResultLabel=new QLabel("",this); // ergebnisdatei in statuszeile
    statusBar()->addWidget(m_pResultLabel,0);
    m_pRangeNLabel=new QLabel("",this); // bereich N in statuszeile
    statusBar()->addWidget(m_pRangeNLabel,0);
    m_pRangeXLabel=new QLabel("",this); // bereich X in statuszeile
    statusBar()->addWidget(m_pRangeXLabel,0);
    m_pRunningLabel=new QLabel("",this); // gestartet bzw. gestoppt in statuszeile
    statusBar()->addWidget(m_pRunningLabel,0);
    m_pSimulationLabel=new QLabel("",this); // simulation oder reale messung in statuszeile
    statusBar()->addWidget(m_pSimulationLabel,0);
    m_pStatusLabel=new QLabel("",this); // normaler ablauf oder fehlerausgaben in statuszeile
    statusBar()->addWidget(m_pStatusLabel,0);
    m_pDummyLabel=new QLabel("",this); // letztes feld ist bereich N in statuszeile
    statusBar()->addWidget(m_pDummyLabel,1);
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
	wmEdit->setTextFormat(PlainText);
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
	m_ConfData.m_sOETFile = QFileDialog::getSaveFileName( m_ConfData.m_sOETFile,
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
    QFileDialog *ResultFileDialog=new QFileDialog ( ".",tr("Ergebnisdateien (*.xml)"),this); 
    ResultFileDialog->setDir(QDir("./results"));
    ResultFileDialog->setCaption(tr("Ergebnisdateien"));
    ResultFileDialog->setMode( QFileDialog::AnyFile);
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
	wmEdit2->setTextFormat(PlainText);
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
	QString s = QFileDialog::getSaveFileName(
		"./results/results.xml",
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
    QString ls = QString(".%1%2").arg(name()).arg(fi.fileName());
    QFile file(ls); 
    if ( file.open( IO_ReadOnly ) ) {
	QDataStream stream( &file );
	int mA, iA, dA, eA;
	stream >> mA >> iA >> dA >> eA;
	stream >> m_widGeometry;
	file.close();
	
	ansichtFehlerMessungAction->setOn(mA);
	ansichtIstwerteAction->setOn(iA);
	ansichtDialogAction->setOn(dA);
	ansichtEigenfehlerAction->setOn(eA);

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
    QString ls = QString(".%1%2").arg(name()).arg(fi.fileName());
    QFile file(ls); 
//    file.remove();
    if ( file.open( IO_Raw | IO_WriteOnly ) ) {
	file.at(0);
	
	int vi;
	
	vi = (isVisible()) ? 1 : 0;
	if (vi)
	    m_widGeometry.SetGeometry(pos(),size());
	m_widGeometry.SetVisible(vi);
	
	QDataStream stream( &file );
	stream << (int)ansichtFehlerMessungAction->isOn() 
	           << (int)ansichtIstwerteAction->isOn()
	           << (int)ansichtDialogAction->isOn()
	           << (int)ansichtEigenfehlerAction->isOn();
	stream << m_widGeometry;
	file.close();
    }
}    

void WMViewBase::StoreSessionSlot()
{
    SessionName = QFileDialog::getSaveFileName( "./Session.ses",
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
    QFileDialog *SessionFileDialog=new QFileDialog ( ".",tr("Sitzung Name (*.ses)"),this); 
    SessionFileDialog->setCaption(tr("Sitzung laden"));
    SessionFileDialog->setMode( QFileDialog::ExistingFile);
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
	    m_nrecentSESFileIds[i]=Messung->insertItem(text,this,SLOT(OpenRecentSESFileSlot(int)),0,i,3+i);
	    Messung->setItemParameter(m_nrecentSESFileIds[i],i);
	}
	else
	{
	    Messung->changeItem(m_nrecentSESFileIds[i],text);
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


void WMViewBase::JustFlashProgSlot()
{
    emit JustFlashProgSignal(); // signal an wm3000 das flash zu prograsmmieren
}


void WMViewBase::JustFlashExportSlot()
{
    QString File = QFileDialog::getSaveFileName( "./JData.xml",
					tr("Datei Name (*.xml)"),
					 this,
					"",
					tr("Justagedaten exportieren"));
    
    if (File != "") // wenn ""  -> es war cancel
	emit JustFlashExportSignal(File);
}


void WMViewBase::JustFlashImportSlot()
{
    QString File = QFileDialog::getOpenFileName("./JData.xml",
					 tr("Datei Name (*.xml)"),
					 this,
					 "",
					 tr("Justagedaten importieren"));
    
    if (File != "") // wenn ""  -> es war cancel
	emit JustFlashImportSignal(File);
}


void WMViewBase::RemoteCtrlInfoSlot(bool remote )
{
    uint i;
    
    for (i = 0; i < Datei->count(); i++) // wenn remote betrieb alle items disablen
	Datei->setItemEnabled (Datei->idAt(i) , !remote) ;
    for (i = 0; i < Messung->count(); i++) // wenn remote betrieb alle items disablen
	Messung->setItemEnabled (Messung->idAt(i) , !remote) ;
    for (i = 0; i < Sprache->count(); i++) // wenn remote betrieb alle items disablen
	Sprache->setItemEnabled (Sprache->idAt(i) , !remote) ;
    Hilfe->setItemEnabled(Hilfe->idAt(7), !remote); // kein selbsttest wenn remote
}


void WMViewBase::SetJustifiedSlot( bool b )
{
    m_bJustified = b;
    ActualizeStates();
}
