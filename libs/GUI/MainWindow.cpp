#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initialize();
}

void MainWindow::initialize()
{
    ui->tableWidget->setColumnCount(1);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->hide();
    ui->tableWidget->verticalHeader()->hide();

    this->downloadsDetails = new QList<DownloadDetailsWidget*>();
}

void MainWindow::addElementToDownload(QString filename, QString blobType, QString fileSize, QString downloadURI)
{
    DownloadDetailsWidget* curr = new DownloadDetailsWidget;
    curr->setValues(filename, blobType, fileSize, downloadURI);
    this->downloadsDetails->push_back(curr);

    int k = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(k);
    ui->tableWidget->setRowHeight(k, curr->height());
    ui->tableWidget->setCellWidget(k, 0, curr);
}

MainWindow::~MainWindow()
{
    delete ui;

    if (this->downloadsDetails != 0) {
        this->downloadsDetails->clear();
        this->downloadsDetails->~QList();
        this->downloadsDetails = 0;
    }
}
