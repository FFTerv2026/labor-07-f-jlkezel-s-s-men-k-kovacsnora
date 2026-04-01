#include "mainwindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QTableWidget>
#include <QHeaderView>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("CSV Viewer");
    resize(800, 600);


    tableWidget = new QTableWidget(this);


    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);


    tableWidget->horizontalHeader()->setStretchLastSection(true);
    tableWidget->setAlternatingRowColors(true);

    setCentralWidget(tableWidget);

    setupMenuBar();
}

MainWindow::~MainWindow() {}

void MainWindow::setupMenuBar()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));


    openAction = new QAction(tr("&Open File..."), this);
    openAction->setShortcut(QKeySequence::Open);

    connect(openAction, &QAction::triggered, this, &MainWindow::openCsvFile);

    fileMenu->addAction(openAction);
}

void MainWindow::openCsvFile()
{

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open CSV File"), "", tr("CSV Files (*.csv);;All Files (*)"));

    if (!fileName.isEmpty()) {
        loadCsv(fileName);
    }
}

void MainWindow::loadCsv(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, tr("Error"), tr("Could not open file."));
        return;
    }


    tableWidget->setRowCount(0);
    tableWidget->setColumnCount(0);

    QTextStream in(&file);
    bool isFirstLine = true;

    while (!in.atEnd()) {
        QString line = in.readLine();

        QStringList fields = line.split(",");

        if (isFirstLine) {

            tableWidget->setColumnCount(fields.size());
            tableWidget->setHorizontalHeaderLabels(fields);
            isFirstLine = false;
        } else {

            int currentRow = tableWidget->rowCount();
            tableWidget->insertRow(currentRow);

            for (int i = 0; i < fields.size(); ++i) {
                QTableWidgetItem *item = new QTableWidgetItem(fields[i].trimmed());
                tableWidget->setItem(currentRow, i, item);
            }
        }
    }

    file.close();


    tableWidget->resizeColumnsToContents();
}
