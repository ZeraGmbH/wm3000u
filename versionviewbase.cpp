#include "versionviewbase.h"
#include "ui_versionviewbase.h"

VersionsViewBase::VersionsViewBase( QWidget* parent ):
    QDialog(parent),
    ui(new Ui::VersionViewBase)
{
    ui->setupUi(this);
}

VersionsViewBase::~VersionsViewBase()
{
    delete ui;
}

void VersionsViewBase::ReceiveVersionData(tVersSerial* p)
{
    pVersion = p;
    ui->DeviceLabel->setText(p->DeviceName);
    ui->DevVersionLabel->setText(p->DeviceVersion);
    ui->SerialNrLabel->setText(p->PCBSerialNr);
    ui->AdjDataText->setText(p->JDataChksum);
    ui->PCBVersionText->setText( p->PCBVersion);
    ui->PCBServerText->setText(p->PCBServer);
    ui->DSPVersionText->setText(p->DSPVersion);
    ui->DSPServerText->setText( p->DSPServer);
}




void VersionsViewBase::ShowVersionSlot()
{
    show();
}
