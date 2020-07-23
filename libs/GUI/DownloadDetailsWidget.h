#ifndef DOWNLOADDETAILSWIDGET_H
#define DOWNLOADDETAILSWIDGET_H

#include <QWidget>

namespace Ui {
class DownloadDetailsWidget;
}

class DownloadDetailsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DownloadDetailsWidget(QWidget *parent = nullptr);
    ~DownloadDetailsWidget();

    void setValues(QString filename, QString blobType, double fileSize);

protected:
    void initialize();

    QString downloadURI;
    double fileSize;

private:
    Ui::DownloadDetailsWidget *ui;
};

#endif // DOWNLOADDETAILSWIDGET_H
