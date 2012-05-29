/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/


void VersionsViewBase::ReceiveVersionData(tVersSerial* p)
{
    pVersion = p;
    DeviceText->setText(p->DeviceName);
    DeviceVersionText->setText(p->DeviceVersion);
    SerialNrText->setText(p->PCBSerialNr);
    AdjDataText->setText(p->JDataChksum);
    PCBVersionText->setText( p->PCBVersion);
    PCBServerText->setText(p->PCBServer);
    DSPVersionText->setText(p->DSPVersion);
    DSPServerText->setText( p->DSPServer);
}




void VersionsViewBase::ShowVersionSlot()
{
    show();
}
