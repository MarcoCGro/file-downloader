#ifndef DOWNLOADDETAILSWIDGET_H
#define DOWNLOADDETAILSWIDGET_H

#include <QWidget>

#include "libs/Web/FileDownloader.h"
#include "libs/Model/DownloadDetails.h"

namespace Ui {
class DownloadDetailsWidget;
}

/**
 * @brief The DownloadDetailsWidget class keeps the graphical definition
 * for an dowload and shows its current state; also, a progress bar is
 * used when the download is still active.
 *
 * @author Marco A.Caballero Gro.
 * @version 1.0
 *
 * @see MainWindow
 */
class DownloadDetailsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DownloadDetailsWidget(QWidget *parent = nullptr);
    ~DownloadDetailsWidget();

    /**
     * @brief When a row is created inside downloads table, this function
     * should be invoked to link with its corresponding details
     *
     * @param widgetId Identifier of this widget which matchs with the
     * row number in the current downloads table
     * @param downloadDetails Properties/Details for the current download
     *
     * @see MainWindow::addElementToDownload
     */
    void setValues(int widgetId, DownloadDetails *downloadDetails);
    /**
     * @brief Updates the identifier for this instance; it's used when
     * another instance of this class is removed from the current
     * downloads table
     *
     * @param id Identifier of the current download
     *
     * @see MainWindow::removeDownloadWidget, removeWidget, on_removeButton_clicked
     */
    void updateId(int id);

    /**
     * @brief Returns the properties/details of the download as
     * a json object
     *
     * @return QJsonObject Qt representation of a json
     *
     * @see DownloadDetails::getValuesAsJson
     */
    QJsonObject getValuesAsJson();

    /**
     * @brief Set the corresponding graphical values from an active
     * download which can be resumed it when the user pushs the
     * button again
     *
     * @see FileDownloader::recoverDownload
     */
    void recoverDownload();
    /**
     * @brief Pause (and release the controller) for the current
     * download when the user closes the application
     *
     * @see MainWindow::closeEvent, FileDownloader::pauseDownload
     */
    void releaseDownload();

signals:
    void removeWidget(int id);

protected:
    void initialize();

    void startDownload();
    void pauseDownload();
    void resumeDownload();
    void openDownload();

    FileDownloader *fileDownloader;
    DownloadDetails *downloadDetails;

    int widgetId;

protected slots:
    void on_removeButton_clicked();
    void on_stateButton_pressed();

    void updateProgress(double bytesReceived);
    void finishDownload();

    void recoverDownloadMessage();

private:
    QString getBytesLabel(double numBytes);

    Ui::DownloadDetailsWidget *ui;
};

#endif // DOWNLOADDETAILSWIDGET_H
