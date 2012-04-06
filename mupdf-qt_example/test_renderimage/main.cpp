/**
 * @file main.cpp
 * @brief test function Page::renderImage
 * @author xiangxw xiangxw5689@126.com
 * @date 2012-03-31
 */

#include "mupdf-qt.h"
#include <QtGui/QApplication>
#include <QtGui/QFileDialog>
#include <QtGui/QLabel>
#include <QtGui/QPixmap>
#include <QtGui/QImage>

int main(int argc, char **argv)
{
	QApplication app(argc, argv);

	QString file = QFileDialog::getOpenFileName(NULL, "Select PDF file", ".", "PDF (*.pdf)");
	if (file.isEmpty()) {
		return 0;
	}

	MuPDF::Document *document = MuPDF::loadDocument(file);
	if (NULL == document) {
		return -1;
	}
	MuPDF::Page *page = document->page(0);
	if (NULL == page) {
		delete document;
		return -1;
	}
	QImage image = page->renderImage();
	QLabel label;
	label.setPixmap(QPixmap::fromImage(image));
	label.show();

	return app.exec();
}
