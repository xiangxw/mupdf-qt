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

class PagePrivate
{
public:
	PagePrivate()
		: context(NULL),
		  document(NULL),
		  page(NULL)
	{
	}
	~PagePrivate()
	{
		if (page) {
			fz_free_page(document, page);
			page = NULL;
		}
	}

	fz_context *context;
	fz_document *document;
	fz_page *page;
	bool transparent;
	int b, g, r, a; // background color
};

struct info_s
{
	fz_context *context;
	fz_pixmap *pixmap;
};

#endif // end MUPDF_PAGE_P_H
