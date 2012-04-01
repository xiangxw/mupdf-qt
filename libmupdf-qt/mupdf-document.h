/**
 * @file mupdf-document.h
 * @brief class Document
 * @author xiangxw xiangxw5689@126.com
 * @date 2012-03-28
 */

#ifndef MUPDF_DOCUMENT_H
#define MUPDF_DOCUMENT_H

#include "mupdf-global.h"

class QString;

namespace Mupdf
{
class Document;
class DocumentPrivate;
class Page;

// class Document
class MUPDF_QT_EXPORT Document
{
public:
	enum PDFInfoType {
		PDFInfoTitle,
		PDFInfoSubject,
		PDFInfoAuthor,
		PDFInfoKeywords,
		PDFInfoCreator,
		PDFInfoProducer,
		PDFInfoCreationDate,
		PDFInfoModDate
	};
	Document(const QString &filePath);
	~Document();
	bool isLoaded() const;
	int numPages() const;
	QString getInfo(Document::PDFInfoType type);
	Page page(int index) const;

private:
	DocumentPrivate *d;
	// disable copy
	Document(const Document &);
	Document &operator = (const Document &);

friend class Page;
};

} // end namespace Mupdf

#endif // end MUPDF_DOCUMENT_H
