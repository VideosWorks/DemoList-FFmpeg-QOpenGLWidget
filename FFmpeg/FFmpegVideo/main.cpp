#include "FFmpegVideo.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	FFmpegVideo w;
	w.show();
	return a.exec();
}
