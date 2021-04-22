#ifndef ROBOTTRACKER_H
#define ROBOTTRACKER_H

#include <QObject>

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/opencv.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

class BallTracker : public QObject
{
    Q_OBJECT

public:
    BallTracker();

private:
    bool stoped;
    cv::Mat frame;
    cv::Rect initialBox ;
    bool isFrameAvailable;
    bool isInitialBoxReceived;
    bool isBallTracked ;
    cv::Ptr<cv::Tracker> trackerFunction;

signals:
    void trackingBox(cv::Rect2d robotBox);

public slots:
    void startTracking();
    void stopTracking();
    void getFrame(cv::Mat newFrame);
    void getDetectingBox(cv::Rect newInitialBox);
};

#endif // ROBOTTRACKER_H
