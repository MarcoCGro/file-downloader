#include "DownloadDetailsWidget.h"
#include "ui_DownloadDetailsWidget.h"

#include <qmath.h>

DownloadDetailsWidget::DownloadDetailsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DownloadDetailsWidget)
{
    ui->setupUi(this);
    initialize();
}

void DownloadDetailsWidget::initialize()
{
    this->resize(this->width(), 80);

    ui->filenameLabel->setText("[filename]");
    ui->blobTypeLabel->setText("[type]");
    ui->rateLabel->setText("[rate]");

    ui->rateLabel->hide();

    ui->progressBar->setValue(0);
    ui->progressBar->hide();

    ui->messagesLabel->setText("");
    ui->messagesLabel->hide();

    this->currentState = DownloadState::NON_STARTED;
}

void DownloadDetailsWidget::setValues(DownloadDetails downloadDetails)
{
    this->downloadDetails = downloadDetails;

    ui->filenameLabel->setText(this->downloadDetails.getFilename());

    ui->blobTypeLabel->setText(this->downloadDetails.getBlobType());
    ui->rateLabel->setText("0 bytes/s");

    double fileSize = this->downloadDetails.getLength();

    double bValue = 0.0;
    QString bUnit;
    if (fileSize < 1024) {
        bValue = fileSize;
        bUnit = "bytes";
    }
    else if (fileSize < pow(1024, 2)) {
        bValue = fileSize / 1024;
        bUnit = "kB";
    }
    else if (fileSize < pow(1024, 3)) {
        bValue = fileSize / pow(1024, 2);
        bUnit = "MB";
    }
    else {
        bValue = fileSize / pow(1024, 3);
        bUnit = "GB";
    }

    ui->lengthLabel->setText(QString::number(bValue, 'f', 2) + " " + bUnit);
}

DownloadDetailsWidget::~DownloadDetailsWidget()
{
    delete ui;
}

void DownloadDetailsWidget::on_stateButton_pressed()
{
    if (this->currentState == DownloadState::NON_STARTED)
        startDownload();
    else if (this->currentState == DownloadState::IN_PROGRESS)
        pauseDownload();
    else if (this->currentState == DownloadState::PAUSED)
        resumeDownload();
}

void DownloadDetailsWidget::startDownload()
{
    this->currentState = DownloadState::IN_PROGRESS;
    ui->stateButton->setText("Pause");
}

void DownloadDetailsWidget::pauseDownload()
{
    this->currentState = DownloadState::PAUSED;
    ui->stateButton->setText("Resume");
}

void DownloadDetailsWidget::resumeDownload()
{
    this->currentState = DownloadState::IN_PROGRESS;
    ui->stateButton->setText("Pause");
}
