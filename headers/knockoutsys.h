#ifndef KNOCKOUTSYS_H
#define KNOCKOUTSYS_H

#include <QMainWindow>
#include <QTableWidget>
#include <QGraphicsScene>

namespace Ui {
class knockOutSys;
}

class knockOutSys : public QMainWindow
{
    Q_OBJECT

public:
    explicit knockOutSys(QWidget *parent = nullptr);
    ~knockOutSys();

public slots:
    void getTableAndDraw(QTableWidget *table);

private:
    Ui::knockOutSys *ui;
    QGraphicsScene *scene;
};

#endif // KNOCKOUTSYS_H
