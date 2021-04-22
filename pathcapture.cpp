#include "pathcapture.h"
#include <QThread>

PathCapture::PathCapture()
{
    srcPointIsNull = true;
    dstPointIsNull = true;
}

void PathCapture::determinePath() {
    using namespace cv;
    if(ballBox.empty())
        return;

    cv::Point p(static_cast<int>(ballBox.x) + static_cast<int>(ballBox.width / 2), static_cast<int>(ballBox.y) + static_cast<int>(ballBox.height / 2));
    if(!firstFrame.empty()){
        double x = p.x - firstFrame.size().width/2 , y = firstFrame.size().height/2 - p.y;
        double angle = atan2(y, x) * (180.0/3.141592653589793238463) ;
        emit positionAnnouncment(
                QString("x = " ) + QString::number(x) + QString("\ty = ") + QString::number(y)
                    + QString("\tangle = ") + QString::number(angle)
                );
    }
    if(srcPointIsNull) {
        srcPoint = p;
        srcPointIsNull = false;
    }
    else {
        if(dstPointIsNull){
            dstPoint = p;
            dstPointIsNull = false;
        }
        else {
            srcPoint = dstPoint;
            dstPoint = p;
        }
        if(!firstFrame.empty())
            cv::line(firstFrame, srcPoint, dstPoint, cv::Scalar(0, 0, 255), 2);
    }
}

void PathCapture::startShowing(){
    stoped = false;
    while(!stoped){
        if(!firstFrame.empty()) {
            if(frameLock.try_lock()){
                firstFrame = frameRecieved;
                frameLock.unlock();
            }
            emit resultDisplay(
                    QPixmap::fromImage(
                              QImage(static_cast<uchar*>(firstFrame.data), firstFrame.cols, firstFrame.rows, static_cast<int>(firstFrame.step), QImage::Format_RGB888)
                              .rgbSwapped()
                        )
                    );

        }
    }
}

void PathCapture::stopShowing(){
    stoped = true;
}

void PathCapture::recieveBox(cv::Rect newBallBox){
    ballBox = newBallBox;
}

void PathCapture::recieveFirstFrame(cv::Mat firstFrameRecieved){
    frameLock.lock();
    frameRecieved = firstFrameRecieved.clone();
    frameLock.unlock();

    if(frameRecieved.size().width < frameRecieved.size().height)
        resize(frameRecieved, frameRecieved, cv::Size(frameRecieved.size().width * 800 / frameRecieved.size().height, 800), 0, 0, cv::INTER_CUBIC);
    else
        resize(frameRecieved, frameRecieved, cv::Size(1200, frameRecieved.size().height * 1200 / frameRecieved.size().width), 0, 0, cv::INTER_CUBIC);

    emit positionAnnouncment(
                QString("frame dimentions : width = ") + QString::number(firstFrameRecieved.size().width) + QString(" , height = ") + QString::number(firstFrameRecieved.size().height)
                );
    cv::line(frameRecieved, cv::Point(frameRecieved.size().width/2, 0), cv::Point(frameRecieved.size().width/2, frameRecieved.size().height), cv::Scalar(0, 0, 0), 1);
    cv::line(frameRecieved, cv::Point(0, frameRecieved.size().height/2), cv::Point(frameRecieved.size().width, frameRecieved.size().height/2), cv::Scalar(0, 0, 0), 1);

    if(firstFrame.empty())
        firstFrame = frameRecieved;
}
