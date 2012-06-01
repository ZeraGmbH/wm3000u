/****************************************************************************
** Form implementation generated from reading ui file 'wmmeasvaluesbase.ui'
**
** Created: Mi Feb 15 08:24:43 2012
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "wmmeasvaluesbase.h"

#include <qvariant.h>
#include <qobject.h>
#include <qlabel.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qpopupmenu.h>
#include </home/peter/C++/WM3000I/bigletter.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "eparameter.h"
#include "tools.h"
#include "complex.h"
#include "wmglobal.h"
#include "formatinfo.h"
#include "wmmeasconfigbase.h"
#include "wmmeasvaluesbase.ui.h"
/*
 *  Constructs a WMMeasValuesBase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
WMMeasValuesBase::WMMeasValuesBase( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "WMMeasValuesBase" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, sizePolicy().hasHeightForWidth() ) );
    QFont f( font() );
    setFont( f ); 
    setFocusPolicy( QDialog::NoFocus );
    setSizeGripEnabled( TRUE );
    setModal( FALSE );
    WMMeasValuesBaseLayout = new QHBoxLayout( this, 11, 6, "WMMeasValuesBaseLayout"); 

    layout4 = new QVBoxLayout( 0, 0, 6, "layout4"); 

    cBigLetter3 = new CBigLetter( this, "cBigLetter3" );
    cBigLetter3->setProperty( "m_nFrame", 0 );
    cBigLetter3->setProperty( "m_nStartSize", 15 );
    layout4->addWidget( cBigLetter3 );

    cBigLetter1 = new CBigLetter( this, "cBigLetter1" );
    cBigLetter1->setEnabled( TRUE );
    cBigLetter1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, cBigLetter1->sizePolicy().hasHeightForWidth() ) );
    cBigLetter1->setMinimumSize( QSize( 16, 0 ) );
    QFont cBigLetter1_font(  cBigLetter1->font() );
    cBigLetter1->setFont( cBigLetter1_font ); 
    cBigLetter1->setProperty( "m_nAlign", 0 );
    cBigLetter1->setProperty( "m_nFrame", 0 );
    cBigLetter1->setProperty( "m_nStartSize", 15 );
    layout4->addWidget( cBigLetter1 );

    cBigLetter2 = new CBigLetter( this, "cBigLetter2" );
    cBigLetter2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, cBigLetter2->sizePolicy().hasHeightForWidth() ) );
    QFont cBigLetter2_font(  cBigLetter2->font() );
    cBigLetter2->setFont( cBigLetter2_font ); 
    cBigLetter2->setProperty( "m_nAlign", 0 );
    cBigLetter2->setProperty( "m_nFrame", 0 );
    cBigLetter2->setProperty( "m_nStartSize", 15 );
    layout4->addWidget( cBigLetter2 );

    mBigAngleName = new CBigLetter( this, "mBigAngleName" );
    mBigAngleName->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, mBigAngleName->sizePolicy().hasHeightForWidth() ) );
    QFont mBigAngleName_font(  mBigAngleName->font() );
    mBigAngleName->setFont( mBigAngleName_font ); 
    mBigAngleName->setProperty( "m_nAlign", 0 );
    mBigAngleName->setProperty( "m_nFrame", 0 );
    mBigAngleName->setProperty( "m_nStartSize", 15 );
    layout4->addWidget( mBigAngleName );
    WMMeasValuesBaseLayout->addLayout( layout4 );

    layout5 = new QVBoxLayout( 0, 0, 6, "layout5"); 

    mBigLoadpointX = new CBigLetter( this, "mBigLoadpointX" );
    mBigLoadpointX->setProperty( "m_nFrame", 0 );
    mBigLoadpointX->setProperty( "m_nStartSize", 15 );
    layout5->addWidget( mBigLoadpointX );

    mBigLoadpointN = new CBigLetter( this, "mBigLoadpointN" );
    mBigLoadpointN->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, mBigLoadpointN->sizePolicy().hasHeightForWidth() ) );
    QFont mBigLoadpointN_font(  mBigLoadpointN->font() );
    mBigLoadpointN->setFont( mBigLoadpointN_font ); 
    mBigLoadpointN->setProperty( "m_nAlign", 0 );
    mBigLoadpointN->setProperty( "m_nFrame", 0 );
    mBigLoadpointN->setProperty( "m_nStartSize", 15 );
    layout5->addWidget( mBigLoadpointN );

    mBigAmplError = new CBigLetter( this, "mBigAmplError" );
    mBigAmplError->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, mBigAmplError->sizePolicy().hasHeightForWidth() ) );
    QFont mBigAmplError_font(  mBigAmplError->font() );
    mBigAmplError->setFont( mBigAmplError_font ); 
    mBigAmplError->setProperty( "m_nAlign", 0 );
    mBigAmplError->setProperty( "m_nFrame", 0 );
    mBigAmplError->setProperty( "m_nStartSize", 15 );
    layout5->addWidget( mBigAmplError );

    mBigAngleError = new CBigLetter( this, "mBigAngleError" );
    mBigAngleError->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, mBigAngleError->sizePolicy().hasHeightForWidth() ) );
    QFont mBigAngleError_font(  mBigAngleError->font() );
    mBigAngleError->setFont( mBigAngleError_font ); 
    mBigAngleError->setProperty( "m_nAlign", 0 );
    mBigAngleError->setProperty( "m_nFrame", 0 );
    mBigAngleError->setProperty( "m_nStartSize", 15 );
    layout5->addWidget( mBigAngleError );
    WMMeasValuesBaseLayout->addLayout( layout5 );

    layout6 = new QVBoxLayout( 0, 0, 6, "layout6"); 

    mBigLPXUnit = new CBigLetter( this, "mBigLPXUnit" );
    mBigLPXUnit->setProperty( "m_nFrame", 0 );
    mBigLPXUnit->setProperty( "m_nStartSize", 15 );
    layout6->addWidget( mBigLPXUnit );

    mBigLPNUnit = new CBigLetter( this, "mBigLPNUnit" );
    mBigLPNUnit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, mBigLPNUnit->sizePolicy().hasHeightForWidth() ) );
    QFont mBigLPNUnit_font(  mBigLPNUnit->font() );
    mBigLPNUnit->setFont( mBigLPNUnit_font ); 
    mBigLPNUnit->setProperty( "m_nAlign", 0 );
    mBigLPNUnit->setProperty( "m_nFrame", 0 );
    mBigLPNUnit->setProperty( "m_nStartSize", 15 );
    layout6->addWidget( mBigLPNUnit );

    mBigErrorUnit = new CBigLetter( this, "mBigErrorUnit" );
    mBigErrorUnit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, mBigErrorUnit->sizePolicy().hasHeightForWidth() ) );
    QFont mBigErrorUnit_font(  mBigErrorUnit->font() );
    mBigErrorUnit->setFont( mBigErrorUnit_font ); 
    mBigErrorUnit->setProperty( "m_nAlign", 0 );
    mBigErrorUnit->setProperty( "m_nFrame", 0 );
    mBigErrorUnit->setProperty( "m_nStartSize", 15 );
    layout6->addWidget( mBigErrorUnit );

    mBigAngleUnit = new CBigLetter( this, "mBigAngleUnit" );
    mBigAngleUnit->setProperty( "m_nAlign", 0 );
    mBigAngleUnit->setProperty( "m_nFrame", 0 );
    mBigAngleUnit->setProperty( "m_nStartSize", 15 );
    layout6->addWidget( mBigAngleUnit );
    WMMeasValuesBaseLayout->addLayout( layout6 );
    languageChange();
    resize( QSize(638, 364).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
WMMeasValuesBase::~WMMeasValuesBase()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void WMMeasValuesBase::languageChange()
{
    setCaption( tr( "Fehlermesswerte" ) );
    cBigLetter3->setProperty( "m_sDisplay", tr( "LP(X):" ) );
    cBigLetter1->setProperty( "m_sDisplay", tr( "LP(N):" ) );
    cBigLetter2->setProperty( "m_sDisplay", tr( "F:" ) );
    mBigAngleName->setProperty( "m_sDisplay", tr( "phi:" ) );
    mBigLoadpointX->setProperty( "m_sDisplay", tr( "---.---" ) );
    mBigLoadpointN->setProperty( "m_sDisplay", tr( "---.---" ) );
    mBigAmplError->setProperty( "m_sDisplay", tr( "-.-" ) );
    mBigAngleError->setProperty( "m_sDisplay", tr( "-.-" ) );
    mBigLPXUnit->setProperty( "m_sDisplay", tr( "%" ) );
    mBigLPNUnit->setProperty( "m_sDisplay", tr( "%" ) );
    mBigErrorUnit->setProperty( "m_sDisplay", tr( "%" ) );
    mBigAngleUnit->setProperty( "m_sDisplay", trUtf8( "\xc2\xb0" ) );
}

