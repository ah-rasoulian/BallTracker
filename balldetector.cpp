#include "balldetector.h"

BallDetector::BallDetector()
{
    isHSVFrameAvailable = false;
}
void BallDetector::startDetecting(){
    using namespace cv;
    stoped = false;
    int redLowH1 = 0, redHighH1 = 4, redLowS1 = 120, redHighS1 = 255, redLowV1 = 120, redHighV1 = 255;
    int redLowH2 = 176, redHighH2 = 180, redLowS2 = 120, redHighS2 = 255, redLowV2 = 120, redHighV2 = 255;
    while (!stoped) {
        if (isHSVFrameAvailable) {
                Mat1b redMask1 , redMask2;
                inRange(hsvFrame , Scalar(redLowH1, redLowS1, redLowV1), Scalar(redHighH1, redHighS1, redHighV1), redMask1);
                inRange(hsvFrame , Scalar(redLowH2, redLowS2, redLowV2), Scalar(redHighH2, redHighS2, redHighV2), redMask2);
                Mat1b redMask = redMask1 | redMask2;
                std::vector<std::vector<cv::Point> > redContours;
                std::vector<cv::Vec4i> redHierarchy;
                cv::findContours(redMask, redContours, redHierarchy, cv::RetrievalModes::RETR_EXTERNAL, cv::ContourApproximationModes::CHAIN_APPROX_SIMPLE);
                size_t largestRedContourIndex = 0;
                double redMaxArea = 0;
                for (size_t i = 0; i < redContours.size(); i++) {
                    double area = cv::contourArea(redContours[i]);
                    if (area > redMaxArea) {
                        largestRedContourIndex = i;
                        redMaxArea = area;
                    }
                }
                if (redMaxArea >= 100) {
                    cv::Rect redRect = cv::boundingRect(redContours[largestRedContourIndex]);
                    emit detectingBox(redRect);
                }
            }
    }
}

void BallDetector::stopDetecting(){
    stoped = true;
}

void BallDetector::getHSVFrame(cv::Mat newHSVFrame){
    hsvFrame = newHSVFrame;
    isHSVFrameAvailable = true;
}
