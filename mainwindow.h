#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QLabel>
#include <QProcess>
#include <QFuture>
#include <QFutureWatcher>
#include <QTime>
#include <QFile>
#include <QSystemTrayIcon>
#include <QAtomicInt>
#include <QToolButton>
#include <QSettings>
#ifdef Q_OS_WIN32
#include <WinSock2.h>
#else
#include <sys/socket.h>
#endif

#pragma comment(lib,"ws2_32.lib") //Winsock Library

class Settings;
class LoginServers;
class GameProcess;
class SelfUpdater;
class GameMods;
class FileScanner;

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  static bool invalidChar(char c);
  static QString stripUnicode(QString &str);
//  static QSettings settingsOptions;

  static QString defaultProtocol;
  static QString defaultLoginAddress;
  static QString defaultLoginPort;

  static QString selectedLoginAddress;
  static QString selectedPort;

  static QString swgFolder;

  static QString patchUrl;
  static QString newsUrl;
  static QString gameExecutable;
  static const QString version;
  static QString selfUpdateUrl;
  static QString swg_install_folder;
  WSADATA wsa;
  struct sockaddr_in server;
  static SOCKET s;
  static int status;
  static char server_reply[2000];
  char const *message;
  QString received_xml;
  QDateTime timestamp;

  public slots:
  void showSettings();
  void statusXmlIsReady();
  void startSWG();
  void loadFinished();
  int readBasiliskServerStatus();
  void startFullScan(bool forceConfigRestore = false);
  static QVector<QPair<QString, qint64> > getRequiredFiles(QString rqd);
  void downloadFinished();
  void downloadFileFinished(QNetworkReply *reply);
  void webPageLoadFinished(bool ok);
  void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
  void triggerNews();
  void updateBasicLoadProgress(QString successFile);
  void updateFullScanProgress(QString successFile, bool success);
  void triggerMultipleInstances(bool newValue);
  void startFileDownload();
  void updateServerStatus();
  void updateLoginServerList();
  void gameProcessFinished(GameProcess* process, int);
  void closeTab(int index);
  void checkForUpdates();
  void requiredFileDownloadFileFinished(QNetworkReply* reply);
  void patchesDownloadFileFinished(QNetworkReply* reply);
  void runUpdateCheckTimer();
  void startSWGSetup();
  void showAboutDialog();
  QFile* getRequiredFilesFile(QString fileName);
  void systemTrayActivated(QSystemTrayIcon::ActivationReason reason);
  void deleteProfiles();
  void startLoadBasicCheck();
  void showGameModsOptions();
  void showMacroEditor();

  void sslErrors(QNetworkReply* reply, const QList<QSslError>& errors);

  void updateDonationMeter();

  void addFileToDownloadSlot(QString file); 

  void toolBarOrientationChanged(Qt::Orientation);
  void startKodanCalculator();
  void installSWGEmu();

  void enableStart();

  static bool copyData(QIODevice &inFile, QIODevice &outFile);

  bool decrementFullScanWorkingThreads() {
      return fullScanWorkingThreads.deref();
  }

  bool doCancelWorkingThreads() {
      return cancelWorkingThreads;
  }

  void appendToFilesToDownloadStringList(const QString& file) {
      filesToDownload.append(file);
  }

signals:
  void startDownload();
  void fileDownloaded(QString);
  void finished();

private:
  Ui::MainWindow *ui;
  LoginServers* loginServers;
  QString defaultserver;
  QString defaultport;
  Settings* settings;
  QNetworkAccessManager networkAccessManager;
  QNetworkAccessManager clientFilesNetworkAccessManager;
  QNetworkAccessManager novaNetworkAccessManager;
  QNetworkAccessManager requiredFilesNetworkManager;
  QNetworkAccessManager patchesNetworkManager;
  QFutureWatcher<int> loadWatcher;
  QFutureWatcher<int> fullScanWatcher;
  QFutureWatcher<int> downloadWatcher;
  volatile bool cancelWorkingThreads;
  QStringList filesToDownload;
  int requiredFilesCount;
  int currentReadFiles;
  int nextFileToDownload;
  QTime lastReceivedBytesTime;
  quint64 lastReceivedBytes;
  QVector<GameProcess*> gameProcesses;
  SelfUpdater* silentSelfUpdater;

  QSystemTrayIcon* systemTrayIcon;
  QMenu* systemTrayMenu;
  QAction* closeAction;
  volatile bool runningFullScan;
  int gameProcessesCount;

  int updateTimeCounter;

  QAtomicInt fullScanWorkingThreads;

  QVector<QToolButton*> toolButtons;
  FileScanner* fileScanner;

protected:
  void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H

