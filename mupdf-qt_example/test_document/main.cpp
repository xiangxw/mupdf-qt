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
#include <QtCore/QDateTime>
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
		qDebug() << "PDF version:" << document->pdfVersion();
		qDebug() << "Page count:" << document->numPages();
		qDebug() << "Title:" << document->title();
		qDebug() << "Subject:" << document->subject();
		qDebug() << "Author:" << document->author();
		qDebug() << "Keywords:" << document->keywords();
		qDebug() << "Creator:" << document->creator();
		qDebug() << "Producer:" << document->producer();
		qDebug() << "CreationDate:" << document->creationDate()
			.toString(Qt::SystemLocaleLongDate);
		qDebug() << "ModDate:" << document->modDate()
			.toString(Qt::SystemLocaleLongDate);
	} else {
		qDebug() << "not loaded";
	}

	delete document;
    return 0;
}
