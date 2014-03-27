/**
 * @file mupdf-document.h
 * @brief class Document
 * @author xiangxw xiangxw5689@126.com
 * @date 2012-04-03
 */

#ifndef MUPDF_DOCUMENT_H
#define MUPDF_DOCUMENT_H

#include "mupdf-global.h"

class QString;
class QDateTime;

class DocumentPrivate;
namespace MuPDF
{
class Document;
class Page;

Document * MUPDF_QT_EXPORT loadDocument(const QString &filePath);

class MUPDF_QT_EXPORT Document
{
public:
	~Document();
	bool needsPassword() const;
	bool authPassword(const QString &password);
	int numPages() const;
	Page *page(int index, float scaleX = 1.0f, float scaleY = 1.0f, float rotation = 0.0f) const;
	QString pdfVersion() const;
	QString title() const;
	QString author() const;
	QString subject() const;
	QString keywords() const;
	QString creator() const;
	QString producer() const;
	QDateTime creationDate() const;
	QDateTime modDate() const;
	void setTransparentRendering(bool enable);
	void setBackgroundColor(int r, int g, int b, int a = 255);

private:
	Document(const QString &filePath);
	// disable copy
	Document(const Document &);
	Document &operator=(const Document &);

	DocumentPrivate *d;

friend class Page;
friend Document *loadDocument(const QString &filePath);
};

} // end namespace MuPDF

#endif // end MUPDF_DOCUMENT_H
