#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/**
 * mainwindow.h
 * @author Valery Toda
 **/

#include <QMainWindow>
#include <QListWidgetItem>
#include <QVector>
#include "entry.h"
#include <vector>

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    void resizeEvent(QResizeEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);

    ~MainWindow();

public slots:
    void loadEntryFromFile();
    void deleteItem();
    void showItem(QListWidgetItem* item);
    void showItemSelected();
    void exportToFile();

private:
    Ui::MainWindow* ui;
    QString filedirectory;
    QVector <float>* steringAngles;
    vector<Entry>* entries;
    QPoint oldWindowPosition;

    void selectEntry();
    bool checkSanity(float value);

};

#endif // MAINWINDOW_H
