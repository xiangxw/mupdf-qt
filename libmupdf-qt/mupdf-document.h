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

namespace MuPDF
{
class Document;
class DocumentPrivate;
class Page;

Document * MUPDF_QT_EXPORT loadDocument(const QString &filePath);

class MUPDF_QT_EXPORT Document
{
public:
	~Document();
	bool needsPassword() const;
	bool authPassword(const QString &password);
	int numPages() const;
	Page *page(int index) const;

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
