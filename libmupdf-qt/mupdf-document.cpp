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
	if (NULL == doc) {
		return NULL;
	}
	if (doc->d->context && doc->d->document) {
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
{
	d = new DocumentPrivate();
	if (NULL == d) {
		return;
	}
	
	// create context
	d->context = fz_new_context(NULL, NULL, FZ_STORE_UNLIMITED);
	if (NULL == d->context) {
		return;
	}
	// open document
	d->document = fz_open_document(d->context,
			filePath.toLocal8Bit().data());
	if (NULL == d->document) {
		return;
	}
	// count pages
	if (!fz_needs_password(d->document)) {
		d->numPages = fz_count_pages(d->document);
	}
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
 * @brief Whether the document needs password
 */
bool Document::needsPassword() const
{
	return fz_needs_password(d->document);
}

/**
 * @brief Authenticate password
 *
 * @param password User password or owner password
 * This will first try user password and then owner password.
 * Owner have full access to the document while user don't
 *
 * @return true if succeed, false if failed
 */
bool Document::authPassword(const QString &password)
{
	bool tmp;
	tmp = fz_authenticate_password(d->document,
			password.toLocal8Bit().data());
	if (tmp) {
		d->numPages = fz_count_pages(d->document);
	}

	return tmp;
}

/**
 * @brief Get number of pages. Return -1 when error occurs
 */
int Document::numPages() const
{
	return d->numPages;
}

/**
 * @brief Get page
 *
 * @param index Page index, begin with 0
 *
 * @return Note: you need delete this manually before document is deleted
 */
Page *Document::page(int index) const
{
	Page *page = new Page(*this, index);
	if (NULL == page) {
		return NULL;
	}
	if (NULL == page->d->page) {
		delete page;
		page = NULL;
		return NULL;
	}
	return page;
}

} // end namespace MuPDF
