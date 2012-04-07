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
#include "mupdf.h"
#include "mupdf-internal.h"
}
#include <QtCore/QDateTime>

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

/**
 * @brief PDF version number, for example: 1.7
 */
QString Document::pdfVersion() const
{
	pdf_document *xref = (pdf_document *)d->document;
	return QString::number(xref->version / 10.0f);
}

/**
 * @brief PDF title
 */
QString Document::title() const
{
	pdf_obj *info = d->info();
	if (NULL == info) {
		return QString();
	}
	char *key = (char *)"Title";
	pdf_obj *obj = pdf_dict_gets(info, key);
	if (NULL == obj) {
		return QString();
	}
	return QString::fromUtf8(pdf_to_utf8(d->context, obj));
}

/**
 * @brief Author of the document
 */
QString Document::author() const
{
	pdf_obj *info = d->info();
	if (NULL == info) {
		return QString();
	}
	char *key = (char *)"Author";
	pdf_obj *obj = pdf_dict_gets(info, key);
	if (NULL == obj) {
		return QString();
	}
	return QString::fromUtf8(pdf_to_utf8(d->context, obj));
}

/**
 * @brief The subject of the document
 */
QString Document::subject() const
{
	pdf_obj *info = d->info();
	if (NULL == info) {
		return QString();
	}
	char *key = (char *)"Subject";
	pdf_obj *obj = pdf_dict_gets(info, key);
	if (NULL == obj) {
		return QString();
	}
	return QString::fromUtf8(pdf_to_utf8(d->context, obj));
}

/**
 * @brief Keywords associated with the document
 */
QString Document::keywords() const
{
	pdf_obj *info = d->info();
	if (NULL == info) {
		return QString();
	}
	char *key = (char *)"Keywords";
	pdf_obj *obj = pdf_dict_gets(info, key);
	if (NULL == obj) {
		return QString();
	}
	return QString::fromUtf8(pdf_to_utf8(d->context, obj));
}

/**
 * @brief If the document was converted to PDF from another format, the name of the application (for example, Adobe FrameMaker®) that created the original document from which it was converted
 */
QString Document::creator() const
{
	pdf_obj *info = d->info();
	if (NULL == info) {
		return QString();
	}
	char *key = (char *)"Creator";
	pdf_obj *obj = pdf_dict_gets(info, key);
	if (NULL == obj) {
		return QString();
	}
	return QString::fromUtf8(pdf_to_utf8(d->context, obj));
}

/**
 * @brief If the document was converted to PDF from another format, the name of the application (for example, Acrobat Distiller) that converted it to PDF
 */
QString Document::producer() const
{
	pdf_obj *info = d->info();
	if (NULL == info) {
		return QString();
	}
	char *key = (char *)"Producer";
	pdf_obj *obj = pdf_dict_gets(info, key);
	if (NULL == obj) {
		return QString();
	}
	return QString::fromUtf8(pdf_to_utf8(d->context, obj));
}

/**
 * @brief The date and time the document was created
 */
QDateTime Document::creationDate() const
{
	pdf_obj *info = d->info();
	if (NULL == info) {
		return QDateTime();
	}
	char *key = (char *)"CreationDate";
	pdf_obj *obj = pdf_dict_gets(info, key);
	if (NULL == obj) {
		return QDateTime();
	}
	QString str = pdf_to_utf8(d->context, obj);
	// see pdf_reference_1.7.pdf 2.8.3 Dates
	return QDateTime::fromString(str.left(16),
			"'D:'yyyyMMddHHmmss");
}

/**
 * @brief The date and time the document was most recently modified
 */
QDateTime Document::modDate() const
{
	pdf_obj *info = d->info();
	if (NULL == info) {
		return QDateTime();
	}
	char *key = (char *)"ModDate";
	pdf_obj *obj = pdf_dict_gets(info, key);
	if (NULL == obj) {
		return QDateTime();
	}
	QString str = pdf_to_utf8(d->context, obj);
	// see pdf_reference_1.7.pdf 2.8.3 Dates
	return QDateTime::fromString(str.left(16),
			"'D:'yyyyMMddHHmmss");
}

} // end namespace MuPDF
