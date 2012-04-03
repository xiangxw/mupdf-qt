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
#include <QtCore/QString>

namespace Mupdf
{

/**
 * @brief Constructor
 *
 * @param filePath Document path
 */
Document::Document(const QString &filePath)
	:d(new DocumentPrivate)
{
	// open document
	d->document = fz_open_document(d->context, filePath.toLocal8Bit().data());
	if (d->document == NULL) {
		return;
	}

	// count pages
	d->numPages = fz_count_pages(d->document);
}

/**
 * @brief Destructor
 */
Document::~Document()
{
	if (d->document) {
		fz_close_document(d->document);
		d->document = NULL;
	}
	if (d) {
		delete d;
		d = NULL;
	}
}

/**
 * @brief Whether the document is successfully loaded
 */
bool Document::isLoaded() const
{
	return d->document;
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

} // end namespace Mupdf
