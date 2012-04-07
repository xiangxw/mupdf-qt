/**
 * @file main.cpp
 * @brief test class Document
 * @author xiangxw xiangxw5689@126.com
 * @date 2012-03-28
 */

#include <QtGui/QApplication>
#include <QtGui/QFileDialog>
#include <QtGui/QInputDialog>
#include <QtGui/QLineEdit>
#include <QDebug>
#include "mupdf-qt.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);

	// open document
	QString file = QFileDialog::getOpenFileName(NULL,
			"Select PDF file", ".", "PDF (*.pdf)");
	if (file.isEmpty()) {
		return 0;
	}
	MuPDF::Document *document = MuPDF::loadDocument(file);
	if (NULL == document) {
		return 0;
	}

	// authenticate
	bool authed = false;
	if (document->needsPassword()) {
		bool ok = true;
		QString password;
		while (!authed && ok) {
			password = QInputDialog::getText(NULL,
					"Please input password", "Password",
					QLineEdit::Password, "", &ok);
			if (ok) {
				authed = document->authPassword(password);
			}
		}
	} else {
		authed = true;
	}

	// print info
	if (authed) {
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
