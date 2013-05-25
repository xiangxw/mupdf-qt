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
#include <QString>
#include <QDateTime>

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
	fz_try(d->context)
	{
		d->document = fz_open_document(d->context,
				filePath.toLocal8Bit().data());
	}
	fz_catch(d->context)
	{
		fz_close_document(d->document);
		d->document = NULL;
		fz_free_context(d->context);
		d->context = NULL;
	}
	if (NULL == d->document) {
		return;
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
	return fz_authenticate_password(d->document,
			password.toLocal8Bit().data());
}

/**
 * @brief Get number of pages. Return -1 when error occurs
 */
int Document::numPages() const
{
	int ret;
	fz_try(d->context)
	{
		ret = fz_count_pages(d->document);
	}
	fz_catch(d->context)
	{
		ret = -1;
	}
	return ret;
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
	return d->info("Title");
}

/**
 * @brief Author of the document
 */
QString Document::author() const
{
	return d->info("Author");
}

/**
 * @brief The subject of the document
 */
QString Document::subject() const
{
	return d->info("Subject");
}

/**
 * @brief Keywords associated with the document
 */
QString Document::keywords() const
{
	return d->info("Keywords");
}

/**
 * @brief If the document was converted to PDF from another format, the name of the application (for example, Adobe FrameMaker®) that created the original document from which it was converted
 */
QString Document::creator() const
{
	return d->info("Creator");
}

/**
 * @brief If the document was converted to PDF from another format, the name of the application (for example, Acrobat Distiller) that converted it to PDF
 */
QString Document::producer() const
{
	return d->info("Producer");
}

/**
 * @brief The date and time the document was created
 */
QDateTime Document::creationDate() const
{
	QString str = d->info("CreationDate");
	if (str.isEmpty()) {
		return QDateTime();
	}
	// see pdf_reference_1.7.pdf 2.8.3 Dates
	return QDateTime::fromString(str.left(16),
			"'D:'yyyyMMddHHmmss");
}

/**
 * @brief The date and time the document was most recently modified
 */
QDateTime Document::modDate() const
{
	QString str = d->info("ModDate");
	if (str.isEmpty()) {
		return QDateTime();
	}
	// see pdf_reference_1.7.pdf 2.8.3 Dates
	return QDateTime::fromString(str.left(16),
			"'D:'yyyyMMddHHmmss");
}

/**
 * @brief Get info of the document
 *
 * @param key info key
 */
QString DocumentPrivate::info(const char * key)
{
	pdf_document *xref = (pdf_document *)document;
	pdf_obj *info = pdf_dict_gets(xref->trailer, (char *)"Info");
	if (NULL == info) {
		return QString();
	}
	pdf_obj *obj = pdf_dict_gets(info, (char *)key);
	if (NULL == obj) {
		return QString();
	}
    //char *str = pdf_to_utf8(context, obj);
    char *str = pdf_to_utf8((pdf_document *)document, obj);
	QString ret = QString::fromUtf8(str);
	free(str);
	return ret;
}

} // end namespace MuPDF
