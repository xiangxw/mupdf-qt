/**
 * @file main.cpp
 * @brief simple PDF viewer
 * @author xiangxw xiangxw5689@126.com
 * @date 2012-04-01
 */

#include "mainwindow.h"
#include <QtGui/QApplication>

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	MainWindow w;
	w.show();
	return app.exec();
}
