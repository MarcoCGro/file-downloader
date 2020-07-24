#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initialize();

    this->endpointRequester = new EndpointRequester(this);
    this->settingsDialog->setEndpointRequester(this->endpointRequester);
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

void MainWindow::addElementToDownload(DownloadDetails *downloadDetails)
{
    DownloadDetailsWidget* currDownloadWidget = new DownloadDetailsWidget;
    currDownloadWidget->setValues(downloadDetails);
    this->downloadsWidgets->push_back(currDownloadWidget);

    int k = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(k);
    ui->tableWidget->setRowHeight(k, currDownloadWidget->height());
    ui->tableWidget->setCellWidget(k, 0, currDownloadWidget);
}

void MainWindow::on_actionRequest_triggered()
{
    this->downloadsWidgets->clear();
    this->endpointRequester->requestFilesDetails(this->settingsDialog->getFilesUrl());

    if (this->endpointRequester->isValidRequest()) {
        QJsonArray jsonArray = this->endpointRequester->getJsonArray();
        for (int i = 0; i < jsonArray.size(); i++) {
            DownloadDetails *currentDetails = new DownloadDetails(jsonArray.at(i).toObject());
            currentDetails->setOutputFilename(this->settingsDialog->getDownloadsDirectory());
            addElementToDownload(currentDetails);
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

    if (this->endpointRequester != nullptr) {
        delete this->endpointRequester;
        this->endpointRequester = nullptr;
    }
}
