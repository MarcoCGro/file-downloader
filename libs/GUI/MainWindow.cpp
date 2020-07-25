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
    this->jsonValidator = new JsonValidator(JSON_EXTENDED_FIELDS);

    loadCurrentDownloads();
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

void MainWindow::loadCurrentDownloads()
{
    QFile file(this->preferencesFilename);

    if (file.exists()) {
        if (!file.open(QIODevice::ReadOnly))
             return;

        QString content = file.readAll();

        bool isJsonValid = false;
        QJsonObject jObject = this->jsonValidator->jsonFromExtendedContent(content, &isJsonValid);

        if (isJsonValid) {
            this->settingsDialog->setFilesUrl(jObject.find("DownloadsURL").value().toString());
            this->settingsDialog->setDownloadsDirectory(jObject.find("DownloadsDirectory").value().toString());

            setDownloads(jObject.find("Downloads").value().toArray());
            recoverCurrentDownloads();
        }
        else {
            showMessage("Something was wrong when current downloads were loaded. Settings file content is invalid.", QMessageBox::Critical);
        }
    }
}

void MainWindow::setDownloads(QJsonArray jsonArray)
{
    for (int i = 0; i < jsonArray.size(); i++) {
        DownloadDetails *currentDetails = new DownloadDetails(jsonArray.at(i).toObject());
        currentDetails->setOutputFilename(this->settingsDialog->getDownloadsDirectory());
        addElementToDownload(currentDetails);
    }
}

void MainWindow::addElementToDownload(DownloadDetails *downloadDetails)
{
    int k = ui->tableWidget->rowCount();

    DownloadDetailsWidget *currDownloadWidget = new DownloadDetailsWidget;
    currDownloadWidget->setValues(k, downloadDetails);
    this->downloadsWidgets->push_back(currDownloadWidget);

    ui->tableWidget->insertRow(k);
    ui->tableWidget->setRowHeight(k, currDownloadWidget->height());
    ui->tableWidget->setCellWidget(k, 0, currDownloadWidget);

    connect(currDownloadWidget, &DownloadDetailsWidget::removeWidget, this, &MainWindow::removeDownloadWidget);
}

void MainWindow::recoverCurrentDownloads()
{
    for (int i = 0; i < this->downloadsWidgets->count(); i++)
        (this->downloadsWidgets->at(i))->recoverDownload();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveCurrentDownloads();
    event->accept();
}

void MainWindow::saveCurrentDownloads()
{
    QJsonObject preferences;

    preferences.insert("DownloadsDirectory", this->settingsDialog->getDownloadsDirectory());
    preferences.insert("DownloadsURL", this->settingsDialog->getFilesUrl());

    QJsonArray jsonArray;

    for (int i = 0; i < this->downloadsWidgets->size(); i++) {
        this->downloadsWidgets->at(i)->releaseDownload();
        QJsonObject jsonObject = this->downloadsWidgets->at(i)->getValuesAsJson();
        jsonArray.insert(i, jsonObject);
    }

    preferences.insert("Downloads", jsonArray);

    QJsonDocument doc(preferences);
    QString strJson(doc.toJson(QJsonDocument::Compact));

    QFile file(this->preferencesFilename);
    if (!file.open(QIODevice::WriteOnly))
        return;

    file.write(strJson.toStdString().data());

    file.flush();
    file.close();
}

void MainWindow::on_actionRequest_triggered()
{
    this->endpointRequester->requestFilesDetails(this->settingsDialog->getFilesUrl());

    if (this->endpointRequester->isValidRequest()) {
        QJsonArray jsonArray = this->endpointRequester->getJsonArray();
        setDownloads(jsonArray);
    }
    else {
        showMessage("Something went wrong. Please verify the specified URL clicking on Settings and try again.", QMessageBox::Critical);
    }
}

void MainWindow::on_actionSettings_triggered()
{
    this->settingsDialog->show();
}

void MainWindow::removeDownloadWidget(int widgetId)
{
    for (int i = widgetId; i < this->downloadsWidgets->size(); i++)
        (this->downloadsWidgets->at(i))->updateId(widgetId);

    (this->downloadsWidgets->at(widgetId))->releaseDownload();
    disconnect(this->downloadsWidgets->at(widgetId), &DownloadDetailsWidget::removeWidget, this, &MainWindow::removeDownloadWidget);

    this->downloadsWidgets->removeAt(widgetId);
    ui->tableWidget->removeRow(widgetId);
}

void MainWindow::showMessage(QString message, QMessageBox::Icon msgType)
{
    this->messageBox.setIcon(msgType);
    this->messageBox.setText(message);
    this->messageBox.show();
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
