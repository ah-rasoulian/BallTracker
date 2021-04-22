#ifndef CHOSENCAMERABROADCASTING_H
#define CHOSENCAMERABROADCASTING_H

#include <QObject>
#include <QMainWindow>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

class chosenCameraBroadcasting : public QObject
{
    Q_OBJECT
public:
    chosenCameraBroadcasting(cv::VideoCapture newCamera);
private:
    cv::Mat frame;
    cv::VideoCapture camera;
    bool stoped;

public slots:
    void startShowing();
    void stopShowing();

signals:
        void chosenCameraCheckShow(QPixmap pixmap);
};

#endif // CHOSENCAMERABROADCASTING_H
