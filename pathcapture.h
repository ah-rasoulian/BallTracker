#ifndef PATHCAPTURE_H
#define PATHCAPTURE_H

#include <QObject>
#include <QMainWindow>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <math.h>
#include <mutex>

class PathCapture : public QObject
{
    Q_OBJECT
public:
    PathCapture();

private:
    cv::Rect ballBox;
    cv::Mat firstFrame;
    cv::Mat frameRecieved;
    cv::Point srcPoint;
    cv::Point dstPoint;
    bool stoped;
    bool srcPointIsNull;
    bool dstPointIsNull;
    std::mutex frameLock;

public slots:
    void recieveBox(cv::Rect newRobotBox);
    void determinePath();
    void recieveFirstFrame(cv::Mat firstFrameRecieved);
    void startShowing();
    void stopShowing();

signals:
    void resultDisplay(QPixmap pixmap);
    void positionAnnouncment(QString position);
};

#endif // PATHCAPTURE_H
