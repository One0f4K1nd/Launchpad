#ifndef ADDLOGINSERVER_H
#define ADDLOGINSERVER_H

#include <QDialog>

namespace Ui {
  class AddLoginServer;
}

class AddLoginServer : public QDialog
{
  Q_OBJECT
  
public:
  explicit AddLoginServer(QWidget *parent = nullptr);
  ~AddLoginServer();

  QString getName();
  QString getHost();
  QString getPort();

  void setName(const QString& name);
  void setHost(const QString& host);
  void setPort(const QString& port);

  void disableNameEdit();

  private slots:
  void on_lineEdit_host_textChanged(const QString &arg1);

  private:
  Ui::AddLoginServer *ui;
};

#endif // ADDLOGINSERVER_H
