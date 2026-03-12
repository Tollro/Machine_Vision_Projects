#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , current_index(0)
{
    ui->setupUi(this);
    this->setWindowTitle("OpenCv_OpenImage");
    QPixmap logo("/home/vboxuser/Pictures/hongxia.jpeg");
    if(!logo.isNull()) {
        ui->logo->setPixmap(logo);
        ui->logo->setScaledContents(true);
    }
    ui->Prompt_msg->setText("欢迎使用，请选择图片");
    
    // 初始化标定板参数（8x5表示8列5行内角点）
    patternSize = cv::Size(8, 5);

    updateStatus("系统就绪，请选择功能");
}

void MainWindow::updateStatus(const QString& message)
{
    ui->Prompt_msg->setText(message);
    qDebug() << message;
}

MainWindow::~MainWindow()
{
    delete ui;

    if (trackingWindow) {
        delete trackingWindow;
    }
}

void MainWindow::displayImage(cv::Mat img)
{
    qDebug() << "=== 开始显示图片，索引:" << current_index << "===";

    // 将 OpenCV Mat 转换为 QPixmap
    cv::Mat rgbImage;
    cv::cvtColor(img, rgbImage, cv::COLOR_BGR2RGB);

    QImage qimage((const unsigned char*)rgbImage.data,
                  rgbImage.cols,
                  rgbImage.rows,
                  rgbImage.step,
                  QImage::Format_RGB888);

    QPixmap pixmap = QPixmap::fromImage(qimage);

    // 创建新窗口显示图像
    createImageWindow("图像", pixmap);
}

void MainWindow::createImageWindow(QString fileName, const QPixmap& pixmap)
{
    // 创建新窗口
    QDialog *imageDialog = new QDialog(this);
    imageDialog->setWindowTitle("图像查看器 - " + QFileInfo(fileName).fileName());
    imageDialog->setMinimumSize(800, 600);

    // 创建主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(imageDialog);

    // 创建工具栏
    QHBoxLayout *toolbarLayout = new QHBoxLayout();

    QLabel *zoomLabel = new QLabel("缩放:");
    QSlider *zoomSlider = new QSlider(Qt::Horizontal);
    zoomSlider->setRange(10, 200); // 10% 到 200%
    zoomSlider->setValue(100);
    zoomSlider->setTickPosition(QSlider::TicksBelow);
    zoomSlider->setTickInterval(10);

    QLabel *zoomValue = new QLabel("100%");
    QPushButton *fitButton = new QPushButton("适应窗口");
    QPushButton *originalButton = new QPushButton("原始大小");

    toolbarLayout->addWidget(zoomLabel);
    toolbarLayout->addWidget(zoomSlider);
    toolbarLayout->addWidget(zoomValue);
    toolbarLayout->addWidget(fitButton);
    toolbarLayout->addWidget(originalButton);
    toolbarLayout->addStretch();

    // 创建图像显示区域
    QScrollArea *scrollArea = new QScrollArea();
    QLabel *imageLabel = new QLabel();
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(false); // 重要：设置为false以保持质量
    imageLabel->setAlignment(Qt::AlignCenter);

    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);
    scrollArea->setWidgetResizable(true);

    // 组装界面
    mainLayout->addLayout(toolbarLayout);
    mainLayout->addWidget(scrollArea);

    // 保存图像数据和UI组件
    ImageData *imageData = new ImageData{
        pixmap,
        imageLabel,
        zoomValue,
        zoomSlider,
        1.0
    };

    // 初始显示
    updateImageDisplay(imageData);

    // 连接信号和槽
    connect(zoomSlider, &QSlider::valueChanged, this, [imageData, this](int value) {
        imageData->currentScale = value / 100.0;
        imageData->zoomValueLabel->setText(QString("%1%").arg(value));
        updateImageDisplay(imageData);
    });

    connect(fitButton, &QPushButton::clicked, this, [imageData, scrollArea, this]() {
        // 计算适应窗口的缩放比例
        QSize viewportSize = scrollArea->viewport()->size();
        QSize imageSize = imageData->originalPixmap.size();

        double scaleX = (viewportSize.width() - 20) / (double)imageSize.width();
        double scaleY = (viewportSize.height() - 20) / (double)imageSize.height();
        double fitScale = qMin(scaleX, scaleY);

        int sliderValue = qRound(fitScale * 100);
        sliderValue = qMax(10, qMin(200, sliderValue)); // 限制在滑块范围内

        imageData->slider->setValue(sliderValue);
    });

    connect(originalButton, &QPushButton::clicked, this, [imageData, this]() {
        imageData->slider->setValue(100);
    });

    // 窗口关闭时清理内存
    connect(imageDialog, &QDialog::finished, this, [imageData, imageDialog]() {
        delete imageData;
        imageDialog->deleteLater();
    });

    // 显示窗口
    imageDialog->resize(1000, 700);
    imageDialog->show();
}

void MainWindow::updateImageDisplay(ImageData *imageData)
{
    if (imageData->originalPixmap.isNull())
        return;

    // 计算缩放后的尺寸
    QSize originalSize = imageData->originalPixmap.size();
    QSize scaledSize = originalSize * imageData->currentScale;

    // 高质量缩放
    QPixmap scaledPixmap = imageData->originalPixmap.scaled(
        scaledSize,
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
        );

    imageData->imageLabel->setPixmap(scaledPixmap);
    imageData->imageLabel->adjustSize(); // 重要：调整QLabel大小以适应缩放后的图像
}

void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    if(index < 0) {
        qDebug() << "Tab索引超出范围:" << index;
        return;
    }
    current_index = index;
    updateStatus(QString("当前显示Tab %1").arg(current_index + 1));
}

void MainWindow::on_OpenImg_Botton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("打开图片"),
        "/home/vboxuser/Pictures",
        tr("图片文件 (*.png *.jpg *.jpeg *.bmp *.tiff)"));

    if(fileName.isEmpty())
    {
        updateStatus("未选择图片文件");
        return;
    }

    memset(&current_img, 0, sizeof(current_img));
    current_img = cv::imread(fileName.toStdString());
    if(current_img.empty())
    {
        QMessageBox::critical(this, "错误", "无法加载图片文件");
        return;
    }

    displayImage(current_img);
}

void MainWindow::on_Gaussian_Filter_clicked()
{
    //高斯滤波
    cv::Mat gaus_img;
    int size = 15, sigmaX = 3, sigmaY = 0;
    size = ui->GaussianKernelSize->value();
    sigmaX = ui->GaussianSigmaX->value();
    sigmaY = ui->GaussianSigmaY->value();
    memset(&gaus_img, 0, sizeof(gaus_img));
    GaussianBlur(current_img, gaus_img, cv::Size (size, size), sigmaX, sigmaY);
    if(gaus_img.empty())
    {
        QMessageBox::critical(this, "错误", "无法加载图片文件");
        return;
    }
    displayImage(gaus_img);
}


void MainWindow::on_Median_Filter_clicked()
{
    //中值滤波
    cv::Mat median_img;
    int size = 5;
    size = ui->MedianAperture->value();
    memset(&median_img, 0, sizeof(median_img));
    medianBlur(current_img, median_img, size);
    if(median_img.empty())
    {
        QMessageBox::critical(this, "错误", "无法加载图片文件");
        return;
    }
    displayImage(median_img);
}


void MainWindow::on_Bilateral_Filter_clicked()
{
    //双边滤波
    cv::Mat bilateral_img;
    int Diameter = 15, SigmaColor = 100, SigmaSpace = 9;
    Diameter = ui->BilateralDiameter->value();
    SigmaColor = ui->BilateralSigmaColor->value();
    SigmaSpace = ui->BilateralSigmaSpace->value();
    memset(&bilateral_img, 0, sizeof(bilateral_img));
    bilateralFilter(current_img, bilateral_img, SigmaSpace, SigmaColor, SigmaSpace);
    if(bilateral_img.empty())
    {
        QMessageBox::critical(this, "错误", "无法加载图片文件");
        return;
    }
    displayImage(bilateral_img);
}


void MainWindow::on_Detect_Blocks_clicked()
{
    // 检测彩色块并标注
    if (current_img.empty()) {
        QMessageBox::warning(this, "警告", "请选择图像");
        return;
    }

    cv::Mat work;
    current_img.copyTo(work);

    std::map<std::string, cv::Scalar> colorMap = {
        {"red", cv::Scalar(0, 0, 255)},      // 红色 BGR
        {"orange", cv::Scalar(0, 165, 255)}, // 橙色 BGR
        {"yellow", cv::Scalar(0, 255, 255)}, // 黄色 BGR
        {"green", cv::Scalar(0, 255, 0)},    // 绿色 BGR
        {"cyan", cv::Scalar(255, 255, 0)},   // 青色 BGR
        {"blue", cv::Scalar(255, 0, 0)},     // 蓝色 BGR
        {"purple", cv::Scalar(128, 0, 128)}  // 紫色 BGR
    };

    // 预处理：灰度 -> 高斯模糊 -> 边缘检测 -> 膨胀以连接边缘
    cv::Mat gray, blurred, edges;
    cv::cvtColor(work, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, blurred, cv::Size(5,5), 1.5);
    cv::Canny(blurred, edges, 50, 150);
    displayImage(edges);
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5,5));
    cv::dilate(edges, edges, kernel);
    // 查找轮廓
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(edges, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    int detected = 0;
    int letterIndex = 0;

    for (size_t i = 0; i < contours.size(); ++i) {
        double area = cv::contourArea(contours[i]);
        if (area < 1000) // 面积滤波
            continue;

        // 近似多边形
        std::vector<cv::Point> approx;
        cv::approxPolyDP(contours[i], approx, 0.02 * cv::arcLength(contours[i], true), true);
        cv::Rect r = cv::boundingRect(approx);// 获取外接矩形

        // 计算该轮廓内的平均颜色
        cv::Mat mask = cv::Mat::zeros(work.size(), CV_8UC1);
        cv::drawContours(mask, contours, (int)i, cv::Scalar(255), cv::FILLED);
        cv::Scalar meanBGR = cv::mean(work, mask);

        // 将平均颜色转为 HSV，以便判定颜色名称
        cv::Mat px(1,1,CV_8UC3, cv::Scalar(meanBGR[0], meanBGR[1], meanBGR[2]));
        cv::Mat pxHSV;
        cv::cvtColor(px, pxHSV, cv::COLOR_BGR2HSV);
        cv::Vec3b hsv = pxHSV.at<cv::Vec3b>(0,0);
        int hue = hsv[0], sat = hsv[1], val = hsv[2];

        auto colorName = std::string("未知");
        bool isColorful = false;
        if (val < 50) {
            colorName = "black";
        } else if (sat < 30) {
            if (val > 200) colorName = "white";
            else colorName = "gray";
        } else {
            isColorful = true;
            if (hue <= 9 || hue >= 160) colorName = "red";
            else if (hue <= 25) colorName = "orange";
            else if (hue <= 35) colorName = "yellow";
            else if (hue <= 85) colorName = "green";
            else if (hue <= 100) colorName = "cyan";
            else if (hue <= 140) colorName = "blue";
            else if (hue <= 160) colorName = "purple";
        }

        if(!isColorful){
            continue;
        }
        // 绘制矩形与文字（带描边）
        cv::Scalar rectColor;
        if (colorMap.find(colorName) != colorMap.end()) {
            rectColor = colorMap[colorName];
        } else {
            rectColor = cv::Scalar(0, 255, 0); // 默认绿色边框
        }
        cv::rectangle(work, r, rectColor, 2);

        char letter = 'A' + (letterIndex % 26);
        std::string label = std::string(1, letter) + " " + colorName;
        int font = cv::FONT_HERSHEY_SIMPLEX;
        double fontScale = std::max(0.6, (r.width / 200.0));
        int thickness = 2;

        // 文字位置：矩形顶部内侧
        cv::Point textOrg(r.x + 4, std::max(16, r.y + 16));

        // 描边（黑）和主色（白）
        cv::putText(work, label, textOrg, font, fontScale, cv::Scalar(0,0,0), thickness+2, cv::LINE_AA);
        cv::putText(work, label, textOrg, font, fontScale, cv::Scalar(255,255,255), thickness, cv::LINE_AA);

        ++detected;
        ++letterIndex;
    }
    updateStatus(QString("检测到 %1 个块").arg(detected));
    displayImage(work);
}


void MainWindow::on_OpenVideoFile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("打开视频文件"),
        "/home/vboxuser/Videos",
        tr("视频文件 (*.mp4 *.avi *.mkv *.mov)"));

    if (fileName.isEmpty()) {
        updateStatus("未选择视频文件");
        return;
    }

    cv::VideoCapture cap(fileName.toStdString());
    if (!cap.isOpened()) {
        QMessageBox::critical(this, "错误", "无法打开视频文件");
        updateStatus("打开视频失败");
        return;
    }

    updateStatus("播放视频: " + fileName);
    cv::Mat frame;
    double fps = cap.get(cv::CAP_PROP_FPS);
    int delay = fps > 1.0 ? static_cast<int>(1000.0 / fps) : 33;

    const std::string winName = "Video File";
    cv::namedWindow(winName, cv::WINDOW_AUTOSIZE);

    while (cap.read(frame)) {
        if (frame.empty()) break;
        cv::imshow(winName, frame);
        int key = cv::waitKey(delay);
        if (key == 27 || key == 'q' || key == 'Q') break;
        // keep UI responsive a little
        QCoreApplication::processEvents();
    }

    cap.release();
    cv::destroyWindow(winName);
    updateStatus("视频播放结束");
}


void MainWindow::on_OpenCam_clicked()
{
    int deviceId = 0; // 默认摄像头 0
    // 可根据需要从 UI 获取 deviceId

    cv::VideoCapture cap;
    if (!cap.open(deviceId, cv::CAP_ANY)) {
        QMessageBox::critical(this, "错误", QString("无法打开摄像头 %1").arg(deviceId));
        updateStatus("打开摄像头失败");
        return;
    }

    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

    updateStatus(QString("打开摄像头 %1").arg(deviceId));
    cv::Mat frame;
    const std::string winName = "Camera";
    cv::namedWindow(winName, cv::WINDOW_AUTOSIZE);

    while (true) {
        if (!cap.read(frame)) break;
        if (frame.empty()) continue;
        cv::imshow(winName, frame);
        int key = cv::waitKey(30);
        if (key == 27 || key == 'q' || key == 'Q') break;
        QCoreApplication::processEvents();
    }

    cap.release();
    cv::destroyWindow(winName);
    updateStatus("摄像头已关闭");
}


void MainWindow::on_CannyVideo_clicked()
{
    int deviceId = 0;

    cv::VideoCapture cap;
    if (!cap.open(deviceId, cv::CAP_ANY)) {
        QMessageBox::critical(this, "错误", QString("无法打开摄像头 %1").arg(deviceId));
        updateStatus("打开摄像头失败");
        return;
    }

    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

    updateStatus("摄像头 Canny 实时处理");
    cv::Mat frame, gray, blurred, edges, edges_bgr;
    const std::string winName = "Canny Camera";
    cv::namedWindow(winName, cv::WINDOW_AUTOSIZE);

    while (true) {
        if (!cap.read(frame)) break;
        if (frame.empty()) continue;

        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(gray, blurred, cv::Size(5,5), 1.5);
        cv::Canny(blurred, edges, 50, 150);
        // 为了更好显示，将 single-channel edges 转为 BGR
        cv::cvtColor(edges, edges_bgr, cv::COLOR_GRAY2BGR);

        // 也可以将原图与边缘合并显示，以下为并排显示
        cv::Mat combined;
        cv::hconcat(frame, edges_bgr, combined);

        cv::imshow(winName, combined);
        int key = cv::waitKey(30);
        if (key == 27 || key == 'q' || key == 'Q') break;
        QCoreApplication::processEvents();
    }

    cap.release();
    cv::destroyWindow(winName);
    updateStatus("Canny 实时处理已停止");
}

void MainWindow::on_Object_Tracking_clicked()
{
    if (!trackingWindow) {
        // 第一次打开，创建窗口
        trackingWindow = new Tracking_Window();
        // 设置窗口属性
        trackingWindow->setAttribute(Qt::WA_DeleteOnClose);
        // 设置窗口标志，使其成为独立的顶层窗口
        trackingWindow->setWindowFlags(trackingWindow->windowFlags() |
                                       Qt::Window);
        trackingWindow->setWindowTitle("目标跟踪窗口");
        // 连接关闭信号
        connect(trackingWindow, &Tracking_Window::destroyed,
                this, [this]() { trackingWindow = nullptr; });
    }

    // 显示窗口
    trackingWindow->show();
    trackingWindow->raise();  // 提升到最前面
    trackingWindow->activateWindow();  // 激活窗口
}

// 检测标定板角点
bool MainWindow::detectChessboardCorners(cv::Mat& img, std::vector<cv::Point2f>& corners)
{
    cv::Mat gray;
    if (img.channels() == 3) {
        cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = img.clone();
    }
    
    // 更新标定板尺寸
    patternSize = cv::Size(ui->PatternWidth->value(),
                           ui->PatternHeight->value());
    
    bool found = cv::findChessboardCorners(gray, patternSize, corners,
                                           cv::CALIB_CB_ADAPTIVE_THRESH + 
                                           cv::CALIB_CB_NORMALIZE_IMAGE);
    
    if (found) {
        cv::TermCriteria criteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 30, 0.001);
        cv::cornerSubPix(gray, corners, cv::Size(11, 11), cv::Size(-1, -1), criteria);
        return true;
    }
    
    return false;
}

// 提取激光线
cv::Mat MainWindow::extractLaserBinaryForDark(cv::Mat& img)
{
    cv::Mat gray;
    if (img.channels() == 3) {
        cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = img.clone();
    }
    
    updateStatus("开始提取激光线（聚焦中心区域）...");
    
    // ===== 1. 创建中心区域掩膜 =====
    updateStatus("步骤1: 创建中心区域掩膜...");
    
    int centerX = gray.cols / 2;
    int centerY = gray.rows / 2;
    
    // 定义中心区域的大小（比如图像尺寸的60%）
    int roiWidth = gray.cols * 0.6;
    int roiHeight = gray.rows * 0.6;
    
    // 创建矩形ROI
    cv::Rect centerROI(centerX - roiWidth/2, centerY - roiHeight/2, roiWidth, roiHeight);
    
    // 确保ROI在图像范围内
    centerROI = centerROI & cv::Rect(0, 0, gray.cols, gray.rows);
    
    // 创建掩膜：中心区域为白色，其他区域为黑色
    cv::Mat mask = cv::Mat::zeros(gray.size(), CV_8UC1);
    mask(centerROI) = 255;
    
    // 显示ROI区域
    cv::Mat roi_display = gray.clone();
    cv::cvtColor(roi_display, roi_display, cv::COLOR_GRAY2BGR);
    cv::rectangle(roi_display, centerROI, cv::Scalar(0, 255, 0), 2);
    cv::putText(roi_display, "Center ROI", 
               cv::Point(centerROI.x + 10, centerROI.y + 30),
               cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 0), 2);
    cv::putText(roi_display, 
               QString("Size: %1x%2").arg(centerROI.width).arg(centerROI.height).toStdString(),
               cv::Point(centerROI.x + 10, centerROI.y + 60),
               cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 0), 2);
    // displayImage(roi_display);
    
    // ===== 2. 预处理：减少噪声并增强对比度 =====
    updateStatus("步骤2: 预处理图像...");
    
    // 2.1 使用中值滤波去除椒盐噪声，保持边缘
    cv::Mat median_blur;
    cv::medianBlur(gray, median_blur, 5);
    
    // 2.2 使用高斯模糊平滑图像
    cv::Mat blurred;
    cv::GaussianBlur(median_blur, blurred, cv::Size(5, 5), 1.5);
    
    // 2.3 使用CLAHE增强对比度
    cv::Mat enhanced;
    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
    clahe->setClipLimit(2.0);
    clahe->setTilesGridSize(cv::Size(8, 8));
    clahe->apply(blurred, enhanced);

    // displayImage(enhanced);
    
    // ===== 3. 激光线提取策略（只在中心区域） =====
    updateStatus("步骤3: 提取激光线（仅中心区域）...");
    
    // 3.1 提取中心区域的图像
    cv::Mat centerRegion = enhanced(centerROI);
    
    // 3.2 计算中心区域的亮度统计
    double minVal, maxVal;
    cv::Point minLoc, maxLoc;
    cv::minMaxLoc(centerRegion, &minVal, &maxVal, &minLoc, &maxLoc);
    
    // 转换为全局坐标
    cv::Point globalMaxLoc(maxLoc.x + centerROI.x, maxLoc.y + centerROI.y);
    
    double meanVal = cv::mean(centerRegion)[0];
    
    // 3.3 计算动态阈值（基于中心区域）
    double dynamicThreshold = meanVal * 1.8;
    dynamicThreshold = std::max(dynamicThreshold, 150.0);
    dynamicThreshold = std::min(dynamicThreshold, maxVal * 0.8);
    
    qDebug() << "中心区域统计 - 最小值:" << minVal << "最大值:" << maxVal 
             << "平均值:" << meanVal << "阈值:" << dynamicThreshold;
    
    // 3.4 对中心区域应用阈值
    cv::Mat centerBinary;
    cv::threshold(centerRegion, centerBinary, dynamicThreshold, 255, cv::THRESH_BINARY);
    
    // 3.5 创建全图二值图像，只保留中心区域的检测结果
    cv::Mat binary = cv::Mat::zeros(gray.size(), CV_8UC1);
    centerBinary.copyTo(binary(centerROI));
    
    // 显示中心区域提取结果
    // cv::Mat center_extract_display;
    // cv::hconcat(centerRegion, centerBinary, center_extract_display);
    // cv::cvtColor(center_extract_display, center_extract_display, cv::COLOR_GRAY2BGR);
    // cv::putText(center_extract_display, "Center Region", cv::Point(10, 30),
    //            cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 0), 2);
    // cv::putText(center_extract_display, "Binary (Center Only)", 
    //            cv::Point(centerRegion.cols + 10, 30),
    //            cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 0), 2);
    // cv::circle(center_extract_display, maxLoc, 8, cv::Scalar(255, 255, 0), 2);
    // displayImage(center_extract_display);
    
    // ===== 4. 形态学操作：连接激光线 =====
    updateStatus("步骤4: 形态学操作连接激光线...");
    
    // 4.1 对中心区域进行形态学操作（注意：只在中心区域）
    cv::Mat centerMorph = binary(centerROI).clone();
    
    // 使用竖直线结构元素
    cv::Mat vertical_kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 15));
    cv::morphologyEx(centerMorph, centerMorph, cv::MORPH_CLOSE, vertical_kernel);
    
    // 更新到全图
    centerMorph.copyTo(binary(centerROI));
    
    // ===== 5. 连通域分析：只保留中心区域的激光线段 =====
    updateStatus("步骤5: 连通域分析...");
    
    // 找到所有连通域（在整个图像中）
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(binary, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    
    qDebug() << "找到" << contours.size() << "个连通域";
    
    if (contours.empty()) {
        updateStatus("未找到激光线");
        return binary;
    }
    
    // 创建新的二值图像，只保留中心区域的连通域
    cv::Mat centerBinaryFinal = cv::Mat::zeros(binary.size(), CV_8UC1);
    
    // 只保留在中心区域内的连通域
    int centerContourCount = 0;
    for (size_t i = 0; i < contours.size(); i++) {
        cv::Rect bbox = cv::boundingRect(contours[i]);
        
        // 计算连通域中心
        cv::Point contourCenter(bbox.x + bbox.width/2, bbox.y + bbox.height/2);
        
        // 如果连通域中心在ROI内，并且面积足够大
        if (centerROI.contains(contourCenter) && cv::contourArea(contours[i]) > 300) {
            cv::drawContours(centerBinaryFinal, contours, i, cv::Scalar(255), cv::FILLED);
            centerContourCount++;
        }
    }
    // displayImage(centerBinaryFinal);
    
    qDebug() << "保留" << centerContourCount << "个中心区域的连通域";
    
    // ===== 6. 直线拟合（基于中心区域的点） =====
    updateStatus("步骤6: 直线拟合...");
    
    // 6.1 提取中心区域内的激光点
    std::vector<cv::Point2f> laserPoints;
    cv::findNonZero(centerBinaryFinal, laserPoints);
    
    if (laserPoints.size() < 10) {
        updateStatus("中心区域激光点太少，尝试在全图中寻找...");
        
        // 备用方案：在全图中寻找激光点
        cv::findNonZero(binary, laserPoints);
        
        if (laserPoints.size() < 10) {
            updateStatus("激光点太少，无法拟合直线");
            return centerBinaryFinal;
        }
    }
    
    // 6.2 对激光点按Y坐标排序
    std::sort(laserPoints.begin(), laserPoints.end(),
              [](const cv::Point2f& a, const cv::Point2f& b) {
                  return a.y < b.y;
              });
    
    // 6.3 使用RANSAC方法拟合直线（更鲁棒）
    cv::Vec4f bestLine;
    std::vector<cv::Point2f> inliers;
    int bestInliers = 0;
    
    // RANSAC迭代
    for (int iteration = 0; iteration < 200; iteration++) {
        // 随机选择两个点
        int idx1 = rand() % laserPoints.size();
        int idx2 = rand() % laserPoints.size();
        
        if (idx1 == idx2 || 
            abs(laserPoints[idx1].x - laserPoints[idx2].x) < 10) {
            continue; // 避免选择太近的点
        }
        
        cv::Point2f p1 = laserPoints[idx1];
        cv::Point2f p2 = laserPoints[idx2];
        
        // 计算直线参数
        float dx = p2.x - p1.x;
        float dy = p2.y - p1.y;
        float length = sqrt(dx*dx + dy*dy);
        
        if (length < 1.0) continue;
        
        float vx = dx / length;
        float vy = dy / length;
        
        // 统计内点
        std::vector<cv::Point2f> currentInliers;
        for (const auto& p : laserPoints) {
            // 点到直线的距离
            float distance = abs(vx * (p.y - p1.y) - vy * (p.x - p1.x));
            
            // 如果距离小于阈值，并且点在中心区域附近
            if (distance < 2.0) {
                currentInliers.push_back(p);
            }
        }
        
        // 更新最佳直线
        if (currentInliers.size() > bestInliers) {
            bestInliers = currentInliers.size();
            inliers = currentInliers;
            bestLine = cv::Vec4f(vx, vy, p1.x, p1.y);
        }
    }
    
    // 6.4 使用内点重新拟合直线
    cv::Vec4f finalLine;
    if (inliers.size() >= 10) {
        cv::fitLine(inliers, finalLine, cv::DIST_L2, 0, 0.01, 0.01);
        qDebug() << "RANSAC拟合成功，内点:" << inliers.size() << "/" << laserPoints.size();
    } else {
        // 回退到最小二乘法
        cv::fitLine(laserPoints, finalLine, cv::DIST_L2, 0, 0.01, 0.01);
        qDebug() << "使用最小二乘法拟合";
    }
    
    float vx = finalLine[0];
    float vy = finalLine[1];
    float angle = atan2(vy, vx) * 180 / CV_PI;
    qDebug() << "拟合直线角度:" << angle << "度";
    
    // ===== 7. 根据拟合的直线生成完整的激光线 =====
    updateStatus("步骤7: 生成完整的激光线...");
    
    // 7.1 找到激光点的Y坐标范围
    float minY = laserPoints[0].y;
    float maxY = laserPoints.back().y;
    
    // 7.2 扩展Y范围到整个中心区域
    minY = std::max(minY, (float)centerROI.y);
    maxY = std::min(maxY, (float)(centerROI.y + centerROI.height));
    
    // 7.3 计算直线在Y坐标范围内的两个端点
    // vx = finalLine[0];
    // vy = finalLine[1];
    float x0 = finalLine[2];
    float y0 = finalLine[3];
    
    cv::Point pt1, pt2;
    
    if (abs(vy) > 0.001) {
        float t1 = (minY - y0) / vy;
        pt1.x = x0 + t1 * vx;
        pt1.y = minY;
        
        float t2 = (maxY - y0) / vy;
        pt2.x = x0 + t2 * vx;
        pt2.y = maxY;
    } else {
        // 如果vy接近0（水平线）
        pt1 = cv::Point(x0, minY);
        pt2 = cv::Point(x0, maxY);
    }
    
    // 7.4 在新的二值图像上绘制完整的激光线
    cv::Mat complete_laser = cv::Mat::zeros(centerBinaryFinal.size(), CV_8UC1);
    cv::line(complete_laser, pt1, pt2, cv::Scalar(255), 3);
    
    // ===== 8. 显示最终结果 =====
    updateStatus("步骤8: 显示最终结果...");
    
    // 8.1 将完整的激光线叠加到原图上
    cv::Mat final_display;
    if (img.channels() == 3) {
        final_display = img.clone();
    } else {
        cv::cvtColor(img, final_display, cv::COLOR_GRAY2BGR);
    }
    
    // 8.2 绘制中心ROI区域
    cv::rectangle(final_display, centerROI, cv::Scalar(0, 255, 0), 2);
    
    // 8.3 绘制原始提取的点（红色）
    for (const auto& p : laserPoints) {
        cv::circle(final_display, p, 2, cv::Scalar(0, 0, 255), -1); // 红色小点
    }
    
    // 8.4 绘制拟合的直线（绿色）
    cv::line(final_display, pt1, pt2, cv::Scalar(0, 255, 0), 2); // 绿色直线
    
    // 8.5 绘制激光线（蓝色）
    cv::line(final_display, pt1, pt2, cv::Scalar(255, 0, 0), 1); // 蓝色细线
    
    // 8.6 显示信息
    QString info = QString("激光线拟合完成 - 角度: %1度").arg(angle, 0, 'f', 1);
    cv::putText(final_display, info.toStdString(), cv::Point(20, 40),
               cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 255, 0), 2);
    
    cv::putText(final_display, 
               QString("检测区域: %1x%2").arg(centerROI.width).arg(centerROI.height).toStdString(),
               cv::Point(20, 80),
               cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 0), 2);
    
    cv::putText(final_display, "绿框: 检测区域", cv::Point(20, 110),
               cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 0), 2);
    cv::putText(final_display, "红点: 激光点", cv::Point(20, 140),
               cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 0, 255), 2);
    cv::putText(final_display, "蓝线: 拟合激光线", cv::Point(20, 170),
               cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(255, 0, 0), 2);
    
    displayImage(final_display);
    
    // 8.7 保存拟合的直线方程
    currentLaserData.lineEquation = finalLine;
    currentLaserData.binaryImage = complete_laser;
    
    qDebug() << "最终拟合直线方程: vx=" << finalLine[0] 
             << ", vy=" << finalLine[1]
             << ", x0=" << finalLine[2]
             << ", y0=" << finalLine[3];
    
    updateStatus(QString("激光线提取完成，角度: %1度，使用中心区域检测").arg(angle, 0, 'f', 1));
    
    return complete_laser;
}

// 提取激光线方程
bool MainWindow::extractLaserLine(cv::Mat& binaryImg, cv::Vec4f& line)
{
    std::vector<cv::Point2f> points;
    cv::findNonZero(binaryImg, points);
    
    if (points.size() < 2) {
        return false;
    }
    
    cv::fitLine(points, line, cv::DIST_L2, 0, 0.01, 0.01);
    return true;
}

// 从角点获取标定板直线
std::vector<cv::Vec4f> MainWindow::getBoardLinesFromCorners(const std::vector<cv::Point2f>& corners)
{
    std::vector<cv::Vec4f> lines;
    
    if (corners.size() != patternSize.width * patternSize.height) {
        qDebug() << "角点数量不匹配: 期望" << patternSize.width * patternSize.height 
                 << "实际" << corners.size();
        return lines;
    }
    
    updateStatus("提取标定板水平线...");
    
    // 1. 只提取水平线（每行8个点拟合一条直线）
    for (int row = 0; row < patternSize.height; row++) {
        std::vector<cv::Point2f> rowPoints;
        
        // 收集这一行的所有点
        for (int col = 0; col < patternSize.width; col++) {
            int index = row * patternSize.width + col;
            if (index < corners.size()) {
                rowPoints.push_back(corners[index]);
            }
        }
        
        // 确保这一行有足够的点来拟合直线
        if (rowPoints.size() >= 2) {
            cv::Vec4f line;
            cv::fitLine(rowPoints, line, cv::DIST_L2, 0, 0.01, 0.01);
            lines.push_back(line);
            
            // 调试信息
            float angle = atan2(line[1], line[0]) * 180 / CV_PI;
            qDebug() << "第" << row << "行水平线 - 角度:" << angle 
                     << ", 点数量:" << rowPoints.size();
        }
    }
    
    // 2. 按Y坐标排序（从上到下）
    std::sort(lines.begin(), lines.end(),
              [](const cv::Vec4f& a, const cv::Vec4f& b) {
                  return a[3] < b[3]; // 按y0排序
              });
    
    // 3. 验证水平线数量
    if (lines.size() != patternSize.height) {
        qDebug() << "警告: 水平线数量(" << lines.size() 
                 << ")不等于标定板行数(" << patternSize.height << ")";
    } else {
        qDebug() << "成功提取" << lines.size() << "条水平线（标定板行数）";
    }
    
    return lines;
}

// 计算直线交点
std::vector<cv::Point2f> MainWindow::findIntersections(const cv::Vec4f& laserLine, 
                                                      const std::vector<cv::Vec4f>& boardLines)
{
    std::vector<cv::Point2f> intersections;
    
    float vx_l = laserLine[0];
    float vy_l = laserLine[1];
    float x0_l = laserLine[2];
    float y0_l = laserLine[3];
    
    qDebug() << "计算激光线与" << boardLines.size() << "条水平线的交点...";
    
    // 直接计算与所有水平线的交点
    for (const auto& boardLine : boardLines) {
        float vx_b = boardLine[0];
        float vy_b = boardLine[1];
        float x0_b = boardLine[2];
        float y0_b = boardLine[3];
        
        // 计算两条直线的交点
        float denominator = vx_l * vy_b - vy_l * vx_b;
        
        if (fabs(denominator) > 1e-6) { // 避免平行线
            float t1 = ((x0_b - x0_l) * vy_b - (y0_b - y0_l) * vx_b) / denominator;
            
            cv::Point2f intersection(x0_l + t1 * vx_l, y0_l + t1 * vy_l);
            
            // 检查交点是否在图像范围内
            int imgWidth = 1000, imgHeight = 1000;
            if (!current_img.empty()) {
                imgWidth = current_img.cols;
                imgHeight = current_img.rows;
            }
            
            // 放宽边界检查
            if (intersection.x >= -100 && intersection.x < imgWidth + 100 && 
                intersection.y >= -100 && intersection.y < imgHeight + 100) {
                intersections.push_back(intersection);
                
                qDebug() << "交点: (" << intersection.x << ", " 
                         << intersection.y << ")";
            }
        }
    }
    
    // 按Y坐标排序（从上到下）
    std::sort(intersections.begin(), intersections.end(),
              [](const cv::Point2f& a, const cv::Point2f& b) {
                  return a.y < b.y;
              });
    
    // 简单验证：交点数量应该等于水平线数量
    if (intersections.size() != boardLines.size()) {
        qDebug() << "警告: 交点数量(" << intersections.size() 
                 << ")不等于水平线数量(" << boardLines.size() << ")";
    }
    
    qDebug() << "最终交点数量:" << intersections.size();
    
    return intersections;
}
// 按Y坐标排序
void MainWindow::sortIntersectionsByY(std::vector<cv::Point2f>& intersections)
{
    std::sort(intersections.begin(), intersections.end(),
              [](const cv::Point2f& a, const cv::Point2f& b) {
                  return a.y < b.y;
              });
}

// 可视化标定板角点
void MainWindow::visualizeChessboardCorners(cv::Mat& img, const std::vector<cv::Point2f>& corners)
{
    cv::Mat result = img.clone();
    if (result.channels() == 1) {
        cv::cvtColor(result, result, cv::COLOR_GRAY2BGR);
    }
    
    if (!corners.empty()) {
        cv::drawChessboardCorners(result, patternSize, corners, true);
        
        // 显示角点数量
        QString info = QString("检测到 %1 个角点").arg(corners.size());
        cv::putText(result, info.toStdString(), cv::Point(20, 30),
                   cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 0), 2);
    }
    
    displayImage(result);
}

// 可视化激光线和交点
void MainWindow::visualizeLaserAndIntersections(cv::Mat& img, const LaserLineData& laserData)
{
    cv::Mat result = img.clone();
    if (result.channels() == 1) {
        cv::cvtColor(result, result, cv::COLOR_GRAY2BGR);
    }
    
    // 绘制激光线
    float vx = laserData.lineEquation[0];
    float vy = laserData.lineEquation[1];
    float x0 = laserData.lineEquation[2];
    float y0 = laserData.lineEquation[3];
    
    cv::Point pt1, pt2;
    pt1.x = x0 - vx * 1000;
    pt1.y = y0 - vy * 1000;
    pt2.x = x0 + vx * 1000;
    pt2.y = y0 + vy * 1000;
    
    cv::line(result, pt1, pt2, cv::Scalar(0, 0, 255), 2); // 红色激光线
    
    // 绘制交点
    for (size_t i = 0; i < laserData.intersections.size(); i++) {
        cv::circle(result, laserData.intersections[i], 8, cv::Scalar(0, 255, 255), -1); // 黄色交点
        cv::circle(result, laserData.intersections[i], 8, cv::Scalar(0, 0, 0), 2); // 黑色边框
        
        // 显示交点编号
        cv::putText(result, std::to_string(i + 1), 
                   cv::Point(laserData.intersections[i].x + 10, laserData.intersections[i].y - 10),
                   cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(255, 255, 255), 2);
        
        // 显示交点坐标
        QString coordText = QString("(%1, %2)").arg(laserData.intersections[i].x, 0, 'f', 1)
                                              .arg(laserData.intersections[i].y, 0, 'f', 1);
        cv::putText(result, coordText.toStdString(), 
                   cv::Point(laserData.intersections[i].x + 10, laserData.intersections[i].y + 25),
                   cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
    }
    
    // 显示信息
    QString info = QString("交点数量: %1").arg(laserData.intersections.size());
    cv::putText(result, info.toStdString(), cv::Point(20, 30),
               cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 0), 2);
    
    displayImage(result);
}

// 从TXT文件加载角点坐标
bool MainWindow::loadCornersFromFile(const QString& filename, std::vector<cv::Point2f>& corners)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        updateStatus("无法打开角点文件: " + filename);
        return false;
    }
    
    corners.clear();
    QTextStream in(&file);
    int lineCount = 0;
    
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        
        // 跳过注释行和空行
        if (line.isEmpty() || line.startsWith('#'))
            continue;
        
        // 解析坐标：格式可以是 "x, y" 或 "x y"
        QStringList parts;
        if (line.contains(',')) {
            parts = line.split(',', Qt::SkipEmptyParts);
        } else {
            parts = line.split(' ', Qt::SkipEmptyParts);
        }
        
        if (parts.size() >= 2) {
            bool ok1, ok2;
            float x = parts[0].trimmed().toFloat(&ok1);
            float y = parts[1].trimmed().toFloat(&ok2);
            
            if (ok1 && ok2) {
                corners.push_back(cv::Point2f(x, y));
                lineCount++;
            }
        }
    }
    
    file.close();
    
    if (corners.empty()) {
        updateStatus("角点文件为空或格式错误: " + filename);
        return false;
    }
    
    updateStatus(QString("从文件加载了 %1 个角点").arg(corners.size()));
    return true;
}

// 保存结果到文件
void MainWindow::saveResultsToFile(const QString& basePath, int pairIndex, 
                                  const std::vector<cv::Point2f>& corners,
                                  const LaserLineData& laserData)
{
    QString dirPath = basePath + "/results";
    QDir dir;
    dir.mkpath(dirPath);
    
    // 1. 保存角点坐标
    QString cornersFile = QString("%1/corners_%2.txt").arg(dirPath).arg(pairIndex, 3, 10, QChar('0'));
    QFile cfile(cornersFile);
    if (cfile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&cfile);
        out << "# 标定板角点坐标\n";
        out << "# 图像对: " << pairIndex << "\n";
        out << "# 生成时间: " << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "\n";
        out << "# 角点数量: " << corners.size() << "\n";
        out << "# 格式: X坐标, Y坐标\n";
        out << "# \n";
        
        for (size_t i = 0; i < corners.size(); i++) {
            out << QString("%1, %2\n")
                   .arg(corners[i].x, 0, 'f', 6)
                   .arg(corners[i].y, 0, 'f', 6);
        }
        cfile.close();
    }
    
    // 2. 保存激光线方程
    QString laserFile = QString("%1/laser_%2.txt").arg(dirPath).arg(pairIndex, 3, 10, QChar('0'));
    QFile lfile(laserFile);
    if (lfile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&lfile);
        out << "# 激光线方程和交点坐标\n";
        out << "# 图像对: " << pairIndex << "\n";
        out << "# 生成时间: " << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "\n";
        out << "# 激光线方程: vx, vy, x0, y0\n";
        out << QString("%1, %2, %3, %4\n")
               .arg(laserData.lineEquation[0], 0, 'f', 6)
               .arg(laserData.lineEquation[1], 0, 'f', 6)
               .arg(laserData.lineEquation[2], 0, 'f', 6)
               .arg(laserData.lineEquation[3], 0, 'f', 6);
        
        out << "\n# 交点坐标 (按Y坐标从小到大排序)\n";
        out << "# 格式: 序号, X坐标, Y坐标\n";
        out << "# \n";
        
        for (size_t i = 0; i < laserData.intersections.size(); i++) {
            out << QString("%1, %2, %3\n")
                   .arg(i + 1)
                   .arg(laserData.intersections[i].x, 0, 'f', 6)
                   .arg(laserData.intersections[i].y, 0, 'f', 6);
        }
        lfile.close();
    }
    
    updateStatus(QString("结果已保存到: %1").arg(dirPath));
}

// 功能1: 批量处理文件夹中的所有图像对
void MainWindow::on_BatchProcessButton_clicked()
{
    QString dirPath = QFileDialog::getExistingDirectory(
        this,
        "选择包含图像对的文件夹",
        "/home/vboxuser/Pictures",
        QFileDialog::ShowDirsOnly
    );
    
    if (dirPath.isEmpty()) {
        return;
    }
    
    updateStatus("开始批量处理图像对...");
    
    // 查找图像对
    QVector<QPair<QString, QString>> imagePairs; // <无激光图像, 有激光图像>
    
    for (int i = 1; i <= 20; i++) {
        QString noLaserPattern = QString("%1-1.*").arg(i);
        QString laserPattern = QString("%1-2.*").arg(i);
        
        QDir dir(dirPath);
        QStringList noLaserFiles = dir.entryList(QStringList() << noLaserPattern, QDir::Files);
        QStringList laserFiles = dir.entryList(QStringList() << laserPattern, QDir::Files);
        
        if (!noLaserFiles.isEmpty() && !laserFiles.isEmpty()) {
            QString noLaserPath = dirPath + "/" + noLaserFiles.first();
            QString laserPath = dirPath + "/" + laserFiles.first();
            imagePairs.append(qMakePair(noLaserPath, laserPath));
            
            qDebug() << "发现图像对" << i << ":" << noLaserFiles.first() << "和" << laserFiles.first();
        }
    }
    
    if (imagePairs.isEmpty()) {
        QMessageBox::warning(this, "警告", 
            "未找到符合格式的图像对！\n"
            "请确保图像命名符合：1-1.bmp, 1-2.bmp, 2-1.bmp, 2-2.bmp, ...");
        return;
    }
    
    updateStatus(QString("找到 %1 对图像，开始处理...").arg(imagePairs.size()));
    
    // 进度对话框
    QProgressDialog progress("批量处理中...", "取消", 0, imagePairs.size(), this);
    progress.setWindowModality(Qt::WindowModal);
    progress.setWindowTitle("批量处理进度");
    
    int successCount = 0;
    
    for (int i = 0; i < imagePairs.size(); i++) {
        progress.setValue(i);
        progress.setLabelText(QString("处理图像对 %1/%2").arg(i + 1).arg(imagePairs.size()));
        
        if (progress.wasCanceled()) {
            updateStatus("用户取消了批量处理");
            break;
        }
        
        QString noLaserPath = imagePairs[i].first;
        QString laserPath = imagePairs[i].second;
        
        // 1. 处理无激光图像 - 检测角点
        cv::Mat noLaserImg = cv::imread(noLaserPath.toStdString());
        if (noLaserImg.empty()) {
            qDebug() << "无法读取无激光图像:" << noLaserPath;
            continue;
        }
        
        std::vector<cv::Point2f> corners;
        if (!detectChessboardCorners(noLaserImg, corners)) {
            qDebug() << "图像对" << (i+1) << ": 角点检测失败";
            continue;
        }
        
        // 2. 处理有激光图像 - 提取激光线和计算交点
        cv::Mat laserImg = cv::imread(laserPath.toStdString());
        if (laserImg.empty()) {
            qDebug() << "无法读取激光图像:" << laserPath;
            continue;
        }
        
        // 针对暗环境提取激光线
        cv::Mat laserBinary = extractLaserBinaryForDark(laserImg);
        if (cv::countNonZero(laserBinary) < 20) {
            qDebug() << "图像对" << (i+1) << ": 激光提取失败";
            continue;
        }
        
        LaserLineData laserData;
        laserData.binaryImage = laserBinary;
        
        if (!extractLaserLine(laserBinary, laserData.lineEquation)) {
            qDebug() << "图像对" << (i+1) << ": 激光线拟合失败";
            continue;
        }
        // 按Y坐标排序
        sortIntersectionsByY(corners);
        laserData.boardLines = getBoardLinesFromCorners(corners);
        laserData.intersections = findIntersections(laserData.lineEquation, laserData.boardLines);
        
        if (laserData.intersections.empty()) {
            qDebug() << "图像对" << (i+1) << ": 未找到交点";
            continue;
        }
        
        sortIntersectionsByY(laserData.intersections);
        // 可视化结果
        visualizeLaserAndIntersections(laserImg, laserData);
        
        // 3. 保存结果
        saveResultsToFile(dirPath, i + 1, corners, laserData);
        successCount++;
        
        QCoreApplication::processEvents();
    }
    
    progress.setValue(imagePairs.size());
    
    // 显示统计结果
    QString summary = QString("批量处理完成！\n");
    summary += QString("总图像对数: %1\n").arg(imagePairs.size());
    summary += QString("成功处理: %1\n").arg(successCount);
    summary += QString("失败: %1\n").arg(imagePairs.size() - successCount);
    
    if (successCount > 0) {
        summary += QString("\n结果已保存到: %1/results/").arg(dirPath);
    }
    
    ui->ResultsTextEdit->setText(summary);
    updateStatus(QString("批量处理完成，成功 %1 对").arg(successCount));
}

// 功能2: 单独处理一个图像检测角点
void MainWindow::on_DetectCornersSingle_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "选择标定板图像",
        "/home/vboxuser/Pictures",
        tr("图片文件 (*.png *.jpg *.jpeg *.bmp *.tiff)")
    );
    
    if (fileName.isEmpty()) {
        return;
    }
    
    current_img = cv::imread(fileName.toStdString());
    if (current_img.empty()) {
        QMessageBox::warning(this, "错误", "无法读取图像文件");
        return;
    }
    
    updateStatus("正在检测标定板角点...");
    
    currentCorners.clear();
    if (detectChessboardCorners(current_img, currentCorners)) {
        // 按Y坐标排序
        sortIntersectionsByY(currentCorners);

        visualizeChessboardCorners(current_img, currentCorners);
        
        QString info = QString("角点检测成功！\n");
        info += QString("检测到 %1 个角点\n").arg(currentCorners.size());
        info += QString("标定板尺寸: %1x%2\n").arg(patternSize.width).arg(patternSize.height);
        info += "\n所有角点坐标:\n";
        
        for (int i = 0; i < currentCorners.size(); i++) {
            info += QString("  %1: (%2, %3)\n")
               .arg(i + 1)
               .arg(QString::number(currentCorners[i].x, 'f', 2))
               .arg(QString::number(currentCorners[i].y, 'f', 2));
        }
        
        // 询问是否保存角点
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "保存角点", 
            "是否保存角点坐标到文件？",
            QMessageBox::Yes | QMessageBox::No);
        
        if (reply == QMessageBox::Yes) {
            // 获取图像文件的基本名（不带扩展名）
            QFileInfo fileInfo(fileName);
            QString baseName = fileInfo.baseName();
            if (baseName.isEmpty()) {
                baseName = "corners";  // 如果图像文件名无效，使用默认名
            }
            // 构建默认文件名：图像名 + "_corners.txt"
            QString defaultFileName = baseName + "_corners.txt";

            QString saveFile = QFileDialog::getSaveFileName(
                this,
                "保存角点坐标",
                QString("/home/vboxuser/Documents") + "/" + defaultFileName,
                "文本文件 (*.txt)"
            );
            
            if (!saveFile.isEmpty()) {
                QFile file(saveFile);
                if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                    QTextStream out(&file);
                    out << "# 标定板角点坐标\n";
                    out << "# 图像文件: " << QFileInfo(fileName).fileName() << "\n";
                    out << "# 生成时间: " << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "\n";
                    out << "# 角点数量: " << currentCorners.size() << "\n";
                    out << "# 格式: X坐标, Y坐标\n";
                    out << "# \n";
                    
                    for (size_t i = 0; i < currentCorners.size(); i++) {
                        out << QString("%1, %2\n")
                               .arg(currentCorners[i].x, 0, 'f', 6)
                               .arg(currentCorners[i].y, 0, 'f', 6);
                    }
                    file.close();
                    
                    updateStatus("角点坐标已保存到: " + saveFile);
                }
            }
        }
        
        ui->ResultsTextEdit->setText(info);
        updateStatus("角点检测完成");
    } else {
        QMessageBox::warning(this, "警告", "未检测到标定板角点，请检查图像和标定板参数");
        updateStatus("角点检测失败");
    }
}

// 功能3: 单独处理激光线与标定板交点
void MainWindow::on_DetectLaserIntersections_clicked()
{
    // 1. 选择激光图像
    QString laserFile = QFileDialog::getOpenFileName(
        this,
        "选择激光图像",
        "/home/vboxuser/Pictures",
        tr("图片文件 (*.png *.jpg *.jpeg *.bmp *.tiff)")
    );
    
    if (laserFile.isEmpty()) {
        return;
    }
    
    cv::Mat laserImg = cv::imread(laserFile.toStdString());
    if (laserImg.empty()) {
        QMessageBox::warning(this, "错误", "无法读取激光图像");
        return;
    }
    
    // 2. 选择角点文件或重新检测角点
    QMessageBox::StandardButton cornerChoice;
    cornerChoice = QMessageBox::question(this, "角点信息", 
        "如何获取标定板角点信息？\n\n"
        "是(Y): 从TXT文件加载角点\n"
        "否(N): 从当前图像检测角点",
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    
    std::vector<cv::Point2f> corners;
    
    if (cornerChoice == QMessageBox::Yes) {
        // 从文件加载角点
        QString cornerFile = QFileDialog::getOpenFileName(
            this,
            "选择角点坐标文件",
            "/home/vboxuser/Documents",
            "文本文件 (*.txt)"
        );
        
        if (cornerFile.isEmpty()) {
            return;
        }
        
        if (!loadCornersFromFile(cornerFile, corners)) {
            QMessageBox::warning(this, "错误", "无法加载角点文件");
            return;
        }
    } 
    else if (cornerChoice == QMessageBox::No) {
        // 从图像检测角点
        updateStatus("正在检测标定板角点...");
        
        // 使用同一张激光图像检测角点
        // 注意：激光可能会干扰角点检测，所以可能需要预处理
        cv::Mat imgForCornerDetection = laserImg.clone();
        
        // 尝试降低激光影响：转换为灰度并增强对比度
        cv::Mat gray;
        cv::cvtColor(imgForCornerDetection, gray, cv::COLOR_BGR2GRAY);
        
        // 使用直方图均衡化增强对比度
        cv::Mat enhanced;
        cv::equalizeHist(gray, enhanced);
        
        if (!detectChessboardCorners(enhanced, corners)) {
            QMessageBox::warning(this, "警告", "未检测到标定板角点");
            return;
        }
        
        // 可视化检测到的角点
        visualizeChessboardCorners(enhanced, corners);
    }
    else {
        return; // 用户取消了
    }
    
    // 3. 提取激光线（针对暗环境优化）
    updateStatus("正在提取激光线...");
    
    current_laser_mask = extractLaserBinaryForDark(laserImg);
    
    // 显示激光二值图像
    cv::Mat laserDisplay = current_laser_mask.clone();
    cv::cvtColor(laserDisplay, laserDisplay, cv::COLOR_GRAY2BGR);
    // displayImage(laserDisplay);
    
    // 4. 提取激光线方程
    updateStatus("正在拟合激光线方程...");
    
    if (!extractLaserLine(current_laser_mask, currentLaserData.lineEquation)) {
        QMessageBox::warning(this, "错误", "无法提取激光线方程");
        updateStatus("激光线拟合失败");
        return;
    }
    
    // 5. 从角点获取标定板直线
    currentLaserData.boardLines = getBoardLinesFromCorners(corners);
    
    // 6. 计算交点
    updateStatus("正在计算交点...");
    
    currentLaserData.intersections = findIntersections(currentLaserData.lineEquation, 
                                                       currentLaserData.boardLines);
    
    if (currentLaserData.intersections.empty()) {
        QMessageBox::warning(this, "警告", "未找到激光线与标定板的交点");
        updateStatus("未找到交点");
        return;
    }
    
    // 7. 按Y坐标排序
    sortIntersectionsByY(currentLaserData.intersections);
    
    // 8. 可视化结果
    visualizeLaserAndIntersections(laserImg, currentLaserData);
    
    // 9. 显示结果信息
    QString info = QString("激光线与标定板交点检测完成！\n");
    info += QString("标定板角点: %1 个\n").arg(corners.size());
    info += QString("激光线方程:\n");
    info += QString("  方向向量: (%1, %2)\n")
            .arg(QString::number(currentLaserData.lineEquation[0], 'f', 3))
            .arg(QString::number(currentLaserData.lineEquation[1], 'f', 3));
    info += QString("  直线上点: (%1, %2)\n")
            .arg(QString::number(currentLaserData.lineEquation[2], 'f', 1))
            .arg(QString::number(currentLaserData.lineEquation[3], 'f', 1));
    info += QString("\n检测到交点: %1 个\n").arg(currentLaserData.intersections.size());
    info += "交点坐标(从上到下):\n";

    for (size_t i = 0; i < currentLaserData.intersections.size(); i++) {
        info += QString("  %1: (%2, %3)\n")
                .arg(i + 1)
                .arg(QString::number(currentLaserData.intersections[i].x, 'f', 2))
                .arg(QString::number(currentLaserData.intersections[i].y, 'f', 2));
    }

    ui->ResultsTextEdit->setText(info);
    updateStatus("激光线与标定板交点检测完成");
    
    // 10. 询问是否保存结果
    QMessageBox::StandardButton saveChoice;
    saveChoice = QMessageBox::question(this, "保存结果", 
    "是否保存处理结果到文件？",
    QMessageBox::Yes | QMessageBox::No);

if (saveChoice == QMessageBox::Yes) {
    // 获取激光图像文件的基本名
    QFileInfo laserFileInfo(laserFile);
    QString baseName = laserFileInfo.baseName();
    if (baseName.isEmpty()) {
        baseName = "laser_results";  // 如果激光文件名无效，使用默认名
    }
    
    QString saveDir = QFileDialog::getExistingDirectory(
        this,
        "/home/vboxuser/Documents",
        QDir::currentPath()
    );
    
    if (!saveDir.isEmpty()) {
        // 创建结果目录，使用激光图像文件名作为目录名
        QString resultsDir = saveDir + "/" + baseName + "_results";
        QDir dir;
        dir.mkpath(resultsDir);
        
        // 保存角点，使用激光图像文件名作为前缀
        QString cornersFile = resultsDir + "/" + baseName + "_corners.txt";
        QFile cfile(cornersFile);
        if (cfile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&cfile);
            out << "# 标定板角点坐标\n";
            out << "# 激光图像: " << QFileInfo(laserFile).fileName() << "\n";
            out << "# 生成时间: " << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "\n";
            out << "# 角点数量: " << corners.size() << "\n";
            out << "# 格式: X坐标, Y坐标\n";
            out << "# \n";
            
            for (size_t i = 0; i < corners.size(); i++) {
                out << QString("%1, %2\n")
                       .arg(corners[i].x, 0, 'f', 6)
                       .arg(corners[i].y, 0, 'f', 6);
            }
            cfile.close();
        }
        
        // 保存激光线方程和交点，使用激光图像文件名作为前缀
        QString laserFileOut = resultsDir + "/" + baseName + "_intersections.txt";
        QFile lfile(laserFileOut);
        if (lfile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&lfile);
            out << "# 激光线方程和交点坐标\n";
            out << "# 激光图像: " << QFileInfo(laserFile).fileName() << "\n";
            out << "# 生成时间: " << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "\n";
            out << "# 激光线方程: vx, vy, x0, y0\n";
            out << QString("%1, %2, %3, %4\n")
                   .arg(currentLaserData.lineEquation[0], 0, 'f', 6)
                   .arg(currentLaserData.lineEquation[1], 0, 'f', 6)
                   .arg(currentLaserData.lineEquation[2], 0, 'f', 6)
                   .arg(currentLaserData.lineEquation[3], 0, 'f', 6);
            
            out << "\n# 交点坐标 (按Y坐标从小到大排序)\n";
            out << "# 格式: 序号, X坐标, Y坐标\n";
            out << "# \n";
            
            for (size_t i = 0; i < currentLaserData.intersections.size(); i++) {
                out << QString("%1, %2, %3\n")
                       .arg(i + 1)
                       .arg(currentLaserData.intersections[i].x, 0, 'f', 6)
                       .arg(currentLaserData.intersections[i].y, 0, 'f', 6);
            }
            lfile.close();
        }
        
        updateStatus("结果已保存到: " + resultsDir);
        }
    }
}
