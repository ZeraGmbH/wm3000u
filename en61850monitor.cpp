/****************************************************************************
** Form implementation generated from reading ui file 'en61850monitor.ui'
**
** Created: Mi Sep 19 07:37:47 2012
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "en61850monitor.h"

#include <qvariant.h>
#include <qobject.h>
#include <qlabel.h>
#include <qtimer.h>
#include <qfileinfo.h>
#include <qpushbutton.h>
#include <qbuttongroup.h>
#include <qcheckbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "en61850.h"
#include "en61850monitor.ui.h"
/*
 *  Constructs a EN61850monbase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
EN61850monbase::EN61850monbase( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "EN61850monbase" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, sizePolicy().hasHeightForWidth() ) );
    setSizeGripEnabled( FALSE );
    setModal( FALSE );
    EN61850monbaseLayout = new QVBoxLayout( this, 11, 6, "EN61850monbaseLayout"); 
    EN61850monbaseLayout->setResizeMode( QLayout::Fixed );

    MonitorCountbuttonGroup = new QButtonGroup( this, "MonitorCountbuttonGroup" );
    MonitorCountbuttonGroup->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, MonitorCountbuttonGroup->sizePolicy().hasHeightForWidth() ) );
    MonitorCountbuttonGroup->setColumnLayout(0, Qt::Vertical );
    MonitorCountbuttonGroup->layout()->setSpacing( 6 );
    MonitorCountbuttonGroup->layout()->setMargin( 11 );
    MonitorCountbuttonGroupLayout = new QVBoxLayout( MonitorCountbuttonGroup->layout() );
    MonitorCountbuttonGroupLayout->setAlignment( Qt::AlignTop );

    layout11 = new QHBoxLayout( 0, 0, 6, "layout11"); 

    ByteCountLabel = new QLabel( MonitorCountbuttonGroup, "ByteCountLabel" );
    ByteCountLabel->setEnabled( FALSE );
    layout11->addWidget( ByteCountLabel );

    ByteCountValLabel = new QLabel( MonitorCountbuttonGroup, "ByteCountValLabel" );
    layout11->addWidget( ByteCountValLabel );
    spacer36 = new QSpacerItem( 210, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout11->addItem( spacer36 );
    MonitorCountbuttonGroupLayout->addLayout( layout11 );

    layout8 = new QHBoxLayout( 0, 0, 6, "layout8"); 

    LostSyncCountLabel = new QLabel( MonitorCountbuttonGroup, "LostSyncCountLabel" );
    LostSyncCountLabel->setEnabled( FALSE );
    layout8->addWidget( LostSyncCountLabel );

    LostSyncValLabel = new QLabel( MonitorCountbuttonGroup, "LostSyncValLabel" );
    layout8->addWidget( LostSyncValLabel );
    spacer37 = new QSpacerItem( 130, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout8->addItem( spacer37 );
    MonitorCountbuttonGroupLayout->addLayout( layout8 );
    EN61850monbaseLayout->addWidget( MonitorCountbuttonGroup );

    layout12 = new QHBoxLayout( 0, 0, 6, "layout12"); 

    MACErrorsbuttonGroup = new QButtonGroup( this, "MACErrorsbuttonGroup" );
    MACErrorsbuttonGroup->setColumnLayout(0, Qt::Vertical );
    MACErrorsbuttonGroup->layout()->setSpacing( 6 );
    MACErrorsbuttonGroup->layout()->setMargin( 11 );
    MACErrorsbuttonGroupLayout = new QVBoxLayout( MACErrorsbuttonGroup->layout() );
    MACErrorsbuttonGroupLayout->setAlignment( Qt::AlignTop );

    layout7 = new QHBoxLayout( 0, 0, 6, "layout7"); 

    layout5 = new QVBoxLayout( 0, 0, 6, "layout5"); 

    RUNTFramecheckBox = new QCheckBox( MACErrorsbuttonGroup, "RUNTFramecheckBox" );
    RUNTFramecheckBox->setEnabled( FALSE );
    layout5->addWidget( RUNTFramecheckBox );

    LateCollisioncheckBox = new QCheckBox( MACErrorsbuttonGroup, "LateCollisioncheckBox" );
    LateCollisioncheckBox->setEnabled( FALSE );
    layout5->addWidget( LateCollisioncheckBox );

    RWTOcheckBox = new QCheckBox( MACErrorsbuttonGroup, "RWTOcheckBox" );
    RWTOcheckBox->setEnabled( FALSE );
    layout5->addWidget( RWTOcheckBox );

    PHysLayercheckBox = new QCheckBox( MACErrorsbuttonGroup, "PHysLayercheckBox" );
    PHysLayercheckBox->setEnabled( FALSE );
    layout5->addWidget( PHysLayercheckBox );
    layout7->addLayout( layout5 );

    layout6 = new QVBoxLayout( 0, 0, 6, "layout6"); 

    AlignErrorcheckBox = new QCheckBox( MACErrorsbuttonGroup, "AlignErrorcheckBox" );
    AlignErrorcheckBox->setEnabled( FALSE );
    layout6->addWidget( AlignErrorcheckBox );

    CRCErrorcheckBox = new QCheckBox( MACErrorsbuttonGroup, "CRCErrorcheckBox" );
    CRCErrorcheckBox->setEnabled( FALSE );
    layout6->addWidget( CRCErrorcheckBox );

    FIFOOvfcheckBox = new QCheckBox( MACErrorsbuttonGroup, "FIFOOvfcheckBox" );
    FIFOOvfcheckBox->setEnabled( FALSE );
    layout6->addWidget( FIFOOvfcheckBox );

    MACSyncLostcheckBox = new QCheckBox( MACErrorsbuttonGroup, "MACSyncLostcheckBox" );
    MACSyncLostcheckBox->setEnabled( FALSE );
    layout6->addWidget( MACSyncLostcheckBox );
    layout7->addLayout( layout6 );
    MACErrorsbuttonGroupLayout->addLayout( layout7 );
    spacer5 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    MACErrorsbuttonGroupLayout->addItem( spacer5 );
    layout12->addWidget( MACErrorsbuttonGroup );

    DecoderErrorbuttonGroup = new QButtonGroup( this, "DecoderErrorbuttonGroup" );
    DecoderErrorbuttonGroup->setColumnLayout(0, Qt::Vertical );
    DecoderErrorbuttonGroup->layout()->setSpacing( 6 );
    DecoderErrorbuttonGroup->layout()->setMargin( 11 );
    DecoderErrorbuttonGroupLayout = new QHBoxLayout( DecoderErrorbuttonGroup->layout() );
    DecoderErrorbuttonGroupLayout->setAlignment( Qt::AlignTop );

    layout8_2 = new QVBoxLayout( 0, 0, 6, "layout8_2"); 

    savPducheckBox = new QCheckBox( DecoderErrorbuttonGroup, "savPducheckBox" );
    savPducheckBox->setEnabled( FALSE );
    savPducheckBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, savPducheckBox->sizePolicy().hasHeightForWidth() ) );
    layout8_2->addWidget( savPducheckBox );

    ASDUcheckBox = new QCheckBox( DecoderErrorbuttonGroup, "ASDUcheckBox" );
    ASDUcheckBox->setEnabled( FALSE );
    layout8_2->addWidget( ASDUcheckBox );

    seqASDUcheckBox = new QCheckBox( DecoderErrorbuttonGroup, "seqASDUcheckBox" );
    seqASDUcheckBox->setEnabled( FALSE );
    layout8_2->addWidget( seqASDUcheckBox );

    seqASDUncheckBox = new QCheckBox( DecoderErrorbuttonGroup, "seqASDUncheckBox" );
    seqASDUncheckBox->setEnabled( FALSE );
    layout8_2->addWidget( seqASDUncheckBox );

    serviceIdentcheckBox = new QCheckBox( DecoderErrorbuttonGroup, "serviceIdentcheckBox" );
    serviceIdentcheckBox->setEnabled( FALSE );
    layout8_2->addWidget( serviceIdentcheckBox );

    smpCountcheckBox = new QCheckBox( DecoderErrorbuttonGroup, "smpCountcheckBox" );
    smpCountcheckBox->setEnabled( FALSE );
    layout8_2->addWidget( smpCountcheckBox );

    confRevcheckBox = new QCheckBox( DecoderErrorbuttonGroup, "confRevcheckBox" );
    confRevcheckBox->setEnabled( FALSE );
    layout8_2->addWidget( confRevcheckBox );
    DecoderErrorbuttonGroupLayout->addLayout( layout8_2 );

    layout11_2 = new QVBoxLayout( 0, 0, 6, "layout11_2"); 

    smpSynchcheckBox = new QCheckBox( DecoderErrorbuttonGroup, "smpSynchcheckBox" );
    smpSynchcheckBox->setEnabled( FALSE );
    layout11_2->addWidget( smpSynchcheckBox );

    seqDatacheckBox = new QCheckBox( DecoderErrorbuttonGroup, "seqDatacheckBox" );
    seqDatacheckBox->setEnabled( FALSE );
    layout11_2->addWidget( seqDatacheckBox );

    ETHMacAdrcheckBox = new QCheckBox( DecoderErrorbuttonGroup, "ETHMacAdrcheckBox" );
    ETHMacAdrcheckBox->setEnabled( FALSE );
    layout11_2->addWidget( ETHMacAdrcheckBox );

    ETHHeadercheckBox = new QCheckBox( DecoderErrorbuttonGroup, "ETHHeadercheckBox" );
    ETHHeadercheckBox->setEnabled( FALSE );
    layout11_2->addWidget( ETHHeadercheckBox );

    PriorityTaggedcheckBox = new QCheckBox( DecoderErrorbuttonGroup, "PriorityTaggedcheckBox" );
    PriorityTaggedcheckBox->setEnabled( FALSE );
    layout11_2->addWidget( PriorityTaggedcheckBox );

    nASDUcheckBox = new QCheckBox( DecoderErrorbuttonGroup, "nASDUcheckBox" );
    nASDUcheckBox->setEnabled( FALSE );
    layout11_2->addWidget( nASDUcheckBox );

    DataSyncLostcheckBox = new QCheckBox( DecoderErrorbuttonGroup, "DataSyncLostcheckBox" );
    DataSyncLostcheckBox->setEnabled( FALSE );
    layout11_2->addWidget( DataSyncLostcheckBox );
    DecoderErrorbuttonGroupLayout->addLayout( layout11_2 );
    layout12->addWidget( DecoderErrorbuttonGroup );
    EN61850monbaseLayout->addLayout( layout12 );

    Layout1 = new QHBoxLayout( 0, 0, 6, "Layout1"); 
    Horizontal_Spacing2 = new QSpacerItem( 300, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout1->addItem( Horizontal_Spacing2 );

    buttonOk = new QPushButton( this, "buttonOk" );
    buttonOk->setAutoDefault( TRUE );
    buttonOk->setDefault( TRUE );
    Layout1->addWidget( buttonOk );

    buttonCancel = new QPushButton( this, "buttonCancel" );
    buttonCancel->setAutoDefault( TRUE );
    Layout1->addWidget( buttonCancel );
    EN61850monbaseLayout->addLayout( Layout1 );
    languageChange();
    resize( QSize(713, 367).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( buttonCancel, SIGNAL( pressed() ), this, SLOT( reject() ) );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
EN61850monbase::~EN61850monbase()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void EN61850monbase::languageChange()
{
    setCaption( tr( "EN61850-9-2 Monitor" ) );
    MonitorCountbuttonGroup->setTitle( tr( "Counters" ) );
    ByteCountLabel->setText( tr( "Bytes received:" ) );
    ByteCountValLabel->setText( tr( "0" ) );
    LostSyncCountLabel->setText( tr( "Syncronization lost: " ) );
    LostSyncValLabel->setText( tr( "0" ) );
    MACErrorsbuttonGroup->setTitle( tr( "MAC Errors" ) );
    RUNTFramecheckBox->setText( tr( "Runt Frame" ) );
    QToolTip::add( RUNTFramecheckBox, QString::null );
    LateCollisioncheckBox->setText( tr( "Late Collision" ) );
    RWTOcheckBox->setText( tr( "Receive Timeout" ) );
    PHysLayercheckBox->setText( tr( "Physical Layer" ) );
    AlignErrorcheckBox->setText( tr( "Alignment" ) );
    CRCErrorcheckBox->setText( tr( "CRC" ) );
    FIFOOvfcheckBox->setText( tr( "FIFO Overflow" ) );
    MACSyncLostcheckBox->setText( tr( "Sync lost" ) );
    DecoderErrorbuttonGroup->setTitle( tr( "Decoder Errors" ) );
    savPducheckBox->setText( tr( "savPdu" ) );
    ASDUcheckBox->setText( tr( "num. of ASDU" ) );
    seqASDUcheckBox->setText( tr( "seq. of ASDU's" ) );
    seqASDUncheckBox->setText( tr( "seq. of ASDU n" ) );
    serviceIdentcheckBox->setText( tr( "Service Ident." ) );
    smpCountcheckBox->setText( tr( "smpCnt" ) );
    confRevcheckBox->setText( tr( "confRev" ) );
    smpSynchcheckBox->setText( tr( "smpSynch" ) );
    seqDatacheckBox->setText( tr( "seq. of Data" ) );
    ETHMacAdrcheckBox->setText( tr( "MAC Adress mismatch" ) );
    ETHHeadercheckBox->setText( tr( "Ethertype, APPID" ) );
    PriorityTaggedcheckBox->setText( tr( "Priority tagged" ) );
    nASDUcheckBox->setText( tr( "ASDU not avail" ) );
    DataSyncLostcheckBox->setText( tr( "Data Sync lost" ) );
    buttonOk->setText( tr( "&OK" ) );
    buttonOk->setAccel( QKeySequence( QString::null ) );
    buttonCancel->setText( tr( "&Reset" ) );
    buttonCancel->setAccel( QKeySequence( tr( "Alt+R" ) ) );
}

