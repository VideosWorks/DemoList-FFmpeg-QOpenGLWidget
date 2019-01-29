#ifndef FFMPEGVIDEO_H
#define FFMPEGVIDEO_H

#include <QtWidgets/QDialog>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QGLShaderProgram>
#include "ui_FFmpegVideo.h"

class FFmpegVideo : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	FFmpegVideo(QWidget *parent = 0);
	~FFmpegVideo();

protected:
	virtual void initializeGL();
	virtual void resizeGL(int w, int h);
	virtual void paintGL();
	virtual void timerEvent(QTimerEvent *event);

private:
	unsigned int m_textures[3];
	QGLShaderProgram m_shaderProgram;
	unsigned int m_yuv[3];
	unsigned char* data[3];

	FILE* file;
};

#endif // FFMPEGVIDEO_H
