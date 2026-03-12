/********************************************************************************
** Form generated from reading UI file 'tracking_window.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRACKING_WINDOW_H
#define UI_TRACKING_WINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Tracking_Window
{
public:
    QFrame *frame;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *Auto_Identify;
    QPushButton *Start_Tracking;
    QPushButton *Stop_Tracking;
    QFrame *frame_2;
    QGraphicsView *graphicsView;
    QLabel *MessageBox;

    void setupUi(QWidget *Tracking_Window)
    {
        if (Tracking_Window->objectName().isEmpty())
            Tracking_Window->setObjectName("Tracking_Window");
        Tracking_Window->resize(850, 550);
        frame = new QFrame(Tracking_Window);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(0, 0, 850, 40));
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        widget = new QWidget(frame);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(0, 0, 851, 41));
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        Auto_Identify = new QPushButton(widget);
        Auto_Identify->setObjectName("Auto_Identify");

        horizontalLayout_2->addWidget(Auto_Identify);

        Start_Tracking = new QPushButton(widget);
        Start_Tracking->setObjectName("Start_Tracking");

        horizontalLayout_2->addWidget(Start_Tracking);

        Stop_Tracking = new QPushButton(widget);
        Stop_Tracking->setObjectName("Stop_Tracking");

        horizontalLayout_2->addWidget(Stop_Tracking);

        frame_2 = new QFrame(Tracking_Window);
        frame_2->setObjectName("frame_2");
        frame_2->setGeometry(QRect(0, 40, 850, 480));
        frame_2->setFrameShape(QFrame::Shape::StyledPanel);
        frame_2->setFrameShadow(QFrame::Shadow::Raised);
        graphicsView = new QGraphicsView(frame_2);
        graphicsView->setObjectName("graphicsView");
        graphicsView->setGeometry(QRect(0, 0, 850, 480));
        MessageBox = new QLabel(Tracking_Window);
        MessageBox->setObjectName("MessageBox");
        MessageBox->setGeometry(QRect(0, 520, 850, 30));

        retranslateUi(Tracking_Window);

        QMetaObject::connectSlotsByName(Tracking_Window);
    } // setupUi

    void retranslateUi(QWidget *Tracking_Window)
    {
        Tracking_Window->setWindowTitle(QCoreApplication::translate("Tracking_Window", "Form", nullptr));
        Auto_Identify->setText(QCoreApplication::translate("Tracking_Window", "Auto Identify", nullptr));
        Start_Tracking->setText(QCoreApplication::translate("Tracking_Window", "Start Tracking", nullptr));
        Stop_Tracking->setText(QCoreApplication::translate("Tracking_Window", "Stop Tracking", nullptr));
        MessageBox->setText(QCoreApplication::translate("Tracking_Window", "MessageBox", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Tracking_Window: public Ui_Tracking_Window {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRACKING_WINDOW_H
