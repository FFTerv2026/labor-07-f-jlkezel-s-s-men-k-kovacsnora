#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>

class QLabel;
class QPushButton;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void ujszerencse();

private:
    void betoltes();
    void stilusBeallitas();

    QLabel *quoteLabel;
    QPushButton *nextButton;
    QStringList osszesIdezet;
    int utolsoIndex = -1;
};

#endif
