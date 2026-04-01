#include "mainwindow.h"
#include <QtWidgets>
#include <QDirIterator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Find Files"));
    resize(700, 500);


    QPushButton *browseButton = new QPushButton(tr("&Browse..."), this);
    connect(browseButton, &QAbstractButton::clicked, this, &MainWindow::browse);

    findButton = new QPushButton(tr("&Find"), this);
    findButton->setDefault(true);
    connect(findButton, &QAbstractButton::clicked, this, &MainWindow::find);


    fileComboBox = new QComboBox;
    fileComboBox->setEditable(true);
    fileComboBox->addItem("*");
    fileComboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    textComboBox = new QComboBox;
    textComboBox->setEditable(true);
    textComboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    directoryComboBox = new QComboBox;
    directoryComboBox->setEditable(true);
    directoryComboBox->addItem(QDir::currentPath());
    directoryComboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);


    filesTable = new QTableWidget(0, 2);
    filesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    filesTable->setHorizontalHeaderLabels({tr("Named"), tr("Size")});
    filesTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    filesTable->verticalHeader()->hide();
    filesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    filesFoundLabel = new QLabel;


    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(new QLabel(tr("Named:")), 0, 0);
    mainLayout->addWidget(fileComboBox, 0, 1, 1, 2);
    mainLayout->addWidget(new QLabel(tr("Containing text:")), 1, 0);
    mainLayout->addWidget(textComboBox, 1, 1, 1, 2);
    mainLayout->addWidget(new QLabel(tr("In directory:")), 2, 0);
    mainLayout->addWidget(directoryComboBox, 2, 1);
    mainLayout->addWidget(browseButton, 2, 2);
    mainLayout->addWidget(filesTable, 3, 0, 1, 3);
    mainLayout->addWidget(filesFoundLabel, 4, 0, 1, 2);
    mainLayout->addWidget(findButton, 4, 2);

    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
}

void MainWindow::browse()
{
    QString directory = QFileDialog::getExistingDirectory(this, tr("Find Files"),
                                                          directoryComboBox->currentText());
    if (!directory.isEmpty()) {
        if (directoryComboBox->findText(directory) == -1)
            directoryComboBox->addItem(directory);
        directoryComboBox->setCurrentIndex(directoryComboBox->findText(directory));
    }
}

void MainWindow::find()
{
    filesTable->setRowCount(0);

    QString fileName = fileComboBox->currentText();
    QString text = textComboBox->currentText();
    QString path = directoryComboBox->currentText();


    if (fileComboBox->findText(fileName) == -1) fileComboBox->addItem(fileName);
    if (textComboBox->findText(text) == -1) textComboBox->addItem(text);


    QStringList filter = fileName.split(";", Qt::SkipEmptyParts);
    QDirIterator it(path, filter, QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);

    QStringList foundFiles;


    QProgressDialog progress(tr("Searching..."), tr("Cancel"), 0, 0, this);
    progress.setWindowModality(Qt::WindowModal);

    while (it.hasNext()) {
        QString filePath = it.next();
        qApp->processEvents();
        if (progress.wasCanceled()) break;

        bool match = true;
        if (!text.isEmpty()) {
            match = false;
            QFile file(filePath);
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream in(&file);
                while (!in.atEnd()) {
                    if (in.readLine().contains(text, Qt::CaseInsensitive)) {
                        match = true;
                        break;
                    }
                }
            }
        }

        if (match) {
            foundFiles << filePath;
        }
    }

    showFiles(foundFiles);
}

void MainWindow::showFiles(const QStringList &paths)
{
    for (const QString &filePath : paths) {
        QFileInfo fileInfo(filePath);
        qint64 size = fileInfo.size();


        QTableWidgetItem *fileNameItem = new QTableWidgetItem(fileInfo.fileName());
        QTableWidgetItem *sizeItem = new QTableWidgetItem(tr("%1 KB").arg((size + 1023) / 1024));
        sizeItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        int row = filesTable->rowCount();
        filesTable->insertRow(row);
        filesTable->setItem(row, 0, fileNameItem);
        filesTable->setItem(row, 1, sizeItem);
    }
    filesFoundLabel->setText(tr("%1 file(s) found").arg(paths.size()));
}
