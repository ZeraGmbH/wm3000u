//Added by qt3to4:
#include <QContextMenuEvent>
#include <QCloseEvent>
#include <QFileInfo>
#include "wmrawactualvalbase.h"
#include "ui_wmrawactualvalbase.h"

const double PI2 = 6.283185307;


WMRawActualValBase::WMRawActualValBase( QWidget* parent):
    QDialog(parent),
    ui(new Ui::WMRawActualValBase)
{
    ui->setupUi(this);
    init();
}


WMRawActualValBase::~WMRawActualValBase()
{
    destroy();
    delete ui;
}

void WMRawActualValBase::init()
{
    ui->XnAmplDisp -> setText( QString("%1 V").arg(0.0,10,'f',5) );
    ui->XnPhaseDisp -> setText( QString("%1 %2").arg(0.0,8,'f',4).arg( trUtf8("°")) );
    ui->XxAmplDisp -> setText( QString("%1 V").arg(0.0,10,'f',5) );
    ui->XxPhaseDisp -> setText( QString("%1 %2").arg(0.0,8,'f',4).arg( trUtf8("°")) );

    m_Timer.setSingleShot(true);
    AmplDispMode = x1;
    AmplPrimSekMode = prim;
    WinkelDispMode = mathpos;
    m_pContextMenu = new WMRawActualConfigBase(this);
    connect(this,SIGNAL(SendVektorDispFormat(bool,int,int,int)),m_pContextMenu,SLOT(ReceiveDisplayConfSlot(bool,int,int,int)));
    connect(m_pContextMenu,SIGNAL(SendVektorDisplayFormat(int,int,int)),this,SLOT(ReceiveVektorDispFormat(int,int,int)));
    connect(&m_Timer, SIGNAL(timeout()), this, SLOT(saveConfiguration()));
    LoadSession(".ses");

}


void WMRawActualValBase::destroy()
{
    SaveSession(".ses");
}


void WMRawActualValBase::closeEvent(QCloseEvent* ce)
{
    m_widGeometry.SetGeometry(pos(),size()); // wir halten visible und geometrie nach
    m_widGeometry.SetVisible(0);
    emit isVisibleSignal(false);
    m_Timer.start(500);
    ce->accept();
}


void WMRawActualValBase::resizeEvent ( QResizeEvent *)
{
    m_Timer.start(500);
}


void WMRawActualValBase::moveEvent( QMoveEvent *)
{
    m_Timer.start(500);
}


void WMRawActualValBase::ShowHideAVSlot( bool b)
{
    if (b) show();else close();
}


void WMRawActualValBase::ReceiveAVDataSlot( cwmActValues *ActValues )
{
    m_ActValues = *ActValues;
    // hier wird später die Anzeige bedient 
    if (isVisible()) {
	double phi;
	double radgrad = 57.295779; // 360/(2*PI) winkel sind im bogenmass
	
	// amplitude der grundschwingung
	// amplitude der grundschwingung
	double ampl;
	if (AmplPrimSekMode == prim)
    {
        if (m_pConfData->m_bDCmeasurement)
            ampl = m_ActValues.VekN.re();
        else
            ampl = fabs(m_ActValues.VekN);
    }
	else
    {
        if (m_pConfData->m_bDCmeasurement)
            ampl = m_ActValues.VekNSek.re();
        else
            ampl = fabs(m_ActValues.VekNSek);
    }

    if (AmplDispMode == x1_SQRT2)
        ampl/=1.414213562;

    ui->XnAmplDisp -> setText( QString("%1 V").arg(ampl,10,'f',5) );
	
	/*
	phi = m_ActValues.PHIN * radgrad;
	if (WinkelDispMode == techpos)
	    phi = 360.0-phi;
	XnPhaseDisp -> setText( QString("%1 %2").arg(phi,8,'f',4).arg( trUtf8("°")) );
	*/
	
	phi = m_ActValues.PHIN; // winkel sind zwischen 0 .. 2PI
	if (WinkelDispMode == techpos)
	    phi = PI2 - phi;
	phi = normWinkelrad_PIPI(phi);
	phi *= radgrad;

    ui->XnPhaseDisp -> setText( QString("%1 %2").arg(phi,8,'f',4).arg( trUtf8("°")) );

    // amplitude der grundschwingung
	if (AmplPrimSekMode == prim)
    {
        if (m_pConfData->m_bDCmeasurement)
            ampl = m_ActValues.VekX.re();
        else
            ampl = fabs(m_ActValues.VekX);
    }
	else
    {
        if (m_pConfData->m_bDCmeasurement)
            ampl = m_ActValues.VekXSek.re();
        else
            ampl = fabs(m_ActValues.VekXSek);
    }

    if (AmplDispMode == x1_SQRT2)
	    ampl/=1.414213562;

    ui->XxAmplDisp -> setText( QString("%1 V").arg(ampl,10,'f',5) );
	
	/*
	phi = m_ActValues.PHIX * radgrad;
	if (WinkelDispMode == techpos)
	    phi = 360.0-phi;
	XxPhaseDisp -> setText( QString("%1 %2").arg(phi,8,'f',4).arg( trUtf8("°")) );
	*/
	
	phi = m_ActValues.PHIX;
	if (WinkelDispMode == techpos)
	    phi = PI2 - phi;
	phi = normWinkelrad_PIPI(phi);
	phi *= radgrad;

    ui->XxPhaseDisp -> setText( QString("%1 %2").arg(phi,8,'f',4).arg( trUtf8("°")) );
    ui->FreqDisp -> setText( QString("%1 Hz").arg(ActValues->Frequenz,9,'f',5) );
    }
}


void WMRawActualValBase::SetConfInfoSlot( cConfData * cd )
{
    m_pConfData = cd;
    if (m_pConfData->m_bDCmeasurement)
    {
        ui->XnPhaseDisp->setVisible(false);
        ui->XxPhaseDisp->setVisible(false);
        ui->FreqDisp->setVisible(false);
        ui->FreqLabel->setVisible(false);
        AmplDispMode = x1;  // im fall von dc messung lassen wir nur x1 zu !!!
    }
    else
    {
        ui->XnPhaseDisp->setVisible(true);
        ui->XxPhaseDisp->setVisible(true);
        ui->FreqDisp->setVisible(true);
        ui->FreqLabel->setVisible(true);
    }
    resize(ui->gridLayout->minimumSize());
}


bool WMRawActualValBase::LoadSession(QString session)
{
    QFileInfo fi(session);
    QString ls = QString("%1/.wm3000u/%2%3").arg(wm3000uHome).arg(name()).arg(fi.fileName());
    QFile file(ls); 
    if ( file.open( QIODevice::ReadOnly ) ) {
	QDataStream stream( &file );
	stream >> m_widGeometry;
	stream >> AmplDispMode;
	stream >> WinkelDispMode,
	stream >> AmplPrimSekMode;
	file.close();
	hide();
	resize(m_widGeometry.m_Size);
	move(m_widGeometry.m_Point);
	if (m_widGeometry.vi)
	{
	    show();
	    emit isVisibleSignal(true);
	}
// FVWM und Gnome verhalten sich anders
#ifndef FVWM 
    move(m_widGeometry.m_Point);
#endif   
	return true;
    }
    return false;
}


void WMRawActualValBase::SaveSession(QString session)
{
    QFileInfo fi(session);
    if(!QDir(QString("%1/.wm3000u/").arg(wm3000uHome)).exists())
    {
      //create temporary object that gets deleted when leaving the control block
      QDir().mkdir(QString("%1/.wm3000u/").arg(wm3000uHome));
    }

    QString ls = QString("%1/.wm3000u/%2%3").arg(wm3000uHome).arg(name()).arg(fi.fileName());
    QFile file(ls); 
//    file.remove();
    if ( file.open( QIODevice::Unbuffered | QIODevice::WriteOnly ) ) {
	file.at(0);
	
	int vi;
	vi = (isVisible()) ? 1 : 0;
	if (vi)
	    m_widGeometry.SetGeometry(pos(),size());
	m_widGeometry.SetVisible(vi);
 
	QDataStream stream( &file );
	stream << m_widGeometry;
	stream << AmplDispMode;
	stream << WinkelDispMode;
	stream << AmplPrimSekMode;
	file.close();
    }
}


void WMRawActualValBase::contextMenuEvent( QContextMenuEvent * )
{
    emit SendVektorDispFormat(m_pConfData->m_bDCmeasurement, AmplDispMode, WinkelDispMode, AmplPrimSekMode);
    m_pContextMenu->show();
}


void WMRawActualValBase::ReceiveVektorDispFormat( int m, int m2, int m3)
{
    AmplDispMode = m;
    WinkelDispMode = m2;
    AmplPrimSekMode = m3;
}


void WMRawActualValBase::saveConfiguration()
{
    SaveSession(".ses");
}
