#include "videoprocessor.h"
#include <QThread>

VideoProcessor::VideoProcessor(std::string cameraPath)
{
    qRegisterMetaType<cv::Rect2d>();
    qRegisterMetaType<cv::Rect>();
    qRegisterMetaType<cv::Rect*>();
    qRegisterMetaType<cv::Mat>();
    isBallTracked = false;
    isCapturingActivate = false;


    bool isCamera;
    int cameraNumber = QString::fromStdString(cameraPath).toInt(&isCamera);

    if(isCamera)
        camera.open(cameraNumber, cv::CAP_DSHOW);
    else
        camera.open(cameraPath);

    tracker = new BallTracker();
    tracker->moveToThread(new QThread());
    connect(tracker->thread(), &QThread::started, tracker, &BallTracker::startTracking);
    connect(tracker->thread(), &QThread::finished, tracker, &BallTracker::deleteLater);
    connect(tracker, &BallTracker::trackingBox, this, &VideoProcessor::getTrackingBox, Qt::DirectConnection);
    connect(this, &VideoProcessor::updateFrame , tracker, &BallTracker::getFrame, Qt::DirectConnection);
    tracker->thread()->start(QThread::LowPriority);

    detector = new BallDetector();
    detector->moveToThread(new QThread());
    connect(detector->thread(), &QThread::started, detector, &BallDetector::startDetecting);
    connect(detector->thread(), &QThread::finished, detector, &BallDetector::deleteLater);
    connect(detector, &BallDetector::detectingBox, tracker, &BallTracker::getDetectingBox, Qt::DirectConnection);
    connect(this, &VideoProcessor::updateHSVFrame , detector, &BallDetector::getHSVFrame, Qt::DirectConnection);
    detector->thread()->start(QThread::LowPriority);
}

VideoProcessor::~VideoProcessor(){
    tracker->stopTracking();
    tracker->thread()->exit();
    tracker->thread()->wait();

    detector->stopDetecting();
    detector->thread()->exit();
    detector->thread()->wait();
}

void VideoProcessor::startVideo(){
    using namespace cv;
    Mat frame1;
    stoped = false;
    while(camera.isOpened() && !stoped){
        camera >> frame1;

        if(frame1.empty())
            continue;

        if(frame1.size().width < frame1.size().height)
            resize(frame1, frame, Size(frame1.size().width * 800 / frame1.size().height, 800), 0, 0, INTER_CUBIC);
        else
            resize(frame1, frame, Size(1600, frame1.size().height * 1600 / frame1.size().width), 0, 0, INTER_CUBIC);

        cv::cvtColor(frame, hsvFrame, cv::COLOR_BGR2HSV);
        emit updateFrame(frame);
        emit updateHSVFrame(hsvFrame);

        if (isCapturingActivate) {
            emit sendBox(ballBox);
        }

        // proccessing
        if(isBallTracked)
            cv::rectangle(frame, ballBox , cv::Scalar(255, 0, 0), 2);

        // displaying
        emit primaryDisplay(
                    QPixmap::fromImage(
                              QImage(static_cast<uchar*>(frame.data), frame.cols, frame.rows, static_cast<int>(frame.step), QImage::Format_RGB888)
                              .rgbSwapped()
                        )
                    );
    }
    camera.release();
}

void VideoProcessor::stopVideo(){
    stoped = true;
}

void VideoProcessor::getTrackingBox(cv::Rect2d newBallBox){
    ballBox = newBallBox;
    isBallTracked = true;
}

void VideoProcessor::startCapturing(){
    isCapturingActivate = true;
    emit transmitFirstFrame(frame);
}

void VideoProcessor::stopCapturing(){
    isCapturingActivate = false;
}
