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

const double PI2 = 6.283185307;

void WMRawActualValBase::init()
{
    AmplDispMode = x1;
    AmplPrimSekMode = prim;
    WinkelDispMode = mathpos;
    m_pContextMenu = new WMRawActualConfigBase(this);
    connect(this,SIGNAL(SendVektorDispFormat(int,int,int)),m_pContextMenu,SLOT(ReceiveDisplayConfSlot(int,int,int)));
    connect(m_pContextMenu,SIGNAL(SendVektorDisplayFormat(int,int,int)),this,SLOT(ReceiveVektorDispFormat(int,int,int)));
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
    ce->accept();
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
	    ampl = fabs(m_ActValues.VekN);
	else
	    ampl = fabs(m_ActValues.VekNSek);
	if (AmplDispMode == x1_SQRT2)
	    ampl/=1.414213562;
	XnAmplDisp -> setText( QString("%1 V").arg(ampl,10,'f',5) );
	
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
	XnPhaseDisp -> setText( QString("%1 %2").arg(phi,8,'f',4).arg( trUtf8("°")) );
	
	
	// amplitude der grundschwingung
	if (AmplPrimSekMode == prim)
	    ampl = fabs(m_ActValues.VekX);
	else
	    ampl = fabs(m_ActValues.VekXSek);
	if (AmplDispMode == x1_SQRT2)
	    ampl/=1.414213562;
	XxAmplDisp -> setText( QString("%1 V").arg(ampl,10,'f',5) );
	
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
	XxPhaseDisp -> setText( QString("%1 %2").arg(phi,8,'f',4).arg( trUtf8("°")) );
	
	FreqDisp -> setText( QString("%1 Hz").arg(ActValues->Frequenz,9,'f',5) );
    }
}


bool WMRawActualValBase::LoadSession(QString session)
{
    QFileInfo fi(session);
    QString ls = QString(".%1%2").arg(name()).arg(fi.fileName());
    QFile file(ls); 
    if ( file.open( IO_ReadOnly ) ) {
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
    QString ls = QString(".%1%2").arg(name()).arg(fi.fileName());
    QFile file(ls); 
//    file.remove();
    if ( file.open( IO_Raw | IO_WriteOnly ) ) {
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
    emit SendVektorDispFormat(AmplDispMode, WinkelDispMode, AmplPrimSekMode);
    m_pContextMenu->show();
}


void WMRawActualValBase::ReceiveVektorDispFormat( int m, int m2, int m3)
{
    AmplDispMode = m;
    WinkelDispMode = m2;
    AmplPrimSekMode = m3;
}


