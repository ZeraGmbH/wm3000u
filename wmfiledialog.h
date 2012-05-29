// headerdatei wmFileDialog

#ifndef WMFILEDIALOG_H
#define WMFILEDIALOG_H

#include <qfiledialog.h>
#include <qpushbutton.h>
#include <qstring.h>
#include <qwidget.h>

class wmFileDialog: public QFileDialog {
public:
    wmFileDialog(const QString& dirName,const QString& filter=QString::null,QWidget* parent = 0,const char* name=0,bool modal=FALSE );
    wmFileDialog(QWidget* parent = 0,const char* name = 0,bool modal=FALSE ) ;
    ~wmFileDialog (){};
    
private:
    QPushButton *DelButton;
    void InitDelButton();
};


#endif
