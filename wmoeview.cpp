/****************************************************************************
** Form implementation generated from reading ui file 'wmoeview.ui'
**
** Created: Mi Aug 24 13:39:10 2005
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.2   edited Nov 24 13:47 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "wmoeview.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "wmglobal.h"
#include "wmoeview.ui.h"
/*
 *  Constructs a WMOeViewBase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
WMOeViewBase::WMOeViewBase( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "WMOeViewBase" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, sizePolicy().hasHeightForWidth() ) );
    setMinimumSize( QSize( 190, 140 ) );
    WMOeViewBaseLayout = new QHBoxLayout( this, 11, 6, "WMOeViewBaseLayout"); 

    layout9 = new QHBoxLayout( 0, 0, 6, "layout9"); 

    layout7 = new QVBoxLayout( 0, 0, 6, "layout7"); 

    PrimLabel = new QLabel( this, "PrimLabel" );
    PrimLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, PrimLabel->sizePolicy().hasHeightForWidth() ) );
    PrimLabel->setFrameShape( QLabel::Box );
    layout7->addWidget( PrimLabel );

    SecLabel = new QLabel( this, "SecLabel" );
    SecLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, SecLabel->sizePolicy().hasHeightForWidth() ) );
    SecLabel->setFrameShape( QLabel::Box );
    SecLabel->setFrameShadow( QLabel::Plain );
    layout7->addWidget( SecLabel );

    LoadpointLabel = new QLabel( this, "LoadpointLabel" );
    LoadpointLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, LoadpointLabel->sizePolicy().hasHeightForWidth() ) );
    LoadpointLabel->setFrameShape( QLabel::Box );
    layout7->addWidget( LoadpointLabel );

    GainLabel = new QLabel( this, "GainLabel" );
    GainLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, GainLabel->sizePolicy().hasHeightForWidth() ) );
    GainLabel->setFrameShape( QLabel::Box );
    layout7->addWidget( GainLabel );

    PhaseLabel = new QLabel( this, "PhaseLabel" );
    PhaseLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, PhaseLabel->sizePolicy().hasHeightForWidth() ) );
    PhaseLabel->setFrameShape( QLabel::Box );
    layout7->addWidget( PhaseLabel );
    layout9->addLayout( layout7 );
    spacer4 = new QSpacerItem( 60, 20, QSizePolicy::Minimum, QSizePolicy::Minimum );
    layout9->addItem( spacer4 );

    layout8 = new QVBoxLayout( 0, 0, 6, "layout8"); 

    PrimValue = new QLabel( this, "PrimValue" );
    PrimValue->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, PrimValue->sizePolicy().hasHeightForWidth() ) );
    PrimValue->setFrameShape( QLabel::Box );
    layout8->addWidget( PrimValue );

    SekValue = new QLabel( this, "SekValue" );
    SekValue->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, SekValue->sizePolicy().hasHeightForWidth() ) );
    SekValue->setFrameShape( QLabel::Box );
    layout8->addWidget( SekValue );

    LoadpointValue = new QLabel( this, "LoadpointValue" );
    LoadpointValue->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, LoadpointValue->sizePolicy().hasHeightForWidth() ) );
    LoadpointValue->setFrameShape( QLabel::Box );
    layout8->addWidget( LoadpointValue );

    GainValue = new QLabel( this, "GainValue" );
    GainValue->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, GainValue->sizePolicy().hasHeightForWidth() ) );
    GainValue->setFrameShape( QLabel::Box );
    layout8->addWidget( GainValue );

    PhaseValue = new QLabel( this, "PhaseValue" );
    PhaseValue->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, PhaseValue->sizePolicy().hasHeightForWidth() ) );
    PhaseValue->setFrameShape( QLabel::Box );
    layout8->addWidget( PhaseValue );
    layout9->addLayout( layout8 );
    WMOeViewBaseLayout->addLayout( layout9 );
    languageChange();
    resize( QSize(200, 144).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
}

/*
 *  Destroys the object and frees any allocated resources
 */
WMOeViewBase::~WMOeViewBase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void WMOeViewBase::languageChange()
{
    setCaption( tr( "Eigenfehleranzeige" ) );
    PrimLabel->setText( trUtf8( "\x50\x72\x69\x6d\xc3\xa4\x72\x3a" ) );
    SecLabel->setText( trUtf8( "\x53\x65\x6b\x75\x6e\x64\xc3\xa4\x72\x3a" ) );
    LoadpointLabel->setText( tr( "Lastpunkt:" ) );
    GainLabel->setText( tr( "Betrag:" ) );
    PhaseLabel->setText( tr( "Winkel:" ) );
    PrimValue->setText( tr( "--------" ) );
    SekValue->setText( tr( "--------" ) );
    LoadpointValue->setText( tr( "--------" ) );
    GainValue->setText( tr( "--------" ) );
    PhaseValue->setText( tr( "--------" ) );
}

