#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "help.h"
#include "input.h"
#include "warning.h"
#include "knockoutsys.h"

#include <QTableWidget>
#include <QFileDialog>
#include <QApplication>

//QXlsx
#include "xlsxdocument.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"

using namespace QXlsx;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Declear vars
QTableWidget *inputtedTable = nullptr;
QTableWidget *fileTab = nullptr;

input *inputPage;

void MainWindow::on_help_clicked()
{
    //Cannot directly create object as it destroies itself when out of scope
    //Declear as global var prevent user click multiple times
    static help *helpPage = new help;
    if (!helpPage->isVisible()){
        helpPage->show();
    }
}


void MainWindow::on_start_clicked()
{
    if (inputtedTable == nullptr || inputtedTable->item(0,0)->text() == ""){
        warning NoInputted;
        NoInputted.errorMessage("Seems you have not input the data yet\nPlease open input tab/import tab");
        NoInputted.exec();
        return;
    }
    static knockOutSys *startPage = new knockOutSys;
    if (!startPage->isVisible()){
        connect(this, &MainWindow::sendTable, startPage, &knockOutSys::getTableAndDraw);
        emit sendTable(inputtedTable);
        startPage->show();
    }
}


void MainWindow::on_input_clicked()
{
    inputPage = new input;
    if (!inputPage->isVisible()){
        inputPage->show();
        connect(inputPage, &input::saveTableToMain, this, &MainWindow::inputPage_table);
    }
}

void MainWindow::on_Import_clicked()
{
    warning BeforeYouKnow;
    BeforeYouKnow.errorMessage("Input directly from .xlsx file assumed the \ndata is inputted correctly.\nThereforce, unexpected results may happen.");
    BeforeYouKnow.exec();
    fileTab = new QTableWidget;
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this, tr("Open Spreadsheet"), "/home/", tr("SpreadSheet (*.xlsx)"));
    Document doc(fileName);
    if (!doc.load()){
        warning CantLoad;
        CantLoad.errorMessage("Error happened when loading the file\n please try again");
        CantLoad.exec();
        return;
    }

    int NoRow = 0;

    while (true){
        if (doc.read(NoRow + 2, 1).toString() == ""){
            fileTab->setRowCount(NoRow);
            break;
        }
        NoRow ++;
    }
    fileTab->setColumnCount(4);

    int i = 2;
    for (int j = 0; j < NoRow; j++){
        if (doc.read(i, 2).toString() == "" ||
            doc.read(i, 3).toString() == "" ||
            doc.read(i, 4).toString() == "" ||
            doc.read(i, 1).toInt() != j){
            warning formatError;
            formatError.errorMessage("Error file format incorrect\nTry use input page to generate a tamplate");
            formatError.exec();
            return;
        }
        fileTab->setItem(j, 0, new QTableWidgetItem(QString::number(j)));
        fileTab->setItem(j, 1, new QTableWidgetItem(doc.read(i, 2).toString()));
        fileTab->setItem(j, 2, new QTableWidgetItem(doc.read(i, 3).toString()));
        fileTab->setItem(j, 3, new QTableWidgetItem(doc.read(i, 4).toString()));
        i++;
    }
    if (fileTab->rowCount() < 1){
        warning noTable;
        noTable.errorMessage("There is nothing/only 1 in the table\nAre you sure to quit?");
        noTable.exec();
    }
    else{
        warning readSuc;
        readSuc.errorMessage("Input succeeded\n"+fileName);
        readSuc.exec();
        inputtedTable = fileTab;
    }
}

void MainWindow::inputPage_table(QTableWidget *table){
    inputtedTable = table;
}
