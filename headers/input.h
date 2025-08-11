#ifndef INPUT_H
#define INPUT_H

#include <QMainWindow>
#include <QTableWidget>

namespace Ui {
class input;
}

class input : public QMainWindow
{
    Q_OBJECT

public:
    explicit input(QWidget *parent = nullptr);
    ~input();

private slots:
    void on_add_clicked();

    void on_delete_2_clicked();

    void on_ClearTable_clicked();

    void on_genXlsx_clicked();

    void on_saveQuit_clicked();

signals:
    void saveTableToMain(QTableWidget *table);

private:
    Ui::input *ui;
};

#endif // INPUT_H
