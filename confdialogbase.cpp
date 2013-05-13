/****************************************************************************
** Form implementation generated from reading ui file 'confdialogbase.ui'
**
** Created: Mo Mai 13 10:38:35 2013
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "confdialogbase.h"

#include <qvariant.h>
#include <qstring.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qspinbox.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "qvalidator.h"
#include "wmglobal.h"
#include "range.h"
#include "confdialogbase.ui.h"

/*
 *  Constructs a ConfDialogBase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
ConfDialogBase::ConfDialogBase( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "ConfDialogBase" );
    setEnabled( TRUE );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, sizePolicy().hasHeightForWidth() ) );
    setMinimumSize( QSize( 0, 0 ) );
    QFont f( font() );
    setFont( f ); 
    setCursor( QCursor( 0 ) );
    setSizeGripEnabled( FALSE );
    setModal( TRUE );
    ConfDialogBaseLayout = new QVBoxLayout( this, 11, 6, "ConfDialogBaseLayout"); 
    ConfDialogBaseLayout->setResizeMode( QLayout::Minimum );

    ConfTabWidget = new QTabWidget( this, "ConfTabWidget" );
    ConfTabWidget->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, ConfTabWidget->sizePolicy().hasHeightForWidth() ) );
    ConfTabWidget->setMinimumSize( QSize( 467, 230 ) );
    QFont ConfTabWidget_font(  ConfTabWidget->font() );
    ConfTabWidget->setFont( ConfTabWidget_font ); 
    ConfTabWidget->setTabShape( QTabWidget::Rounded );

    Widget8 = new QWidget( ConfTabWidget, "Widget8" );

    ModeButtonGroup = new QButtonGroup( Widget8, "ModeButtonGroup" );
    ModeButtonGroup->setGeometry( QRect( 12, 12, 123, 116 ) );
    ModeButtonGroup->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, ModeButtonGroup->sizePolicy().hasHeightForWidth() ) );
    QFont ModeButtonGroup_font(  ModeButtonGroup->font() );
    ModeButtonGroup->setFont( ModeButtonGroup_font ); 
    ModeButtonGroup->setFrameShadow( QButtonGroup::Sunken );
    ModeButtonGroup->setMargin( 0 );
    ModeButtonGroup->setAlignment( int( QButtonGroup::AlignTop ) );

    Mode0RadioButton = new QRadioButton( ModeButtonGroup, "Mode0RadioButton" );
    Mode0RadioButton->setGeometry( QRect( 11, 22, 91, 20 ) );
    Mode0RadioButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, Mode0RadioButton->sizePolicy().hasHeightForWidth() ) );

    Mode2RadioButton = new QRadioButton( ModeButtonGroup, "Mode2RadioButton" );
    Mode2RadioButton->setGeometry( QRect( 11, 48, 72, 20 ) );
    Mode2RadioButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, Mode2RadioButton->sizePolicy().hasHeightForWidth() ) );

    Mode3RadioButton = new QRadioButton( ModeButtonGroup, "Mode3RadioButton" );
    Mode3RadioButton->setGeometry( QRect( 11, 74, 101, 20 ) );
    Mode3RadioButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, Mode3RadioButton->sizePolicy().hasHeightForWidth() ) );
    QFont Mode3RadioButton_font(  Mode3RadioButton->font() );
    Mode3RadioButton->setFont( Mode3RadioButton_font ); 
    ConfTabWidget->insertTab( Widget8, QString::fromLatin1("") );

    Widget9 = new QWidget( ConfTabWidget, "Widget9" );
    Widget9Layout = new QVBoxLayout( Widget9, 11, 6, "Widget9Layout"); 

    layout46 = new QHBoxLayout( 0, 0, 6, "layout46"); 

    layout45 = new QVBoxLayout( 0, 0, 6, "layout45"); 

    CmpKorrGroupBox = new QGroupBox( Widget9, "CmpKorrGroupBox" );
    CmpKorrGroupBox->setColumnLayout(0, Qt::Vertical );
    CmpKorrGroupBox->layout()->setSpacing( 6 );
    CmpKorrGroupBox->layout()->setMargin( 11 );
    CmpKorrGroupBoxLayout = new QGridLayout( CmpKorrGroupBox->layout() );
    CmpKorrGroupBoxLayout->setAlignment( Qt::AlignTop );

    CmpKorrLabel2 = new QLabel( CmpKorrGroupBox, "CmpKorrLabel2" );

    CmpKorrGroupBoxLayout->addWidget( CmpKorrLabel2, 1, 0 );

    CmpKorrLabel1 = new QLabel( CmpKorrGroupBox, "CmpKorrLabel1" );

    CmpKorrGroupBoxLayout->addWidget( CmpKorrLabel1, 0, 0 );

    layout16 = new QVBoxLayout( 0, 0, 6, "layout16"); 

    CmpKorrLineEdit1 = new QLineEdit( CmpKorrGroupBox, "CmpKorrLineEdit1" );
    CmpKorrLineEdit1->setFocusPolicy( QLineEdit::StrongFocus );
    CmpKorrLineEdit1->setMaxLength( 8 );
    CmpKorrLineEdit1->setDragEnabled( TRUE );
    CmpKorrLineEdit1->setReadOnly( FALSE );
    layout16->addWidget( CmpKorrLineEdit1 );

    CmpKorrLineEdit2 = new QLineEdit( CmpKorrGroupBox, "CmpKorrLineEdit2" );
    CmpKorrLineEdit2->setMaxLength( 8 );
    layout16->addWidget( CmpKorrLineEdit2 );

    CmpKorrGroupBoxLayout->addMultiCellLayout( layout16, 0, 1, 1, 1 );
    layout45->addWidget( CmpKorrGroupBox );

    CmpCorrGroupBox = new QGroupBox( Widget9, "CmpCorrGroupBox" );
    CmpCorrGroupBox->setColumnLayout(0, Qt::Vertical );
    CmpCorrGroupBox->layout()->setSpacing( 6 );
    CmpCorrGroupBox->layout()->setMargin( 11 );
    CmpCorrGroupBoxLayout = new QVBoxLayout( CmpCorrGroupBox->layout() );
    CmpCorrGroupBoxLayout->setAlignment( Qt::AlignTop );

    CmpCorrCheckBox = new QCheckBox( CmpCorrGroupBox, "CmpCorrCheckBox" );
    CmpCorrGroupBoxLayout->addWidget( CmpCorrCheckBox );
    layout45->addWidget( CmpCorrGroupBox );
    layout46->addLayout( layout45 );
    spacer14_2 = new QSpacerItem( 306, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout46->addItem( spacer14_2 );
    Widget9Layout->addLayout( layout46 );
    spacer13_3 = new QSpacerItem( 20, 52, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Widget9Layout->addItem( spacer13_3 );
    ConfTabWidget->insertTab( Widget9, QString::fromLatin1("") );

    TabPage = new QWidget( ConfTabWidget, "TabPage" );
    TabPageLayout = new QGridLayout( TabPage, 1, 1, 11, 6, "TabPageLayout"); 
    spacer58 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    TabPageLayout->addItem( spacer58, 0, 2 );
    spacer59 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    TabPageLayout->addItem( spacer59, 1, 2 );

    TIntegrationGroupBox = new QGroupBox( TabPage, "TIntegrationGroupBox" );
    TIntegrationGroupBox->setColumnLayout(0, Qt::Vertical );
    TIntegrationGroupBox->layout()->setSpacing( 6 );
    TIntegrationGroupBox->layout()->setMargin( 11 );
    TIntegrationGroupBoxLayout = new QHBoxLayout( TIntegrationGroupBox->layout() );
    TIntegrationGroupBoxLayout->setAlignment( Qt::AlignTop );

    TIntegrationLabel = new QLabel( TIntegrationGroupBox, "TIntegrationLabel" );
    TIntegrationGroupBoxLayout->addWidget( TIntegrationLabel );

    TIntegrationSpinBox = new QSpinBox( TIntegrationGroupBox, "TIntegrationSpinBox" );
    TIntegrationSpinBox->setMaxValue( 20 );
    TIntegrationSpinBox->setMinValue( 1 );
    TIntegrationSpinBox->setLineStep( 1 );
    TIntegrationGroupBoxLayout->addWidget( TIntegrationSpinBox );
    spacer57 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    TIntegrationGroupBoxLayout->addItem( spacer57 );

    TabPageLayout->addWidget( TIntegrationGroupBox, 1, 1 );

    FreqButtonGroup = new QButtonGroup( TabPage, "FreqButtonGroup" );
    FreqButtonGroup->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, FreqButtonGroup->sizePolicy().hasHeightForWidth() ) );
    QFont FreqButtonGroup_font(  FreqButtonGroup->font() );
    FreqButtonGroup->setFont( FreqButtonGroup_font ); 
    FreqButtonGroup->setColumnLayout(0, Qt::Vertical );
    FreqButtonGroup->layout()->setSpacing( 6 );
    FreqButtonGroup->layout()->setMargin( 11 );
    FreqButtonGroupLayout = new QVBoxLayout( FreqButtonGroup->layout() );
    FreqButtonGroupLayout->setAlignment( Qt::AlignTop );

    F16RadioButton = new QRadioButton( FreqButtonGroup, "F16RadioButton" );
    F16RadioButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, F16RadioButton->sizePolicy().hasHeightForWidth() ) );
    QFont F16RadioButton_font(  F16RadioButton->font() );
    F16RadioButton->setFont( F16RadioButton_font ); 
    FreqButtonGroupLayout->addWidget( F16RadioButton );

    F50RadioButton = new QRadioButton( FreqButtonGroup, "F50RadioButton" );
    F50RadioButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, F50RadioButton->sizePolicy().hasHeightForWidth() ) );
    QFont F50RadioButton_font(  F50RadioButton->font() );
    F50RadioButton->setFont( F50RadioButton_font ); 
    FreqButtonGroupLayout->addWidget( F50RadioButton );

    F60RadioButton = new QRadioButton( FreqButtonGroup, "F60RadioButton" );
    F60RadioButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, F60RadioButton->sizePolicy().hasHeightForWidth() ) );
    QFont F60RadioButton_font(  F60RadioButton->font() );
    F60RadioButton->setFont( F60RadioButton_font ); 
    FreqButtonGroupLayout->addWidget( F60RadioButton );

    TabPageLayout->addWidget( FreqButtonGroup, 0, 0 );

    SPeriodeButtonGroup = new QButtonGroup( TabPage, "SPeriodeButtonGroup" );
    SPeriodeButtonGroup->setColumnLayout(0, Qt::Vertical );
    SPeriodeButtonGroup->layout()->setSpacing( 6 );
    SPeriodeButtonGroup->layout()->setMargin( 11 );
    SPeriodeButtonGroupLayout = new QVBoxLayout( SPeriodeButtonGroup->layout() );
    SPeriodeButtonGroupLayout->setAlignment( Qt::AlignTop );

    S80RadioButton = new QRadioButton( SPeriodeButtonGroup, "S80RadioButton" );
    SPeriodeButtonGroupLayout->addWidget( S80RadioButton );

    S256RadioButton = new QRadioButton( SPeriodeButtonGroup, "S256RadioButton" );
    SPeriodeButtonGroupLayout->addWidget( S256RadioButton );
    spacer27 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    SPeriodeButtonGroupLayout->addItem( spacer27 );

    TabPageLayout->addWidget( SPeriodeButtonGroup, 1, 0 );

    CmpIntervallGroupBox = new QGroupBox( TabPage, "CmpIntervallGroupBox" );
    CmpIntervallGroupBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, CmpIntervallGroupBox->sizePolicy().hasHeightForWidth() ) );
    QFont CmpIntervallGroupBox_font(  CmpIntervallGroupBox->font() );
    CmpIntervallGroupBox->setFont( CmpIntervallGroupBox_font ); 
    CmpIntervallGroupBox->setColumnLayout(0, Qt::Vertical );
    CmpIntervallGroupBox->layout()->setSpacing( 6 );
    CmpIntervallGroupBox->layout()->setMargin( 11 );
    CmpIntervallGroupBoxLayout = new QHBoxLayout( CmpIntervallGroupBox->layout() );
    CmpIntervallGroupBoxLayout->setAlignment( Qt::AlignTop );

    CmpIntervallLabel = new QLabel( CmpIntervallGroupBox, "CmpIntervallLabel" );
    CmpIntervallLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, CmpIntervallLabel->sizePolicy().hasHeightForWidth() ) );
    QFont CmpIntervallLabel_font(  CmpIntervallLabel->font() );
    CmpIntervallLabel->setFont( CmpIntervallLabel_font ); 
    CmpIntervallGroupBoxLayout->addWidget( CmpIntervallLabel );

    CmpIntervallSpinBox = new QSpinBox( CmpIntervallGroupBox, "CmpIntervallSpinBox" );
    CmpIntervallSpinBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, CmpIntervallSpinBox->sizePolicy().hasHeightForWidth() ) );
    CmpIntervallSpinBox->setMaxValue( 26 );
    CmpIntervallSpinBox->setMinValue( 4 );
    CmpIntervallSpinBox->setValue( 16 );
    CmpIntervallGroupBoxLayout->addWidget( CmpIntervallSpinBox );
    spacer9_2 = new QSpacerItem( 40, 20, QSizePolicy::Preferred, QSizePolicy::Minimum );
    CmpIntervallGroupBoxLayout->addItem( spacer9_2 );

    TabPageLayout->addWidget( CmpIntervallGroupBox, 0, 1 );
    ConfTabWidget->insertTab( TabPage, QString::fromLatin1("") );

    TabPage_2 = new QWidget( ConfTabWidget, "TabPage_2" );
    TabPageLayout_2 = new QVBoxLayout( TabPage_2, 11, 6, "TabPageLayout_2"); 

    layout37 = new QHBoxLayout( 0, 0, 6, "layout37"); 

    layout36 = new QVBoxLayout( 0, 0, 6, "layout36"); 

    SyncButtonGroup = new QButtonGroup( TabPage_2, "SyncButtonGroup" );
    SyncButtonGroup->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, SyncButtonGroup->sizePolicy().hasHeightForWidth() ) );
    SyncButtonGroup->setExclusive( FALSE );
    SyncButtonGroup->setRadioButtonExclusive( TRUE );
    SyncButtonGroup->setColumnLayout(0, Qt::Vertical );
    SyncButtonGroup->layout()->setSpacing( 6 );
    SyncButtonGroup->layout()->setMargin( 11 );
    SyncButtonGroupLayout = new QHBoxLayout( SyncButtonGroup->layout() );
    SyncButtonGroupLayout->setAlignment( Qt::AlignTop );

    layout101 = new QVBoxLayout( 0, 0, 6, "layout101"); 

    SyncRadioButton1 = new QRadioButton( SyncButtonGroup, "SyncRadioButton1" );
    SyncRadioButton1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, SyncRadioButton1->sizePolicy().hasHeightForWidth() ) );
    layout101->addWidget( SyncRadioButton1 );

    SyncRadioButton2 = new QRadioButton( SyncButtonGroup, "SyncRadioButton2" );
    SyncRadioButton2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, SyncRadioButton2->sizePolicy().hasHeightForWidth() ) );
    layout101->addWidget( SyncRadioButton2 );
    SyncButtonGroupLayout->addLayout( layout101 );

    layout102 = new QVBoxLayout( 0, 0, 6, "layout102"); 
    spacer77 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Preferred );
    layout102->addItem( spacer77 );

    layout95 = new QHBoxLayout( 0, 0, 6, "layout95"); 
    spacer12 = new QSpacerItem( 16, 20, QSizePolicy::Preferred, QSizePolicy::Minimum );
    layout95->addItem( spacer12 );

    TSyncLabel = new QLabel( SyncButtonGroup, "TSyncLabel" );
    TSyncLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, TSyncLabel->sizePolicy().hasHeightForWidth() ) );
    layout95->addWidget( TSyncLabel );

    TSyncSpinBox = new QSpinBox( SyncButtonGroup, "TSyncSpinBox" );
    TSyncSpinBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, TSyncSpinBox->sizePolicy().hasHeightForWidth() ) );
    TSyncSpinBox->setMaxValue( 2500 );
    TSyncSpinBox->setMinValue( 500 );
    TSyncSpinBox->setLineStep( 100 );
    layout95->addWidget( TSyncSpinBox );
    layout102->addLayout( layout95 );
    SyncButtonGroupLayout->addLayout( layout102 );
    layout36->addWidget( SyncButtonGroup );

    groupBox = new QGroupBox( TabPage_2, "groupBox" );
    groupBox->setAlignment( int( QGroupBox::AlignVCenter | QGroupBox::AlignLeft ) );

    SSynccheckBox = new QCheckBox( groupBox, "SSynccheckBox" );
    SSynccheckBox->setGeometry( QRect( 10, 30, 225, 20 ) );
    layout36->addWidget( groupBox );
    layout37->addLayout( layout36 );
    spacer85 = new QSpacerItem( 207, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout37->addItem( spacer85 );
    TabPageLayout_2->addLayout( layout37 );
    spacer86 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    TabPageLayout_2->addItem( spacer86 );
    ConfTabWidget->insertTab( TabPage_2, QString::fromLatin1("") );

    TabPage_3 = new QWidget( ConfTabWidget, "TabPage_3" );
    TabPageLayout_3 = new QGridLayout( TabPage_3, 1, 1, 11, 6, "TabPageLayout_3"); 

    EVTratioGroupBox = new QGroupBox( TabPage_3, "EVTratioGroupBox" );
    EVTratioGroupBox->setEnabled( TRUE );
    EVTratioGroupBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)1, 0, 0, EVTratioGroupBox->sizePolicy().hasHeightForWidth() ) );
    EVTratioGroupBox->setColumnLayout(0, Qt::Vertical );
    EVTratioGroupBox->layout()->setSpacing( 6 );
    EVTratioGroupBox->layout()->setMargin( 11 );
    EVTratioGroupBoxLayout = new QGridLayout( EVTratioGroupBox->layout() );
    EVTratioGroupBoxLayout->setAlignment( Qt::AlignTop );

    RatioSekEVTLineEdit = new QLineEdit( EVTratioGroupBox, "RatioSekEVTLineEdit" );
    RatioSekEVTLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, RatioSekEVTLineEdit->sizePolicy().hasHeightForWidth() ) );
    RatioSekEVTLineEdit->setMaximumSize( QSize( 75, 32767 ) );
    RatioSekEVTLineEdit->setMaxLength( 9 );

    EVTratioGroupBoxLayout->addMultiCellWidget( RatioSekEVTLineEdit, 1, 2, 1, 1 );

    RatioSekXLabel_3 = new QLabel( EVTratioGroupBox, "RatioSekXLabel_3" );
    RatioSekXLabel_3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)0, 0, 0, RatioSekXLabel_3->sizePolicy().hasHeightForWidth() ) );

    EVTratioGroupBoxLayout->addWidget( RatioSekXLabel_3, 2, 0 );

    RatioPrimEVTLineEdit = new QLineEdit( EVTratioGroupBox, "RatioPrimEVTLineEdit" );
    RatioPrimEVTLineEdit->setEnabled( TRUE );
    RatioPrimEVTLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, RatioPrimEVTLineEdit->sizePolicy().hasHeightForWidth() ) );
    RatioPrimEVTLineEdit->setMaximumSize( QSize( 75, 32767 ) );
    RatioPrimEVTLineEdit->setMaxLength( 9 );

    EVTratioGroupBoxLayout->addWidget( RatioPrimEVTLineEdit, 0, 1 );

    RatioPrimXLabel_3 = new QLabel( EVTratioGroupBox, "RatioPrimXLabel_3" );
    RatioPrimXLabel_3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)0, 0, 0, RatioPrimXLabel_3->sizePolicy().hasHeightForWidth() ) );

    EVTratioGroupBoxLayout->addWidget( RatioPrimXLabel_3, 0, 0 );

    evtPrim_3radioButton = new QRadioButton( EVTratioGroupBox, "evtPrim_3radioButton" );

    EVTratioGroupBoxLayout->addMultiCellWidget( evtPrim_3radioButton, 0, 1, 2, 2 );

    evtSek_3radioButton = new QRadioButton( EVTratioGroupBox, "evtSek_3radioButton" );

    EVTratioGroupBoxLayout->addWidget( evtSek_3radioButton, 2, 2 );

    evtPrim_w3radioButton = new QRadioButton( EVTratioGroupBox, "evtPrim_w3radioButton" );

    EVTratioGroupBoxLayout->addMultiCellWidget( evtPrim_w3radioButton, 0, 1, 3, 3 );

    evtSek_w3radioButton = new QRadioButton( EVTratioGroupBox, "evtSek_w3radioButton" );

    EVTratioGroupBoxLayout->addWidget( evtSek_w3radioButton, 2, 3 );

    TabPageLayout_3->addWidget( EVTratioGroupBox, 1, 1 );

    NTRatioGroupBox = new QGroupBox( TabPage_3, "NTRatioGroupBox" );
    NTRatioGroupBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)1, 0, 0, NTRatioGroupBox->sizePolicy().hasHeightForWidth() ) );
    NTRatioGroupBox->setColumnLayout(0, Qt::Vertical );
    NTRatioGroupBox->layout()->setSpacing( 6 );
    NTRatioGroupBox->layout()->setMargin( 11 );
    NTRatioGroupBoxLayout = new QGridLayout( NTRatioGroupBox->layout() );
    NTRatioGroupBoxLayout->setAlignment( Qt::AlignTop );

    RatioSekNLineEdit = new QLineEdit( NTRatioGroupBox, "RatioSekNLineEdit" );
    RatioSekNLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, RatioSekNLineEdit->sizePolicy().hasHeightForWidth() ) );
    RatioSekNLineEdit->setMaximumSize( QSize( 75, 32767 ) );
    RatioSekNLineEdit->setMaxLength( 9 );

    NTRatioGroupBoxLayout->addWidget( RatioSekNLineEdit, 1, 1 );

    RatioPrimNLineEdit = new QLineEdit( NTRatioGroupBox, "RatioPrimNLineEdit" );
    RatioPrimNLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, RatioPrimNLineEdit->sizePolicy().hasHeightForWidth() ) );
    RatioPrimNLineEdit->setMaximumSize( QSize( 75, 32767 ) );
    RatioPrimNLineEdit->setMaxLength( 9 );

    NTRatioGroupBoxLayout->addWidget( RatioPrimNLineEdit, 0, 1 );

    RatioPrimXLabel_2 = new QLabel( NTRatioGroupBox, "RatioPrimXLabel_2" );
    RatioPrimXLabel_2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)0, 0, 0, RatioPrimXLabel_2->sizePolicy().hasHeightForWidth() ) );

    NTRatioGroupBoxLayout->addWidget( RatioPrimXLabel_2, 0, 0 );

    RatioSekXLabel_2 = new QLabel( NTRatioGroupBox, "RatioSekXLabel_2" );
    RatioSekXLabel_2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)0, 0, 0, RatioSekXLabel_2->sizePolicy().hasHeightForWidth() ) );

    NTRatioGroupBoxLayout->addWidget( RatioSekXLabel_2, 1, 0 );

    nPrim_3radioButton = new QRadioButton( NTRatioGroupBox, "nPrim_3radioButton" );

    NTRatioGroupBoxLayout->addWidget( nPrim_3radioButton, 0, 2 );

    nSek_3radioButton = new QRadioButton( NTRatioGroupBox, "nSek_3radioButton" );

    NTRatioGroupBoxLayout->addWidget( nSek_3radioButton, 1, 2 );

    nPrim_w3radioButton = new QRadioButton( NTRatioGroupBox, "nPrim_w3radioButton" );

    NTRatioGroupBoxLayout->addWidget( nPrim_w3radioButton, 0, 3 );

    nSek_w3radioButton = new QRadioButton( NTRatioGroupBox, "nSek_w3radioButton" );

    NTRatioGroupBoxLayout->addWidget( nSek_w3radioButton, 1, 3 );

    TabPageLayout_3->addWidget( NTRatioGroupBox, 0, 0 );
    spacer44 = new QSpacerItem( 201, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    TabPageLayout_3->addItem( spacer44, 1, 2 );
    spacer43 = new QSpacerItem( 201, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    TabPageLayout_3->addItem( spacer43, 0, 2 );

    NTRatioGroupBox2 = new QGroupBox( TabPage_3, "NTRatioGroupBox2" );
    NTRatioGroupBox2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)1, 0, 0, NTRatioGroupBox2->sizePolicy().hasHeightForWidth() ) );
    NTRatioGroupBox2->setColumnLayout(0, Qt::Vertical );
    NTRatioGroupBox2->layout()->setSpacing( 6 );
    NTRatioGroupBox2->layout()->setMargin( 11 );
    NTRatioGroupBox2Layout = new QGridLayout( NTRatioGroupBox2->layout() );
    NTRatioGroupBox2Layout->setAlignment( Qt::AlignTop );

    RatioNSekComboBox = new QComboBox( FALSE, NTRatioGroupBox2, "RatioNSekComboBox" );
    RatioNSekComboBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)0, 0, 0, RatioNSekComboBox->sizePolicy().hasHeightForWidth() ) );

    NTRatioGroupBox2Layout->addWidget( RatioNSekComboBox, 1, 1 );

    RatioSekNLabel = new QLabel( NTRatioGroupBox2, "RatioSekNLabel" );
    RatioSekNLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)0, 0, 0, RatioSekNLabel->sizePolicy().hasHeightForWidth() ) );

    NTRatioGroupBox2Layout->addWidget( RatioSekNLabel, 1, 0 );

    RatioNPrimComboBox = new QComboBox( FALSE, NTRatioGroupBox2, "RatioNPrimComboBox" );
    RatioNPrimComboBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)0, 0, 0, RatioNPrimComboBox->sizePolicy().hasHeightForWidth() ) );

    NTRatioGroupBox2Layout->addWidget( RatioNPrimComboBox, 0, 1 );
    spacer22 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    NTRatioGroupBox2Layout->addItem( spacer22, 1, 2 );
    spacer21 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    NTRatioGroupBox2Layout->addItem( spacer21, 0, 2 );

    RatioPrimNLabel = new QLabel( NTRatioGroupBox2, "RatioPrimNLabel" );
    RatioPrimNLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)0, 0, 0, RatioPrimNLabel->sizePolicy().hasHeightForWidth() ) );

    NTRatioGroupBox2Layout->addWidget( RatioPrimNLabel, 0, 0 );

    TabPageLayout_3->addWidget( NTRatioGroupBox2, 0, 1 );

    XTRatioGroupBox = new QGroupBox( TabPage_3, "XTRatioGroupBox" );
    XTRatioGroupBox->setEnabled( TRUE );
    XTRatioGroupBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)1, 0, 0, XTRatioGroupBox->sizePolicy().hasHeightForWidth() ) );
    XTRatioGroupBox->setColumnLayout(0, Qt::Vertical );
    XTRatioGroupBox->layout()->setSpacing( 6 );
    XTRatioGroupBox->layout()->setMargin( 11 );
    XTRatioGroupBoxLayout = new QGridLayout( XTRatioGroupBox->layout() );
    XTRatioGroupBoxLayout->setAlignment( Qt::AlignTop );

    RatioSekXLineEdit = new QLineEdit( XTRatioGroupBox, "RatioSekXLineEdit" );
    RatioSekXLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, RatioSekXLineEdit->sizePolicy().hasHeightForWidth() ) );
    RatioSekXLineEdit->setMaximumSize( QSize( 75, 32767 ) );
    RatioSekXLineEdit->setMaxLength( 9 );

    XTRatioGroupBoxLayout->addMultiCellWidget( RatioSekXLineEdit, 1, 2, 1, 1 );

    RatioSekXLabel = new QLabel( XTRatioGroupBox, "RatioSekXLabel" );
    RatioSekXLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)0, 0, 0, RatioSekXLabel->sizePolicy().hasHeightForWidth() ) );

    XTRatioGroupBoxLayout->addWidget( RatioSekXLabel, 2, 0 );

    RatioPrimXLineEdit = new QLineEdit( XTRatioGroupBox, "RatioPrimXLineEdit" );
    RatioPrimXLineEdit->setEnabled( TRUE );
    RatioPrimXLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, RatioPrimXLineEdit->sizePolicy().hasHeightForWidth() ) );
    RatioPrimXLineEdit->setMaximumSize( QSize( 75, 32767 ) );
    RatioPrimXLineEdit->setMaxLength( 9 );

    XTRatioGroupBoxLayout->addWidget( RatioPrimXLineEdit, 0, 1 );

    RatioPrimXLabel = new QLabel( XTRatioGroupBox, "RatioPrimXLabel" );
    RatioPrimXLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)0, 0, 0, RatioPrimXLabel->sizePolicy().hasHeightForWidth() ) );

    XTRatioGroupBoxLayout->addWidget( RatioPrimXLabel, 0, 0 );

    xPrim_3radioButton = new QRadioButton( XTRatioGroupBox, "xPrim_3radioButton" );

    XTRatioGroupBoxLayout->addMultiCellWidget( xPrim_3radioButton, 0, 1, 2, 2 );

    xSek_3radioButton = new QRadioButton( XTRatioGroupBox, "xSek_3radioButton" );

    XTRatioGroupBoxLayout->addWidget( xSek_3radioButton, 2, 2 );

    xPrim_w3radioButton = new QRadioButton( XTRatioGroupBox, "xPrim_w3radioButton" );

    XTRatioGroupBoxLayout->addMultiCellWidget( xPrim_w3radioButton, 0, 1, 3, 3 );

    xSek_w3radioButton = new QRadioButton( XTRatioGroupBox, "xSek_w3radioButton" );

    XTRatioGroupBoxLayout->addWidget( xSek_w3radioButton, 2, 3 );

    TabPageLayout_3->addWidget( XTRatioGroupBox, 1, 0 );
    ConfTabWidget->insertTab( TabPage_3, QString::fromLatin1("") );

    TabPage_4 = new QWidget( ConfTabWidget, "TabPage_4" );
    TabPageLayout_4 = new QVBoxLayout( TabPage_4, 11, 6, "TabPageLayout_4"); 

    groupBox7 = new QGroupBox( TabPage_4, "groupBox7" );
    groupBox7->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, groupBox7->sizePolicy().hasHeightForWidth() ) );
    groupBox7->setColumnLayout(0, Qt::Vertical );
    groupBox7->layout()->setSpacing( 0 );
    groupBox7->layout()->setMargin( 11 );
    groupBox7Layout = new QHBoxLayout( groupBox7->layout() );
    groupBox7Layout->setAlignment( Qt::AlignTop );

    layout69 = new QVBoxLayout( 0, 0, 6, "layout69"); 

    MacMULabel = new QLabel( groupBox7, "MacMULabel" );
    MacMULabel->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignLeft ) );
    layout69->addWidget( MacMULabel );

    MacWMLabel = new QLabel( groupBox7, "MacWMLabel" );
    MacWMLabel->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignLeft ) );
    layout69->addWidget( MacWMLabel );
    groupBox7Layout->addLayout( layout69 );

    layout78 = new QVBoxLayout( 0, 0, 6, "layout78"); 

    layout67 = new QHBoxLayout( 0, 0, 6, "layout67"); 

    MacSLineEdit1 = new QLineEdit( groupBox7, "MacSLineEdit1" );
    MacSLineEdit1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, MacSLineEdit1->sizePolicy().hasHeightForWidth() ) );
    MacSLineEdit1->setMaximumSize( QSize( 30, 20 ) );
    MacSLineEdit1->setMaxLength( 2 );
    MacSLineEdit1->setFrame( FALSE );
    MacSLineEdit1->setAlignment( int( QLineEdit::AlignHCenter ) );
    layout67->addWidget( MacSLineEdit1 );

    textLabel4 = new QLabel( groupBox7, "textLabel4" );
    textLabel4->setMaximumSize( QSize( 19, 16 ) );
    textLabel4->setTextFormat( QLabel::AutoText );
    layout67->addWidget( textLabel4 );

    MacSLineEdit2 = new QLineEdit( groupBox7, "MacSLineEdit2" );
    MacSLineEdit2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, MacSLineEdit2->sizePolicy().hasHeightForWidth() ) );
    MacSLineEdit2->setMaximumSize( QSize( 30, 20 ) );
    MacSLineEdit2->setMaxLength( 2 );
    MacSLineEdit2->setFrame( FALSE );
    MacSLineEdit2->setAlignment( int( QLineEdit::AlignHCenter ) );
    layout67->addWidget( MacSLineEdit2 );

    textLabel5 = new QLabel( groupBox7, "textLabel5" );
    textLabel5->setMaximumSize( QSize( 19, 16 ) );
    layout67->addWidget( textLabel5 );

    MacSLineEdit3 = new QLineEdit( groupBox7, "MacSLineEdit3" );
    MacSLineEdit3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, MacSLineEdit3->sizePolicy().hasHeightForWidth() ) );
    MacSLineEdit3->setMaximumSize( QSize( 30, 20 ) );
    MacSLineEdit3->setMaxLength( 2 );
    MacSLineEdit3->setFrame( FALSE );
    MacSLineEdit3->setAlignment( int( QLineEdit::AlignHCenter ) );
    layout67->addWidget( MacSLineEdit3 );

    textLabel6 = new QLabel( groupBox7, "textLabel6" );
    textLabel6->setMaximumSize( QSize( 19, 16 ) );
    layout67->addWidget( textLabel6 );

    MacSLineEdit4 = new QLineEdit( groupBox7, "MacSLineEdit4" );
    MacSLineEdit4->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, MacSLineEdit4->sizePolicy().hasHeightForWidth() ) );
    MacSLineEdit4->setMaximumSize( QSize( 30, 20 ) );
    MacSLineEdit4->setMaxLength( 2 );
    MacSLineEdit4->setFrame( FALSE );
    MacSLineEdit4->setAlignment( int( QLineEdit::AlignHCenter ) );
    layout67->addWidget( MacSLineEdit4 );

    textLabel7 = new QLabel( groupBox7, "textLabel7" );
    textLabel7->setMaximumSize( QSize( 19, 16 ) );
    layout67->addWidget( textLabel7 );

    MacSLineEdit5 = new QLineEdit( groupBox7, "MacSLineEdit5" );
    MacSLineEdit5->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, MacSLineEdit5->sizePolicy().hasHeightForWidth() ) );
    MacSLineEdit5->setMaximumSize( QSize( 30, 20 ) );
    MacSLineEdit5->setMaxLength( 2 );
    MacSLineEdit5->setFrame( FALSE );
    MacSLineEdit5->setAlignment( int( QLineEdit::AlignHCenter ) );
    layout67->addWidget( MacSLineEdit5 );

    textLabel8 = new QLabel( groupBox7, "textLabel8" );
    textLabel8->setMaximumSize( QSize( 19, 16 ) );
    layout67->addWidget( textLabel8 );

    MacSLineEdit6 = new QLineEdit( groupBox7, "MacSLineEdit6" );
    MacSLineEdit6->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, MacSLineEdit6->sizePolicy().hasHeightForWidth() ) );
    MacSLineEdit6->setMaximumSize( QSize( 30, 20 ) );
    MacSLineEdit6->setMaxLength( 2 );
    MacSLineEdit6->setFrame( FALSE );
    MacSLineEdit6->setAlignment( int( QLineEdit::AlignHCenter ) );
    layout67->addWidget( MacSLineEdit6 );
    spacer47 = new QSpacerItem( 16, 20, QSizePolicy::Preferred, QSizePolicy::Minimum );
    layout67->addItem( spacer47 );
    layout78->addLayout( layout67 );

    layout66 = new QHBoxLayout( 0, 0, 6, "layout66"); 

    MacDLineEdit1 = new QLineEdit( groupBox7, "MacDLineEdit1" );
    MacDLineEdit1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, MacDLineEdit1->sizePolicy().hasHeightForWidth() ) );
    MacDLineEdit1->setMaximumSize( QSize( 30, 20 ) );
    MacDLineEdit1->setMaxLength( 2 );
    MacDLineEdit1->setFrame( FALSE );
    MacDLineEdit1->setAlignment( int( QLineEdit::AlignHCenter ) );
    layout66->addWidget( MacDLineEdit1 );

    textLabel4_2 = new QLabel( groupBox7, "textLabel4_2" );
    textLabel4_2->setMaximumSize( QSize( 19, 16 ) );
    textLabel4_2->setTextFormat( QLabel::AutoText );
    layout66->addWidget( textLabel4_2 );

    MacDLineEdit2 = new QLineEdit( groupBox7, "MacDLineEdit2" );
    MacDLineEdit2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, MacDLineEdit2->sizePolicy().hasHeightForWidth() ) );
    MacDLineEdit2->setMaximumSize( QSize( 30, 20 ) );
    MacDLineEdit2->setMaxLength( 2 );
    MacDLineEdit2->setFrame( FALSE );
    MacDLineEdit2->setAlignment( int( QLineEdit::AlignHCenter ) );
    layout66->addWidget( MacDLineEdit2 );

    textLabel5_2 = new QLabel( groupBox7, "textLabel5_2" );
    textLabel5_2->setMaximumSize( QSize( 19, 16 ) );
    layout66->addWidget( textLabel5_2 );

    MacDLineEdit3 = new QLineEdit( groupBox7, "MacDLineEdit3" );
    MacDLineEdit3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, MacDLineEdit3->sizePolicy().hasHeightForWidth() ) );
    MacDLineEdit3->setMaximumSize( QSize( 30, 20 ) );
    MacDLineEdit3->setMaxLength( 2 );
    MacDLineEdit3->setFrame( FALSE );
    MacDLineEdit3->setAlignment( int( QLineEdit::AlignHCenter ) );
    layout66->addWidget( MacDLineEdit3 );

    textLabel6_2 = new QLabel( groupBox7, "textLabel6_2" );
    textLabel6_2->setMaximumSize( QSize( 19, 16 ) );
    layout66->addWidget( textLabel6_2 );

    MacDLineEdit4 = new QLineEdit( groupBox7, "MacDLineEdit4" );
    MacDLineEdit4->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, MacDLineEdit4->sizePolicy().hasHeightForWidth() ) );
    MacDLineEdit4->setMaximumSize( QSize( 30, 20 ) );
    MacDLineEdit4->setMaxLength( 2 );
    MacDLineEdit4->setFrame( FALSE );
    MacDLineEdit4->setAlignment( int( QLineEdit::AlignHCenter ) );
    layout66->addWidget( MacDLineEdit4 );

    textLabel7_2 = new QLabel( groupBox7, "textLabel7_2" );
    textLabel7_2->setMaximumSize( QSize( 19, 16 ) );
    layout66->addWidget( textLabel7_2 );

    MacDLineEdit5 = new QLineEdit( groupBox7, "MacDLineEdit5" );
    MacDLineEdit5->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, MacDLineEdit5->sizePolicy().hasHeightForWidth() ) );
    MacDLineEdit5->setMaximumSize( QSize( 30, 20 ) );
    MacDLineEdit5->setMaxLength( 2 );
    MacDLineEdit5->setFrame( FALSE );
    MacDLineEdit5->setAlignment( int( QLineEdit::AlignHCenter ) );
    layout66->addWidget( MacDLineEdit5 );

    textLabel8_2 = new QLabel( groupBox7, "textLabel8_2" );
    textLabel8_2->setMaximumSize( QSize( 19, 16 ) );
    layout66->addWidget( textLabel8_2 );

    MacDLineEdit6 = new QLineEdit( groupBox7, "MacDLineEdit6" );
    MacDLineEdit6->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, MacDLineEdit6->sizePolicy().hasHeightForWidth() ) );
    MacDLineEdit6->setMaximumSize( QSize( 30, 20 ) );
    MacDLineEdit6->setMaxLength( 2 );
    MacDLineEdit6->setFrame( FALSE );
    MacDLineEdit6->setAlignment( int( QLineEdit::AlignHCenter ) );
    layout66->addWidget( MacDLineEdit6 );
    spacer46 = new QSpacerItem( 16, 20, QSizePolicy::Preferred, QSizePolicy::Minimum );
    layout66->addItem( spacer46 );
    layout78->addLayout( layout66 );
    groupBox7Layout->addLayout( layout78 );
    spacer51 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox7Layout->addItem( spacer51 );
    TabPageLayout_4->addWidget( groupBox7 );

    layout33 = new QHBoxLayout( 0, 0, 6, "layout33"); 

    groupBox11 = new QGroupBox( TabPage_4, "groupBox11" );
    groupBox11->setColumnLayout(0, Qt::Vertical );
    groupBox11->layout()->setSpacing( 6 );
    groupBox11->layout()->setMargin( 11 );
    groupBox11Layout = new QGridLayout( groupBox11->layout() );
    groupBox11Layout->setAlignment( Qt::AlignTop );

    TPIDtextLabel = new QLabel( groupBox11, "TPIDtextLabel" );

    groupBox11Layout->addWidget( TPIDtextLabel, 0, 2 );

    textLabel4_3 = new QLabel( groupBox11, "textLabel4_3" );

    groupBox11Layout->addWidget( textLabel4_3, 1, 2 );

    textLabel2 = new QLabel( groupBox11, "textLabel2" );

    groupBox11Layout->addWidget( textLabel2, 0, 0 );

    textLabel3 = new QLabel( groupBox11, "textLabel3" );

    groupBox11Layout->addWidget( textLabel3, 1, 0 );

    UPrioritylineEdit = new QLineEdit( groupBox11, "UPrioritylineEdit" );
    UPrioritylineEdit->setEnabled( TRUE );
    UPrioritylineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, UPrioritylineEdit->sizePolicy().hasHeightForWidth() ) );
    UPrioritylineEdit->setMaximumSize( QSize( 16, 20 ) );
    UPrioritylineEdit->setAlignment( int( QLineEdit::AlignLeft ) );

    groupBox11Layout->addWidget( UPrioritylineEdit, 0, 1 );

    CFIlineEdit = new QLineEdit( groupBox11, "CFIlineEdit" );
    CFIlineEdit->setEnabled( TRUE );
    CFIlineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, CFIlineEdit->sizePolicy().hasHeightForWidth() ) );
    CFIlineEdit->setMaximumSize( QSize( 16, 20 ) );
    CFIlineEdit->setAlignment( int( QLineEdit::AlignLeft ) );

    groupBox11Layout->addWidget( CFIlineEdit, 1, 1 );

    TPIDlineEdit = new QLineEdit( groupBox11, "TPIDlineEdit" );
    TPIDlineEdit->setEnabled( FALSE );
    TPIDlineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, TPIDlineEdit->sizePolicy().hasHeightForWidth() ) );
    TPIDlineEdit->setMinimumSize( QSize( 40, 20 ) );
    TPIDlineEdit->setMaximumSize( QSize( 40, 20 ) );
    TPIDlineEdit->setAlignment( int( QLineEdit::AlignLeft ) );

    groupBox11Layout->addWidget( TPIDlineEdit, 0, 3 );

    VIDlineEdit = new QLineEdit( groupBox11, "VIDlineEdit" );
    VIDlineEdit->setMinimumSize( QSize( 30, 20 ) );
    VIDlineEdit->setMaximumSize( QSize( 40, 20 ) );

    groupBox11Layout->addWidget( VIDlineEdit, 1, 3 );
    spacer45 = new QSpacerItem( 16, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    groupBox11Layout->addItem( spacer45, 2, 1 );
    layout33->addWidget( groupBox11 );

    groupBox12 = new QGroupBox( TabPage_4, "groupBox12" );
    groupBox12->setColumnLayout(0, Qt::Vertical );
    groupBox12->layout()->setSpacing( 6 );
    groupBox12->layout()->setMargin( 11 );
    groupBox12Layout = new QGridLayout( groupBox12->layout() );
    groupBox12Layout->setAlignment( Qt::AlignTop );

    textLabel5_3 = new QLabel( groupBox12, "textLabel5_3" );

    groupBox12Layout->addWidget( textLabel5_3, 0, 0 );

    textLabel6_3 = new QLabel( groupBox12, "textLabel6_3" );

    groupBox12Layout->addWidget( textLabel6_3, 1, 0 );

    EthTypelineEdit = new QLineEdit( groupBox12, "EthTypelineEdit" );
    EthTypelineEdit->setEnabled( FALSE );
    EthTypelineEdit->setMinimumSize( QSize( 40, 20 ) );
    EthTypelineEdit->setMaximumSize( QSize( 40, 20 ) );
    EthTypelineEdit->setAlignment( int( QLineEdit::AlignLeft ) );

    groupBox12Layout->addWidget( EthTypelineEdit, 0, 1 );

    APPIDlineEdit = new QLineEdit( groupBox12, "APPIDlineEdit" );
    APPIDlineEdit->setMinimumSize( QSize( 40, 20 ) );
    APPIDlineEdit->setMaximumSize( QSize( 40, 20 ) );
    APPIDlineEdit->setAlignment( int( QLineEdit::AlignLeft ) );

    groupBox12Layout->addWidget( APPIDlineEdit, 1, 1 );
    spacer46_2 = new QSpacerItem( 16, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    groupBox12Layout->addItem( spacer46_2, 2, 0 );
    layout33->addWidget( groupBox12 );

    groupBox9 = new QGroupBox( TabPage_4, "groupBox9" );
    groupBox9->setColumnLayout(0, Qt::Vertical );
    groupBox9->layout()->setSpacing( 6 );
    groupBox9->layout()->setMargin( 11 );
    groupBox9Layout = new QGridLayout( groupBox9->layout() );
    groupBox9Layout->setAlignment( Qt::AlignTop );

    FirstASDUTextLabel = new QLabel( groupBox9, "FirstASDUTextLabel" );

    groupBox9Layout->addWidget( FirstASDUTextLabel, 0, 0 );

    DatasetTextLabel = new QLabel( groupBox9, "DatasetTextLabel" );

    groupBox9Layout->addWidget( DatasetTextLabel, 2, 0 );

    LastASDUtextLabel = new QLabel( groupBox9, "LastASDUtextLabel" );

    groupBox9Layout->addWidget( LastASDUtextLabel, 1, 0 );

    LastASDUlineEdit = new QLineEdit( groupBox9, "LastASDUlineEdit" );
    LastASDUlineEdit->setMaximumSize( QSize( 16, 20 ) );

    groupBox9Layout->addWidget( LastASDUlineEdit, 1, 1 );

    SetlineEdit = new QLineEdit( groupBox9, "SetlineEdit" );
    SetlineEdit->setMaximumSize( QSize( 16, 20 ) );

    groupBox9Layout->addWidget( SetlineEdit, 2, 1 );

    FirstASDUlineEdit = new QLineEdit( groupBox9, "FirstASDUlineEdit" );
    FirstASDUlineEdit->setMaximumSize( QSize( 16, 20 ) );

    groupBox9Layout->addWidget( FirstASDUlineEdit, 0, 1 );
    layout33->addWidget( groupBox9 );
    TabPageLayout_4->addLayout( layout33 );
    ConfTabWidget->insertTab( TabPage_4, QString::fromLatin1("") );

    LogPage = new QWidget( ConfTabWidget, "LogPage" );
    LogPageLayout = new QVBoxLayout( LogPage, 11, 6, "LogPageLayout"); 

    layout24 = new QHBoxLayout( 0, 0, 6, "layout24"); 

    LogfileSizeGroupBox = new QGroupBox( LogPage, "LogfileSizeGroupBox" );
    LogfileSizeGroupBox->setColumnLayout(0, Qt::Vertical );
    LogfileSizeGroupBox->layout()->setSpacing( 6 );
    LogfileSizeGroupBox->layout()->setMargin( 11 );
    LogfileSizeGroupBoxLayout = new QHBoxLayout( LogfileSizeGroupBox->layout() );
    LogfileSizeGroupBoxLayout->setAlignment( Qt::AlignTop );

    LogfileSizeTextLabel = new QLabel( LogfileSizeGroupBox, "LogfileSizeTextLabel" );
    LogfileSizeGroupBoxLayout->addWidget( LogfileSizeTextLabel );

    LogfileSizeSpinbox = new QSpinBox( LogfileSizeGroupBox, "LogfileSizeSpinbox" );
    LogfileSizeSpinbox->setMaxValue( 64 );
    LogfileSizeSpinbox->setMinValue( 1 );
    LogfileSizeGroupBoxLayout->addWidget( LogfileSizeSpinbox );

    LogfileSizeTextLabel2 = new QLabel( LogfileSizeGroupBox, "LogfileSizeTextLabel2" );
    QFont LogfileSizeTextLabel2_font(  LogfileSizeTextLabel2->font() );
    LogfileSizeTextLabel2_font.setPointSize( 12 );
    LogfileSizeTextLabel2->setFont( LogfileSizeTextLabel2_font ); 
    LogfileSizeGroupBoxLayout->addWidget( LogfileSizeTextLabel2 );
    layout24->addWidget( LogfileSizeGroupBox );
    spacer49 = new QSpacerItem( 239, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout24->addItem( spacer49 );
    LogPageLayout->addLayout( layout24 );
    spacer50 = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
    LogPageLayout->addItem( spacer50 );
    ConfTabWidget->insertTab( LogPage, QString::fromLatin1("") );
    ConfDialogBaseLayout->addWidget( ConfTabWidget );

    layout19 = new QHBoxLayout( 0, 0, 6, "layout19"); 
    Horizontal_Spacing2 = new QSpacerItem( 254, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout19->addItem( Horizontal_Spacing2 );

    buttonOk = new QPushButton( this, "buttonOk" );
    buttonOk->setAutoDefault( TRUE );
    buttonOk->setDefault( TRUE );
    layout19->addWidget( buttonOk );

    buttonCancel = new QPushButton( this, "buttonCancel" );
    buttonCancel->setAutoDefault( TRUE );
    layout19->addWidget( buttonCancel );
    ConfDialogBaseLayout->addLayout( layout19 );
    languageChange();
    resize( QSize(489, 290).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( nSek_w3radioButton, SIGNAL( clicked() ), this, SLOT( nSek_w3radioButtonChecked() ) );
    connect( nSek_3radioButton, SIGNAL( clicked() ), this, SLOT( nSek_3radioButtonChecked() ) );
    connect( nPrim_w3radioButton, SIGNAL( clicked() ), this, SLOT( nPrim_w3radioButtonChecked() ) );
    connect( nPrim_3radioButton, SIGNAL( clicked() ), this, SLOT( nPrim_3radioButtonChecked() ) );
    connect( Mode3RadioButton, SIGNAL( clicked() ), this, SLOT( ApplyDataSlot() ) );
    connect( Mode2RadioButton, SIGNAL( clicked() ), this, SLOT( ApplyDataSlot() ) );
    connect( Mode0RadioButton, SIGNAL( clicked() ), this, SLOT( ApplyDataSlot() ) );
    connect( CmpCorrCheckBox, SIGNAL( clicked() ), this, SLOT( ApplyDataSlot() ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( S256RadioButton, SIGNAL( clicked() ), this, SLOT( S256RadioButtonChecked() ) );
    connect( S80RadioButton, SIGNAL( clicked() ), this, SLOT( S80RadioButtonChecked() ) );
    connect( evtPrim_3radioButton, SIGNAL( clicked() ), this, SLOT( evtPrim_3radioButtonChecked() ) );
    connect( evtPrim_w3radioButton, SIGNAL( clicked() ), this, SLOT( evtPrim_w3radioButtonChecked() ) );
    connect( evtSek_3radioButton, SIGNAL( clicked() ), this, SLOT( evtSek_3radioButtonChecked() ) );
    connect( evtSek_w3radioButton, SIGNAL( clicked() ), this, SLOT( evtSek_w3radioButtonChecked() ) );
    connect( xPrim_3radioButton, SIGNAL( clicked() ), this, SLOT( xPrim_3radioButtonChecked() ) );
    connect( xPrim_w3radioButton, SIGNAL( clicked() ), this, SLOT( xPrim_w3radioButtonChecked() ) );
    connect( xSek_3radioButton, SIGNAL( clicked() ), this, SLOT( xSek_3radioButtonChecked() ) );
    connect( xSek_w3radioButton, SIGNAL( clicked() ), this, SLOT( xSek_w3radioButtonChecked() ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( abortSlot() ) );

    // tab order
    setTabOrder( ConfTabWidget, buttonOk );
    setTabOrder( buttonOk, buttonCancel );
    setTabOrder( buttonCancel, Mode0RadioButton );
    setTabOrder( Mode0RadioButton, Mode2RadioButton );
    setTabOrder( Mode2RadioButton, Mode3RadioButton );

    // buddies
    CmpKorrLabel2->setBuddy( CmpKorrLineEdit2 );
    CmpKorrLabel1->setBuddy( CmpKorrLineEdit1 );
    TIntegrationLabel->setBuddy( TIntegrationSpinBox );
    CmpIntervallLabel->setBuddy( CmpIntervallSpinBox );
    TSyncLabel->setBuddy( TSyncSpinBox );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
ConfDialogBase::~ConfDialogBase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ConfDialogBase::languageChange()
{
    setCaption( tr( "Einstellungen" ) );
    ModeButtonGroup->setTitle( tr( "Messung" ) );
    Mode0RadioButton->setText( tr( "Un/Ux &abs." ) );
    Mode0RadioButton->setAccel( QKeySequence( tr( "Alt+A" ) ) );
    Mode2RadioButton->setText( tr( "Un/&EVT" ) );
    Mode2RadioButton->setAccel( QKeySequence( tr( "Alt+E" ) ) );
    Mode3RadioButton->setText( tr( "Un/n&Convent" ) );
    Mode3RadioButton->setAccel( QKeySequence( tr( "Alt+C" ) ) );
    ConfTabWidget->changeTab( Widget8, tr( "Modus" ) );
    CmpKorrGroupBox->setTitle( trUtf8( "\x50\x68\x61\x73\x65\x6e\x66\x65\x68\x6c\x65\x72\x20\x50\x72\xc3\xbc\x66\x6c\x69\x6e\x67" ) );
    CmpKorrLabel2->setText( tr( "&Totzeit[ms]:" ) );
    CmpKorrLabel1->setText( trUtf8( "\x26\x50\x68\x61\x73\x65\x6e\x6c\x61\x67\x65\x5b\xc2\xb0\x5d\x3a" ) );
    CmpKorrLineEdit1->setText( QString::null );
    CmpCorrGroupBox->setTitle( tr( "Korrektur Normwandler" ) );
    CmpCorrCheckBox->setText( tr( "A&ktiv" ) );
    CmpCorrCheckBox->setAccel( QKeySequence( tr( "Alt+K" ) ) );
    ConfTabWidget->changeTab( Widget9, tr( "Berechnung" ) );
    TIntegrationGroupBox->setTitle( tr( "Integrationszeit" ) );
    TIntegrationLabel->setText( tr( "&Zeit[sec]:" ) );
    FreqButtonGroup->setTitle( tr( "Signalfrequenz" ) );
    F16RadioButton->setText( tr( "16.66 Hz" ) );
    F50RadioButton->setText( tr( "50 Hz" ) );
    F60RadioButton->setText( tr( "60 Hz" ) );
    SPeriodeButtonGroup->setTitle( tr( "Abtastungen" ) );
    S80RadioButton->setText( tr( "  80 / Periode" ) );
    S256RadioButton->setText( tr( "256 / Periode" ) );
    CmpIntervallGroupBox->setTitle( tr( "Messintervall" ) );
    CmpIntervallLabel->setText( tr( "&Perioden:" ) );
    ConfTabWidget->changeTab( TabPage, tr( "Messung" ) );
    SyncButtonGroup->setTitle( tr( "PPS-Signal" ) );
    SyncRadioButton1->setText( tr( "&Extern" ) );
    SyncRadioButton1->setAccel( QKeySequence( tr( "Alt+E" ) ) );
    SyncRadioButton2->setText( tr( "&Intern" ) );
    SyncRadioButton2->setAccel( QKeySequence( tr( "Alt+I" ) ) );
    TSyncLabel->setText( tr( "&TSync[ms]:" ) );
    groupBox->setTitle( QString::null );
    SSynccheckBox->setText( tr( "Strenge EN61850 Synchronisation" ) );
    ConfTabWidget->changeTab( TabPage_2, tr( "Synchronisation" ) );
    EVTratioGroupBox->setTitle( tr( "EVT" ) );
    RatioSekXLabel_3->setText( tr( "Sek:" ) );
    RatioPrimXLabel_3->setText( tr( "Prim:" ) );
    evtPrim_3radioButton->setText( tr( "/3" ) );
    evtSek_3radioButton->setText( tr( "/3" ) );
    evtPrim_w3radioButton->setText( tr( "/sqrt(3)" ) );
    evtSek_w3radioButton->setText( tr( "/sqrt(3)" ) );
    NTRatioGroupBox->setTitle( tr( "N" ) );
    RatioPrimXLabel_2->setText( tr( "Prim:" ) );
    RatioSekXLabel_2->setText( tr( "Sek:" ) );
    nPrim_3radioButton->setText( tr( "/3" ) );
    nSek_3radioButton->setText( tr( "/3" ) );
    nPrim_w3radioButton->setText( tr( "/sqrt(3)" ) );
    nSek_w3radioButton->setText( tr( "/sqrt(3)" ) );
    NTRatioGroupBox2->setTitle( tr( "N" ) );
    RatioNSekComboBox->setCurrentItem( 0 );
    RatioSekNLabel->setText( tr( "Sek:" ) );
    RatioPrimNLabel->setText( tr( "Prim:" ) );
    XTRatioGroupBox->setTitle( tr( "X" ) );
    RatioSekXLabel->setText( tr( "Sek:" ) );
    RatioPrimXLabel->setText( tr( "Prim:" ) );
    xPrim_3radioButton->setText( tr( "/3" ) );
    xSek_3radioButton->setText( tr( "/3" ) );
    xPrim_w3radioButton->setText( tr( "/sqrt(3)" ) );
    xSek_w3radioButton->setText( tr( "/sqrt(3)" ) );
    ConfTabWidget->changeTab( TabPage_3, trUtf8( "\x54\x65\x69\x6c\x65\x72\x76\x65\x72\x68\xc3\xa4\x6c\x74\x6e\x69\x73\x73\x65" ) );
    groupBox7->setTitle( tr( "MAC adresses 61850-9-2" ) );
    MacMULabel->setText( tr( "Merging unit:" ) );
    MacWMLabel->setText( tr( "WM3000:" ) );
    MacSLineEdit1->setText( tr( "FF" ) );
    textLabel4->setText( tr( "-" ) );
    MacSLineEdit2->setText( tr( "FF" ) );
    textLabel5->setText( tr( "-" ) );
    MacSLineEdit3->setText( tr( "FF" ) );
    textLabel6->setText( tr( "-" ) );
    MacSLineEdit4->setText( tr( "FF" ) );
    textLabel7->setText( tr( "-" ) );
    MacSLineEdit5->setText( tr( "FF" ) );
    textLabel8->setText( tr( "-" ) );
    MacSLineEdit6->setText( tr( "FF" ) );
    MacDLineEdit1->setText( tr( "FF" ) );
    textLabel4_2->setText( tr( "-" ) );
    MacDLineEdit2->setText( tr( "FF" ) );
    textLabel5_2->setText( tr( "-" ) );
    MacDLineEdit3->setText( tr( "FF" ) );
    textLabel6_2->setText( tr( "-" ) );
    MacDLineEdit4->setText( tr( "FF" ) );
    textLabel7_2->setText( tr( "-" ) );
    MacDLineEdit5->setText( tr( "FF" ) );
    textLabel8_2->setText( tr( "-" ) );
    MacDLineEdit6->setText( tr( "FF" ) );
    groupBox11->setTitle( tr( "Priority tagged" ) );
    TPIDtextLabel->setText( tr( "TPID:" ) );
    textLabel4_3->setText( tr( "VID:" ) );
    textLabel2->setText( tr( "User priority:" ) );
    textLabel3->setText( tr( "CFI:" ) );
    UPrioritylineEdit->setText( tr( "0" ) );
    CFIlineEdit->setText( tr( "0" ) );
    TPIDlineEdit->setText( tr( "0000" ) );
    VIDlineEdit->setText( tr( "0" ) );
    groupBox12->setTitle( tr( "Ethertype header" ) );
    textLabel5_3->setText( tr( "Ethertype:" ) );
    textLabel6_3->setText( tr( "APPID:" ) );
    EthTypelineEdit->setText( tr( "0000" ) );
    APPIDlineEdit->setText( tr( "0000" ) );
    groupBox9->setTitle( tr( "Data set" ) );
    FirstASDUTextLabel->setText( tr( "First ASDU:" ) );
    DatasetTextLabel->setText( tr( "Set:" ) );
    LastASDUtextLabel->setText( tr( "Last ASDU:" ) );
    LastASDUlineEdit->setText( tr( "1" ) );
    SetlineEdit->setText( tr( "1" ) );
    FirstASDUlineEdit->setText( tr( "1" ) );
    ConfTabWidget->changeTab( TabPage_4, tr( "nConvent" ) );
    LogfileSizeGroupBox->setTitle( tr( "Logfile" ) );
    LogfileSizeTextLabel->setText( trUtf8( "\x47\x72\xc3\xb6\x73\x73\x65" ) );
    LogfileSizeTextLabel2->setText( tr( "Kb" ) );
    ConfTabWidget->changeTab( LogPage, tr( "Log" ) );
    buttonOk->setText( tr( "&OK" ) );
    buttonOk->setAccel( QKeySequence( QString::null ) );
    buttonCancel->setText( tr( "&Abbruch" ) );
    buttonCancel->setAccel( QKeySequence( tr( "Alt+A" ) ) );
}

