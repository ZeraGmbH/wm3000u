#ifndef WMVIEWBASE_H
#define WMVIEWBASE_H

#include <qvariant.h>
#include <q3mainwindow.h>
#include <qlabel.h>
#include <QTimer>

#include "wmglobal.h"
#include "wmeditor.h"
#include "widgeom.h"

namespace Ui {
    class WMViewBase;
}

class WMViewBase : public Q3MainWindow
{
    Q_OBJECT

public:
    explicit WMViewBase( QWidget* parent = 0);
    ~WMViewBase();
    virtual QString strippedName( QString s );
    virtual bool LoadSession( QString session );
    void removeJustageItem();
    void configureWM1000Items();
    void configureWMwoDC();

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
    virtual void SetFreqStatSlot(bool b);

signals:
    void SendConfDataSignal(cConfData*);
    void SaveSessionSignal(QString);
    void LoadSessionSignal(QString);
    void StoreResultSignal();
    void StartRangeObsermatic();
    void JustFlashProgSignal();
    void JustFlashImportSignal(QString);
    void JustFlashExportSignal(QString);
    void UIansichtFehlerMessungActionToggled(bool);
    void UIansichtFehlerMessungActionSet(bool);
    void UIansichtEigenfehlerActionToggled(bool);
    void UIansichtEigenfehlerActionSet(bool);
    void UIansichtIstwerteActionToggled(bool);
    void UIansichtIstwerteActionSet(bool);
    void UIansichtOffsetActionToggled(bool);
    void UIansichtOffsetActionSet(bool);
    void UIansichtDialogActionToggled(bool);
    void UIansichtDialogActionSet(bool);
    void UIansichtEN61850ActionToggled(bool);
    void UIansichtEN61850ActionSet(bool);
    void UIhilfeManualActionActivated();
    void UIeinstellungenConfActionActivated();
    void UIeinstellungenBereichActionActivated();
    void UIJustageAmplitudeActionActivated();
    void UIJustagePhaseActionActivated();
    void UIJustageKoeffBerechnungActionActivated();
    void UIJustageOffsetActionActivated();
    void UIJustageOffsetBerechnungActionActivated();
    void UIJustageOffsetVarActionActivated();
    void UIMessungOffsetKanalNActivated();
    void UIMessungOffsetKanalXActivated();
    void UIhilfeInfo_ber_QtActionActivated();
    void UIhilfeInfo_ber_ZeraActionActivated();
    void UIhilfeInfoActionActivated();
    void UIhilfeSelbsttestActionActivated();
    void UIdateiBeendenActionActivated();
    void UIhilfeVersionActionActivated();
    void UIhilfeReleaseInfoActionActivated();

protected:
    virtual void closeEvent( QCloseEvent * ce );
    virtual void resizeEvent ( QResizeEvent *);
    virtual void moveEvent( QMoveEvent *);


private:
    Ui::WMViewBase *ui;
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
    QLabel* m_pFreqLabel;
    QLabel* m_pDummyLabel;
    QLabel* m_pOETLabel;
    int m_nrecentOEFileIds[nmaxRecentOEFiles];
    QStringList recentOETFiles;
    int m_nrecentMVFileIds[nmaxRecentMVFiles];
    wmEditor* wmEdit2;
    QStringList recentSESFiles;
    bool m_bJustified;
    bool m_bFreqQuestionable;

    void init();
    void destroy();
    void UpdateRecentSESList( QString ses );

private slots:
    virtual void StoreSessionSlot();
    virtual void LoadSessionSlot();
    void OpenRecentSESFileSlot( int index );
    virtual void SetGermanSlot();
    virtual void SetEnglishSlot();
    virtual void SetPolishSlot();
    virtual void JustFlashProgSlot();
    virtual void JustFlashExportSlot();
    virtual void JustFlashImportSlot();
    virtual void SaveDefaultSessionSlot(bool);

};

#endif // WMVIEWBASE_H
