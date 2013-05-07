/****************************************************************************
** Form interface generated from reading ui file 'wmoeviewbase.ui'
**
** Created: Mo Mai 6 14:50:12 2013
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef WMOEVIEWBASE_H
#define WMOEVIEWBASE_H

#include <qvariant.h>
#include <qdialog.h>
#include "ownerror.h"
#include "widgeom.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;

class WMOeViewBase : public QDialog
{
    Q_OBJECT

public:
    WMOeViewBase( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~WMOeViewBase();

    QLabel* WandlerLabel;
    QLabel* PrimLabel;
    QLabel* SecLabel;
    QLabel* LoadpointLabel;
    QLabel* AmplLabel;
    QLabel* PhaseLabel;
    QLabel* WandlerName;
    QLabel* PrimDisp;
    QLabel* SekDisp;
    QLabel* LoadpointDisp;
    QLabel* AmplDisp;
    QLabel* PhaseDisp;

public slots:
    virtual void ReceiveOEViewDataSlot( cOwnErrorViewData * oe );
    virtual void ShowHideOESlot( bool b );
    void SaveSession( QString session );
    bool LoadSession( QString session );

signals:
    void isVisibleSignal(bool);

protected:
    virtual void closeEvent( QCloseEvent * ce );

    QHBoxLayout* WMOeViewBaseLayout;
    QSpacerItem* spacer4;
    QVBoxLayout* layout3;
    QVBoxLayout* layout4;

protected slots:
    virtual void languageChange();

private:
    cWidgetGeometry m_widGeometry;
    cOwnErrorViewData m_OwnErrorView;

    void init();
    void destroy();

};

#endif // WMOEVIEWBASE_H
