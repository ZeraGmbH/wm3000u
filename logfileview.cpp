// definition CLogFileView

#include <qstring.h>
#include <q3textedit.h>
#include <qevent.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <q3textstream.h>
//Added by qt3to4:
#include <QResizeEvent>
#include <QCloseEvent>

#include "wmglobal.h"
#include "logfileview.h"

CLogFileView::CLogFileView(const QString cap,const long max,QWidget * parent, const char * wname)
    :	QDialog(parent,wname)
{
    setCaption(cap);
    m_pText=new Q3TextEdit(this);
    m_pText->setTextFormat(Qt::LogText);
    m_pText->setMaxLogLines(max);
    setMinimumSize(200,200);
    LoadSession(".ses");
    showT.start(2000);
    QObject::connect(&showT,SIGNAL(timeout()),this,SLOT(showList())); 
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
    m_loglist.append(s.stripWhiteSpace()); // neue daten in die liste
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


void CLogFileView::SaveSession(QString session)
{
    QFileInfo fi(session);
    QString ls = QString("%1.%2%3").arg(wm3000uHome).arg(name()).arg(fi.fileName());
    QFile file(ls); 
//    file.remove();
    if ( file.open( QIODevice::Unbuffered | QIODevice::WriteOnly ) ) {
	file.at(0);
	
	int vi;
	vi = isVisible();
	if (vi) 
	    m_widGeometry.SetGeometry(pos(),size());
	m_widGeometry.SetVisible(vi);
	
	QDataStream stream( &file );
	stream << m_widGeometry;
	file.close();
    }
}


bool CLogFileView::LoadSession(QString session)
	
{
    QFileInfo fi(session);
    QString ls = QString("%1.%2%3").arg(wm3000uHome).arg(name()).arg(fi.fileName());
    QFile file(ls); 
    if ( file.open( QIODevice::ReadOnly ) ) {
	QDataStream stream( &file );
	stream >> m_widGeometry;
	file.close();
	hide();
	resize(m_widGeometry.m_Size);
	move(m_widGeometry.m_Point);
	if (m_widGeometry.vi)
	{
	    show();
	    emit isVisibleSignal(true);
	}
// FVWM und Gnome verhalten sich anders
#ifndef FVWM 
    move(m_widGeometry.m_Point);
#endif   
	return true;
    }
    return false;
}


void CLogFileView::resizeEvent (QResizeEvent * ) 
{
    m_pText->resize(size());
}   

void CLogFileView::closeEvent (QCloseEvent* ce)
{
    m_widGeometry.SetGeometry(pos(),size());
    m_widGeometry.SetVisible(0);
    emit isVisibleSignal(false);
    ce->accept();
}   


void CLogFileView::show()
{
    QDialog::show();
}
