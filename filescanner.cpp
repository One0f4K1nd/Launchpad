#include "filescanner.h"
#include "mainwindow.h"
#include <QSettings>
#include <QtConcurrent/QtConcurrent>

FileScanner::FileScanner(MainWindow *main) {
    mainWindow = main;
}

bool FileScanner::checkSwgFolder(const QString& dirString) {
    QDir dir(dirString);

    if (!dir.exists())
        return false;

    QVector<QPair<QString, qint64> > fileListToCheck = MainWindow::getRequiredFiles("downloadlist.txt");

    int fileCountToCheck = fileListToCheck.size() / 3;
    int currentFiles = 0;

   // if (isARequiredFile())

    QStringList files = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);

    //qDebug() << "checking for " << fileCountToCheck << " files";

    for (int i = 0; i < files.size(); ++i) {
        if (isARequiredFile(files.at(i)))
            currentFiles++;
    }

    //qDebug() << "found " << currentFiles << " swg files";

    if (currentFiles < fileCountToCheck)
        return false;

    return true;
}

bool FileScanner::isARequiredFile(const QString& name) {
    QString fileName = "downloadlist.txt";
    QVector<QPair<QString, qint64> > fileListToCheck = MainWindow::getRequiredFiles(fileName);

    for (int i = 0; i < fileListToCheck.size(); ++i) {
        const QPair<QString, qint64>& data = fileListToCheck.at(i);

        if (name.toLower() == data.first.toLower())
            return true;
    }

    return false;
}

int FileScanner::loadAndBasicCheckFiles(QString swgFolder) {
    QString fileName = "downloadlist.txt";
    QVector<QPair<QString, qint64> > fileListToCheck = MainWindow::getRequiredFiles(fileName);

    for (int i = 0; i < fileListToCheck.size() && !mainWindow->doCancelWorkingThreads(); ++i) {
        const QPair<QString, qint64>& data = fileListToCheck.at(i);
        QString file = swgFolder + "/" + data.first;

        QFile fileObject(file);

        if (!fileObject.exists()) {
            qDebug() << file << "doesnt exist";
            return -1;
        }

        qint64 fileObjectSize = fileObject.size();

        if (!data.first.contains(".exe") && fileObjectSize != data.second) {
            qDebug() << file << " size mismatch found: " << fileObjectSize << " expected: " << data.second;
            return -1;
        }

        if (!mainWindow->doCancelWorkingThreads())
            emit requiredFileExists(file);
    }

    return 0;
}

void FileScanner::fullScanMultiThreaded(bool ) {
    QFile* file3 = mainWindow->getRequiredFilesFile("downloadlist.txt");

    QSettings settings;
    QString swgFolder = settings.value("swg_folder").toString();

    while (!file3->atEnd()) {
        QByteArray line = file3->readLine();

        //QRegExp rx("(\\ |\\,|\\.|\\;|\\t)"); //RegEx for ' ' or ',' or '.' or ':' or '\t'
        QList<QByteArray> query = line.split(';');

        QString name = query.at(0);
        QString size = query.at(1);
        QString md5 = query.at(2).trimmed();

        QString fullFile = swgFolder + "/" + name;

        QtConcurrent::run(this, &FileScanner::fullScanFile, fullFile, name, size.toLongLong(), md5);
    }

    delete file3;
//    delete filesupport;
}

void FileScanner::fullScanFile(const QString& file, const QString& name, qint64, const QString &md5) {
    QFile fileObject(file);

    if (!fileObject.exists()) {
        qDebug() << file << "doesnt exist";

        emit addFileToDownload(MainWindow::patchUrl + name);

        emit fullScannedFile(name, false);

        mainWindow->decrementFullScanWorkingThreads();

        return;
    }

    QCryptographicHash crypto(QCryptographicHash::Md5);
    if (!fileObject.open(QFile::ReadOnly)) {
        qDebug() << "could not open file:" << file;

        emit addFileToDownload(MainWindow::patchUrl + name);

        emit fullScannedFile(name, false);

        mainWindow->decrementFullScanWorkingThreads();

        return;
    }

    while (!fileObject.atEnd() && !mainWindow->doCancelWorkingThreads()){
        crypto.addData(fileObject.read(8192));
    }

    QByteArray hash = crypto.result();
    QString calculatedHash = hash.toHex().toUpper().trimmed();

    int compareResult = calculatedHash.compare(md5.toUpper().trimmed());

    if (compareResult != 0 && !mainWindow->doCancelWorkingThreads()) {
        qDebug() << "hash mismatch for:" << file << " compare result:" << compareResult;

        qDebug() << "calculated hash of:" << file << " is:" << calculatedHash << " and specified one is:" << md5.toUpper().trimmed();

        emit addFileToDownload(MainWindow::patchUrl + name);
    }

    emit fullScannedFile(name, compareResult == 0);

    mainWindow->decrementFullScanWorkingThreads();
}

int FileScanner::fullScanSingleThreaded(bool ) {
    QFile* file = mainWindow->getRequiredFilesFile("downloadlist.txt");

    QSettings settings;
    QString swgFolder = settings.value("swg_folder").toString();

    int res = 0;

    while (!file->atEnd()) {
        QByteArray line = file->readLine();

        //QRegExp rx("(\\ |\\,|\\.|\\;|\\t)"); //RegEx for ' ' or ',' or '.' or ':' or '\t'
        QList<QByteArray> query = line.split(';');

        QString name = query.at(0);
        QString size = query.at(1);
        QString md5 = query.at(2).trimmed();

        QString file3 = swgFolder + "/" + name;

        QFile fileObject(file3);

        if (!fileObject.exists()) {
            qDebug() << file3 << "doesnt exist";

            mainWindow->appendToFilesToDownloadStringList(MainWindow::patchUrl + name);

            if (!mainWindow->doCancelWorkingThreads())
                emit fullScannedFile(name, false);

            continue;
        }

        QCryptographicHash crypto(QCryptographicHash::Md5);
        if (!fileObject.open(QFile::ReadOnly)) {
            qDebug() << "could not open file:" << file;

            mainWindow->appendToFilesToDownloadStringList(MainWindow::patchUrl + name);

            if (!mainWindow->doCancelWorkingThreads())
                emit fullScannedFile(name, false);

            continue;
        }

        while (!fileObject.atEnd() && !mainWindow->doCancelWorkingThreads()){
            crypto.addData(fileObject.read(8192));
        }

        QByteArray hash = crypto.result();
        QString calculatedHash = hash.toHex().toUpper().trimmed();

        int compareResult = calculatedHash.compare(md5.toUpper().trimmed());

        if (compareResult != 0 && !mainWindow->doCancelWorkingThreads()) {
            qDebug() << "hash mismatch for:" << file3 << " compare result:" << compareResult;

            qDebug() << "calculated hash of:" << file3 << " is:" << calculatedHash << " and specified one is:" << md5.toUpper().trimmed();
            res = 2;

            mainWindow->appendToFilesToDownloadStringList(MainWindow::patchUrl + name);
        }

        if (!mainWindow->doCancelWorkingThreads())
            emit fullScannedFile(name, compareResult == 0);
    }

    delete file;

    return res;
}

