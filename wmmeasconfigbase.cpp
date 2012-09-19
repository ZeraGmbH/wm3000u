/****************************************************************************
** Form implementation generated from reading ui file 'wmmeasconfigbase.ui'
**
** Created: Mo Feb 13 08:30:01 2012
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "wmmeasconfigbase.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qcombobox.h>
#include <qradiobutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "eparameter.h"
#include "wmglobal.h"
#include "wmmeasconfigbase.ui.h"
/*
 *  Constructs a WMMeasConfigBase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
WMMeasConfigBase::WMMeasConfigBase( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "WMMeasConfigBase" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, sizePolicy().hasHeightForWidth() ) );
    setSizeGripEnabled( FALSE );
    WMMeasConfigBaseLayout = new QVBoxLayout( this, 11, 6, "WMMeasConfigBaseLayout"); 
    WMMeasConfigBaseLayout->setResizeMode( QLayout::Fixed );

    layout4 = new QHBoxLayout( 0, 0, 6, "layout4"); 

    groupBox40 = new QGroupBox( this, "groupBox40" );
    groupBox40->setLineWidth( 0 );
    groupBox40->setColumnLayout(0, Qt::Vertical );
    groupBox40->layout()->setSpacing( 6 );
    groupBox40->layout()->setMargin( 11 );
    groupBox40Layout = new QVBoxLayout( groupBox40->layout() );
    groupBox40Layout->setAlignment( Qt::AlignTop );
    spacer54 = new QSpacerItem( 20, 8, QSizePolicy::Minimum, QSizePolicy::Expanding );
    groupBox40Layout->addItem( spacer54 );

    textLabel1 = new QLabel( groupBox40, "textLabel1" );
    textLabel1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, textLabel1->sizePolicy().hasHeightForWidth() ) );
    groupBox40Layout->addWidget( textLabel1 );
    spacer55 = new QSpacerItem( 16, 8, QSizePolicy::Minimum, QSizePolicy::Expanding );
    groupBox40Layout->addItem( spacer55 );

    textLabel2 = new QLabel( groupBox40, "textLabel2" );
    textLabel2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, textLabel2->sizePolicy().hasHeightForWidth() ) );
    groupBox40Layout->addWidget( textLabel2 );
    spacer56 = new QSpacerItem( 16, 8, QSizePolicy::Minimum, QSizePolicy::Expanding );
    groupBox40Layout->addItem( spacer56 );

    textLabel3 = new QLabel( groupBox40, "textLabel3" );
    textLabel3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, textLabel3->sizePolicy().hasHeightForWidth() ) );
    groupBox40Layout->addWidget( textLabel3 );
    spacer57 = new QSpacerItem( 20, 58, QSizePolicy::Minimum, QSizePolicy::Expanding );
    groupBox40Layout->addItem( spacer57 );
    layout4->addWidget( groupBox40 );

    LPgroupBox = new QGroupBox( this, "LPgroupBox" );
    LPgroupBox->setColumnLayout(0, Qt::Vertical );
    LPgroupBox->layout()->setSpacing( 6 );
    LPgroupBox->layout()->setMargin( 11 );
    LPgroupBoxLayout = new QVBoxLayout( LPgroupBox->layout() );
    LPgroupBoxLayout->setAlignment( Qt::AlignTop );

    LPspinBoxFW = new QSpinBox( LPgroupBox, "LPspinBoxFW" );
    LPspinBoxFW->setMaxValue( 10 );
    LPspinBoxFW->setMinValue( 5 );
    LPgroupBoxLayout->addWidget( LPspinBoxFW );

    LPspinBoxRS = new QSpinBox( LPgroupBox, "LPspinBoxRS" );
    LPspinBoxRS->setMaxValue( 6 );
    LPspinBoxRS->setMinValue( 0 );
    LPgroupBoxLayout->addWidget( LPspinBoxRS );

    LPcomboBox = new QComboBox( FALSE, LPgroupBox, "LPcomboBox" );
    LPgroupBoxLayout->addWidget( LPcomboBox );

    TotalRMSradioButton = new QRadioButton( LPgroupBox, "TotalRMSradioButton" );
    LPgroupBoxLayout->addWidget( TotalRMSradioButton );

    H1radioButton = new QRadioButton( LPgroupBox, "H1radioButton" );
    LPgroupBoxLayout->addWidget( H1radioButton );
    layout4->addWidget( LPgroupBox );

    ErrgroupBox = new QGroupBox( this, "ErrgroupBox" );
    ErrgroupBox->setColumnLayout(0, Qt::Vertical );
    ErrgroupBox->layout()->setSpacing( 6 );
    ErrgroupBox->layout()->setMargin( 11 );
    ErrgroupBoxLayout = new QVBoxLayout( ErrgroupBox->layout() );
    ErrgroupBoxLayout->setAlignment( Qt::AlignTop );

    ErrspinBoxFW = new QSpinBox( ErrgroupBox, "ErrspinBoxFW" );
    ErrspinBoxFW->setMaxValue( 10 );
    ErrspinBoxFW->setMinValue( 5 );
    ErrgroupBoxLayout->addWidget( ErrspinBoxFW );

    ErrspinBoxRS = new QSpinBox( ErrgroupBox, "ErrspinBoxRS" );
    ErrspinBoxRS->setMaxValue( 6 );
    ErrspinBoxRS->setMinValue( 0 );
    ErrgroupBoxLayout->addWidget( ErrspinBoxRS );

    ErrcomboBox = new QComboBox( FALSE, ErrgroupBox, "ErrcomboBox" );
    ErrgroupBoxLayout->addWidget( ErrcomboBox );

    IECradioButton = new QRadioButton( ErrgroupBox, "IECradioButton" );
    IECradioButton->setChecked( FALSE );
    ErrgroupBoxLayout->addWidget( IECradioButton );

    ANSIradioButton = new QRadioButton( ErrgroupBox, "ANSIradioButton" );
    ErrgroupBoxLayout->addWidget( ANSIradioButton );
    layout4->addWidget( ErrgroupBox );

    WgroupBox = new QGroupBox( this, "WgroupBox" );
    WgroupBox->setColumnLayout(0, Qt::Vertical );
    WgroupBox->layout()->setSpacing( 6 );
    WgroupBox->layout()->setMargin( 11 );
    WgroupBoxLayout = new QVBoxLayout( WgroupBox->layout() );
    WgroupBoxLayout->setAlignment( Qt::AlignTop );

    WspinBoxFW = new QSpinBox( WgroupBox, "WspinBoxFW" );
    WspinBoxFW->setMaxValue( 10 );
    WspinBoxFW->setMinValue( 5 );
    WgroupBoxLayout->addWidget( WspinBoxFW );

    WspinBoxRS = new QSpinBox( WgroupBox, "WspinBoxRS" );
    WspinBoxRS->setMaxValue( 6 );
    WspinBoxRS->setMinValue( 0 );
    WgroupBoxLayout->addWidget( WspinBoxRS );

    WcomboBox = new QComboBox( FALSE, WgroupBox, "WcomboBox" );
    WgroupBoxLayout->addWidget( WcomboBox );
    spacer53 = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
    WgroupBoxLayout->addItem( spacer53 );
    layout4->addWidget( WgroupBox );
    WMMeasConfigBaseLayout->addLayout( layout4 );

    layout48 = new QHBoxLayout( 0, 0, 6, "layout48"); 
    Horizontal_Spacing2 = new QSpacerItem( 180, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout48->addItem( Horizontal_Spacing2 );

    buttonOk = new QPushButton( this, "buttonOk" );
    buttonOk->setAutoDefault( TRUE );
    buttonOk->setDefault( TRUE );
    layout48->addWidget( buttonOk );

    buttonCancel = new QPushButton( this, "buttonCancel" );
    buttonCancel->setAutoDefault( TRUE );
    layout48->addWidget( buttonCancel );
    WMMeasConfigBaseLayout->addLayout( layout48 );
    languageChange();
    resize( QSize(693, 371).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( SendData2AppSlot() ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( LPcomboBox, SIGNAL( activated(int) ), this, SLOT( LPComboActivatedSlot(int) ) );
    connect( ErrcomboBox, SIGNAL( activated(int) ), this, SLOT( ErrComboActivatedSlot(int) ) );
    connect( WcomboBox, SIGNAL( activated(int) ), this, SLOT( WComboActivatedSlot(int) ) );
    connect( IECradioButton, SIGNAL( clicked() ), this, SLOT( IECCheckedSlot() ) );
    connect( ANSIradioButton, SIGNAL( clicked() ), this, SLOT( ANSICheckedSlot() ) );
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( TotalRMSradioButton, SIGNAL( clicked() ), this, SLOT( TotalRMScheckedSlot() ) );
    connect( H1radioButton, SIGNAL( clicked() ), this, SLOT( H1RMScheckedSlot() ) );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
WMMeasConfigBase::~WMMeasConfigBase()
{
<<<<<<< HEAD
    // no need to delete child widgets, Qt does it all for us
=======
    destroy();
    delete ui;
}

void WMMeasConfigBase::init()
{
   int i;
   
   m_LPUnit[0] = LoadpointUnit + LPProzent;
   m_LPUnit[1] = VoltageUnit + VoltV;
   m_LPUnit[2] = VoltageUnit + VoltkV;
    
   for (i = 0; i < 3; i++)
        ui->LPcomboBox->insertItem(m_LPUnit[i]->Name);
       
    m_ErrUnit[0] = ErrorUnit + ErrProzent;
    m_ErrUnit[1] = ErrorUnit + Errppm;
    // FcomboBox-	>insertItem(ErrorUnit[ErrPromill]->Name);
    
    for (i = 0; i < 2; i++)
        ui->ErrcomboBox->insertItem(m_ErrUnit[i]->Name);

    m_AngleUnit[0] = AngleUnit + Anglegrad;
    m_AngleUnit[1] = AngleUnit + Anglemin;
    m_AngleUnit[2] = AngleUnit + Anglecrad;
    m_AngleUnit[3] = AngleUnit + Anglemrad;
    m_AngleUnit[4] = AngleUnit + Angleurad;
    m_AngleUnit[5] = AngleUnit + Anglerad;
    
    for (i = 0; i < 6; i++)
        ui->WcomboBox->insertItem(m_AngleUnit[i]->Name);
    
    m_nDisplayMode = IEC; // wmglobal
    m_nLPDisplayMode = totalRms;
}


void WMMeasConfigBase::ReceiveFormatInfoSlot(int m, int m2, int n, cFormatInfo* fi )
{
    int i;
    for(i = 0; i < n; i++, fi++)
	m_Format[i] = *fi;
    
    m_nDisplayMode = m;
    m_nLPDisplayMode = m2;
    
    ui->LPspinBoxFW->setValue(m_Format[0].FieldWidth);
    ui->LPspinBoxRS->setValue(m_Format[0].Resolution);
    ui->LPcomboBox->setCurrentText(m_Format[0].UnitInfo.Name);
		
    ui->ErrspinBoxFW->setValue(m_Format[1].FieldWidth);
    ui->ErrspinBoxRS->setValue(m_Format[1].Resolution);
    ui->ErrcomboBox->setCurrentText(m_Format[1].UnitInfo.Name);
    
    ui->WspinBoxFW->setValue(m_Format[2].FieldWidth);
    ui->WspinBoxRS->setValue(m_Format[2].Resolution);
    ui->WcomboBox->setCurrentText(m_Format[2].UnitInfo.Name);
 
    Actualize();
}



void WMMeasConfigBase::LPComboActivatedSlot(int index)
{
    m_Format[0].UnitInfo = *m_LPUnit[index];
>>>>>>> 5a91ce3... Contextmenu of measurement values extended with mrad.
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void WMMeasConfigBase::languageChange()
{
    setCaption( tr( "Messwert Formatsteuerung" ) );
    groupBox40->setTitle( QString::null );
    textLabel1->setText( tr( "Feldbreite:" ) );
    textLabel2->setText( trUtf8( "\x41\x75\x66\x6c\xc3\xb6\x73\x75\x6e\x67\x3a" ) );
    textLabel3->setText( tr( "Einheit:" ) );
    LPgroupBox->setTitle( tr( "Lastpunkt" ) );
    QToolTip::add( LPspinBoxFW, QString::null );
    QWhatsThis::add( LPspinBoxFW, QString::null );
    QToolTip::add( LPspinBoxRS, QString::null );
    QToolTip::add( LPcomboBox, QString::null );
    TotalRMSradioButton->setText( tr( "Total RMS" ) );
    H1radioButton->setText( tr( "1. Harm. RMS" ) );
    ErrgroupBox->setTitle( tr( "Fehler" ) );
    QToolTip::add( ErrspinBoxFW, QString::null );
    QToolTip::add( ErrspinBoxRS, QString::null );
    QToolTip::add( ErrcomboBox, QString::null );
    IECradioButton->setText( tr( "IEC" ) );
    ANSIradioButton->setText( tr( "ANSI" ) );
    WgroupBox->setTitle( tr( "Winkel" ) );
    QToolTip::add( WspinBoxFW, QString::null );
    QToolTip::add( WspinBoxRS, QString::null );
    QToolTip::add( WcomboBox, QString::null );
    buttonOk->setText( tr( "&OK" ) );
    buttonOk->setAccel( QKeySequence( QString::null ) );
    buttonCancel->setText( tr( "&Cancel" ) );
    buttonCancel->setAccel( QKeySequence( QString::null ) );
}

