/****************************************************************************
** Form implementation generated from reading ui file 'wmviewbase.ui'
**
** Created: Mi Feb 15 10:21:36 2012
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "wmviewbase.h"

#include <qvariant.h>
#include <qfiledialog.h>
#include <qlabel.h>
#include <qstatusbar.h>
#include <qtextbrowser.h>
#include <qmessagebox.h>
#include <qevent.h>
#include <qstring.h>
#include <qtextedit.h>
#include <qapplication.h>
#include <qfile.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qaction.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qtoolbar.h>
#include <qimage.h>
#include <qpixmap.h>

#include "wmeditor.h"
#include "wmfiledialog.h"
#include "wmglobal.h"
#include "wmviewbase.ui.h"
/*
 *  Constructs a WMViewBase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 */
WMViewBase::WMViewBase( QWidget* parent, const char* name, WFlags fl )
    : QMainWindow( parent, name, fl )
{
    (void)statusBar();
    if ( !name )
	setName( "WMViewBase" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, sizePolicy().hasHeightForWidth() ) );
    setMinimumSize( QSize( 21, 230 ) );
    setBackgroundOrigin( QMainWindow::WidgetOrigin );
    QFont f( font() );
    setFont( f ); 

    // actions
    dateiEigenfehlertabelleAction = new QAction( this, "dateiEigenfehlertabelleAction" );
    dateiEigenfehlertabelleAction->setOn( FALSE );
    dateiBeendenAction = new QAction( this, "dateiBeendenAction" );
    messungBereichAction = new QAction( this, "messungBereichAction" );
    messungSpeichernSitzungAction = new QAction( this, "messungSpeichernSitzungAction" );
    messungLaden_SitzungAction = new QAction( this, "messungLaden_SitzungAction" );
    messungStartAction = new QAction( this, "messungStartAction" );
    messungSimulationAction = new QAction( this, "messungSimulationAction" );
    messungSimulationAction->setToggleAction( TRUE );
    ansichtFehlerMessungAction = new QAction( this, "ansichtFehlerMessungAction" );
    ansichtFehlerMessungAction->setToggleAction( TRUE );
    ansichtEigenfehlerAction = new QAction( this, "ansichtEigenfehlerAction" );
    ansichtEigenfehlerAction->setToggleAction( TRUE );
    ansichtDialogAction = new QAction( this, "ansichtDialogAction" );
    ansichtDialogAction->setToggleAction( TRUE );
    einstellungenConfAction = new QAction( this, "einstellungenConfAction" );
    hilfeInfoAction = new QAction( this, "hilfeInfoAction" );
    hilfeInfo_ber_ZeraAction = new QAction( this, "hilfeInfo_ber_ZeraAction" );
    hilfeManualAction = new QAction( this, "hilfeManualAction" );
    hilfeInfo_ber_QtAction = new QAction( this, "hilfeInfo_ber_QtAction" );
    dateiErgebnisdateiAction = new QAction( this, "dateiErgebnisdateiAction" );
    ansichtIstwerteAction = new QAction( this, "ansichtIstwerteAction" );
    ansichtIstwerteAction->setToggleAction( TRUE );
    eigenfehlertabelleoeffnenAction = new QAction( this, "eigenfehlertabelleoeffnenAction" );
    eigenfehlertabelleSchliessenAction = new QAction( this, "eigenfehlertabelleSchliessenAction" );
    eigenfehlertabelleBearbeitenAction = new QAction( this, "eigenfehlertabelleBearbeitenAction" );
    ergebnisSchliessenAction = new QAction( this, "ergebnisSchliessenAction" );
    ergebnisBearbeitenAction = new QAction( this, "ergebnisBearbeitenAction" );
    messungSpeichernMesswerteAction = new QAction( this, "messungSpeichernMesswerteAction" );
    ergebnis_ffnenAction = new QAction( this, "ergebnis_ffnenAction" );
    eigenfehlertabelleNeuAction = new QAction( this, "eigenfehlertabelleNeuAction" );
    einstellungenBereichAction = new QAction( this, "einstellungenBereichAction" );
    einstellungenTestRAutoAction = new QAction( this, "einstellungenTestRAutoAction" );
    JustageAmplitudeAction = new QAction( this, "JustageAmplitudeAction" );
    JustagePhaseAction = new QAction( this, "JustagePhaseAction" );
    Action = new QAction( this, "Action" );
    Action_2 = new QAction( this, "Action_2" );
    Action_3 = new QAction( this, "Action_3" );
    Action_4 = new QAction( this, "Action_4" );
    JustageKoeffBerechnungAction = new QAction( this, "JustageKoeffBerechnungAction" );
    ansichtEN61850Action = new QAction( this, "ansichtEN61850Action" );
    ansichtEN61850Action->setToggleAction( TRUE );
    spracheDeutschAction = new QAction( this, "spracheDeutschAction" );
    spracheDeutschAction->setIconSet( QIconSet( QPixmap::fromMimeSource( "de.png" ) ) );
    spracheEnglischAction = new QAction( this, "spracheEnglischAction" );
    spracheEnglischAction->setIconSet( QIconSet( QPixmap::fromMimeSource( "gb.png" ) ) );
    spracheEnglischAction_2 = new QAction( this, "spracheEnglischAction_2" );
    spracheEnglischAction_2->setIconSet( QIconSet( QPixmap::fromMimeSource( "gb.png" ) ) );
    justageFlashProgAction = new QAction( this, "justageFlashProgAction" );
    justageFlasExportAction = new QAction( this, "justageFlasExportAction" );
    justageFlashImportAction = new QAction( this, "justageFlashImportAction" );
    hilfeVersionAction = new QAction( this, "hilfeVersionAction" );
    Action_5 = new QAction( this, "Action_5" );
    hilfeSelbsttestAction = new QAction( this, "hilfeSelbsttestAction" );


    // toolbars


    // menubar
    MenuBarEditor = new QMenuBar( this, "MenuBarEditor" );

    QFont MenuBarEditor_font(  MenuBarEditor->font() );
    MenuBarEditor->setFont( MenuBarEditor_font ); 
    MenuBarEditor->setFrameShape( QMenuBar::MenuBarPanel );

    Datei = new QPopupMenu( this );
    popupMenu = new QPopupMenu( this );
    Datei->insertItem( dateiEigenfehlertabelleAction->iconSet(), tr( "&Eigenfehlertabelle" ), popupMenu );
    eigenfehlertabelleoeffnenAction->addTo( popupMenu );
    eigenfehlertabelleSchliessenAction->addTo( popupMenu );
    eigenfehlertabelleBearbeitenAction->addTo( popupMenu );
    Datei->insertSeparator();
    popupMenu_6 = new QPopupMenu( this );
    Datei->insertItem( dateiErgebnisdateiAction->iconSet(), tr( "E&rgebnisdatei" ), popupMenu_6 );
    ergebnis_ffnenAction->addTo( popupMenu_6 );
    ergebnisSchliessenAction->addTo( popupMenu_6 );
    ergebnisBearbeitenAction->addTo( popupMenu_6 );
    Datei->insertSeparator();
    dateiBeendenAction->addTo( Datei );
    MenuBarEditor->insertItem( QString(""), Datei, 3 );

    Messung = new QPopupMenu( this );
    Messung->insertSeparator();
    messungSpeichernSitzungAction->addTo( Messung );
    messungLaden_SitzungAction->addTo( Messung );
    Messung->insertSeparator();
    messungStartAction->addTo( Messung );
    messungSpeichernMesswerteAction->addTo( Messung );
    Messung->insertSeparator();
    messungSimulationAction->addTo( Messung );
    MenuBarEditor->insertItem( QString(""), Messung, 4 );

    Ansicht = new QPopupMenu( this );
    ansichtFehlerMessungAction->addTo( Ansicht );
    ansichtIstwerteAction->addTo( Ansicht );
    ansichtEigenfehlerAction->addTo( Ansicht );
    ansichtDialogAction->addTo( Ansicht );
    ansichtEN61850Action->addTo( Ansicht );
    MenuBarEditor->insertItem( QString(""), Ansicht, 5 );

    Einstellungen = new QPopupMenu( this );
    einstellungenConfAction->addTo( Einstellungen );
    einstellungenBereichAction->addTo( Einstellungen );
    MenuBarEditor->insertItem( QString(""), Einstellungen, 6 );

    Hilfe = new QPopupMenu( this );
    hilfeInfoAction->addTo( Hilfe );
    hilfeInfo_ber_ZeraAction->addTo( Hilfe );
    hilfeInfo_ber_QtAction->addTo( Hilfe );
    Hilfe->insertSeparator();
    hilfeVersionAction->addTo( Hilfe );
    Hilfe->insertSeparator();
    hilfeSelbsttestAction->addTo( Hilfe );
    MenuBarEditor->insertItem( QString(""), Hilfe, 7 );

    Sprache = new QPopupMenu( this );
    spracheDeutschAction->addTo( Sprache );
    spracheEnglischAction_2->addTo( Sprache );
    MenuBarEditor->insertItem( QString(""), Sprache, 8 );

    Justage = new QPopupMenu( this );
    JustageAmplitudeAction->addTo( Justage );
    JustagePhaseAction->addTo( Justage );
    JustageKoeffBerechnungAction->addTo( Justage );
    justageFlashProgAction->addTo( Justage );
    justageFlasExportAction->addTo( Justage );
    justageFlashImportAction->addTo( Justage );
    MenuBarEditor->insertItem( QString(""), Justage, 9 );

    languageChange();
    resize( QSize(540, 403).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( messungSimulationAction, SIGNAL( activated() ), this, SLOT( SimulationSlot() ) );
    connect( messungStartAction, SIGNAL( activated() ), this, SLOT( StartSlot() ) );
    connect( eigenfehlertabelleoeffnenAction, SIGNAL( activated() ), this, SLOT( OpenOETFileSlot() ) );
    connect( eigenfehlertabelleSchliessenAction, SIGNAL( activated() ), this, SLOT( CloseOETFileSlot() ) );
    connect( eigenfehlertabelleBearbeitenAction, SIGNAL( activated() ), this, SLOT( EditOETFileSlot() ) );
    connect( ergebnis_ffnenAction, SIGNAL( activated() ), this, SLOT( OpenResultFileSlot() ) );
    connect( ergebnisSchliessenAction, SIGNAL( activated() ), this, SLOT( CloseResultFileSlot() ) );
    connect( ergebnisBearbeitenAction, SIGNAL( activated() ), this, SLOT( EditResultFileSlot() ) );
    connect( messungSpeichernSitzungAction, SIGNAL( activated() ), this, SLOT( StoreSessionSlot() ) );
    connect( messungLaden_SitzungAction, SIGNAL( activated() ), this, SLOT( LoadSessionSlot() ) );
    connect( dateiBeendenAction, SIGNAL( activated() ), this, SLOT( close() ) );
    connect( messungSpeichernMesswerteAction, SIGNAL( activated() ), this, SLOT( StoreResultSlot() ) );
    connect( dateiBeendenAction, SIGNAL( activated() ), this, SLOT( close() ) );
    connect( spracheDeutschAction, SIGNAL( activated() ), this, SLOT( SetGermanSlot() ) );
    connect( spracheEnglischAction_2, SIGNAL( activated() ), this, SLOT( SetEnglishSlot() ) );
    connect( justageFlashProgAction, SIGNAL( activated() ), this, SLOT( JustFlashProgSlot() ) );
    connect( justageFlashImportAction, SIGNAL( activated() ), this, SLOT( JustFlashImportSlot() ) );
    connect( justageFlasExportAction, SIGNAL( activated() ), this, SLOT( JustFlashExportSlot() ) );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
WMViewBase::~WMViewBase()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void WMViewBase::languageChange()
{
    setCaption( tr( "Wandlermesseinrichtung WM3000U" ) );
    dateiEigenfehlertabelleAction->setText( tr( "&Eigenfehlertabelle" ) );
    dateiEigenfehlertabelleAction->setMenuText( tr( "&Eigenfehlertabelle" ) );
    dateiEigenfehlertabelleAction->setToolTip( tr( "&Eigenfehlertabelle" ) );
    dateiEigenfehlertabelleAction->setStatusTip( tr( "Menu Eigenfehlertabelle" ) );
    dateiBeendenAction->setText( tr( "&Beenden" ) );
    dateiBeendenAction->setMenuText( tr( "&Beenden" ) );
    dateiBeendenAction->setStatusTip( tr( "Beendet das laufende Programm" ) );
    dateiBeendenAction->setAccel( tr( "Ctrl+X" ) );
    messungBereichAction->setText( tr( "&Bereich" ) );
    messungBereichAction->setMenuText( tr( "&Bereich" ) );
    messungBereichAction->setStatusTip( trUtf8( "\x42\x65\x72\x65\x69\x63\x68\x65\x20\x61\x75\x73\x77\xc3\xa4\x68\x6c\x65\x6e" ) );
    messungBereichAction->setAccel( tr( "F4" ) );
    messungSpeichernSitzungAction->setText( tr( "&Speichern Sitzung" ) );
    messungSpeichernSitzungAction->setMenuText( tr( "&Speichern Sitzung" ) );
    messungSpeichernSitzungAction->setStatusTip( tr( "Speichert alle aktuellen Einstellungen in einer Datei" ) );
    messungLaden_SitzungAction->setText( tr( "&Laden Sitzung" ) );
    messungLaden_SitzungAction->setMenuText( tr( "&Laden Sitzung" ) );
    messungLaden_SitzungAction->setStatusTip( tr( "Liesst alle Einstellungen aus einer Datei" ) );
    messungStartAction->setText( tr( "&Start" ) );
    messungStartAction->setMenuText( tr( "&Start" ) );
    messungStartAction->setStatusTip( tr( "Messung starten" ) );
    messungStartAction->setAccel( tr( "F5" ) );
    messungSimulationAction->setText( tr( "Si&mulation" ) );
    messungSimulationAction->setMenuText( tr( "S&imulation" ) );
    messungSimulationAction->setStatusTip( trUtf8( "\x53\x69\x6d\x75\x6c\x69\x65\x72\x74\x65\x20\x4d\x65\x73\x73\x75\x6e\x67\x20\x64\x75\x72\x63\x68\x66\xc3\xbc\x68\x72\x65\x6e" ) );
    ansichtFehlerMessungAction->setText( tr( "&FehlerMessung" ) );
    ansichtFehlerMessungAction->setMenuText( tr( "&Fehlerwerte" ) );
    ansichtFehlerMessungAction->setStatusTip( tr( "Fehlermesswerte Fenster" ) );
    ansichtEigenfehlerAction->setText( tr( "&Eigenfehler" ) );
    ansichtEigenfehlerAction->setMenuText( tr( "&Eigenfehler" ) );
    ansichtEigenfehlerAction->setStatusTip( tr( "Eigenfehler Fenster" ) );
    ansichtDialogAction->setText( tr( "&Dialog" ) );
    ansichtDialogAction->setMenuText( tr( "&Dialog" ) );
    ansichtDialogAction->setStatusTip( tr( "Dialog Fenster" ) );
    einstellungenConfAction->setText( tr( "&Konfiguration" ) );
    einstellungenConfAction->setMenuText( tr( "&Konfiguration" ) );
    einstellungenConfAction->setStatusTip( tr( "Messeinrichtung konfigurieren" ) );
    hilfeInfoAction->setText( tr( "&Info" ) );
    hilfeInfoAction->setMenuText( tr( "&Info" ) );
    hilfeInfoAction->setStatusTip( tr( "Informationen zur Messeinrichtung" ) );
    hilfeInfo_ber_ZeraAction->setText( tr( "Informationen zu Zera" ) );
    hilfeInfo_ber_ZeraAction->setMenuText( trUtf8( "\x49\x6e\x66\x6f\x20\xc3\xbc\x62\x65\x72\x20\x26\x5a\x65\x72\x61" ) );
    hilfeInfo_ber_ZeraAction->setStatusTip( tr( "Informationen zu ZERA" ) );
    hilfeManualAction->setText( tr( "&Manual" ) );
    hilfeManualAction->setMenuText( tr( "&Manual" ) );
    hilfeManualAction->setStatusTip( tr( "Lesen Sie hier die Bedienungsanleitung" ) );
    hilfeInfo_ber_QtAction->setText( tr( "Informationen zu Qt" ) );
    hilfeInfo_ber_QtAction->setMenuText( trUtf8( "\x49\x6e\x66\x6f\x20\xc3\xbc\x62\x65\x72\x20\x26\x51\x74" ) );
    dateiErgebnisdateiAction->setText( tr( "E&rgebnisdatei" ) );
    dateiErgebnisdateiAction->setMenuText( tr( "E&rgebnisdatei" ) );
    dateiErgebnisdateiAction->setStatusTip( tr( "Schliesst die geladene Eigenfehlertabelle" ) );
    ansichtIstwerteAction->setText( tr( "Istwerte" ) );
    ansichtIstwerteAction->setMenuText( tr( "&Vektoren" ) );
    ansichtIstwerteAction->setStatusTip( tr( "Istwerte Fenster" ) );
    eigenfehlertabelleoeffnenAction->setText( trUtf8( "\x26\xc3\x96\x66\x66\x6e\x65\x6e" ) );
    eigenfehlertabelleoeffnenAction->setMenuText( trUtf8( "\x26\xc3\x96\x66\x66\x6e\x65\x6e" ) );
    eigenfehlertabelleoeffnenAction->setStatusTip( trUtf8( "\xc3\x96\x66\x66\x6e\x65\x6e\x20\x65\x69\x6e\x65\x72\x20\x45\x69\x67\x65\x6e\x66\x65\x68\x6c\x65\x72\x74\x61\x62\x65\x6c\x6c\x65" ) );
    eigenfehlertabelleSchliessenAction->setText( tr( "&Schliessen" ) );
    eigenfehlertabelleSchliessenAction->setMenuText( tr( "&Schliessen" ) );
    eigenfehlertabelleSchliessenAction->setStatusTip( tr( "Schliessen der Eigenfehlertabelle" ) );
    eigenfehlertabelleBearbeitenAction->setText( tr( "&Bearbeiten" ) );
    eigenfehlertabelleBearbeitenAction->setMenuText( tr( "&Bearbeiten" ) );
    eigenfehlertabelleBearbeitenAction->setStatusTip( tr( "Bearbeiten einer Eigenfehlertabelle" ) );
    ergebnisSchliessenAction->setText( tr( "&Schliessen" ) );
    ergebnisSchliessenAction->setMenuText( tr( "&Schliessen" ) );
    ergebnisSchliessenAction->setStatusTip( tr( "Schliessen der Ergebnisdatei" ) );
    ergebnisBearbeitenAction->setText( tr( "&Bearbeiten" ) );
    ergebnisBearbeitenAction->setMenuText( tr( "&Bearbeiten" ) );
    ergebnisBearbeitenAction->setStatusTip( tr( "Bearbeiten einer Ergebnisdatei" ) );
    messungSpeichernMesswerteAction->setText( tr( "Speichern Messwerte" ) );
    messungSpeichernMesswerteAction->setMenuText( tr( "Speichern &Messwerte" ) );
    messungSpeichernMesswerteAction->setAccel( tr( "F6" ) );
    ergebnis_ffnenAction->setText( trUtf8( "\x26\xc3\x96\x66\x66\x6e\x65\x6e" ) );
    ergebnis_ffnenAction->setMenuText( trUtf8( "\x26\xc3\x96\x66\x66\x6e\x65\x6e" ) );
    ergebnis_ffnenAction->setStatusTip( trUtf8( "\xc3\x96\x66\x66\x6e\x65\x6e\x20\x64\x65\x72\x20\x45\x72\x67\x65\x62\x6e\x69\x73\x64\x61\x74\x65\x69" ) );
    eigenfehlertabelleNeuAction->setText( tr( "&Neu" ) );
    eigenfehlertabelleNeuAction->setMenuText( tr( "&Neu" ) );
    einstellungenBereichAction->setText( tr( "&Bereich" ) );
    einstellungenBereichAction->setMenuText( tr( "&Bereiche" ) );
    einstellungenBereichAction->setStatusTip( trUtf8( "\x42\x65\x72\x65\x69\x63\x68\x65\x20\x61\x6e\x77\xc3\xa4\x68\x6c\x65\x6e" ) );
    einstellungenBereichAction->setAccel( tr( "F7" ) );
    einstellungenTestRAutoAction->setText( tr( "BAuto" ) );
    einstellungenTestRAutoAction->setMenuText( tr( "BereichAuto" ) );
    JustageAmplitudeAction->setText( tr( "Amplitude" ) );
    JustageAmplitudeAction->setMenuText( tr( "&Amplitude" ) );
    JustageAmplitudeAction->setStatusTip( tr( "Amplitudenabgleich mit externer Steuerung" ) );
    JustagePhaseAction->setText( tr( "Phasen" ) );
    JustagePhaseAction->setMenuText( tr( "&Phase" ) );
    JustagePhaseAction->setStatusTip( tr( "Phasenlage Justage automatisch" ) );
    Action->setText( tr( "Action" ) );
    Action_2->setText( tr( "Action_2" ) );
    Action_3->setText( tr( "Action_3" ) );
    Action_4->setText( tr( "Action_4" ) );
    JustageKoeffBerechnungAction->setText( QString::null );
    JustageKoeffBerechnungAction->setMenuText( tr( "Phasen&berechnung" ) );
    JustageKoeffBerechnungAction->setStatusTip( tr( "Phasenlage Koeffizientenberechnung" ) );
    ansichtEN61850Action->setText( tr( "E&N61850" ) );
    ansichtEN61850Action->setMenuText( tr( "E&N61850" ) );
    ansichtEN61850Action->setStatusTip( tr( "EN61850 Statusanzeige" ) );
    spracheDeutschAction->setText( tr( "Deutsch" ) );
    spracheDeutschAction->setMenuText( tr( "Deutsch" ) );
    spracheEnglischAction->setText( tr( "Englisch" ) );
    spracheEnglischAction->setMenuText( tr( "Englisch" ) );
    spracheEnglischAction_2->setText( tr( "Englisch" ) );
    spracheEnglischAction_2->setMenuText( tr( "Englisch" ) );
    justageFlashProgAction->setText( tr( "new item" ) );
    justageFlashProgAction->setMenuText( tr( "F&lash programmieren" ) );
    justageFlasExportAction->setText( tr( "new item" ) );
    justageFlasExportAction->setMenuText( tr( "Flash data &export" ) );
    justageFlashImportAction->setText( tr( "new item" ) );
    justageFlashImportAction->setMenuText( tr( "Flash data &import" ) );
    hilfeVersionAction->setText( tr( "&Version" ) );
    hilfeVersionAction->setMenuText( tr( "&Version" ) );
    hilfeVersionAction->setStatusTip( trUtf8( "\xc3\x9c\x62\x65\x72\x73\x69\x63\x68\x74\x20\x56\x65\x72\x73\x69\x6f\x6e\x65\x6e" ) );
    Action_5->setText( tr( "Unnamed" ) );
    hilfeSelbsttestAction->setText( tr( "new item" ) );
    hilfeSelbsttestAction->setMenuText( tr( "&Selbsttest" ) );
    hilfeSelbsttestAction->setStatusTip( tr( "Selbsttest starten" ) );
    Datei->changeItem( Datei->idAt( 0 ), tr( "&Eigenfehlertabelle" ) );
    Datei->changeItem( Datei->idAt( 2 ), tr( "E&rgebnisdatei" ) );
    if (MenuBarEditor->findItem(3))
        MenuBarEditor->findItem(3)->setText( tr( "&Datei" ) );
    if (MenuBarEditor->findItem(4))
        MenuBarEditor->findItem(4)->setText( tr( "&Messung" ) );
    if (MenuBarEditor->findItem(5))
        MenuBarEditor->findItem(5)->setText( tr( "&Ansicht" ) );
    if (MenuBarEditor->findItem(6))
        MenuBarEditor->findItem(6)->setText( tr( "&Einstellungen" ) );
    if (MenuBarEditor->findItem(7))
        MenuBarEditor->findItem(7)->setText( tr( "&Hilfe" ) );
    if (MenuBarEditor->findItem(8))
        MenuBarEditor->findItem(8)->setText( tr( "&Sprache" ) );
    if (MenuBarEditor->findItem(9))
        MenuBarEditor->findItem(9)->setText( tr( "&Justage" ) );
}

<<<<<<< HEAD
=======

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
    ResultFileDialog->setDir(QDir("./results"));
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
	
        ui->ansichtFehlerMessungAction->setOn(mA);
        ui->ansichtIstwerteAction->setOn(iA);
        ui->ansichtDialogAction->setOn(dA);
        ui->ansichtEigenfehlerAction->setOn(eA);

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
    if ( file.open( QIODevice::Unbuffered | QIODevice::WriteOnly ) ) {
	file.at(0);
	
	int vi;
	
	vi = (isVisible()) ? 1 : 0;
	if (vi)
	    m_widGeometry.SetGeometry(pos(),size());
	m_widGeometry.SetVisible(vi);
	
	QDataStream stream( &file );
        stream << (int)ui->ansichtFehlerMessungAction->isOn()
                   << (int)ui->ansichtIstwerteAction->isOn()
                   << (int)ui->ansichtDialogAction->isOn()
                   << (int)ui->ansichtEigenfehlerAction->isOn();
	stream << m_widGeometry;
	file.close();
    }
}    

void WMViewBase::StoreSessionSlot()
{
    SessionName = Q3FileDialog::getSaveFileName( "./Session.ses",
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
    Q3FileDialog *SessionFileDialog=new Q3FileDialog ( ".",tr("Sitzung Name (*.ses)"),this);
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


void WMViewBase::JustFlashProgSlot()
{
    emit JustFlashProgSignal(); // signal an wm3000 das flash zu prograsmmieren
}


void WMViewBase::JustFlashExportSlot()
{
    QString File = Q3FileDialog::getSaveFileName( "./JData.xml",
					tr("Datei Name (*.xml)"),
					 this,
					"",
					tr("Justagedaten exportieren"));
    
    if (File != "") // wenn ""  -> es war cancel
	emit JustFlashExportSignal(File);
}


void WMViewBase::JustFlashImportSlot()
{
    QString File = Q3FileDialog::getOpenFileName("./JData.xml",
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
    ActualizeStates();
}
>>>>>>> 5a91ce3... Contextmenu of measurement values extended with mrad.
