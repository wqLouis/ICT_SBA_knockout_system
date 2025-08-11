#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_help_clicked();

    void on_start_clicked();

    void on_input_clicked();

    void on_Import_clicked();

public slots:
    void inputPage_table(QTableWidget *table);

signals:
    void sendTable(QTableWidget *table);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
