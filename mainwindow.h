#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <videoprocessor.h>
#include <chosencamerabroadcasting.h>
#include <pathcapture.h>
#include <QThread>
#include <QTimer>
#include <QtPlugin>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

class VideoProcessor;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pathButton_clicked();

    void on_goBackButton_clicked();

    void on_resultPage_clicked();

    void on_nextButton_clicked();

    void on_checkButton_clicked();

private:
    VideoProcessor *processor;
    PathCapture *capture;
    chosenCameraBroadcasting *chosenCameraShow;
    Ui::MainWindow *ui;
    QTimer* captureTimer;
    std::string cameraPath;
    int captureInterval;
    void endConstrutor();
    bool isChosenCameraShowExist;
    cv::VideoCapture chosenCamera;


signals:
    void startCapturing();
    void stopCapturing();
};

#endif // MAINWINDOW_H
