/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/


void WMRawActualConfigBase::init()
{
    AmplDisplayMode = x1;
    AmplPrimSekMode=prim;
    WinkelDisplayMode = mathpos; 
}


void WMRawActualConfigBase::ReceiveDisplayConfSlot( int m, int m2, int m3)
{
     AmplDisplayMode = m;
     WinkelDisplayMode = m2;  
     AmplPrimSekMode = m3;
     ActualizeDisplay();
}


void WMRawActualConfigBase::ActualizeDisplay()
{
    bool isx1 = (AmplDisplayMode == x1);

    Amplx1radioButton->setChecked(isx1);
    Amplx1_sqrt2radioButton->setChecked(!isx1);
    
    bool ismathpos = (WinkelDisplayMode == mathpos);
    
    MathradioButton->setChecked(ismathpos);
    TechradioButton->setChecked(!ismathpos);
    
    bool isPrim = (AmplPrimSekMode == prim);
    
    PrimradioButton->setChecked(isPrim);
    SekradioButton->setChecked(!isPrim);
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
