#include "installfromswg.h"
#include "ui_installfromswg.h"
#include <QSettings>
#include <QDir>
#include "mainwindow.h"
#include <QTimer>
#include <QFutureWatcher>
#include <QPalette>
#include <QMessageBox>
#include <QFileDialog>

#if QT_VERSION >= 0x050000
#include <QtConcurrent/QtConcurrentRun>
#endif

InstallFromSWG::InstallFromSWG(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InstallFromSWG) {
    ui->setupUi(this);

    cancelThreads = false;

    connect(&copyWatcher, SIGNAL(finished()), this, SLOT(copyFinished()));

    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(0);

    connect(this, SIGNAL(fileCopiedSignal(QString,bool)), this, SLOT(fileCopied(QString,bool)));
}

InstallFromSWG::~InstallFromSWG() {
    delete ui;
}

void InstallFromSWG::copyFinished() {
    int result = copyWatcher.result();

    ui->progressBar->setValue(ui->progressBar->maximum());

    if (result == 0)
        ui->label->setText("Installation finished.");
    else
        ui->label->setText("Installation failed.");

    qDebug() << "copy finished with result " << result;

    done(result);
}

void InstallFromSWG::closeEvent(QCloseEvent* event) {
    qDebug() << "close event";

    cancelThreads = true;

    if (copyWatcher.isRunning()) {
        copyWatcher.cancel();
        copyWatcher.waitForFinished();
    }

    done(2);

    QDialog::closeEvent(event);
}

int InstallFromSWG::copyFiles() {
    QVector<QPair<QString, qint64> > requiredFiles = MainWindow::getRequiredFiles("downloadlist.txt");

    for (int i = 0; i < requiredFiles.size() && !cancelThreads; ++i) {
        const QPair<QString, qint64>& file = requiredFiles.at(i);

        if (file.first.contains("/")) {
            //QString dir = emuFolder + file.first.mid(0, file.first.lastIndexOf("/"));

            //QDir(dir).mkpath(".");
        }

//#ifdef Q_OS_WIN32
//        bool result = QFile::copy(swgfolder + "\\" + file.first, emuFolder + file.first);
//#else
//        bool result = QFile::copy(swgfolder + "/" + file.first, emuFolder + file.first);
//#endif

        qDebug() << file.first << " added to download list...";
        emit fileCopiedSignal(file.first, 0);
    }

    qDebug() << "Downloads Queued...";
    return 0;
}

void InstallFromSWG::fileCopied(const QString& file, bool success) {
    if (success) {
        ui->label->setText(file + " successfully installed");
    }

    ui->progressBar->setValue(ui->progressBar->value() + 1);
}

int InstallFromSWG::checkSWGFolder() {
    QDir dir(swgfolder);

    if (!dir.exists())
        return 1;

    QStringList filesToCheck;
    filesToCheck << "Mss32.dll" << "miles/Msseax.m3d" << "texture/loading/space/images/space_load_pvp.dds";

    for (int i = 0; i < filesToCheck.size(); ++i) {
#ifdef Q_OS_WIN32
        if (!QFile(swgfolder + "\\" + filesToCheck.at(i)).exists())
            return 2;
#else
        if (!QFile(swgfolder + "/" + filesToCheck.at(i)).exists())
            return 2;
#endif
    }

    return 0;
}

int InstallFromSWG::installFiles() {
//    QSettings settings;

//    QMessageBox::information(this, "SWGMTGEmu", "Please choose a valid Star Wars Galaxies installation");

//    swgfolder = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
//                                                   "/home",
//                                                   QFileDialog::ShowDirsOnly
//                                                   | QFileDialog::DontResolveSymlinks);

//    int validFolder = checkSWGFolder();

//    if (validFolder != 0) {
//        QMessageBox::warning(this, "Folder", "The folder you selected isnt a valid Star Wars Galaxies installation!");

//        return 1;
//    }

    QMessageBox::information(this, "SWGMTGEmu", "Please choose where you want to install SWGMTGEmu");

    emuFolder = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

//    if (!QDir(emuFolder).exists() || emuFolder.isEmpty()) {
//        QMessageBox::warning(this, "Folder", "The SWGMTGEmu folder you selected isn't a valid directory");

//        return 1;
//    }

    //qDebug() << emuFolder;
    emuFolder = emuFolder + "/SWGMTGEmu/";

    if (QDir(emuFolder).exists()) {
        if (QMessageBox::question(this, "Warning", "SWGMTGEmu folder already exists, do you want to overwrite?") != QMessageBox::Yes)
            return 3;
    } else {
        if (!QDir(emuFolder).mkpath(".")) {
            QMessageBox::warning(this, "ERROR", "Could not create the SWGMTGEmu folder!");
            return 4;
        }
    }

    QVector<QPair<QString, qint64> > requiredFiles = MainWindow::getRequiredFiles("downloadlist.txt");

    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(requiredFiles.size());

    QFuture<int> future = QtConcurrent::run(this, &InstallFromSWG::copyFiles);
    copyWatcher.setFuture(future);

    return exec();
}
