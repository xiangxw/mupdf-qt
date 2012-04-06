/**
 * @file mupdf-page_p.h
 * @brief class PagePrivate
 * @author xiangxw xiangxw5689@126.com
 * @date 2012-04-03
 */

#ifndef MUPDF_PAGE_P_H
#define MUPDF_PAGE_P_H

#include "mupdf-document.h"
#include "mupdf-document_p.h"
extern "C" {
#include "fitz.h"
}

namespace MuPDF
{

class PagePrivate
{
public:
	PagePrivate():
		context(NULL),
		document(NULL),
		page(NULL),
		pixmap(NULL)
	{
	}
	~PagePrivate()
	{
		if (pixmap) {
			fz_drop_pixmap(context, pixmap);
			pixmap = NULL;
		}
		if (page) {
			fz_free_page(document, page);
			page = NULL;
		}
	}
	void rgba2bgra(unsigned char *data, int size);

	fz_context *context;
	fz_document *document;
	fz_page *page;
	fz_pixmap *pixmap; // page image
};

} // end namespace MuPDF

#endif // end MUPDF_PAGE_P_H
