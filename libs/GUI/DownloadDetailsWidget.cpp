#include "DownloadDetailsWidget.h"
#include "ui_DownloadDetailsWidget.h"

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

void DownloadDetailsWidget::setValues(QString filename, QString blobType, QString fileSize, QString downloadURI)
{
    ui->filenameLabel->setText(filename);
    ui->blobTypeLabel->setText(blobType);
    ui->rateLabel->setText("0 bytes/s");

    this->fileSize = fileSize.toDouble();
    this->downloadURI = downloadURI;
}

DownloadDetailsWidget::~DownloadDetailsWidget()
{
    delete ui;
}
