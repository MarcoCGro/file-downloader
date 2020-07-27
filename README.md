# file-downloader

Qt application used to download a list of files from a URL provided. This application contains a settings dialog
which allows to users define the output directory and change the URL with similar definition for the downloads.

The downloads are shown as a list of widgets (that can be removed as it's needed) with its current progress
and a button which manages the next step to do with the active or complete download.

When a user closes the main application, all current downloads shown are saved to a file on disk. Once user
reopens the application, all of these downloads are recovered depending of theirs last state. So, pending
(paused) downloads can be continued. If a pending download is resumed, and if the server provides/maanages
Accept-Ranges header for http requests, it will continue from the number of received bytes before to pause
it, otherwise the download will be restarted from the beginning.
