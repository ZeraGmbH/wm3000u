#ifndef EN61850MONBASE_H
#define EN61850MONBASE_H

#include <qvariant.h>
#include <qdialog.h>
#include <QTimer>

#include "en61850.h"
#include "widgeom.h"

namespace Ui {
    class EN61850monbase;
}

class EN61850monbase : public QDialog
{
    Q_OBJECT

public:
    explicit EN61850monbase( QWidget* parent = 0);
    ~EN61850monbase();

public slots:
    virtual void ShowHideSlot( bool b );
    virtual void SetETHStatusSlot( cEN61850Info * ethInfo );
    bool LoadSession( QString session );
    void SaveSession( QString session );

signals:
    void InformationRequest();
    void isVisibleSignal(bool);
    void ResetETHStatus();

protected:
    int newVariable;
    virtual void closeEvent( QCloseEvent * ce );
    virtual void resizeEvent ( QResizeEvent *);
    virtual void moveEvent( QMoveEvent *);

protected slots:
    virtual void accept();
    virtual void reject();


private:
    Ui::EN61850monbase *ui;
    QTimer *m_pTimer;
    cWidgetGeometry m_widGeometry;
    cEN61850Info ETHStatus;
    QTimer m_Timer;

    virtual void init();
    virtual void destroy();

private slots:
    void TimerSlot();
    void saveConfiguration();

};

#endif // EN61850MONBASE_H
