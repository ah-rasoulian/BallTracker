#ifndef VIDEOPROCESSOR_H
#define VIDEOPROCESSOR_H

#include <QObject>
#include <QMainWindow>
#include <balltracker.h>
#include <balldetector.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

Q_DECLARE_METATYPE(cv::Rect2d)
Q_DECLARE_METATYPE(cv::Rect)
Q_DECLARE_METATYPE(cv::Rect*)
Q_DECLARE_METATYPE(cv::Mat)


class VideoProcessor : public QObject
{
    Q_OBJECT

public:
    VideoProcessor(std::string cameraPath);
    ~VideoProcessor();

private:
    BallTracker *tracker;
    BallDetector* detector;
    cv::VideoCapture camera;
    cv::Mat frame, hsvFrame, resultFrame, primaryFrame;
    cv::Rect2d ballBox;
    bool stoped;
    bool isBallTracked;
    bool isCapturingActivate;

signals:
    void primaryDisplay(QPixmap pixmap);
    void updateHSVFrame(cv::Mat hsvFrame);
    void updateFrame(cv::Mat frame);
    void sendBox(cv::Rect Box);
    void transmitFirstFrame(cv::Mat firstFrame);

public slots:
    void startVideo();
    void stopVideo();
    void getTrackingBox(cv::Rect2d newBallBox);
    void startCapturing();
    void stopCapturing();
};

#endif // VIDEOPROCESSOR_H
