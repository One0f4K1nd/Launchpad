#include "addloginserver.h"
#include "ui_addloginserver.h"

AddLoginServer::AddLoginServer(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::AddLoginServer) {
  ui->setupUi(this);
}

AddLoginServer::~AddLoginServer() {
  delete ui;
}

QString AddLoginServer::getName() {
  return ui->lineEdit_name->text();
}

QString AddLoginServer::getHost() {
  return ui->lineEdit_host->text();
}

QString AddLoginServer::getPort() {
    return QString::number(ui->spinBox_port->value());
}

void AddLoginServer::disableNameEdit() {
    ui->lineEdit_name->setEnabled(false);
}

void AddLoginServer::setName(const QString& name) {
    ui->lineEdit_name->setText(name);
}

void AddLoginServer::setHost(const QString& host) {
    ui->lineEdit_host->setText(host);
}

void AddLoginServer::setPort(const QString& port) {
    ui->spinBox_port->setValue(port.toInt());
}

void AddLoginServer::on_lineEdit_host_textChanged(const QString &arg1)
{
    ui->lineEdit_name->setText(arg1);
}
