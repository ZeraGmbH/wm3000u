//Added by qt3to4:
#include <QCloseEvent>
#include <QFileInfo>
#include "wmoeviewbase.h"
#include "ui_wmoeviewbase.h"

WMOeViewBase::WMOeViewBase( QWidget* parent):
    QDialog(parent),
    ui(new Ui::WMOeViewBase )
{
    ui->setupUi(this);
    init();
}

WMOeViewBase::~WMOeViewBase()
{
    destroy();
    delete ui;
}


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
    ui->WandlerName->setText(m_OwnErrorView.m_sTrName);
    ui->PrimDisp->setText(m_OwnErrorView.m_sPrim);
    ui->SekDisp->setText(m_OwnErrorView.m_sSec);
    ui->LoadpointDisp->setText(m_OwnErrorView.m_sLoad);
    
    if (m_OwnErrorView.m_bValid)
    {
        ui->AmplDisp->setText(m_OwnErrorView.m_sAmpl);
        ui->PhaseDisp->setText(m_OwnErrorView.m_sPhase);
    }
    else
    {
        ui->AmplDisp->setText("--------");
        ui->PhaseDisp->setText("--------");
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
    QString ls = QString("%1.%2%3").arg(wm3000uHome).arg(name()).arg(fi.fileName());
    QFile file(ls); 
 //   file.remove();
    if ( file.open( QIODevice::Unbuffered | QIODevice::WriteOnly ) ) {
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
    QString ls = QString("%1.%2%3").arg(wm3000uHome).arg(name()).arg(fi.fileName());
    QFile file(ls); 
    if ( file.open( QIODevice::ReadOnly ) ) {
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


