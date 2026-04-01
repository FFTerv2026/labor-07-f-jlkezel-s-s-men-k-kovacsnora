#include "mainwindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFile>
#include <QTextStream>
#include <QRandomGenerator>
#include <QCoreApplication>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    quoteLabel = new QLabel("Betöltés...");
    quoteLabel->setWordWrap(true);
    quoteLabel->setAlignment(Qt::AlignCenter);

    nextButton = new QPushButton("Új motiváció");

    layout->addWidget(quoteLabel, 1);
    layout->addWidget(nextButton);
    layout->setContentsMargins(40, 40, 40, 40);

    setCentralWidget(centralWidget);
    setWindowTitle("Idézet Generátor");
    resize(600, 400);

    stilusBeallitas();
    betoltes();
    ujszerencse();

    connect(nextButton, &QPushButton::clicked, this, &MainWindow::ujszerencse);
}

void MainWindow::betoltes() {
    // Ez a sor garantálja, hogy a program a saját mappájában keresse a fájlt
    QString filename = "idezetek.txt";
    QString path = QCoreApplication::applicationDirPath() + "/" + filename;

    QFile file(path);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        osszesIdezet.clear();
        while (!in.atEnd()) {
            QString sor = in.readLine().trimmed();
            if (!sor.isEmpty()) {
                osszesIdezet << sor;
            }
        }
        file.close();
    }

    if (osszesIdezet.isEmpty()) {
        // Ha nem találja, megmutatjuk a felhasználónak, hol kereste a program
        quoteLabel->setText("HIBA: Nem találom a fájlt!\n\nIde másold be: \n" + path);
    }
}

void MainWindow::ujszerencse() {
    if (osszesIdezet.isEmpty()) return;

    // Ha csak 1 idézet van
    if (osszesIdezet.size() == 1) {
        quoteLabel->setText("“" + osszesIdezet[0] + "”");
        return;
    }

    int ujIndex;
    do {
        ujIndex = QRandomGenerator::global()->bounded(osszesIdezet.size());
    } while (ujIndex == utolsoIndex);

    utolsoIndex = ujIndex;
    quoteLabel->setText("“" + osszesIdezet[ujIndex] + "”");
}

void MainWindow::stilusBeallitas() {
    this->setStyleSheet(R"(
        QMainWindow {
            background-color: #1a1a2e;
        }
        QLabel {
            color: #e94560;
            font-family: 'Segoe UI', 'Arial';
            font-size: 22px;
            font-weight: bold;
            font-style: italic;
        }
        QPushButton {
            background-color: #16213e;
            color: white;
            border: 2px solid #e94560;
            border-radius: 15px;
            padding: 15px;
            font-size: 16px;
        }
        QPushButton:hover {
            background-color: #e94560;
            color: #1a1a2e;
        }
    )");
}
