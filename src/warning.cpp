#include "warning.h"
#include "ui_warning.h"

warning::warning(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::warning)
{
    ui->setupUi(this);
}

warning::~warning()
{
    delete ui;
}

void warning::errorMessage(QString message)
{
    ui->Error->setText(message);
}

void warning::on_pushButton_clicked()
{
    this->close();
}

