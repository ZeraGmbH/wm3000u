/****************************************************************************
** Form implementation generated from reading ui file 'wmrawactualconfigbase.ui'
**
** Created: Mo Feb 13 08:30:11 2012
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "wmrawactualconfigbase.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "wmglobal.h"
#include "wmrawactualconfigbase.ui.h"
/*
 *  Constructs a WMRawActualConfigBase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
WMRawActualConfigBase::WMRawActualConfigBase( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "WMRawActualConfigBase" );
    setSizeGripEnabled( TRUE );
    WMRawActualConfigBaseLayout = new QVBoxLayout( this, 11, 6, "WMRawActualConfigBaseLayout"); 

    layout3 = new QHBoxLayout( 0, 0, 6, "layout3"); 

    AmplitudebuttonGroup = new QButtonGroup( this, "AmplitudebuttonGroup" );
    AmplitudebuttonGroup->setColumnLayout(0, Qt::Vertical );
    AmplitudebuttonGroup->layout()->setSpacing( 6 );
    AmplitudebuttonGroup->layout()->setMargin( 11 );
    AmplitudebuttonGroupLayout = new QVBoxLayout( AmplitudebuttonGroup->layout() );
    AmplitudebuttonGroupLayout->setAlignment( Qt::AlignTop );

    Amplx1radioButton = new QRadioButton( AmplitudebuttonGroup, "Amplx1radioButton" );
    AmplitudebuttonGroupLayout->addWidget( Amplx1radioButton );

    Amplx1_sqrt2radioButton = new QRadioButton( AmplitudebuttonGroup, "Amplx1_sqrt2radioButton" );
    AmplitudebuttonGroupLayout->addWidget( Amplx1_sqrt2radioButton );
    layout3->addWidget( AmplitudebuttonGroup );

    primsekbuttonGroup = new QButtonGroup( this, "primsekbuttonGroup" );
    primsekbuttonGroup->setColumnLayout(0, Qt::Vertical );
    primsekbuttonGroup->layout()->setSpacing( 6 );
    primsekbuttonGroup->layout()->setMargin( 11 );
    primsekbuttonGroupLayout = new QVBoxLayout( primsekbuttonGroup->layout() );
    primsekbuttonGroupLayout->setAlignment( Qt::AlignTop );

    PrimradioButton = new QRadioButton( primsekbuttonGroup, "PrimradioButton" );
    primsekbuttonGroupLayout->addWidget( PrimradioButton );

    SekradioButton = new QRadioButton( primsekbuttonGroup, "SekradioButton" );
    primsekbuttonGroupLayout->addWidget( SekradioButton );
    layout3->addWidget( primsekbuttonGroup );

    WinkelbuttonGroup = new QButtonGroup( this, "WinkelbuttonGroup" );
    WinkelbuttonGroup->setColumnLayout(0, Qt::Vertical );
    WinkelbuttonGroup->layout()->setSpacing( 6 );
    WinkelbuttonGroup->layout()->setMargin( 11 );
    WinkelbuttonGroupLayout = new QVBoxLayout( WinkelbuttonGroup->layout() );
    WinkelbuttonGroupLayout->setAlignment( Qt::AlignTop );

    MathradioButton = new QRadioButton( WinkelbuttonGroup, "MathradioButton" );
    WinkelbuttonGroupLayout->addWidget( MathradioButton );

    TechradioButton = new QRadioButton( WinkelbuttonGroup, "TechradioButton" );
    WinkelbuttonGroupLayout->addWidget( TechradioButton );
    layout3->addWidget( WinkelbuttonGroup );
    WMRawActualConfigBaseLayout->addLayout( layout3 );

    Layout1 = new QHBoxLayout( 0, 0, 6, "Layout1"); 
    Horizontal_Spacing2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout1->addItem( Horizontal_Spacing2 );

    buttonOk = new QPushButton( this, "buttonOk" );
    buttonOk->setAutoDefault( TRUE );
    buttonOk->setDefault( TRUE );
    Layout1->addWidget( buttonOk );

    buttonCancel = new QPushButton( this, "buttonCancel" );
    buttonCancel->setAutoDefault( TRUE );
    Layout1->addWidget( buttonCancel );
    WMRawActualConfigBaseLayout->addLayout( Layout1 );
    languageChange();
    resize( QSize(454, 141).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( SendData2AppSlot() ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( Amplx1radioButton, SIGNAL( clicked() ), this, SLOT( amplx1checkedSlot() ) );
    connect( Amplx1_sqrt2radioButton, SIGNAL( clicked() ), this, SLOT( amplx1_sq2checkedSlot() ) );
    connect( MathradioButton, SIGNAL( clicked() ), this, SLOT( winkelmathcheckedSlot() ) );
    connect( TechradioButton, SIGNAL( clicked() ), this, SLOT( winkeltechcheckedSlot() ) );
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( PrimradioButton, SIGNAL( clicked() ), this, SLOT( primCheckedSlot() ) );
    connect( SekradioButton, SIGNAL( clicked() ), this, SLOT( sekCheckedSlot() ) );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
WMRawActualConfigBase::~WMRawActualConfigBase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void WMRawActualConfigBase::languageChange()
{
    setCaption( tr( "Vektoren Formatsteuerung" ) );
    AmplitudebuttonGroup->setTitle( tr( "Amplitude" ) );
    Amplx1radioButton->setText( tr( "x 1" ) );
    Amplx1_sqrt2radioButton->setText( tr( "x 1/SQRT(2)" ) );
    primsekbuttonGroup->setTitle( tr( "prim/sek" ) );
    PrimradioButton->setText( trUtf8( "\x70\x72\x69\x6d\xc3\xa4\x72" ) );
    SekradioButton->setText( trUtf8( "\x73\x65\x6b\x75\x6e\x64\xc3\xa4\x72" ) );
    WinkelbuttonGroup->setTitle( tr( "Winkel" ) );
    MathradioButton->setText( tr( "math. positiv" ) );
    TechradioButton->setText( tr( "techn. positiv" ) );
    buttonOk->setText( tr( "&OK" ) );
    buttonOk->setAccel( QKeySequence( QString::null ) );
    buttonCancel->setText( tr( "&Cancel" ) );
    buttonCancel->setAccel( QKeySequence( QString::null ) );
}

