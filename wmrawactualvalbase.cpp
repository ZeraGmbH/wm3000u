/****************************************************************************
** Form implementation generated from reading ui file 'wmrawactualvalbase.ui'
**
** Created: Di Aug 19 11:14:32 2014
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "wmrawactualvalbase.h"

#include <qvariant.h>
#include <math.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qpopupmenu.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "wmglobal.h"
#include "complex.h"
#include "tools.h"
#include "widgeom.h"
#include "wmrawactualvalbase.ui.h"
/*
 *  Constructs a WMRawActualValBase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
WMRawActualValBase::WMRawActualValBase( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "WMRawActualValBase" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, sizePolicy().hasHeightForWidth() ) );
    setMinimumSize( QSize( 0, 0 ) );
    setMouseTracking( FALSE );
    WMRawActualValBaseLayout = new QGridLayout( this, 1, 1, 11, 6, "WMRawActualValBaseLayout"); 
    WMRawActualValBaseLayout->setResizeMode( QLayout::Fixed );
    spacer20 = new QSpacerItem( 33, 20, QSizePolicy::Minimum, QSizePolicy::Minimum );
    WMRawActualValBaseLayout->addItem( spacer20, 1, 1 );

    XnLabel = new QLabel( this, "XnLabel" );
    XnLabel->setFrameShape( QLabel::NoFrame );

    WMRawActualValBaseLayout->addWidget( XnLabel, 0, 0 );

    XxLabel = new QLabel( this, "XxLabel" );
    XxLabel->setFrameShape( QLabel::NoFrame );

    WMRawActualValBaseLayout->addWidget( XxLabel, 1, 0 );

    FreqLabel = new QLabel( this, "FreqLabel" );
    FreqLabel->setFrameShape( QLabel::NoFrame );

    WMRawActualValBaseLayout->addWidget( FreqLabel, 2, 0 );

    XnAmplDisp = new QLabel( this, "XnAmplDisp" );
    XnAmplDisp->setFrameShape( QLabel::NoFrame );

    WMRawActualValBaseLayout->addWidget( XnAmplDisp, 0, 2 );

    XxAmplDisp = new QLabel( this, "XxAmplDisp" );
    XxAmplDisp->setFrameShape( QLabel::NoFrame );
    XxAmplDisp->setAlignment( int( QLabel::AlignVCenter ) );

    WMRawActualValBaseLayout->addWidget( XxAmplDisp, 1, 2 );

    FreqDisp = new QLabel( this, "FreqDisp" );
    FreqDisp->setFrameShape( QLabel::NoFrame );

    WMRawActualValBaseLayout->addWidget( FreqDisp, 2, 2 );

    XnPhaseDisp = new QLabel( this, "XnPhaseDisp" );
    XnPhaseDisp->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, XnPhaseDisp->sizePolicy().hasHeightForWidth() ) );
    XnPhaseDisp->setFrameShape( QLabel::NoFrame );

    WMRawActualValBaseLayout->addWidget( XnPhaseDisp, 0, 3 );

    XxPhaseDisp = new QLabel( this, "XxPhaseDisp" );
    XxPhaseDisp->setFrameShape( QLabel::NoFrame );

    WMRawActualValBaseLayout->addWidget( XxPhaseDisp, 1, 3 );
    spacer17 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Minimum );
    WMRawActualValBaseLayout->addItem( spacer17, 2, 3 );
    languageChange();
    resize( QSize(180, 90).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
WMRawActualValBase::~WMRawActualValBase()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void WMRawActualValBase::languageChange()
{
    setCaption( tr( "Vektoren" ) );
    XnLabel->setText( tr( "Un:" ) );
    XxLabel->setText( tr( "Ux:" ) );
    FreqLabel->setText( tr( "F:" ) );
    XnAmplDisp->setText( tr( "---------" ) );
    XxAmplDisp->setText( tr( "---------" ) );
    FreqDisp->setText( tr( "--------" ) );
    XnPhaseDisp->setText( tr( "---------" ) );
    XxPhaseDisp->setText( tr( "--------- " ) );
}

