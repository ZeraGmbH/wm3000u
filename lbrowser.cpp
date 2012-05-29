// definition LittleBrowser

#include <qstring.h>
#include <qwidget.h>
#include <qmainwindow.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qframe.h>
#include <qpixmap.h>
#include <qiconset.h>
#include <qtextbrowser.h>

#include 	"lbrowser.h"

cLittleBrowser::cLittleBrowser( const QString& home, const QString& path,QWidget* parent, const char *name )  : QMainWindow( parent, name,Qt::WType_Dialog )

{
    m_pbrowser = new QTextBrowser( this );
    m_pbrowser->mimeSourceFactory()->setFilePath(path );
    m_pbrowser->setFrameStyle( QFrame::Panel | QFrame::Sunken );
    setCaption(name);
    setCentralWidget( m_pbrowser );
    m_sFileName = home;
    resize( 500,300 );

    QToolBar* toolbar = new QToolBar( this );
    addToolBar( toolbar, "Toolbar");
    QToolButton* button;

    button = new QToolButton(QIconSet(QPixmap("./images/home.png")), trUtf8("zum Anfang"), "", m_pbrowser, SLOT(home()), toolbar );
    connect( m_pbrowser, SIGNAL( backwardAvailable(bool) ), button, SLOT( setEnabled(bool) ) );
    connect( m_pbrowser, SIGNAL( forwardAvailable(bool) ), button, SLOT( setDisabled(bool) ) );
    button->setEnabled( FALSE );
    button = new QToolButton(QIconSet(QPixmap("./images/previous.png")),trUtf8("zurück"),"", m_pbrowser, SLOT(backward()), toolbar );
    connect( m_pbrowser, SIGNAL( backwardAvailable(bool) ), button, SLOT( setEnabled(bool) ) );
    button->setEnabled( FALSE );
    button = new QToolButton(QIconSet(QPixmap("./images/next.png")),trUtf8("vorwärts"),"", m_pbrowser, SLOT(forward()), toolbar );
    connect( m_pbrowser, SIGNAL( forwardAvailable(bool) ), button, SLOT( setEnabled(bool) ) );
    button = new QToolButton(QIconSet(QPixmap("./images/zoomin.png")),trUtf8("vergrössern"),"", m_pbrowser, SLOT(zoomIn()), toolbar );
    button = new QToolButton(QIconSet(QPixmap("./images/zoomout.png")),trUtf8("verkleinern"),"", m_pbrowser, SLOT(zoomOut()), toolbar );
    button = new QToolButton(QIconSet(QPixmap("./images/book.png")),trUtf8("Index"),"", m_pbrowser, SLOT(home()), toolbar );
    button = new QToolButton(QIconSet(QPixmap("./images/doneandnext.png")),trUtf8("Bedienungsanleitung schliessen"),"", this, SLOT(close()), toolbar );
    
    toolbar->setHorizontallyStretchable(true);
    toolbar->setVerticallyStretchable(true);
    setDockEnabled( DockLeft, FALSE );
    setDockEnabled( DockRight, FALSE );
    setWFlags(Qt::WStyle_StaysOnTop);
}

void cLittleBrowser::show()
{
    m_pbrowser -> setSource(m_sFileName);
    QMainWindow::show();
}
