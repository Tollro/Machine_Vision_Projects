/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QLabel *logo;
    QLabel *label_3;
    QSpacerItem *horizontalSpacer;
    QLabel *label;
    QPushButton *OpenImg_Botton;
    QTabWidget *Tab4;
    QWidget *tab;
    QWidget *tab_2;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *Gaussian_Filter;
    QPushButton *Median_Filter;
    QPushButton *Bilateral_Filter;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *label_4;
    QDoubleSpinBox *GaussianKernelSize;
    QLabel *label_2;
    QDoubleSpinBox *GaussianSigmaX;
    QLabel *label_5;
    QDoubleSpinBox *GaussianSigmaY;
    QWidget *formLayoutWidget_2;
    QFormLayout *formLayout_2;
    QLabel *label_6;
    QDoubleSpinBox *MedianAperture;
    QWidget *formLayoutWidget_3;
    QFormLayout *formLayout_3;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QDoubleSpinBox *BilateralDiameter;
    QDoubleSpinBox *BilateralSigmaColor;
    QDoubleSpinBox *BilateralSigmaSpace;
    QWidget *tab_3;
    QPushButton *Detect_Blocks;
    QWidget *tab_4;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QPushButton *OpenVideoFile;
    QPushButton *OpenCam;
    QPushButton *CannyVideo;
    QWidget *tab_5;
    QPushButton *Object_Tracking;
    QWidget *tab_6;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_8;
    QSpinBox *PatternWidth;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_12;
    QSpinBox *PatternHeight;
    QTextEdit *ResultsTextEdit;
    QWidget *widget1;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *BatchProcessButton;
    QPushButton *DetectCornersSingle;
    QPushButton *DetectLaserIntersections;
    QLabel *Prompt_msg;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1033, 711);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        widget = new QWidget(centralwidget);
        widget->setObjectName("widget");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName("horizontalLayout");
        logo = new QLabel(widget);
        logo->setObjectName("logo");
        logo->setMaximumSize(QSize(25, 25));
        logo->setText(QString::fromUtf8(""));
        logo->setPixmap(QPixmap(QString::fromUtf8("logo.jpg")));
        logo->setScaledContents(true);

        horizontalLayout->addWidget(logo);

        label_3 = new QLabel(widget);
        label_3->setObjectName("label_3");

        horizontalLayout->addWidget(label_3);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        label = new QLabel(widget);
        label->setObjectName("label");

        horizontalLayout->addWidget(label);


        verticalLayout->addWidget(widget);

        OpenImg_Botton = new QPushButton(centralwidget);
        OpenImg_Botton->setObjectName("OpenImg_Botton");

        verticalLayout->addWidget(OpenImg_Botton);

        Tab4 = new QTabWidget(centralwidget);
        Tab4->setObjectName("Tab4");
        tab = new QWidget();
        tab->setObjectName("tab");
        Tab4->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        layoutWidget = new QWidget(tab_2);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(0, 0, 1011, 31));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        Gaussian_Filter = new QPushButton(layoutWidget);
        Gaussian_Filter->setObjectName("Gaussian_Filter");

        horizontalLayout_2->addWidget(Gaussian_Filter);

        Median_Filter = new QPushButton(layoutWidget);
        Median_Filter->setObjectName("Median_Filter");

        horizontalLayout_2->addWidget(Median_Filter);

        Bilateral_Filter = new QPushButton(layoutWidget);
        Bilateral_Filter->setObjectName("Bilateral_Filter");

        horizontalLayout_2->addWidget(Bilateral_Filter);

        formLayoutWidget = new QWidget(tab_2);
        formLayoutWidget->setObjectName("formLayoutWidget");
        formLayoutWidget->setGeometry(QRect(20, 40, 231, 101));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName("formLayout");
        formLayout->setContentsMargins(0, 0, 0, 0);
        label_4 = new QLabel(formLayoutWidget);
        label_4->setObjectName("label_4");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, label_4);

        GaussianKernelSize = new QDoubleSpinBox(formLayoutWidget);
        GaussianKernelSize->setObjectName("GaussianKernelSize");
        GaussianKernelSize->setMinimum(3.000000000000000);
        GaussianKernelSize->setMaximum(99.000000000000000);
        GaussianKernelSize->setSingleStep(2.000000000000000);
        GaussianKernelSize->setValue(15.000000000000000);

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, GaussianKernelSize);

        label_2 = new QLabel(formLayoutWidget);
        label_2->setObjectName("label_2");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, label_2);

        GaussianSigmaX = new QDoubleSpinBox(formLayoutWidget);
        GaussianSigmaX->setObjectName("GaussianSigmaX");
        GaussianSigmaX->setValue(3.000000000000000);

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, GaussianSigmaX);

        label_5 = new QLabel(formLayoutWidget);
        label_5->setObjectName("label_5");

        formLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, label_5);

        GaussianSigmaY = new QDoubleSpinBox(formLayoutWidget);
        GaussianSigmaY->setObjectName("GaussianSigmaY");

        formLayout->setWidget(2, QFormLayout::ItemRole::FieldRole, GaussianSigmaY);

        formLayoutWidget_2 = new QWidget(tab_2);
        formLayoutWidget_2->setObjectName("formLayoutWidget_2");
        formLayoutWidget_2->setGeometry(QRect(390, 40, 231, 41));
        formLayout_2 = new QFormLayout(formLayoutWidget_2);
        formLayout_2->setObjectName("formLayout_2");
        formLayout_2->setContentsMargins(0, 0, 0, 0);
        label_6 = new QLabel(formLayoutWidget_2);
        label_6->setObjectName("label_6");

        formLayout_2->setWidget(0, QFormLayout::ItemRole::LabelRole, label_6);

        MedianAperture = new QDoubleSpinBox(formLayoutWidget_2);
        MedianAperture->setObjectName("MedianAperture");
        MedianAperture->setMinimum(3.000000000000000);
        MedianAperture->setMaximum(99.000000000000000);
        MedianAperture->setSingleStep(2.000000000000000);

        formLayout_2->setWidget(0, QFormLayout::ItemRole::FieldRole, MedianAperture);

        formLayoutWidget_3 = new QWidget(tab_2);
        formLayoutWidget_3->setObjectName("formLayoutWidget_3");
        formLayoutWidget_3->setGeometry(QRect(720, 40, 221, 101));
        formLayout_3 = new QFormLayout(formLayoutWidget_3);
        formLayout_3->setObjectName("formLayout_3");
        formLayout_3->setContentsMargins(0, 0, 0, 0);
        label_9 = new QLabel(formLayoutWidget_3);
        label_9->setObjectName("label_9");

        formLayout_3->setWidget(0, QFormLayout::ItemRole::LabelRole, label_9);

        label_10 = new QLabel(formLayoutWidget_3);
        label_10->setObjectName("label_10");

        formLayout_3->setWidget(1, QFormLayout::ItemRole::LabelRole, label_10);

        label_11 = new QLabel(formLayoutWidget_3);
        label_11->setObjectName("label_11");

        formLayout_3->setWidget(2, QFormLayout::ItemRole::LabelRole, label_11);

        BilateralDiameter = new QDoubleSpinBox(formLayoutWidget_3);
        BilateralDiameter->setObjectName("BilateralDiameter");
        BilateralDiameter->setValue(15.000000000000000);

        formLayout_3->setWidget(0, QFormLayout::ItemRole::FieldRole, BilateralDiameter);

        BilateralSigmaColor = new QDoubleSpinBox(formLayoutWidget_3);
        BilateralSigmaColor->setObjectName("BilateralSigmaColor");
        BilateralSigmaColor->setMaximum(500.000000000000000);
        BilateralSigmaColor->setSingleStep(10.000000000000000);
        BilateralSigmaColor->setValue(100.000000000000000);

        formLayout_3->setWidget(1, QFormLayout::ItemRole::FieldRole, BilateralSigmaColor);

        BilateralSigmaSpace = new QDoubleSpinBox(formLayoutWidget_3);
        BilateralSigmaSpace->setObjectName("BilateralSigmaSpace");
        BilateralSigmaSpace->setValue(9.000000000000000);

        formLayout_3->setWidget(2, QFormLayout::ItemRole::FieldRole, BilateralSigmaSpace);

        Tab4->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName("tab_3");
        Detect_Blocks = new QPushButton(tab_3);
        Detect_Blocks->setObjectName("Detect_Blocks");
        Detect_Blocks->setGeometry(QRect(430, 10, 141, 71));
        Detect_Blocks->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        Tab4->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName("tab_4");
        gridLayoutWidget = new QWidget(tab_4);
        gridLayoutWidget->setObjectName("gridLayoutWidget");
        gridLayoutWidget->setGeometry(QRect(180, 20, 251, 211));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        OpenVideoFile = new QPushButton(gridLayoutWidget);
        OpenVideoFile->setObjectName("OpenVideoFile");
        OpenVideoFile->setMaximumSize(QSize(16777215, 17000));

        gridLayout->addWidget(OpenVideoFile, 0, 0, 1, 1);

        OpenCam = new QPushButton(gridLayoutWidget);
        OpenCam->setObjectName("OpenCam");
        OpenCam->setMaximumSize(QSize(16777215, 17000));

        gridLayout->addWidget(OpenCam, 1, 0, 1, 1);

        CannyVideo = new QPushButton(tab_4);
        CannyVideo->setObjectName("CannyVideo");
        CannyVideo->setGeometry(QRect(490, 70, 151, 81));
        Tab4->addTab(tab_4, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName("tab_5");
        Object_Tracking = new QPushButton(tab_5);
        Object_Tracking->setObjectName("Object_Tracking");
        Object_Tracking->setGeometry(QRect(400, 30, 191, 91));
        Tab4->addTab(tab_5, QString());
        tab_6 = new QWidget();
        tab_6->setObjectName("tab_6");
        layoutWidget1 = new QWidget(tab_6);
        layoutWidget1->setObjectName("layoutWidget1");
        layoutWidget1->setGeometry(QRect(10, 90, 131, 91));
        verticalLayout_2 = new QVBoxLayout(layoutWidget1);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        label_8 = new QLabel(layoutWidget1);
        label_8->setObjectName("label_8");

        horizontalLayout_4->addWidget(label_8);

        PatternWidth = new QSpinBox(layoutWidget1);
        PatternWidth->setObjectName("PatternWidth");
        PatternWidth->setValue(8);

        horizontalLayout_4->addWidget(PatternWidth);


        verticalLayout_2->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        label_12 = new QLabel(layoutWidget1);
        label_12->setObjectName("label_12");

        horizontalLayout_5->addWidget(label_12);

        PatternHeight = new QSpinBox(layoutWidget1);
        PatternHeight->setObjectName("PatternHeight");
        PatternHeight->setValue(5);

        horizontalLayout_5->addWidget(PatternHeight);


        verticalLayout_2->addLayout(horizontalLayout_5);

        ResultsTextEdit = new QTextEdit(tab_6);
        ResultsTextEdit->setObjectName("ResultsTextEdit");
        ResultsTextEdit->setGeometry(QRect(240, 100, 521, 431));
        widget1 = new QWidget(tab_6);
        widget1->setObjectName("widget1");
        widget1->setGeometry(QRect(270, 10, 451, 41));
        horizontalLayout_3 = new QHBoxLayout(widget1);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        BatchProcessButton = new QPushButton(widget1);
        BatchProcessButton->setObjectName("BatchProcessButton");

        horizontalLayout_3->addWidget(BatchProcessButton);

        DetectCornersSingle = new QPushButton(widget1);
        DetectCornersSingle->setObjectName("DetectCornersSingle");

        horizontalLayout_3->addWidget(DetectCornersSingle);

        DetectLaserIntersections = new QPushButton(widget1);
        DetectLaserIntersections->setObjectName("DetectLaserIntersections");

        horizontalLayout_3->addWidget(DetectLaserIntersections);

        Tab4->addTab(tab_6, QString());

        verticalLayout->addWidget(Tab4);

        Prompt_msg = new QLabel(centralwidget);
        Prompt_msg->setObjectName("Prompt_msg");

        verticalLayout->addWidget(Prompt_msg);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        Tab4->setCurrentIndex(5);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Qt\345\233\276\345\203\217\345\267\245\347\250\213", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\347\224\265\344\277\241231 \346\261\240\345\245\224\347\277\260 3230432010", nullptr));
        OpenImg_Botton->setText(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200\345\233\276\345\203\217\357\274\210browser\357\274\211", nullptr));
        Tab4->setTabText(Tab4->indexOf(tab), QCoreApplication::translate("MainWindow", "Tab 1", nullptr));
        Gaussian_Filter->setText(QCoreApplication::translate("MainWindow", "Gaussian_Filter", nullptr));
        Median_Filter->setText(QCoreApplication::translate("MainWindow", "Median_Filter", nullptr));
        Bilateral_Filter->setText(QCoreApplication::translate("MainWindow", "Bilateral_Filter", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Kernel Size: ", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Sigma X: ", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Sigma Y: ", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "Aperture Size: ", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "Diameter: ", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "Sigma Color: ", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "Sigma Space: ", nullptr));
        Tab4->setTabText(Tab4->indexOf(tab_2), QCoreApplication::translate("MainWindow", "Tab 2", nullptr));
        Detect_Blocks->setText(QCoreApplication::translate("MainWindow", "Detect Blocks", nullptr));
        Tab4->setTabText(Tab4->indexOf(tab_3), QCoreApplication::translate("MainWindow", "Tab 3", nullptr));
        OpenVideoFile->setText(QCoreApplication::translate("MainWindow", "OpenVideoFile", nullptr));
        OpenCam->setText(QCoreApplication::translate("MainWindow", "OpenCam", nullptr));
        CannyVideo->setText(QCoreApplication::translate("MainWindow", "CannyVideo", nullptr));
        Tab4->setTabText(Tab4->indexOf(tab_4), QCoreApplication::translate("MainWindow", "Tab 4", nullptr));
        Object_Tracking->setText(QCoreApplication::translate("MainWindow", "Object Tracking", nullptr));
        Tab4->setTabText(Tab4->indexOf(tab_5), QCoreApplication::translate("MainWindow", "Tab 5", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "col\357\274\232", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "row\357\274\232", nullptr));
        BatchProcessButton->setText(QCoreApplication::translate("MainWindow", "\346\211\271\351\207\217\345\244\204\347\220\206\345\233\276\345\203\217\345\257\271", nullptr));
        DetectCornersSingle->setText(QCoreApplication::translate("MainWindow", "\345\215\225\347\213\254\346\243\200\346\265\213\350\247\222\347\202\271", nullptr));
        DetectLaserIntersections->setText(QCoreApplication::translate("MainWindow", "\346\243\200\346\265\213\346\277\200\345\205\211\344\272\244\347\202\271", nullptr));
        Tab4->setTabText(Tab4->indexOf(tab_6), QCoreApplication::translate("MainWindow", "Tab 6", nullptr));
        Prompt_msg->setText(QCoreApplication::translate("MainWindow", "Prompt Messages", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
