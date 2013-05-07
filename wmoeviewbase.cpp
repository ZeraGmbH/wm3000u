/****************************************************************************
** Form implementation generated from reading ui file 'wmoeviewbase.ui'
**
** Created: Mo Mai 6 13:09:41 2013
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "wmoeviewbase.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "wmglobal.h"
#include "ownerror.h"
#include "wmoeviewbase.ui.h"
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
    setBackgroundOrigin( QDialog::WidgetOrigin );
    QFont f( font() );
    setFont( f ); 
    setModal( FALSE );
    WMOeViewBaseLayout = new QHBoxLayout( this, 11, 6, "WMOeViewBaseLayout"); 
    WMOeViewBaseLayout->setResizeMode( QLayout::Fixed );

    layout3 = new QVBoxLayout( 0, 0, 6, "layout3"); 

    WandlerLabel = new QLabel( this, "WandlerLabel" );
    WandlerLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, WandlerLabel->sizePolicy().hasHeightForWidth() ) );
    WandlerLabel->setFrameShape( QLabel::NoFrame );
    layout3->addWidget( WandlerLabel );

    PrimLabel = new QLabel( this, "PrimLabel" );
    PrimLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, PrimLabel->sizePolicy().hasHeightForWidth() ) );
    PrimLabel->setFrameShape( QLabel::NoFrame );
    layout3->addWidget( PrimLabel );

    SecLabel = new QLabel( this, "SecLabel" );
    SecLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, SecLabel->sizePolicy().hasHeightForWidth() ) );
    SecLabel->setFrameShape( QLabel::NoFrame );
    SecLabel->setFrameShadow( QLabel::Plain );
    layout3->addWidget( SecLabel );

    LoadpointLabel = new QLabel( this, "LoadpointLabel" );
    LoadpointLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, LoadpointLabel->sizePolicy().hasHeightForWidth() ) );
    LoadpointLabel->setFrameShape( QLabel::NoFrame );
    layout3->addWidget( LoadpointLabel );

    AmplLabel = new QLabel( this, "AmplLabel" );
    AmplLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, AmplLabel->sizePolicy().hasHeightForWidth() ) );
    AmplLabel->setFrameShape( QLabel::NoFrame );
    layout3->addWidget( AmplLabel );

    PhaseLabel = new QLabel( this, "PhaseLabel" );
    PhaseLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, PhaseLabel->sizePolicy().hasHeightForWidth() ) );
    PhaseLabel->setFrameShape( QLabel::NoFrame );
    layout3->addWidget( PhaseLabel );
    WMOeViewBaseLayout->addLayout( layout3 );
    spacer4 = new QSpacerItem( 10, 20, QSizePolicy::Minimum, QSizePolicy::Minimum );
    WMOeViewBaseLayout->addItem( spacer4 );

    layout4 = new QVBoxLayout( 0, 0, 6, "layout4"); 

    WandlerName = new QLabel( this, "WandlerName" );
    WandlerName->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, WandlerName->sizePolicy().hasHeightForWidth() ) );
    WandlerName->setFrameShape( QLabel::NoFrame );
    layout4->addWidget( WandlerName );

    PrimDisp = new QLabel( this, "PrimDisp" );
    PrimDisp->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, PrimDisp->sizePolicy().hasHeightForWidth() ) );
    PrimDisp->setFrameShape( QLabel::NoFrame );
    layout4->addWidget( PrimDisp );

    SekDisp = new QLabel( this, "SekDisp" );
    SekDisp->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, SekDisp->sizePolicy().hasHeightForWidth() ) );
    SekDisp->setFrameShape( QLabel::NoFrame );
    layout4->addWidget( SekDisp );

    LoadpointDisp = new QLabel( this, "LoadpointDisp" );
    LoadpointDisp->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, LoadpointDisp->sizePolicy().hasHeightForWidth() ) );
    LoadpointDisp->setFrameShape( QLabel::NoFrame );
    layout4->addWidget( LoadpointDisp );

    AmplDisp = new QLabel( this, "AmplDisp" );
    AmplDisp->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, AmplDisp->sizePolicy().hasHeightForWidth() ) );
    AmplDisp->setFrameShape( QLabel::NoFrame );
    layout4->addWidget( AmplDisp );

    PhaseDisp = new QLabel( this, "PhaseDisp" );
    PhaseDisp->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, PhaseDisp->sizePolicy().hasHeightForWidth() ) );
    PhaseDisp->setFrameShape( QLabel::NoFrame );
    layout4->addWidget( PhaseDisp );
    WMOeViewBaseLayout->addLayout( layout4 );
    languageChange();
    resize( QSize(190, 150).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
WMOeViewBase::~WMOeViewBase()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void WMOeViewBase::languageChange()
{
    setCaption( tr( "Eigenfehler" ) );
    WandlerLabel->setText( tr( "Wandler:" ) );
    PrimLabel->setText( trUtf8( "\x50\x72\x69\x6d\xc3\xa4\x72\x3a" ) );
    SecLabel->setText( trUtf8( "\x53\x65\x6b\x75\x6e\x64\xc3\xa4\x72\x3a" ) );
    LoadpointLabel->setText( tr( "Lastpunkt:" ) );
    AmplLabel->setText( tr( "Betrag:" ) );
    PhaseLabel->setText( tr( "Winkel:" ) );
    WandlerName->setText( tr( "--------" ) );
    PrimDisp->setText( tr( "--------" ) );
    SekDisp->setText( tr( "--------" ) );
    LoadpointDisp->setText( tr( "--------" ) );
    AmplDisp->setText( tr( "--------" ) );
    PhaseDisp->setText( tr( "--------" ) );
}

