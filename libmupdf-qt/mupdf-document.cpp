/**
 * @file mupdf-document.cpp
 * @brief class Document
 * @author xiangxw xiangxw5689@126.com
 * @date 2012-04-03
 */

#include "mupdf-document.h"
#include "mupdf-document_p.h"
#include "mupdf-page.h"
#include "mupdf-page_p.h"
extern "C" {
#include "fitz.h"
}

namespace MuPDF
{

/**
 * @brief Load document
 *
 * @param filePath Document path
 *
 * @return Return NULL if failed.(Note: you need delete manually when it's useless)
 */
Document *loadDocument(const QString &filePath)
{
	Document *doc = new Document(filePath);
	if (doc == NULL) {
		return NULL;
	}
	if (doc->d->document) {
		return doc;
	}
	delete doc; doc = NULL;
	return NULL;
}

/**
 * @brief Constructor
 *
 * @param filePath Document path
 */
Document::Document(const QString &filePath)
	:d(new DocumentPrivate(filePath))
{

}

/**
 * @brief Destructor
 */
Document::~Document()
{
	if (d) {
		delete d;
		d = NULL;
	}
}

/**
 * @brief Get number of pages. Return -1 when error occurs
 */
int Document::numPages() const
{
	if (d->document == NULL) {
		return -1;
	}
	return d->numPages;
}

/**
 * @brief Get page
 *
 * @param index Page index, begin with 0
 *
 * @return Page use implicit data share, so there is no deep copy here
 */
Page Document::page(int index) const
{
	return Page(*this, index);
}

} // end namespace MuPDF
