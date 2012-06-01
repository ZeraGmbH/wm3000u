#ifndef WMRAWACTUALVALBASE_H
#define WMRAWACTUALVALBASE_H

#include <qvariant.h>
#include <qdialog.h>
#include "wmglobal.h"
#include "widgeom.h"
#include "wmrawactualconfigbase.h"
#include "tools.h"

namespace Ui {
    class WMRawActualValBase;
}

class WMRawActualValBase : public QDialog
{
    Q_OBJECT

public:
    explicit WMRawActualValBase( QWidget* parent = 0);
    ~WMRawActualValBase();

public slots:
    virtual void ShowHideAVSlot( bool b );
    virtual void ReceiveAVDataSlot( cwmActValues * ActValues );
    bool LoadSession( QString session );
    void SaveSession( QString session );
    virtual void ReceiveVektorDispFormat( int m, int m2, int m3 );

signals:
    void isVisibleSignal(bool);
    void SendVektorDispFormat(int,int,int);

protected:
    virtual void closeEvent( QCloseEvent * ce );
    virtual void contextMenuEvent( QContextMenuEvent * );

private:
    Ui::WMRawActualValBase *ui;
    int AmplPrimSekMode;
    cwmActValues m_ActValues;
    cWidgetGeometry m_widGeometry;
    int AmplDispMode;
    int WinkelDispMode;
    WMRawActualConfigBase* m_pContextMenu;

    void init();
    void destroy();

};

#endif // WMRAWACTUALVALBASE_H
