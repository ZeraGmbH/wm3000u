/****************************************************************************
** Form interface generated from reading ui file 'wmrawactualvalbase.ui'
**
** Created: Mo Mai 6 14:50:12 2013
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef WMRAWACTUALVALBASE_H
#define WMRAWACTUALVALBASE_H

#include <qvariant.h>
#include <qdialog.h>
#include "wmglobal.h"
#include "widgeom.h"
#include "wmrawactualconfigbase.h"
#include "tools.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;

class WMRawActualValBase : public QDialog
{
    Q_OBJECT

public:
    WMRawActualValBase( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~WMRawActualValBase();

    QLabel* XnLabel;
    QLabel* XxLabel;
    QLabel* FreqLabel;
    QLabel* XnAmplDisp;
    QLabel* XxAmplDisp;
    QLabel* FreqDisp;
    QLabel* XnPhaseDisp;
    QLabel* XxPhaseDisp;

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

    QGridLayout* WMRawActualValBaseLayout;
    QSpacerItem* spacer20;
    QSpacerItem* spacer17;

protected slots:
    virtual void languageChange();

private:
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
