/**
 * @file main.cpp
 * @brief test class Document
 * @author xiangxw xiangxw5689@126.com
 * @date 2012-03-28
 */

#include <QtGui/QApplication>
#include <QtGui/QFileDialog>
#include <QDebug>
#include "mupdf-qt.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);

	QString file = QFileDialog::getOpenFileName(NULL, "Select PDF file", ".", "PDF (*.pdf)");
	if (file.isEmpty()) {
		return 0;
	}
	MuPDF::Document *document = MuPDF::loadDocument(file);
	if (document) {
		qDebug() << "Page count:" << document->numPages();
//		qDebug() << "Title:" << document.getInfo(MuPDF::Document::PDFInfoTitle);
//		qDebug() << "Subject:" << document.getInfo(MuPDF::Document::PDFInfoSubject);
//		qDebug() << "Author:" << document.getInfo(MuPDF::Document::PDFInfoAuthor);
//		qDebug() << "Keywords:" << document.getInfo(MuPDF::Document::PDFInfoKeywords);
//		qDebug() << "Creator:" << document.getInfo(MuPDF::Document::PDFInfoCreator);
//		qDebug() << "Producer:" << document.getInfo(MuPDF::Document::PDFInfoProducer);
//		qDebug() << "CreationDate:" << document.getInfo(MuPDF::Document::PDFInfoCreationDate);
//		qDebug() << "ModDate:" << document.getInfo(MuPDF::Document::PDFInfoModDate);
		delete document;
	} else {
		qDebug() << "not loaded";
	}

	return 0;
}
