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


void EN61850monbase::init()
{
    setWindowOpacity ( 0.5 );
    ETHStatus.ByteCount[0] = 0;
    ETHStatus.ByteCount[1] = 0;
    ETHStatus.SyncLostCount = 0;
    ETHStatus.ETHErrors = 0;
        
    m_pTimer = new QTimer();
    QObject::connect(m_pTimer,SIGNAL(timeout()),this,SLOT(TimerSlot()));
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
    ce->accept();
}


void EN61850monbase::TimerSlot()
{
    emit InformationRequest(); // anfrage an wm3000 die status infos zu besorgen
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
	    
    ByteCountValLabel->setText(s);
    
    count = ETHStatus.SyncLostCount;
    s = QString("%1").arg( count, 0, 'f', 0 ); // keine nachkommastellen
    LostSyncValLabel->setText(s);
    
    stat = ETHStatus.ETHErrors;
    
    savPducheckBox->setChecked(stat & savPdu);
    ASDUcheckBox->setChecked(stat & noASDU);
    seqASDUcheckBox->setChecked(stat & seqASDU);
    seqASDUncheckBox->setChecked(stat & seqASDUn);
    serviceIdentcheckBox->setChecked(stat & svID);
    smpCountcheckBox->setChecked(stat & smpCnt);
    confRevcheckBox->setChecked(stat & confRev);
    smpSynchcheckBox->setChecked(stat & smpSync);
    seqDatacheckBox->setChecked(stat & seqData);
    
    MACSyncLostcheckBox->setChecked(stat & macSyncLost);
    DataSyncLostcheckBox->setChecked(stat & dataSyncLost);
    nASDUcheckBox->setChecked(stat & ASDUnavail);
    ETHMacAdrcheckBox->setChecked(stat & ETHMacAdressError);
    ETHHeadercheckBox->setChecked(stat & ETHHeaderAppIdError);
    PriorityTaggedcheckBox->setChecked(stat & ETHPriorityTaggedError);
    
    FIFOOvfcheckBox->setChecked(stat & FifoOverflow);
    CRCErrorcheckBox->setChecked(stat & CRCError);
    AlignErrorcheckBox->setChecked(stat & AlignError);
    PHysLayercheckBox->setChecked(stat & PhysicalLayer);
    RWTOcheckBox->setChecked(stat & ReceiveWDogTimeout);
    LateCollisioncheckBox->setChecked(stat & LateCollisionSeen);
    RUNTFramecheckBox->setChecked(stat & RuntFrame);
}


bool EN61850monbase::LoadSession( QString session )
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
	if (m_widGeometry.vi) show();
// FVWM und Gnome verhalten sich anders
#ifndef FVWM 
	move(m_widGeometry.m_Point);
#endif   
    	return true;
    }
    return false;
}


void EN61850monbase::SaveSession( QString session )
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
	file.close();
    }
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
