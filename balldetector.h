#ifndef ROBOTDETECTOR_H
#define ROBOTDETECTOR_H

#include <QObject>

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/opencv.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

class BallDetector : public QObject
{
    Q_OBJECT

public:
    BallDetector();

private:
    bool stoped;
    bool isHSVFrameAvailable;
    cv::Mat hsvFrame;

signals:
    void detectingBox(cv::Rect robotBox);

public slots:
    void startDetecting();
    void stopDetecting();
    void getHSVFrame(cv::Mat newHSVFrame);
};

#endif // ROBOTDETECTOR_H
