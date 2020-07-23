#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initialize();

    this->networkRequester = new NetworkRequester(this);
    this->settingsDialog->setNetworkRequester(this->networkRequester);
}

void MainWindow::initialize()
{
    ui->toolBar->setIconSize(QSize(50, 50));
    ui->toolBar->setMovable(false);

    this->settingsDialog = new SettingsDialog(this);

    ui->tableWidget->setColumnCount(1);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->hide();
    ui->tableWidget->verticalHeader()->hide();

    this->downloadsWidgets = new QList<DownloadDetailsWidget*>();
}

void MainWindow::addElementToDownload(QString filename, QString blobType, double fileSize)
{
    DownloadDetailsWidget* curr = new DownloadDetailsWidget;
    curr->setValues(filename, blobType, fileSize);
    this->downloadsWidgets->push_back(curr);

    int k = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(k);
    ui->tableWidget->setRowHeight(k, curr->height());
    ui->tableWidget->setCellWidget(k, 0, curr);
}

void MainWindow::on_actionRequest_triggered()
{
    this->downloadsWidgets->clear();

    this->downloadsDetailsList.clear();
    this->networkRequester->requestFilesDetails(this->settingsDialog->getFilesUrl());

    if (this->networkRequester->isValidRequest()) {
        QJsonArray jsonArray = this->networkRequester->getJsonArray();
        for (int i = 0; i < jsonArray.size(); i++) {
            DownloadDetails currentDetails(jsonArray.at(i).toObject());
            this->downloadsDetailsList.push_back(currentDetails);
            addElementToDownload(currentDetails.getFilename(), currentDetails.getBlobType(), currentDetails.getLength());
        }
    }
}

void MainWindow::on_actionSettings_triggered()
{
    this->settingsDialog->show();
}

MainWindow::~MainWindow()
{
    delete ui;

    if (this->downloadsWidgets != nullptr) {
        this->downloadsWidgets->clear();
        delete this->downloadsWidgets;
        this->downloadsWidgets = nullptr;
    }

    if (this->networkRequester != nullptr) {
        delete this->networkRequester;
        this->networkRequester = nullptr;
    }
}
