//Added by qt3to4:
#include <QCloseEvent>
#include <QTimer>
#include <QFileInfo>
#include "csessionhelper.h"
#include "en61850monitor.h"
#include "ui_en61850monitor.h"
#include "wmglobal.h"

EN61850monbase::EN61850monbase( QWidget* parent):
    QDialog(parent),
    ui(new Ui::EN61850monbase)
{
    ui->setupUi(this);
    init();
}

EN61850monbase::~EN61850monbase()
{
    destroy();
    delete ui;
}

void EN61850monbase::init()
{
    m_Timer.setSingleShot(true);
    ETHStatus.ByteCount[0] = 0;
    ETHStatus.ByteCount[1] = 0;
    ETHStatus.SyncLostCount = 0;
    ETHStatus.ETHErrors = 0;
        
    m_pTimer = new QTimer();
    QObject::connect(m_pTimer,SIGNAL(timeout()),this,SLOT(TimerSlot()));
    connect(&m_Timer, SIGNAL(timeout()), this, SLOT(saveConfiguration()));
    LoadSession(".ses");
}


void EN61850monbase::destroy()
{
    delete m_pTimer;
    SaveSession(".ses");
}


void EN61850monbase::ShowHideSlot(bool b)
{
    if (b) {
        show();
        emit InformationRequest(); // anfrage an wm3000 die status infos zu besorgen
        m_pTimer->start(2000); // wenn sichtbar -> timer läuft
    }
    else {
        close();
        m_pTimer->stop();
    }
}


void EN61850monbase::closeEvent( QCloseEvent * ce )
{
    m_widGeometry.SetGeometry(pos(),size());
    m_widGeometry.SetVisible(0);
    emit isVisibleSignal(false);
    m_Timer.start(500);
    ce->accept();
}


void EN61850monbase::resizeEvent ( QResizeEvent *)
{
    m_Timer.start(500);
}


void EN61850monbase::moveEvent( QMoveEvent *)
{
    m_Timer.start(500);
}


void EN61850monbase::TimerSlot()
{
    emit InformationRequest(); // anfrage an wm3000 die status infos zu besorgen
}


void EN61850monbase::saveConfiguration()
{
    SaveSession(".ses");
}


void EN61850monbase::SetETHStatusSlot( cEN61850Info *ethInfo )
{
    QString s;
    double count;
    ulong stat;
 
    ETHStatus = *ethInfo;
    count = ETHStatus.ByteCount[0]*4294967296.0+ethInfo->ByteCount[1];
    s = QString("%1").arg( count, 0, 'f', 0 ); // keine nachkommastellen
    uint i,p,l;
    p = l = s.length();
    i = 1;
    while (p>3) {
	s.insert(l-(i*3),'.');
	i++;
	p -= 3;
    }
	    
    ui->ByteCountValLabel->setText(s);
    
    count = ETHStatus.SyncLostCount;
    s = QString("%1").arg( count, 0, 'f', 0 ); // keine nachkommastellen
    ui->LostSyncValLabel->setText(s);
    
    stat = ETHStatus.ETHErrors;
    
    ui->savPducheckBox->setChecked(stat & savPdu);
    ui->ASDUcheckBox->setChecked(stat & noASDU);
    ui->seqASDUcheckBox->setChecked(stat & seqASDU);
    ui->seqASDUncheckBox->setChecked(stat & seqASDUn);
    ui->serviceIdentcheckBox->setChecked(stat & svID);
    ui->smpCountcheckBox->setChecked(stat & smpCnt);
    ui->confRevcheckBox->setChecked(stat & confRev);
    ui->smpSynchcheckBox->setChecked(stat & smpSync);
    ui->seqDatacheckBox->setChecked(stat & seqData);
    
    ui->MACSyncLostcheckBox->setChecked(stat & macSyncLost);
    ui->DataSyncLostcheckBox->setChecked(stat & dataSyncLost);
    ui->nASDUcheckBox->setChecked(stat & ASDUnavail);
    ui->ETHMacAdrcheckBox->setChecked(stat & ETHMacAdressError);
    ui->ETHHeadercheckBox->setChecked(stat & ETHHeaderAppIdError);
    ui->PriorityTaggedcheckBox->setChecked(stat & ETHPriorityTaggedError);
    
    ui->FIFOOvfcheckBox->setChecked(stat & FifoOverflow);
    ui->CRCErrorcheckBox->setChecked(stat & CRCError);
    ui->AlignErrorcheckBox->setChecked(stat & AlignError);
    ui->PHysLayercheckBox->setChecked(stat & PhysicalLayer);
    ui->RWTOcheckBox->setChecked(stat & ReceiveWDogTimeout);
    ui->LateCollisioncheckBox->setChecked(stat & LateCollisionSeen);
    ui->RUNTFramecheckBox->setChecked(stat & RuntFrame);
}


bool EN61850monbase::LoadSession( QString session )
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


void EN61850monbase::SaveSession( QString session )
{
  cSessionHelper::writeSession(this,m_widGeometry,session);
}


void EN61850monbase::accept()
{
    emit isVisibleSignal(false);
    QDialog::accept();
}


void EN61850monbase::reject()
{
    emit ResetETHStatus();
}
