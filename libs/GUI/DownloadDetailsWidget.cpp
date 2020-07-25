#include "DownloadDetailsWidget.h"
#include "ui_DownloadDetailsWidget.h"

#include <QDesktopServices>
#include <qmath.h>

DownloadDetailsWidget::DownloadDetailsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DownloadDetailsWidget)
{
    ui->setupUi(this);
    initialize();
}

DownloadDetailsWidget::~DownloadDetailsWidget()
{
    delete ui;

    if (this->fileDownloader != nullptr) {
        delete this->fileDownloader;
        this->fileDownloader = nullptr;
    }
}

void DownloadDetailsWidget::initialize()
{
    this->resize(this->width(), 90);

    this->widgetId = 0;
    ui->removeButton->setIcon(ui->actionRemove->icon());

    ui->filenameLabel->setText("[filename]");
    ui->blobTypeLabel->setText("[type]");
    ui->rateLabel->setText("[rate]");

    ui->rateLabel->hide();

    ui->progressBar->setValue(0);
    ui->progressBar->hide();

    ui->messagesLabel->setText("");
    ui->messagesLabel->hide();

    this->fileDownloader = new FileDownloader(this);
    this->downloadDetails = nullptr;

    connect(this->fileDownloader, &FileDownloader::updateProgress, this, &DownloadDetailsWidget::updateProgress);
    connect(this->fileDownloader, &FileDownloader::recoverProgress, this, &DownloadDetailsWidget::recoverDownloadMessage);
    connect(this->fileDownloader, &FileDownloader::finished, this, &DownloadDetailsWidget::finishDownload);
}

void DownloadDetailsWidget::setValues(int widgetId, DownloadDetails *downloadDetails)
{
    this->widgetId = widgetId;
    this->downloadDetails = downloadDetails;

    ui->filenameLabel->setText(this->downloadDetails->getFilename());

    ui->blobTypeLabel->setText(this->downloadDetails->getBlobType());
    ui->rateLabel->setText("0 bytes");

    double fileSize = this->downloadDetails->getLength();

    ui->lengthLabel->setText(getBytesLabel(fileSize));
    ui->progressBar->setRange(0, this->downloadDetails->getLength());
}

void DownloadDetailsWidget::updateId(int id)
{
    this->widgetId = id;
}

QJsonObject DownloadDetailsWidget::getValuesAsJson()
{
    return this->downloadDetails->getValuesAsJson();
}

void DownloadDetailsWidget::releaseDownload()
{
    if (this->downloadDetails->getState() == DownloadDetails::DownloadState::IN_PROGRESS)
        this->pauseDownload();
}

void DownloadDetailsWidget::on_removeButton_clicked()
{
    emit removeWidget(this->widgetId);
}

void DownloadDetailsWidget::on_stateButton_pressed()
{

    if (this->downloadDetails->getState() == DownloadDetails::DownloadState::NON_STARTED)
        startDownload();
    else if (this->downloadDetails->getState() == DownloadDetails::DownloadState::IN_PROGRESS)
        pauseDownload();
    else if (this->downloadDetails->getState() == DownloadDetails::DownloadState::PAUSED)
        resumeDownload();
    else if (this->downloadDetails->getState() == DownloadDetails::DownloadState::FINISHED)
        openDownload();
}

void DownloadDetailsWidget::updateProgress(int bytesReceived)
{
    if (!ui->messagesLabel->isHidden())
        ui->messagesLabel->hide();

    if (!ui->stateButton->isEnabled())
        ui->stateButton->setEnabled(true);

    ui->progressBar->setValue(bytesReceived);
    ui->rateLabel->setText(getBytesLabel(bytesReceived));
    ui->rateLabel->show();

    this->downloadDetails->setNumReceivedBytes(bytesReceived);
}

void DownloadDetailsWidget::recoverDownloadMessage()
{
    ui->progressBar->hide();
    ui->messagesLabel->setText("Please, wait, your download is being recovered...");
    ui->messagesLabel->show();
}

void DownloadDetailsWidget::recoverDownload()
{
    if (this->downloadDetails->getState() == DownloadDetails::DownloadState::PAUSED) {
        ui->stateButton->setText("Resume");
    }
    else if (this->downloadDetails->getState() == DownloadDetails::DownloadState::FINISHED) {
        ui->stateButton->setText("Open");
        return;
    }
    else {
        return;
    }

    this->fileDownloader->recoverDownload(this->downloadDetails);
}

void DownloadDetailsWidget::startDownload()
{
    if (!ui->messagesLabel->isHidden())
        ui->messagesLabel->hide();

    ui->stateButton->setText("Pause");
    ui->progressBar->show();

    this->downloadDetails->setState(DownloadDetails::DownloadState::IN_PROGRESS);
    this->fileDownloader->startDownload(this->downloadDetails);
}

void DownloadDetailsWidget::pauseDownload()
{
    ui->stateButton->setText("Resume");
    this->downloadDetails->setState(DownloadDetails::DownloadState::PAUSED);

    this->fileDownloader->pauseDownload();
}

void DownloadDetailsWidget::resumeDownload()
{
    ui->stateButton->setText("Pause");
    ui->stateButton->setEnabled(false);

    if (ui->progressBar->isHidden()) {
        ui->progressBar->setValue(this->downloadDetails->getNumReceivedBytes());
        ui->progressBar->show();
    }

    this->downloadDetails->setState(DownloadDetails::DownloadState::IN_PROGRESS);

    this->fileDownloader->resumeDownload();
}

void DownloadDetailsWidget::openDownload()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(this->downloadDetails->getOutputFilename()));
}

void DownloadDetailsWidget::finishDownload()
{
    if (this->fileDownloader->isValidRequest()) {
        ui->stateButton->setText("Open");
        this->downloadDetails->setState(DownloadDetails::DownloadState::FINISHED);
    }
    else {
        ui->stateButton->setText("Download");
        ui->stateButton->setEnabled(true);

        ui->messagesLabel->setText(this->fileDownloader->getCurrentMessage());
        ui->messagesLabel->show();

        this->downloadDetails->setState(DownloadDetails::DownloadState::NON_STARTED);
    }

    ui->progressBar->hide();
    ui->rateLabel->hide();
}

QString DownloadDetailsWidget::getBytesLabel(double numBytes)
{
    double bValue = 0.0;
    QString bUnit;

    if (numBytes < 1024) {
        bValue = numBytes;
        bUnit = "bytes";
    }
    else if (numBytes < pow(1024, 2)) {
        bValue = numBytes / 1024;
        bUnit = "kB";
    }
    else if (numBytes < pow(1024, 3)) {
        bValue = numBytes / pow(1024, 2);
        bUnit = "MB";
    }
    else {
        bValue = numBytes / pow(1024, 3);
        bUnit = "GB";
    }

    return QString::number(bValue, 'f', 2) + " " + bUnit;
}
