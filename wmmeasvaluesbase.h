/****************************************************************************
** Form interface generated from reading ui file 'wmmeasvaluesbase.ui'
**
** Created: Di Aug 19 11:06:40 2014
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef WMMEASVALUESBASE_H
#define WMMEASVALUESBASE_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qdialog.h>
#include <qlabel.h>
#include "bigletter.h"
#include "wmglobal.h"
#include "widgeom.h"
#include "wmmeasconfigbase.h"
#include "formatinfo.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class CBigLetter;

class WMMeasValuesBase : public QDialog
{
    Q_OBJECT

public:
    WMMeasValuesBase( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~WMMeasValuesBase();

    CBigLetter* cBigLetter3;
    CBigLetter* cBigLetter1;
    CBigLetter* cBigLetter2;
    CBigLetter* mBigAngleName;
    CBigLetter* mBigLoadpointX;
    CBigLetter* mBigLoadpointN;
    CBigLetter* mBigAmplError;
    CBigLetter* mBigAngleError;
    CBigLetter* mBigLPXUnit;
    CBigLetter* mBigLPNUnit;
    CBigLetter* mBigErrorUnit;
    CBigLetter* mBigAngleUnit;

public slots:
    virtual void ShowHideMVSlot( bool b );
    virtual void SetActualValuesSlot( cwmActValues * av );
    virtual void ActualizeLPSlot( cwmActValues * av );
    virtual void SetConfInfoSlot( cConfData * cd );
    virtual void ActualizeLoadPoint();
    bool LoadSession( QString session );
    void SaveSession( QString session );
    virtual void ReceiveFormatInfoSlot( int m, int m2, int n, cFormatInfo * fi );

signals:
    void isVisibleSignal(bool);
    void SendFormatInfoSignal(int, int, int, cFormatInfo*);

protected:
    virtual void closeEvent( QCloseEvent * ce );
    virtual void resizeEvent( QResizeEvent * e );
    virtual void contextMenuEvent( QContextMenuEvent * );

    QHBoxLayout* WMMeasValuesBaseLayout;
    QVBoxLayout* layout4;
    QVBoxLayout* layout5;
    QVBoxLayout* layout6;

protected slots:
    virtual void languageChange();

private:
    cWidgetGeometry m_widGeometry;
    cwmActValues m_ActValues;
    cConfData m_ConfData;
    WMMeasConfigBase* m_pContextMenu;
    cFormatInfo m_Format[3];
    int m_nDisplayMode;
    int m_nLPDisplayMode;

    QPixmap image0;

    void init();
    void destroy();
    virtual void ActualizeDisplay();

};

#endif // WMMEASVALUESBASE_H
