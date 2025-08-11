#ifndef WARNING_H
#define WARNING_H

#include <QDialog>

namespace Ui {
class warning;
}

class warning : public QDialog
{
    Q_OBJECT

public:
    explicit warning(QWidget *parent = nullptr);
    ~warning();

private:
    Ui::warning *ui;

public slots:
    void errorMessage(QString message);
private slots:
    void on_pushButton_clicked();
};


#endif // WARNING_H
