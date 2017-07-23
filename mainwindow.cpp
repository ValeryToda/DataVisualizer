#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qfiledialog.h>
#include <qdir.h>
#include <qdebug.h>
#include <QListWidgetItem>
#include <QIODevice>
#include <QPainter>
#include <QFile>
#include <QShortcut>
#include <iostream>
#include <QDateTime>
#include <QResizeEvent>
#include <qtextcodec.h>

/**
 * mainwindow.cpp
 * @author Valery Toda
 **/

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);

    QShortcut *deleteShortcut = new QShortcut(QKeySequence(Qt::Key_Delete), this);
    entries = new vector<Entry>();
    this->filedirectory = qApp->applicationDirPath();

    ui->entriesListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(ui->loadfilePushButton, SIGNAL(clicked()), this, SLOT(loadEntryFromFile()));
    connect(ui->entriesListWidget, SIGNAL(itemSelectionChanged()), this, SLOT(showItemSelected()));
    connect(ui->entriesListWidget, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(showItem(QListWidgetItem *)));
    connect(deleteShortcut, SIGNAL(activated()), this, SLOT(deleteItem()));
    connect(ui->ExportfilePushButton, SIGNAL(clicked()), this, SLOT(exportToFile()));
    ui->ExportfilePushButton->setDisabled(true);
    ui->numberOfEntriesLabel->setText(QString("0"));
}

void MainWindow::loadEntryFromFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select a CSV-file"),
                                                    this->filedirectory,
                                                    tr("File (*.csv)"));
    if (fileName.isEmpty()){
        return;
    }

    ui->entriesListWidget->clear();
    ui->labelCenter->clear();
    ui->labelLeft->clear();
    ui->labelRight->clear();
    ui->numberOfEntriesLabel->clear();
    ui->steeringAngleLabel->clear();
    ui->ExportfilePushButton->setDisabled(true);
    ui->loadfilePushButton->setDisabled(true);

    // Update filePath
    this->filedirectory = QFileInfo(fileName).dir().absolutePath();

    ui->csvFilepath_label->setText(fileName);
    // load item in Listview
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly|QIODevice::Text)){
        return;
    }

    for (ulong i=0; i<entries->size(); i++) {
        entries->erase(entries->begin() + i);
    }
    entries->clear();

    qDebug() << entries->size();
    bool firstLine = true;
    while(!file.atEnd()){

        QString line = file.readLine();
        if (firstLine){
            firstLine = false;
            continue;
        }

        QStringList items = line.split(',');
        QString imagePathLeft, imagePathCenter, imagePathRight;

        //check if the filepaths are relatives or absolute
        if (QFileInfo(items[0]).isRelative()){
            //qDebug() << "relative";
            QStringList fn = items.first().split('/');
            QString name = fn.last();

            // remove any occurence of "center_", "right_" or "left_"
            name.remove(QString("center_")).remove(QString("left_")).remove(QString("right_"));

            // Set image paths
            imagePathLeft = QString (this->filedirectory+"/IMG/left_"+name);
            imagePathCenter = QString (this->filedirectory+"/IMG/center_"+name);
            imagePathRight = QString (this->filedirectory+"/IMG/right_"+name);

        }else{ // No Need to guess the absolutepath
            //qDebug() << "absolute";
            imagePathCenter = QString(items[0]).trimmed();
            imagePathLeft = QString(items[1]).trimmed();
            imagePathRight = QString(items[2]).trimmed();
        }

        // create item only if the 3 files exists
        // check if the 3 images files exists
        if (QFileInfo(imagePathCenter).isReadable() && QFileInfo(imagePathRight).isReadable() && QFileInfo(imagePathLeft).isReadable()){

            QListWidgetItem *item = new QListWidgetItem(ui->entriesListWidget);
            item->setText(QFileInfo(imagePathCenter).baseName().remove(QString("center_")).remove(QString("left_")).remove(QString("right_")));
            item->setTextAlignment(Qt::AlignLeft);
            item->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
            float angle = items[3].toFloat();
            // check if angle is ok!
            if (checkSanity(angle)){
                ui->entriesListWidget->addItem(item);
                entries->push_back(Entry(imagePathLeft, imagePathCenter, imagePathRight, angle));
            }
        }
    }

    file.close();

    if (ui->entriesListWidget->count() > 0){
        ui->entriesListWidget->setCurrentRow(0);
        ui->ExportfilePushButton->setEnabled(true);
    }
    ui->numberOfEntriesLabel->setText(QString("%1").arg(ui->entriesListWidget->count()));
    ui->loadfilePushButton->setEnabled(true);
}

bool MainWindow::checkSanity(float value){
    bool result = true;
    if (qIsInf(value)){
        result = false;
    }else if (qIsNaN(value)){
        result = false;
    }
    return result;
}

void MainWindow::resizeEvent(QResizeEvent* event){
    //Ignore event!!
    event->ignore();
}

void MainWindow::deleteItem()
{
    int row = this->ui->entriesListWidget->row(this->ui->entriesListWidget->currentItem());
    QStringList relatedFiles;
    relatedFiles << entries->at(row).imagePathCenter()\
                 << entries->at(row).imagePathLeft()\
                 << entries->at(row).imagePathRight();

    qDebug() << entries->at(row).imagePathRight();
    // remove the related files (center_*, left_*, right_*)
    foreach (const QString &relFile, relatedFiles) {
        QFile currentFile(relFile);
        if(!currentFile.exists())
            continue;
        currentFile.remove();
    }

    entries->erase(entries->begin() + row);
    QListWidgetItem * current_item = ui->entriesListWidget->takeItem(row);
    qDebug() << "current_item.text: " << current_item->text();
    delete current_item;

    ui->numberOfEntriesLabel->setText(QString("%1").arg(ui->entriesListWidget->count()));

}

void MainWindow::showItemSelected()
{
    QListWidgetItem * item = ui->entriesListWidget->currentItem();
    this->showItem(item);
}

void MainWindow::showItem(QListWidgetItem *)
{
    int row = ui->entriesListWidget->currentRow();
/*
    QString imagePathLeft(this->filedirectory+"left_"+item->text());
    QString imagePathCenter(this->filedirectory+"center_"+item->text());
    QString imagePathRight(this->filedirectory+"right_"+item->text());
*/
    QString imagePathLeft = entries->at(row).imagePathLeft();
    QString imagePathCenter = entries->at(row).imagePathCenter();
    QString imagePathRight = entries->at(row).imagePathRight();

    ui->labelCenter->setPixmap(QPixmap(imagePathCenter));
    ui->labelCenter->setScaledContents(true);
    ui->labelCenter->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    ui->labelLeft->setPixmap(QPixmap(imagePathLeft));
    ui->labelLeft->setScaledContents(true);
    ui->labelLeft->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    ui->labelRight->setPixmap(QPixmap(imagePathRight));
    ui->labelRight->setScaledContents(true);
    ui->labelRight->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    int pos = ui->entriesListWidget->currentRow();

    QString txt = QString("%1").arg(entries->at(pos).steeringAngle());
    ui->steeringAngleLabel->setText(txt);
}

void MainWindow::exportToFile(){

    QString dirname = QFileDialog::getExistingDirectory(this, tr("Select a directory"), this->filedirectory);

    if (dirname.isEmpty()){
        return;
    }
    QDateTime local(QDateTime::currentDateTime());
    QString suffix = local.toString("yyyy_MM_dd_hh_mm_ss");
    QString filepath = dirname +"/"+QString("export_%1.csv").arg(suffix);
    qDebug() << filepath;
    QFile file(filepath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        //TODO: visual feedback to user;
        ui->csvSavedFilepathLabel->setText(QString("Could not open file %1").arg(filepath));
        return;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    for (std::vector<Entry>::iterator it = entries->begin() ; it != entries->end(); ++it){
        out << (*it).toString();
    }
    ui->csvSavedFilepathLabel->setText(QString("<a href='file:///%1'>%2</a>").arg(filepath, QFileInfo(filepath).fileName()));
    ui->csvSavedFilepathLabel->setOpenExternalLinks(true);
    QApplication::restoreOverrideCursor();
}

void MainWindow::mousePressEvent(QMouseEvent* event){
    oldWindowPosition = event->globalPos();
}

void MainWindow::mouseMoveEvent(QMouseEvent* event){
    QPoint changeInPosition = event->globalPos() - oldWindowPosition;
    int gotoX = this->x() + changeInPosition.x();
    int gotoY = this->y() + changeInPosition.y();
    this->move(gotoX, gotoY);
    oldWindowPosition = event->globalPos();
}

MainWindow::~MainWindow(){
    delete ui;
    entries->clear();
    delete entries;
}

