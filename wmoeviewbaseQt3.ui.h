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

void WMOeViewBase::init()
{
    LoadSession(".ses");
}


void WMOeViewBase::destroy()
{
    SaveSession(".ses");
}


void WMOeViewBase::ReceiveOEViewDataSlot(cOwnErrorViewData *oe)
{
    m_OwnErrorView=*oe;
    WandlerName->setText(m_OwnErrorView.m_sTrName);
    PrimDisp->setText(m_OwnErrorView.m_sPrim);
    SekDisp->setText(m_OwnErrorView.m_sSec); 
    LoadpointDisp->setText(m_OwnErrorView.m_sLoad);
    
    if (m_OwnErrorView.m_bValid)
    {
	AmplDisp->setText(m_OwnErrorView.m_sAmpl); 
	PhaseDisp->setText(m_OwnErrorView.m_sPhase); 
    }
    else
    {
	AmplDisp->setText("--------"); 
	PhaseDisp->setText("--------"); 
    }
}


void WMOeViewBase::closeEvent( QCloseEvent* ce)
{
    m_widGeometry.SetGeometry(pos(),size());
    m_widGeometry.SetVisible(0);
    emit isVisibleSignal(false); 
    ce->accept();
}


void WMOeViewBase::ShowHideOESlot( bool b )
{
  if (b) show();else close();
}


void WMOeViewBase::SaveSession(QString session)
{
    QFileInfo fi(session);
    QString ls = QString(".%1%2").arg(name()).arg(fi.fileName());
    QFile file(ls); 
 //   file.remove();
    if ( file.open( IO_Raw | IO_WriteOnly ) ) {
	file.at(0);
	
	int vi;
	vi = (isVisible()) ? 1 : 0;
	if (vi) 
	    m_widGeometry.SetGeometry(pos(),size());
	m_widGeometry.SetVisible(vi);
	
	QDataStream stream( &file );
	stream << m_widGeometry;
	file.close();
    }
}


bool WMOeViewBase::LoadSession(QString session)
{
    QFileInfo fi(session);
    QString ls = QString(".%1%2").arg(name()).arg(fi.fileName());
    QFile file(ls); 
    if ( file.open( IO_ReadOnly ) ) {
	QDataStream stream( &file );
	stream >> m_widGeometry;
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


