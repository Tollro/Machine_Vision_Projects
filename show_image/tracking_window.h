#ifndef TRACKING_WINDOW_H
#define TRACKING_WINDOW_H

#include <QWidget>
#include <QTimer>
#include <QGraphicsScene>
#include <QImage>
#include <opencv2/opencv.hpp>

namespace Ui {
class Tracking_Window;
}

class Tracking_Window : public QWidget
{
    Q_OBJECT

public:
    explicit Tracking_Window(QWidget *parent = nullptr);
    ~Tracking_Window();

private slots:
    void on_Start_Tracking_clicked();
    void on_Stop_Tracking_clicked();
    void on_Auto_Identify_clicked();

    // timer slot to grab frames and run CamShift
    void onFrameTimer();

private:
    Ui::Tracking_Window *ui;

    // helpers
    QTimer *frameTimer;
    QGraphicsScene *scene;
    QImage matToQImage(const cv::Mat &mat);
    void initTrackerFromFrame(const cv::Mat &frame);

    // tracking state
    cv::VideoCapture cap;
    cv::Rect trackWindow;
    cv::Mat hist;               // hue histogram
    bool histInitialized;
    bool trackingRunning;
    int deviceId;
};

#endif // TRACKING_WINDOW_H
