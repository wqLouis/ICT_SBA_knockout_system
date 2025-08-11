#ifndef LOADFILE_H
#define LOADFILE_H

#include <QWidget>

namespace Ui {
class loadfile;
}

class loadfile : public QWidget
{
    Q_OBJECT

public:
    explicit loadfile(QWidget *parent = nullptr);
    ~loadfile();

private:
    Ui::loadfile *ui;
};

#endif // LOADFILE_H
