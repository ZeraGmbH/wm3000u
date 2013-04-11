#ifndef CONFDIALOGBASE_H
#define CONFDIALOGBASE_H

#include <qvariant.h>
#include <qdialog.h>
#include <QRadioButton>
#include "qvalidator.h"
#include "wmglobal.h"

namespace Ui {
    class ConfDialogBase;
}

class ConfDialogBase : public QDialog
{
    Q_OBJECT

public:
    explicit ConfDialogBase( QWidget* parent = 0);
    ~ConfDialogBase();
    virtual bool is_3( const QString & s );
    virtual bool is_w3( const QString & s );

public slots:
    virtual void SetConfInfoSlot( cConfData * cd );
    virtual void SetConfListSlot( QStringList & NPItems, QStringList & NSItems );
    virtual void accept();
    virtual void abortSlot();
    virtual void SetnConventMenu();
    virtual void RemoteCtrlInfoSlot( bool remote );

signals:
    void SendConfDataSignal(cConfData*);

protected:
    virtual void SetModeMenu();
    virtual void SetCmpMenu();
    virtual void SetSyncMenu();
    virtual void SetRatioMenu();
    virtual void SetLogMenu();
    virtual void SetMessungMenu();


private:
    Ui::ConfDialogBase *ui;
    cConfData m_ConfData, m_ConfDataTemp;
    bool m_bRemoteCtrl;
    QString m_sText;

    void init();
    void Actualize();
    virtual const QString & baseUnitText( const QString & s );
    virtual const QString & genRatioText( QString s, QRadioButton * qrb_3, QRadioButton * qrb_w3 );

private slots:
    void ApplyDataSlot();
    virtual void nPrim_3radioButtonChecked();
    virtual void nPrim_w3radioButtonChecked();
    virtual void nSek_3radioButtonChecked();
    virtual void nSek_w3radioButtonChecked();
    virtual void xPrim_3radioButtonChecked();
    virtual void xPrim_w3radioButtonChecked();
    virtual void xSek_3radioButtonChecked();
    virtual void xSek_w3radioButtonChecked();
    virtual void evtPrim_3radioButtonChecked();
    virtual void evtPrim_w3radioButtonChecked();
    virtual void evtSek_3radioButtonChecked();
    virtual void evtSek_w3radioButtonChecked();
    virtual void S80RadioButtonChecked();
    virtual void S256RadioButtonChecked();

};

#endif // CONFDIALOGBASE_H
