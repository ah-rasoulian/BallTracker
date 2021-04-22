#include "chosencamerabroadcasting.h"

chosenCameraBroadcasting::chosenCameraBroadcasting(cv::VideoCapture newCamera)
{
    camera = newCamera;
}
void chosenCameraBroadcasting::startShowing(){
    stoped = false;
    while(!stoped){
        camera >> frame;
        if(!frame.empty()) {
            if(frame.size().width < frame.size().height)
                resize(frame, frame, cv::Size(frame.size().width * 800 / frame.size().height, 800), 0, 0, cv::INTER_CUBIC);
            else
                resize(frame, frame, cv::Size(1200, frame.size().height * 1200 / frame.size().width), 0, 0, cv::INTER_CUBIC);

            emit chosenCameraCheckShow(
                    QPixmap::fromImage(
                              QImage(static_cast<uchar*>(frame.data), frame.cols, frame.rows, static_cast<int>(frame.step), QImage::Format_RGB888)
                              .rgbSwapped()
                        )
                    );
        }
    }
}

void chosenCameraBroadcasting::stopShowing(){
    stoped = true;
}
