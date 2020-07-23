#ifndef DOWNLOADDETAILSWIDGET_H
#define DOWNLOADDETAILSWIDGET_H

#include <QWidget>
#include "libs/Web/DownloadDetails.h"

namespace Ui {
class DownloadDetailsWidget;
}

class DownloadDetailsWidget : public QWidget
{
    Q_OBJECT

public:
    enum DownloadState {
        NON_STARTED = 0,
        IN_PROGRESS,
        PAUSED,
        FINISHED
    };

    explicit DownloadDetailsWidget(QWidget *parent = nullptr);
    ~DownloadDetailsWidget();

    void setValues(DownloadDetails downloadDetails);

protected:
    void initialize();

    void startDownload();
    void pauseDownload();
    void resumeDownload();

    DownloadDetails downloadDetails;
    DownloadState currentState;

private slots:
    void on_stateButton_pressed();

private:
    Ui::DownloadDetailsWidget *ui;
};

#endif // DOWNLOADDETAILSWIDGET_H
