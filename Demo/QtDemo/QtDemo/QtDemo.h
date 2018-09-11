#ifndef QTDEMO_H
#define QTDEMO_H

#include <QtWidgets/QDialog>
#include "ui_QtDemo.h"

class QtDemo : public QDialog
{
	Q_OBJECT

public:
	QtDemo(QWidget *parent = 0);
	~QtDemo();

private:
	Ui::QtDemoClass ui;
};

#endif // QTDEMO_H
