/********************************************************************************
** Form generated from reading UI file 'FFmpegVideo.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FFMPEGVIDEO_H
#define UI_FFMPEGVIDEO_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_FFmpegVideoClass
{
public:

    void setupUi(QDialog *FFmpegVideoClass)
    {
        if (FFmpegVideoClass->objectName().isEmpty())
            FFmpegVideoClass->setObjectName(QStringLiteral("FFmpegVideoClass"));
        FFmpegVideoClass->resize(600, 400);

        retranslateUi(FFmpegVideoClass);

        QMetaObject::connectSlotsByName(FFmpegVideoClass);
    } // setupUi

    void retranslateUi(QDialog *FFmpegVideoClass)
    {
        FFmpegVideoClass->setWindowTitle(QApplication::translate("FFmpegVideoClass", "FFmpegVideo", 0));
    } // retranslateUi

};

namespace Ui {
    class FFmpegVideoClass: public Ui_FFmpegVideoClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FFMPEGVIDEO_H
