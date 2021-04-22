#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore/QObject>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QThread::currentThread()->setPriority(QThread::TimeCriticalPriority);
    ui->setupUi(this);
    isChosenCameraShowExist = false;
}

void MainWindow::endConstrutor(){
    processor = new VideoProcessor(cameraPath);
    processor->moveToThread(new QThread());
    connect(processor->thread(), &QThread::started, processor, &VideoProcessor::startVideo);
    connect(processor->thread(), &QThread::finished, processor, &VideoProcessor::deleteLater);
    connect(processor, SIGNAL(primaryDisplay(QPixmap)), ui->primaryLabel, SLOT(setPixmap(QPixmap)));
    connect(this, &MainWindow::startCapturing, processor, &VideoProcessor::startCapturing, Qt::DirectConnection);
    connect(this, &MainWindow::stopCapturing, processor, &VideoProcessor::stopCapturing, Qt::DirectConnection);
    processor->thread()->start(QThread::HighPriority);

    ui->stackedWidget->setCurrentIndex(1);

    captureTimer = new QTimer(this);
    capture = new PathCapture();
    capture->moveToThread(new QThread());
    connect(capture->thread(), &QThread::started, capture, &PathCapture::startShowing);
    connect(capture->thread(), &QThread::finished, capture, &PathCapture::deleteLater);
    connect(capture, SIGNAL(resultDisplay(QPixmap)), ui->resultLabel, SLOT(setPixmap(QPixmap)));
    connect(processor, &VideoProcessor::sendBox, capture, &PathCapture::recieveBox, Qt::DirectConnection);
    connect(processor, &VideoProcessor::transmitFirstFrame, capture, &PathCapture::recieveFirstFrame, Qt::DirectConnection);
    connect(captureTimer, &QTimer::timeout, capture, &PathCapture::determinePath, Qt::DirectConnection);
    connect(capture, SIGNAL(positionAnnouncment(QString)), ui->result_text, SLOT(append(QString)), Qt::DirectConnection);
    capture->thread()->start(QThread::LowPriority);
}

MainWindow::~MainWindow()
{
    if (ui->stackedWidget->currentIndex() != 0){
        captureTimer->deleteLater();

        processor->stopVideo();
        processor->thread()->exit();
        processor->thread()->wait();

        capture->stopShowing();
        capture->thread()->exit();
        capture->thread()->wait();
    }

    delete ui;
}

void MainWindow::on_pathButton_clicked()
{
    if(ui->pathButton->text() == "start"){
        emit startCapturing();
        ui->pathButton->setText("stop");
        captureTimer->start(captureInterval);
    }
    else{
        emit stopCapturing();
        ui->pathButton->setText("start");
        captureTimer->stop();
    }
}

void MainWindow::on_goBackButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_resultPage_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_nextButton_clicked()
{
    if(isChosenCameraShowExist){
        chosenCamera.release();
        chosenCameraShow->stopShowing();
        chosenCameraShow->thread()->exit();
        chosenCameraShow->thread()->wait();
        isChosenCameraShowExist = false;
        endConstrutor();
    }
    else
        ui->chosenCameraLabel->setText("first you need to check your input validation!!!");

}

void MainWindow::on_checkButton_clicked()
{
    ui->chosenCameraLabel->setText("Processing");
    if(isChosenCameraShowExist){
        chosenCamera.release();
        chosenCameraShow->stopShowing();
        chosenCameraShow->thread()->exit();
        chosenCameraShow->thread()->wait();
        isChosenCameraShowExist = false;
    }

    bool isIntervalEnteredCorrectly;
    captureInterval = ui->captureIntervalText->text().toInt(&isIntervalEnteredCorrectly);
    if(!isIntervalEnteredCorrectly){
        ui->chosenCameraLabel->setText("capture interval is wrong!!! please type an integer.");
        return;
    }

    bool isCamera;
    int cameraNumber;
    cameraNumber = ui->cameraPathText->text().toInt(&isCamera);
    cameraPath = ui->cameraPathText->text().toStdString();

    if(isCamera)
        chosenCamera.open(cameraNumber, cv::CAP_DSHOW);
    else
        chosenCamera.open(cameraPath);

    if(!chosenCamera.isOpened()){
        ui->chosenCameraLabel->setText("there is no such path to a camera or file !!!");
    }
    else {
        chosenCameraShow = new chosenCameraBroadcasting(chosenCamera);
        chosenCameraShow->moveToThread(new QThread());
        connect(chosenCameraShow->thread(), &QThread::started, chosenCameraShow, &chosenCameraBroadcasting::startShowing);
        connect(chosenCameraShow->thread(), &QThread::finished, chosenCameraShow, &chosenCameraBroadcasting::deleteLater);
        connect(chosenCameraShow, SIGNAL(chosenCameraCheckShow(QPixmap)), ui->chosenCameraLabel, SLOT(setPixmap(QPixmap)), Qt::DirectConnection);
        chosenCameraShow->thread()->start(QThread::LowestPriority);
        isChosenCameraShowExist = true;
    }
}
