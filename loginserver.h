#ifndef LOGINSERVER_H
#define LOGINSERVER_H

#include <QListWidgetItem>

class LoginServer : public QListWidgetItem {
protected:
  QString host;
  QString port;

public:
  LoginServer(const LoginServer& s) : QListWidgetItem(s) {
    initialize(s);
  }

  LoginServer(const QString& name, const QString& host, const QString& port) {
    this->host = host;
    this->port = port;

    setText(name);
  }

  LoginServer& operator=(const LoginServer& s) {
    if (this == &s)
      return *this;

    initialize(s);

    return *this;
  }

  void initialize(const LoginServer& s) {
    host = s.host;
    port = s.port;

    setText(s.text());
  }

  void setHost(const QString& host) {
      this->host = host;
  }

  void setPort(const QString& port) {
      this->port = port;
  }

  QString getHost() {
    return host;
  }

  QString getName() {
      return text();
  }

  QString getPort() {
    return port;
  }
};

#endif // LOGINSERVER_H
