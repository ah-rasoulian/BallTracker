#include "balltracker.h"

BallTracker::BallTracker()
{
    isFrameAvailable = false;
    isInitialBoxReceived = false;
    isBallTracked = false;
    trackerFunction = cv::TrackerKCF::create();
}

void BallTracker::startTracking(){
    using namespace cv;
    stoped = false;
    while (!stoped) {
        if(isBallTracked){
            cv::Rect2d box;
            bool trackSuccess = trackerFunction->update(frame, box);
                if (trackSuccess){
                    isBallTracked = true;
                    emit trackingBox(box);
                }
                else
                    isBallTracked = false;
        }
        else {
            if(isInitialBoxReceived){
                trackerFunction->init(frame, initialBox);
                isInitialBoxReceived = false;
                isBallTracked = true;
                emit trackingBox(initialBox);
            }
        }
        emit trackingBox(initialBox);
    }
}

void BallTracker::stopTracking(){
    stoped = true;
}

void BallTracker::getDetectingBox(cv::Rect newInitialBox){
    initialBox = newInitialBox;
}

void BallTracker::getFrame(cv::Mat newFrame){
    frame = newFrame;
    isFrameAvailable = true;
}
