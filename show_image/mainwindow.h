#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tracking_window.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QScrollArea>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QDebug>
#include <QProgressDialog>
#include <QCoreApplication>
#include <QDir>
#include <QDateTime>
#include <opencv2/opencv.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_tabWidget_tabBarClicked(int index);
    void on_OpenImg_Botton_clicked();

    void on_Gaussian_Filter_clicked();

    void on_Median_Filter_clicked();

    void on_Bilateral_Filter_clicked();

    void on_Detect_Blocks_clicked();

    void on_OpenVideoFile_clicked();

    void on_OpenCam_clicked();

    void on_CannyVideo_clicked();

    void on_Object_Tracking_clicked();

    // 功能1: 批量处理文件夹中的所有图像对
    void on_BatchProcessButton_clicked();
    
    // 功能2: 单独处理一个图像检测角点
    void on_DetectCornersSingle_clicked();
    
    // 功能3: 单独处理激光线与标定板交点
    void on_DetectLaserIntersections_clicked();

private:
    Ui::MainWindow *ui;

    Tracking_Window *trackingWindow = nullptr;  // 跟踪窗口指针

    int current_index;
    cv::Mat current_img;
    
    // 图像数据结构
    struct ImageData {
        QPixmap originalPixmap;
        QLabel *imageLabel;
        QLabel *zoomValueLabel;
        QSlider *slider;
        double currentScale;
    };

    // 标定板结构
    struct ChessboardData {
        std::vector<cv::Point2f> corners;  // 角点坐标
        cv::Size patternSize;              // 标定板内角点尺寸
        bool detected;                     // 是否检测成功
    };

    // 激光线数据
    struct LaserLineData {
        cv::Mat binaryImage;               // 激光二值图像
        cv::Vec4f lineEquation;            // 直线方程参数 (vx, vy, x0, y0)
        std::vector<cv::Point2f> intersections; // 交点坐标
        std::vector<cv::Vec4f> boardLines; // 标定板直线
    };

     // 标定板参数
    cv::Size patternSize;
    std::vector<cv::Point2f> currentCorners;
    
    // 激光线数据
    cv::Mat current_laser_mask;
    LaserLineData currentLaserData;


    // 私有函数
    void updateStatus(const QString& message);
    void displayImage(cv::Mat img);
    void createImageWindow(QString fileName, const QPixmap& pixmap);
    void updateImageDisplay(ImageData *imageData);

    bool detectChessboardCorners(cv::Mat& img, std::vector<cv::Point2f>& corners);
    cv::Mat extractLaserBinaryForDark(cv::Mat& img);
    bool extractLaserLine(cv::Mat& binaryImg, cv::Vec4f& line);
    std::vector<cv::Vec4f> getBoardLinesFromCorners(const std::vector<cv::Point2f>& corners);
    std::vector<cv::Point2f> findIntersections(const cv::Vec4f& laserLine, 
                                               const std::vector<cv::Vec4f>& boardLines);
    void sortIntersectionsByY(std::vector<cv::Point2f>& intersections);
    void visualizeChessboardCorners(cv::Mat& img, const std::vector<cv::Point2f>& corners);
    void visualizeLaserAndIntersections(cv::Mat& img, const LaserLineData& laserData);
    bool loadCornersFromFile(const QString& filename, std::vector<cv::Point2f>& corners);
    void saveResultsToFile(const QString& basePath, int pairIndex, 
                          const std::vector<cv::Point2f>& corners,
                          const LaserLineData& laserData);
};

#endif // MAINWINDOW_H
