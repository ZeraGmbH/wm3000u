//Added by qt3to4:
#include <QCloseEvent>
#include <QFileInfo>

#include "csessionhelper.h"
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
    m_Timer.setSingleShot(true);
    connect(&m_Timer, SIGNAL(timeout()), this, SLOT(saveConfiguration()));
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
    m_Timer.start(500);
    ce->accept();
}


void WMOeViewBase::resizeEvent ( QResizeEvent *)
{
    m_Timer.start(500);
}


void WMOeViewBase::moveEvent( QMoveEvent *)
{
    m_Timer.start(500);
}


void WMOeViewBase::ShowHideOESlot( bool b )
{
  if (b) show();else close();
}


void WMOeViewBase::SaveSession(QString session)
{
  cSessionHelper::writeSession(this, m_widGeometry, session);
}


bool WMOeViewBase::LoadSession(QString session)
{
  cWidgetGeometry tmpGeometry;
  tmpGeometry=cSessionHelper::readSession(this, session);
  if(tmpGeometry.m_Size.isValid())
  {
    m_widGeometry=tmpGeometry;
    return true;
  }
  else
  {
    return false;
  }
}


void WMOeViewBase::saveConfiguration()
{
    SaveSession(".ses");
}

