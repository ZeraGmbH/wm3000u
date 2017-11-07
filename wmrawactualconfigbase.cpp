#include "wmrawactualconfigbase.h"
#include "ui_wmrawactualconfigbase.h"
#include "wmglobal.h"

WMRawActualConfigBase::WMRawActualConfigBase( QWidget* parent):
    QDialog(parent),
    ui(new Ui::WMRawActualConfigBase)
{
    ui->setupUi(this);
    init();
}

WMRawActualConfigBase::~WMRawActualConfigBase()
{
    destroy();
    delete ui;
}

void WMRawActualConfigBase::init()
{
    AmplDisplayMode = x1;
    AmplPrimSekMode=prim;
    WinkelDisplayMode = mathpos; 
}


void WMRawActualConfigBase::ReceiveDisplayConfSlot(bool dc, int m, int m2, int m3)
{
    dcMode = dc;
    AmplDisplayMode = m;
    WinkelDisplayMode = m2;
    AmplPrimSekMode = m3;
    ActualizeDisplay();
}


void WMRawActualConfigBase::ActualizeDisplay()
{
    bool isx1 = (AmplDisplayMode == x1);

    ui->Amplx1radioButton->setChecked(isx1);
    ui->Amplx1_sqrt2radioButton->setChecked(!isx1);
    
    bool ismathpos = (WinkelDisplayMode == mathpos);
    
    ui->MathradioButton->setChecked(ismathpos);
    ui->TechradioButton->setChecked(!ismathpos);
    
    if (dcMode)
    {
        ui->Amplx1_sqrt2radioButton->setEnabled(false);
        ui->MathradioButton->setEnabled(false);
        ui->TechradioButton->setEnabled(false);
    }
    else
    {
        ui->Amplx1_sqrt2radioButton->setEnabled(true);
        ui->MathradioButton->setEnabled(true);
        ui->TechradioButton->setEnabled(true);
    }

    bool isPrim = (AmplPrimSekMode == prim);
    
    ui->PrimradioButton->setChecked(isPrim);
    ui->SekradioButton->setChecked(!isPrim);
}



void WMRawActualConfigBase::amplx1checkedSlot()
{
    AmplDisplayMode = x1;
    ActualizeDisplay();
}


void WMRawActualConfigBase::amplx1_sq2checkedSlot()
{
    AmplDisplayMode = x1_SQRT2;
    ActualizeDisplay();
}


void WMRawActualConfigBase::winkelmathcheckedSlot()
{
    WinkelDisplayMode = mathpos; 
    ActualizeDisplay();
}


void WMRawActualConfigBase::winkeltechcheckedSlot()
{
    WinkelDisplayMode = techpos;
    ActualizeDisplay();
}


void WMRawActualConfigBase::SendData2AppSlot()
{
    emit SendVektorDisplayFormat(AmplDisplayMode, WinkelDisplayMode, AmplPrimSekMode);
}


void WMRawActualConfigBase::primCheckedSlot()
{
    AmplPrimSekMode = prim;
    ActualizeDisplay();
}


void WMRawActualConfigBase::sekCheckedSlot()
{
    AmplPrimSekMode = sek;
    ActualizeDisplay();
}
