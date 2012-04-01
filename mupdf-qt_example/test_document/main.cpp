/**
 * @file main.cpp
 * @brief test class Document
 * @author xiangxw xiangxw5689@126.com
 * @date 2012-03-28
 */

#include "mupdf-qt.h"
#include <QtGui/QApplication>
#include <QtGui/QFileDialog>
#include <QDebug>

int main(int argc, char **argv)
{
	QApplication app(argc, argv);

	QString file = QFileDialog::getOpenFileName(NULL, "Select PDF file", ".", "PDF (*.pdf)");
	if (file.isEmpty()) {
		return 0;
	}
	Mupdf::Document document(file);
	if (document.isLoaded()) {
		qDebug() << "Page count:" << document.numPages();
		qDebug() << "Title:" << document.getInfo(Mupdf::Document::PDFInfoTitle);
		qDebug() << "Subject:" << document.getInfo(Mupdf::Document::PDFInfoSubject);
		qDebug() << "Author:" << document.getInfo(Mupdf::Document::PDFInfoAuthor);
		qDebug() << "Keywords:" << document.getInfo(Mupdf::Document::PDFInfoKeywords);
		qDebug() << "Creator:" << document.getInfo(Mupdf::Document::PDFInfoCreator);
		qDebug() << "Producer:" << document.getInfo(Mupdf::Document::PDFInfoProducer);
		qDebug() << "CreationDate:" << document.getInfo(Mupdf::Document::PDFInfoCreationDate);
		qDebug() << "ModDate:" << document.getInfo(Mupdf::Document::PDFInfoModDate);
	} else {
		qDebug() << "not loaded";
	}

	return 0;
}
