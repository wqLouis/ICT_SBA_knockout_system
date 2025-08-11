//Qt
#include "input.h"
#include "warning.h"
#include "mainwindow.h"
#include "ui_input.h"
#include <QTableWidget>
#include <QVector>

//QXlsx
#include "xlsxdocument.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"

input::input(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::input)
{
    ui->setupUi(this);
}

input::~input()
{
    delete ui;
}

using namespace std;
using namespace QXlsx;

// Declare vars
QTableWidget *table = nullptr;
int seedCount = 0;
QVector<QString> schoolList;
QVector<bool> seedList;

void input::on_add_clicked()
{
    QString name, school, isSeedStr;
    bool isSeed, validInput;
    validInput = true;
    table = ui->tableWidget;
    table->setEditTriggers(QAbstractItemView::NoEditTriggers); // Disable direct edit for users

    name = ui->Name->text();
    school = ui->Sku->text();
    isSeed = ui->seedplayer->isChecked();

    if (isSeed){
        isSeedStr = "true";
    }
    else{
        isSeedStr = "false";
    }

    int seedCount = 0;
    for (int i = 0; i < seedList.size(); i++){
        if (seedList[i]){
            seedCount ++;
        }
    }

    if (seedCount == 4 && isSeed){//check seed player
        warning invalidinput;
        invalidinput.errorMessage("More than 4 seed players\nAre you sure?");
        invalidinput.exec();
        validInput = false;
    }

    int schoolCount = 0;
    for (int i = 0; i < schoolList.size(); i++){
        if (school == schoolList[i]){
            schoolCount ++;
        }
    }

    if (schoolCount == 2){
        warning invalidinput;
        invalidinput.errorMessage("More than 2 players from same school\nAre you sure?");
        invalidinput.exec();
        validInput = false;
    }

    if (school.isEmpty() || name.isEmpty() || school == "" || name == ""){//check empty field
        warning invalidinput;
        invalidinput.errorMessage("Error in input\nThere may be empty field");
        invalidinput.exec();
        validInput = false;
    }

    if (validInput){//add to table when input valid
        table->setRowCount(table->rowCount()+1);

        int rowNo = table->rowCount()-1;
        table->setItem(rowNo, 0, new QTableWidgetItem(QString::number(rowNo)));
        table->setItem(rowNo, 1, new QTableWidgetItem(name));
        table->setItem(rowNo, 2, new QTableWidgetItem(school));
        table->setItem(rowNo, 3, new QTableWidgetItem(isSeedStr));

        schoolList.push_back(school);
        seedList.push_back(isSeed);
    }
}

void input::on_delete_2_clicked()
{
    QTableWidget *table = ui->tableWidget;
    if (table->rowCount() > 0){
        table->setRowCount(table->rowCount()-1);
        schoolList.pop_back();
        seedList.pop_back();
    }
}

void input::on_ClearTable_clicked()
{
    QTableWidget *table = ui->tableWidget;
    table->setRowCount(0);
    schoolList.clear();
    seedList.clear();
}

void input::on_genXlsx_clicked()
{
    QTableWidget *table = ui->tableWidget;
    Document genedFile;

    genedFile.write(1,1,"ID");
    genedFile.write(1,2,"Name");
    genedFile.write(1,3,"School");
    genedFile.write(1,4,"Is seed player?");

    for (int i = 0;i < table->rowCount(); i++){
        for (int j = 0; j < 4; j++){
            //QXlsx excel file starts with 1,1 not 0,0
            if (j == 0){
                genedFile.write(i+2 ,j+1 , table->item(i,j)->text().toInt());
            }
            else{
                genedFile.write(i+2 ,j+1 , table->item(i,j)->text().toStdString().c_str());
            }
        }
    }

    genedFile.saveAs(QDateTime::currentDateTime().toString("ddMMyyyyhhmmss").append(".xlsx"));
}
void input::on_saveQuit_clicked()
{
    if (table == nullptr){
        warning noTable;
        noTable.errorMessage("There is nothing/only 1 in the table\nAre you sure to quit?");
        noTable.exec();
        return;
    }
    if (table->rowCount() > 1){
        emit saveTableToMain(table);
    }
    else{
        warning noTable;
        noTable.errorMessage("There is nothing/only 1 in the table\nAre you sure to quit?");
        noTable.exec();
    }
    schoolList.clear();
    seedList.clear();
    this->close();
}
