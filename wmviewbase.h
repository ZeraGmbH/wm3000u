/****************************************************************************
** Form interface generated from reading ui file 'wmviewbase.ui'
**
** Created: Mo Mai 6 14:50:11 2013
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef WMVIEWBASE_H
#define WMVIEWBASE_H

#include <qvariant.h>
#include <qmainwindow.h>
#include <qlabel.h>
#include "wmglobal.h"
#include "wmeditor.h"
#include "widgeom.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QAction;
class QActionGroup;
class QToolBar;
class QPopupMenu;

class WMViewBase : public QMainWindow
{
    Q_OBJECT

public:
    WMViewBase( QWidget* parent = 0, const char* name = 0, WFlags fl = WType_TopLevel );
    ~WMViewBase();

    QMenuBar *MenuBarEditor;
    QPopupMenu *Datei;
    QPopupMenu *popupMenu;
    QPopupMenu *popupMenu_6;
    QPopupMenu *Messung;
    QPopupMenu *Ansicht;
    QPopupMenu *Einstellungen;
    QPopupMenu *Hilfe;
    QPopupMenu *Sprache;
    QPopupMenu *Justage;
    QAction* dateiEigenfehlertabelleAction;
    QAction* dateiBeendenAction;
    QAction* messungBereichAction;
    QAction* messungSpeichernSitzungAction;
    QAction* messungLaden_SitzungAction;
    QAction* messungStartAction;
    QAction* messungSimulationAction;
    QAction* ansichtFehlerMessungAction;
    QAction* ansichtEigenfehlerAction;
    QAction* ansichtDialogAction;
    QAction* einstellungenConfAction;
    QAction* hilfeInfoAction;
    QAction* hilfeInfo_ber_ZeraAction;
    QAction* hilfeManualAction;
    QAction* hilfeInfo_ber_QtAction;
    QAction* dateiErgebnisdateiAction;
    QAction* ansichtIstwerteAction;
    QAction* eigenfehlertabelleoeffnenAction;
    QAction* eigenfehlertabelleSchliessenAction;
    QAction* eigenfehlertabelleBearbeitenAction;
    QAction* ergebnisSchliessenAction;
    QAction* ergebnisBearbeitenAction;
    QAction* messungSpeichernMesswerteAction;
    QAction* ergebnis_ffnenAction;
    QAction* eigenfehlertabelleNeuAction;
    QAction* einstellungenBereichAction;
    QAction* einstellungenTestRAutoAction;
    QAction* JustageAmplitudeAction;
    QAction* JustagePhaseAction;
    QAction* Action;
    QAction* Action_2;
    QAction* Action_3;
    QAction* Action_4;
    QAction* JustageKoeffBerechnungAction;
    QAction* ansichtEN61850Action;
    QAction* spracheDeutschAction;
    QAction* spracheEnglischAction;
    QAction* spracheEnglischAction_2;
    QAction* justageFlashProgAction;
    QAction* justageFlasExportAction;
    QAction* justageFlashImportAction;
    QAction* hilfeVersionAction;
    QAction* Action_5;
    QAction* hilfeSelbsttestAction;

    virtual QString strippedName( QString s );
    virtual bool LoadSession( QString session );

public slots:
    virtual void SetViewConfDataInfoSlot( cConfData * cd );
    virtual void ActualizeStates();
    virtual void OpenOETFileSlot();
    virtual void UpdateRecentFileList( QStringList & sl, QString f );
    virtual void OpenRecentOETFileSlot( int index );
    virtual void SimulationSlot();
    virtual void StartSlot();
    virtual void CreateStatusBar();
    virtual void CloseOETFileSlot();
    virtual void EditOETFileSlot();
    virtual void ReceiveOETFileSlot( QString nText );
    virtual void OpenResultFileSlot();
    virtual void EditResultFileSlot();
    virtual void ReceiveResultFileSlot( QString nText );
    virtual void CloseResultFileSlot();
    virtual void StoreResultSlot();
    virtual void OpenRecentResultFileSlot( int index );
    void SaveSession( QString session );
    virtual void RemoteCtrlInfoSlot( bool remote );
    virtual void SetJustifiedSlot( bool b );

signals:
    void SendConfDataSignal(cConfData*);
    void SaveSessionSignal(QString);
    void LoadSessionSignal(QString);
    void StoreResultSignal();
    void StartRangeObsermatic();
    void JustFlashProgSignal();
    void JustFlashImportSignal(QString);
    void JustFlashExportSignal(QString);

protected:
    virtual void closeEvent( QCloseEvent * ce );


protected slots:
    virtual void languageChange();

private:
    int m_nrecentSESFileIds[nmaxRecentSESFiles];
    cWidgetGeometry m_widGeometry;
    QString SessionName;
    QLabel* m_pResultLabel;
    QStringList recentResultFiles;
    wmEditor* wmEdit;
    QStringList recentMVFiles;
    QLabel* m_pStatusLabel;
    QLabel* m_pSimulationLabel;
    QLabel* m_pRunningLabel;
    QLabel* m_pRangeXLabel;
    QLabel* m_pRangeNLabel;
    cConfData m_ConfData;
    QLabel* m_pDummyLabel;
    QLabel* m_pOETLabel;
    int m_nrecentOEFileIds[nmaxRecentOEFiles];
    QStringList recentOETFiles;
    int m_nrecentMVFileIds[nmaxRecentMVFiles];
    wmEditor* wmEdit2;
    QStringList recentSESFiles;
    bool m_bJustified;

    void init();
    void destroy();
    void UpdateRecentSESList( QString ses );

private slots:
    virtual void StoreSessionSlot();
    virtual void LoadSessionSlot();
    void OpenRecentSESFileSlot( int index );
    virtual void SetGermanSlot();
    virtual void SetEnglishSlot();
    virtual void JustFlashProgSlot();
    virtual void JustFlashExportSlot();
    virtual void JustFlashImportSlot();

};

#endif // WMVIEWBASE_H
