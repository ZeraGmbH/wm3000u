/****************************************************************************
** Form implementation generated from reading ui file 'versionviewbase.ui'
**
** Created: Mi Feb 15 08:25:05 2012
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "versionviewbase.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "versionviewbase.ui.h"
/*
 *  Constructs a VersionsViewBase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
VersionsViewBase::VersionsViewBase( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "VersionsViewBase" );
    VersionsViewBaseLayout = new QHBoxLayout( this, 11, 6, "VersionsViewBaseLayout"); 

    layout7 = new QVBoxLayout( 0, 0, 6, "layout7"); 

    DeviceLabel = new QLabel( this, "DeviceLabel" );
    layout7->addWidget( DeviceLabel );

    DevVersionLabel = new QLabel( this, "DevVersionLabel" );
    layout7->addWidget( DevVersionLabel );

    SerialNrLabel = new QLabel( this, "SerialNrLabel" );
    layout7->addWidget( SerialNrLabel );

    AdjDataLabel = new QLabel( this, "AdjDataLabel" );
    layout7->addWidget( AdjDataLabel );

    PCBVersionLabel = new QLabel( this, "PCBVersionLabel" );
    layout7->addWidget( PCBVersionLabel );

    PCBServerLabel = new QLabel( this, "PCBServerLabel" );
    layout7->addWidget( PCBServerLabel );

    DSPVersionLabel = new QLabel( this, "DSPVersionLabel" );
    layout7->addWidget( DSPVersionLabel );

    DSPServerLabel = new QLabel( this, "DSPServerLabel" );
    layout7->addWidget( DSPServerLabel );
    VersionsViewBaseLayout->addLayout( layout7 );
    spacer4 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    VersionsViewBaseLayout->addItem( spacer4 );

    layout8 = new QVBoxLayout( 0, 0, 6, "layout8"); 

    DeviceText = new QLabel( this, "DeviceText" );
    layout8->addWidget( DeviceText );

    DeviceVersionText = new QLabel( this, "DeviceVersionText" );
    layout8->addWidget( DeviceVersionText );

    SerialNrText = new QLabel( this, "SerialNrText" );
    layout8->addWidget( SerialNrText );

    AdjDataText = new QLabel( this, "AdjDataText" );
    layout8->addWidget( AdjDataText );

    PCBVersionText = new QLabel( this, "PCBVersionText" );
    layout8->addWidget( PCBVersionText );

    PCBServerText = new QLabel( this, "PCBServerText" );
    layout8->addWidget( PCBServerText );

    DSPVersionText = new QLabel( this, "DSPVersionText" );
    layout8->addWidget( DSPVersionText );

    DSPServerText = new QLabel( this, "DSPServerText" );
    layout8->addWidget( DSPServerText );
    VersionsViewBaseLayout->addLayout( layout8 );
    languageChange();
    resize( QSize(231, 196).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
}

/*
 *  Destroys the object and frees any allocated resources
 */
VersionsViewBase::~VersionsViewBase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void VersionsViewBase::languageChange()
{
    setCaption( tr( "Versions" ) );
    DeviceLabel->setText( tr( "Device:" ) );
    DevVersionLabel->setText( tr( "Device Version:" ) );
    SerialNrLabel->setText( tr( "Serial Number:" ) );
    AdjDataLabel->setText( tr( "AdjustmentData:" ) );
    PCBVersionLabel->setText( tr( "PCB Version:" ) );
    PCBServerLabel->setText( tr( "PCB Server:" ) );
    DSPVersionLabel->setText( tr( "DSP Version:" ) );
    DSPServerLabel->setText( tr( "DSP Server:" ) );
    DeviceText->setText( tr( "----------" ) );
    DeviceVersionText->setText( tr( "----------" ) );
    SerialNrText->setText( tr( "----------" ) );
    AdjDataText->setText( tr( "----------" ) );
    PCBVersionText->setText( tr( "----------" ) );
    PCBServerText->setText( tr( "----------" ) );
    DSPVersionText->setText( tr( "----------" ) );
    DSPServerText->setText( tr( "----------" ) );
}

