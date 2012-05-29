// implementierung wmfiledialog

#include "wmfiledialog.h"

wmFileDialog::wmFileDialog(const QString& dirName,const QString& filter,QWidget* parent,const char* name,bool modal)
    : QFileDialog(dirName,filter,parent,name,modal) {
    InitDelButton();
}


wmFileDialog::wmFileDialog(QWidget* parent,const char* name,bool modal)
    : QFileDialog(parent,name,modal) {
    InitDelButton();
}


void wmFileDialog::InitDelButton() {
    DelButton = new QPushButton("&Delete",this);
    DelButton->show();
    addWidgets(NULL,NULL,DelButton);
}
