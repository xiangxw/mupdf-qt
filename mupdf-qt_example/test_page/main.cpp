/**
 * @file main.cpp
 * @brief test function Page::renderImage
 * @author xiangxw xiangxw5689@126.com
 * @date 2012-03-31
 */

#include "mupdf-qt.h"
#include <QImage>
#include <QDebug>

int main(int argc, char **argv)
{
	// open document
	QString file = argv[1];
	if (file.isEmpty()) {
		return 1;
	}
	MuPDF::Document *document = MuPDF::loadDocument(file);
	if (NULL == document) {
		return -1;
	}

	// load page
	MuPDF::Page *page = document->page(0);
	if (NULL == page) {
		delete document;
		return -1;
	}
	page->setTransform(1.0f, 1.0f, 90.0f);

	// test Page::size()
	qDebug() << page->size();

	// test Page::renderImage()
	{
		// image should be deleted before document is deleted
		QImage image = page->renderImage();
		if (!image.save("a.png")) {
			return 1;
		}
	}

	delete page;
	delete document;
	return 0;
}
