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
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  static QString patchUrl;
  static QString newsUrl;
  static QString gameExecutable;
  static const QString version;
  static QString selfUpdateUrl;  
  volatile bool modded;
  WSADATA wsa;
  struct sockaddr_in server;
  static SOCKET s;
  static int status;
  static char server_reply[2000];
//  char const *p = "abc";
  char const *message;
  static QString received_xml;


  public slots:
  void showSettings();
  void statusXmlIsReady();
  void startSWG();
  void loadFinished();
  int readBasiliskServerStatus();
  void startFullScan(bool forceConfigRestore = false);
  static QVector<QPair<QString, qint64> > getRequiredFiles();
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
  QFile* getRequiredFilesFile();
  void systemTrayActivated(QSystemTrayIcon::ActivationReason reason);
  void deleteProfiles();
  void startLoadBasicCheck();
  void showGameModsOptions();
  void showMacroEditor();
  int extractFiles();

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

