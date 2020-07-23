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

    this->fileSize = 0.0;
    this->downloadURI = "";
}

void DownloadDetailsWidget::setValues(QString filename, QString blobType, double fileSize)
{
    ui->filenameLabel->setText(filename);

    ui->blobTypeLabel->setText(blobType);
    ui->rateLabel->setText("0 bytes/s");

    this->fileSize = fileSize;

    double bValue = 0.0;
    QString bUnit;
    if (this->fileSize < 1024) {
        bValue = this->fileSize;
        bUnit = "bytes";
    }
    else if (this->fileSize < pow(1024, 2)) {
        bValue = this->fileSize / 1024;
        bUnit = "kB";
    }
    else if (this->fileSize < pow(1024, 3)) {
        bValue = this->fileSize / pow(1024, 2);
        bUnit = "MB";
    }
    else {
        bValue = this->fileSize / pow(1024, 3);
        bUnit = "GB";
    }

    ui->lengthLabel->setText(QString::number(bValue, 'f', 2) + " " + bUnit);
}

DownloadDetailsWidget::~DownloadDetailsWidget()
{
    delete ui;
}
