/**
 * @file mupdf-document.cpp
 * @brief class Document
 * @author xiangxw xiangxw5689@126.com
 * @date 2012-03-28
 */

#include "mupdf-document.h"
#include "mupdf-document_p.h"
#include "mupdf-page.h"
#include "mupdf-page_p.h"
#include <QtCore/QString>
#include <QtCore/QFile>
extern "C" {
#include "fitz.h"
#include "mupdf.h"
}

namespace Mupdf
{

/**
 * @brief Constructor
 *
 * @param filePath Document path
 */
Document::Document(const QString &filePath)
	:d(new DocumentPrivate())
{
	fz_error error;

	/* open xref */
	if (QFile::exists(filePath) == false) {
		return;
	}
	error = pdf_open_xref(&(d->xref), filePath.toLocal8Bit().constData(), NULL);
	if (error) {
		pdf_free_xref(d->xref);
		d->xref = NULL;
		return;
	}

	/* load pdf tree */
	error = pdf_load_page_tree(d->xref);
	if (error) {
		pdf_free_xref(d->xref);
		d->xref = NULL;
		return;
	}

	/* get number of pages */
	d->numPages = pdf_count_pages(d->xref);
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
 * @brief Whether the document is successfully loaded
 */
bool Document::isLoaded() const {return d->xref;}

/**
 * @brief Get number of pages. Return 0 when error occurs
 */
int Document::numPages() const
{
	if (d->xref == NULL) {
		return 0;
	}
	return d->numPages;
}

/**
 * @brief Get info of PDF document
 *
 * @param type Info type
 *
 * @return A invalid(not empty) value is returned when error occur
 */
QString Document::getInfo(Document::PDFInfoType type)
{
	if (d->xref == NULL) {
		return QString();
	}
	if (d->info == NULL) {
		d->info = fz_dict_gets(d->xref->trailer, "Info");
		if (d->info == NULL) {
			return QString();
		}
	}

	fz_obj *obj = NULL;
	switch (type) {
		case Document::PDFInfoTitle:
			obj = fz_dict_gets(d->info, "Title");
			if (obj) {
				return pdf_to_utf8(obj);
			}
			break;
		case PDFInfoSubject:
			obj = fz_dict_gets(d->info, "Subject");
			if (obj) {
				return pdf_to_utf8(obj);
			}
			break;
		case PDFInfoAuthor:
			obj = fz_dict_gets(d->info, "Author");
			if (obj) {
				return pdf_to_utf8(obj);
			}
			break;
		case PDFInfoKeywords:
			obj = fz_dict_gets(d->info, "Keywords");
			if (obj) {
				return pdf_to_utf8(obj);
			}
			break;
		case PDFInfoCreator:
			obj = fz_dict_gets(d->info, "Creator");
			if (obj) {
				return pdf_to_utf8(obj);
			}
			break;
		case PDFInfoProducer:
			obj = fz_dict_gets(d->info, "Producer");
			if (obj) {
				return pdf_to_utf8(obj);
			}
			break;
		case PDFInfoCreationDate:
			obj = fz_dict_gets(d->info, "CreationDate");
			if (obj) {
				return pdf_to_utf8(obj);
			}
			break;
		case PDFInfoModDate:
			obj = fz_dict_gets(d->info, "ModDate");
			if (obj) {
				return pdf_to_utf8(obj);
			}
			break;
		default:
			break;
	}
	return "";
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
	Page page(*this, index);
	return page;
}

} // end namespace Mupdf
