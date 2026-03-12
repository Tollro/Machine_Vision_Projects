#include "tracking_window.h"
#include "ui_tracking_window.h"

#include <QMessageBox>
#include <QDebug>
#include <QPixmap>

Tracking_Window::Tracking_Window(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Tracking_Window)
    , frameTimer(new QTimer(this))
    , scene(new QGraphicsScene(this))
    , histInitialized(false)
    , trackingRunning(false)
    , deviceId(0)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(scene);
    scene->setBackgroundBrush(Qt::black);
    connect(frameTimer, &QTimer::timeout, this, &Tracking_Window::onFrameTimer);
}

Tracking_Window::~Tracking_Window()
{
    if (frameTimer->isActive()) frameTimer->stop();
    if (cap.isOpened()) cap.release();
    delete ui;
}

// Convert cv::Mat (BGR/GRAY/4ch) to QImage (copied)
QImage Tracking_Window::matToQImage(const cv::Mat &mat)
{
    if (mat.empty()) return QImage();

    cv::Mat rgb;
    if (mat.type() == CV_8UC3) {
        cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
        return QImage(rgb.data, rgb.cols, rgb.rows, static_cast<int>(rgb.step), QImage::Format_RGB888).copy();
    } else if (mat.type() == CV_8UC1) {
        return QImage(mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_Grayscale8).copy();
    } else if (mat.type() == CV_8UC4) {
        cv::cvtColor(mat, rgb, cv::COLOR_BGRA2RGBA);
        return QImage(rgb.data, rgb.cols, rgb.rows, static_cast<int>(rgb.step), QImage::Format_RGBA8888).copy();
    }
    return QImage();
}

// Try to detect the largest colorful block in `frame` and initialize hue histogram + trackWindow.
void Tracking_Window::initTrackerFromFrame(const cv::Mat &frame)
{
    if (frame.empty()) {
        QMessageBox::warning(this, "Warning", "输入帧为空，无法识别");
        return;
    }

    cv::Mat hsv;
    cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

    // Mask by saturation & value to avoid grayscale/very dark regions
    cv::Mat mask;
    cv::inRange(hsv, cv::Scalar(0, 50, 50), cv::Scalar(180, 255, 255), mask);

    // Morphology to clean noise
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5,5));
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);
    cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel);

    // Find contours and pick largest
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    if (contours.empty()) {
        QMessageBox::information(this, "Info", "未检测到明显的彩色块");
        return;
    }

    size_t bestIdx = 0;
    double bestArea = 0;
    for (size_t i = 0; i < contours.size(); ++i) {
        double a = cv::contourArea(contours[i]);
        if (a > bestArea) { bestArea = a; bestIdx = i; }
    }

    if (bestArea < 500) {
        QMessageBox::information(this, "Info", "检测到的块太小，无法初始化追踪");
        return;
    }

    cv::Rect r = cv::boundingRect(contours[bestIdx]);
    // Expand slightly
    r.x = std::max(0, r.x - 5);
    r.y = std::max(0, r.y - 5);
    r.width = std::min(frame.cols - r.x, r.width + 10);
    r.height = std::min(frame.rows - r.y, r.height + 10);

    // Compute hue histogram inside ROI using mask
    cv::Mat roi(hsv, r);
    cv::Mat roiMask(mask(r));

    int hbins = 30;
    int channels[] = {0};
    int histSize[] = { hbins };
    float hranges[] = { 0, 180 };
    const float* ranges[] = { hranges };

    cv::calcHist(&roi, 1, channels, roiMask, hist, 1, histSize, ranges);
    cv::normalize(hist, hist, 0, 255, cv::NORM_MINMAX);

    trackWindow = r;
    histInitialized = true;

    // Show initialized rectangle to user
    cv::Mat preview = frame.clone();
    cv::rectangle(preview, r, cv::Scalar(0,255,0), 2);
    QImage img = matToQImage(preview);
    scene->clear();
    scene->addPixmap(QPixmap::fromImage(img).scaled(ui->graphicsView->size(), Qt::KeepAspectRatio));
    ui->graphicsView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

// Start camera capture and tracking loop (timer)
void Tracking_Window::on_Start_Tracking_clicked()
{
    if (trackingRunning) {
        QMessageBox::information(this, "Info", "追踪已在运行");
        return;
    }

    if (!cap.isOpened()) {
        if (!cap.open(deviceId, cv::CAP_ANY)) {
            QMessageBox::critical(this, "错误", "无法打开摄像头");
            return;
        }
        cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
        cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    }

    trackingRunning = true;
    frameTimer->start(30); // ~33 fps
}

// Stop camera and tracking
void Tracking_Window::on_Stop_Tracking_clicked()
{
    if (frameTimer->isActive()) frameTimer->stop();
    if (cap.isOpened()) cap.release();
    scene->clear();
    histInitialized = false;
    trackingRunning = false;
}

// Capture one frame (from opened camera or temporary) and auto-identify block
void Tracking_Window::on_Auto_Identify_clicked()
{
    cv::Mat frame;
    bool usedTemp = false;

    if (cap.isOpened()) {
        if (!cap.read(frame) || frame.empty()) {
            QMessageBox::warning(this, "Warning", "从摄像头读取帧失败");
            return;
        }
    } else {
        // open temporary capture to grab one frame
        cv::VideoCapture tmp(0, cv::CAP_ANY);
        if (!tmp.isOpened()) {
            QMessageBox::critical(this, "错误", "无法打开摄像头进行自动识别");
            return;
        }
        tmp.set(cv::CAP_PROP_FRAME_WIDTH, 640);
        tmp.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
        if (!tmp.read(frame) || frame.empty()) {
            QMessageBox::warning(this, "Warning", "读取临时帧失败");
            tmp.release();
            return;
        }
        tmp.release();
        usedTemp = true;
    }

    initTrackerFromFrame(frame);
}

// Timer handler: grab frame, compute backproject + camshift if hist initialized, display
void Tracking_Window::onFrameTimer()
{
    if (!cap.isOpened()) {
        // nothing to do
        return;
    }

    cv::Mat frame;
    if (!cap.read(frame) || frame.empty()) {
        qDebug() << "Failed to read frame from camera";
        on_Stop_Tracking_clicked();
        return;
    }

    cv::Mat display = frame.clone();

    if (histInitialized) {
        // compute backprojection on hue channel
        cv::Mat hsv;
        cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

        int channels[] = {0};
        float hranges[] = {0,180};
        const float* ranges[] = { hranges };
        cv::Mat backproj;
        cv::calcBackProject(&hsv, 1, channels, hist, backproj, ranges, 1, true);

        // mask by sat/value to reduce noise
        cv::Mat mask;
        cv::inRange(hsv, cv::Scalar(0, 30, 30), cv::Scalar(180, 255, 255), mask);
        backproj &= mask;

        // camshift
        cv::TermCriteria term(cv::TermCriteria::EPS | cv::TermCriteria::COUNT, 10, 1);
        cv::RotatedRect rr = cv::CamShift(backproj, trackWindow, term);

        // draw rotated rect
        cv::Point2f pts[4];
        rr.points(pts);
        for (int i = 0; i < 4; ++i) cv::line(display, pts[i], pts[(i+1)%4], cv::Scalar(0,255,0), 2);
        cv::circle(display, rr.center, 3, cv::Scalar(0,0,255), -1);
    }

    QImage img = matToQImage(display);
    scene->clear();
    scene->addPixmap(QPixmap::fromImage(img).scaled(ui->graphicsView->size(), Qt::KeepAspectRatio));
    ui->graphicsView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

