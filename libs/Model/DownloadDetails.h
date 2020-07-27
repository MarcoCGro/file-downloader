#ifndef DOWNLOADDETAILS_H
#define DOWNLOADDETAILS_H

#include <QJsonObject>
#include <QString>

/**
 * @brief The DownloadDetails class defines and manages the fields
 * of a download as its state and current number of received bytes.
 *
 * @author Marco A.Caballero Gro.
 * @version 1.0
 */
class DownloadDetails
{
public:
    /**
     * @brief Defines the current state for a download instance
     */
    enum DownloadState {
        NON_STARTED = 0,
        IN_PROGRESS,
        PAUSED,
        FINISHED
    };

    /**
     * @brief Build an instance from the QJsonObject provided
     * after a file reading
     *
     * @param jsonObject Contains the values and current state
     * for this download instance
     *
     * @see MainWindow::loadCurrentDownloads
     */
    DownloadDetails(QJsonObject jsonObject);

    /**
     * @brief Build a complete version of the instance as a json
     * object which can be saves to disk eventually
     *
     * @return QJonObject The final details for this download
     * instance
     */
    QJsonObject getValuesAsJson();

    DownloadState getState() const;
    void setState(const DownloadState &value);

    bool getAcceptRanges() const;
    void setAcceptRanges(bool value);

    int getNumReceivedBytes() const;
    void setNumReceivedBytes(int value);

    QString getOutputFilename() const;
    void setOutputFilename(const QString &path);
    void setFinalFilename(const QString &filename);

    QString getFilename() const;
    void setFilename(const QString &value);

    int getLength() const;
    void setLength(int value);

    QString getBlobType() const;
    void setBlobType(const QString &value);

    QString getDownloadURI() const;
    void setDownloadURI(const QString &value);

    void printData();

protected:
    DownloadState state;
    bool acceptRanges;
    int numReceivedBytes;
    QString outputFilename;

    QString filename;
    double length;
    QString blobType;
    QString downloadURI;
};

#endif // DOWNLOADDETAILS_H
