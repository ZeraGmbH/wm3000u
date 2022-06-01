// definition CLogFileView

#include <qstring.h>
#include <q3textedit.h>
#include <qevent.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <q3textstream.h>
#include <QResizeEvent>
#include <QCloseEvent>
#include <QTime>

#include "csessionhelper.h"
#include "wmglobal.h"
#include "logfileview.h"

CLogFileView::CLogFileView(const QString cap,const long max,QWidget * parent, const char * wname)
    :	QDialog(parent,wname)
{
    m_Timer.setSingleShot(true);
    setCaption(cap);
    m_pText=new Q3TextEdit(this);
    m_pText->setTextFormat(Qt::LogText);
    m_pText->setMaxLogLines(max);
    setMinimumSize(200,200);
    LoadSession(".ses");
    showT.start(2000);
    QObject::connect(&showT,SIGNAL(timeout()),this,SLOT(showList())); 
    connect(&m_Timer, SIGNAL(timeout()), this, SLOT(saveConfiguration()));
}


CLogFileView::~CLogFileView()
{
    SaveSession(".ses");
}


void CLogFileView::ShowHideLogFileSlot(bool b)
{
    if (b) show();else close();
}    


void CLogFileView::AddLogTextSlot(const QString& s)
{	
    m_loglist.append(QTime::currentTime().toString("HH:mm:ss:zzz") + ": " + s.stripWhiteSpace()); // neue daten in die liste
}


void CLogFileView::showList()
{
    if (m_loglist.count()) { // ist überhaupt etwas gesendet worden ?
	QString s;
	Q3TextStream ts( &s, QIODevice::WriteOnly );
    int i;
	for (i = 0; i < (m_loglist.count()-1); i++) 
	    ts << m_loglist[i] << "\n";
	ts << m_loglist[i];
	m_loglist.clear();
	m_pText->append(s);
    }
}


void CLogFileView::saveConfiguration()
{
    SaveSession(".ses");
}


void CLogFileView::SaveSession(QString session)
{
  cSessionHelper::writeSession(this, m_widGeometry, session);
}


bool CLogFileView::LoadSession(QString session)
	
{
  cWidgetGeometry tmpGeometry;
  tmpGeometry=cSessionHelper::readSession(this, session);
  if(tmpGeometry.m_Size.isValid())
  {
    m_widGeometry=tmpGeometry;
    return true;
  }
  else
  {
    return false;
  }
}


void CLogFileView::resizeEvent (QResizeEvent * ) 
{
    m_pText->resize(size());
    m_Timer.start(500);
}   


void CLogFileView::closeEvent (QCloseEvent* ce)
{
    m_widGeometry.SetGeometry(pos(),size());
    m_widGeometry.SetVisible(0);
    emit isVisibleSignal(false);
    m_Timer.start(500);
    ce->accept();
}   


void CLogFileView::moveEvent( QMoveEvent *)
{
    m_Timer.start(500);
}


void CLogFileView::show()
{
    QDialog::show();
}
