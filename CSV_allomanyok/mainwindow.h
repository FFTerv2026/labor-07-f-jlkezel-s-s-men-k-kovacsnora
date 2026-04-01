#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QTableWidget;
class QAction;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openCsvFile();

private:
    void setupMenuBar();
    void loadCsv(const QString &fileName);

    QTableWidget *tableWidget;
    QAction *openAction;
};

#endif // MAINWINDOW_H
