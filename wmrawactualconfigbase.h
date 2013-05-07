/****************************************************************************
** Form interface generated from reading ui file 'wmrawactualconfigbase.ui'
**
** Created: Mi Sep 19 07:33:29 2012
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef WMRAWACTUALCONFIGBASE_H
#define WMRAWACTUALCONFIGBASE_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QButtonGroup;
class QRadioButton;
class QPushButton;

class WMRawActualConfigBase : public QDialog
{
    Q_OBJECT

public:
    WMRawActualConfigBase( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~WMRawActualConfigBase();

    QButtonGroup* AmplitudebuttonGroup;
    QRadioButton* Amplx1radioButton;
    QRadioButton* Amplx1_sqrt2radioButton;
    QButtonGroup* primsekbuttonGroup;
    QRadioButton* PrimradioButton;
    QRadioButton* SekradioButton;
    QButtonGroup* WinkelbuttonGroup;
    QRadioButton* MathradioButton;
    QRadioButton* TechradioButton;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;

public slots:
    virtual void ReceiveDisplayConfSlot( int m, int m2, int m3 );

signals:
    void SendVektorDisplayFormat(int,int,int);

protected:
    QVBoxLayout* WMRawActualConfigBaseLayout;
    QHBoxLayout* layout3;
    QVBoxLayout* AmplitudebuttonGroupLayout;
    QVBoxLayout* primsekbuttonGroupLayout;
    QVBoxLayout* WinkelbuttonGroupLayout;
    QHBoxLayout* Layout1;
    QSpacerItem* Horizontal_Spacing2;

protected slots:
    virtual void languageChange();

private:
    int AmplDisplayMode;
    int WinkelDisplayMode;
    int AmplPrimSekMode;

    virtual void init();
    virtual void ActualizeDisplay();

private slots:
    virtual void amplx1checkedSlot();
    virtual void amplx1_sq2checkedSlot();
    virtual void winkelmathcheckedSlot();
    virtual void winkeltechcheckedSlot();
    virtual void SendData2AppSlot();
    virtual void primCheckedSlot();
    virtual void sekCheckedSlot();

};

#endif // WMRAWACTUALCONFIGBASE_H
