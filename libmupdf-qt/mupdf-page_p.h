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
#include <mupdf/fitz.h>
}

class PagePrivate
{
public:
	PagePrivate()
		: context(NULL),
		  document(NULL),
		  page(NULL),
		  display_list(NULL),
		  list_device(NULL),
		  scaleX(1.0f),
		  scaleY(1.0f),
		  rotation(0.0f)
	{
	}
	~PagePrivate()
	{
		if (page) {
			fz_free_page(document, page);
			page = NULL;
		}
		if (display_list) {
			fz_drop_display_list(context, display_list);
			display_list = NULL;
		}
		if (list_device) {
			fz_free_device(list_device);
			list_device = NULL;
		}
	}

	void updateDisplayList();

	fz_context *context;
	fz_document *document;
	fz_page *page;
	fz_display_list *display_list;
	fz_device *list_device;
	float scaleX, scaleY, rotation;
	bool transparent;
	int b, g, r, a; // background color
};

struct info_s
{
	fz_context *context;
	fz_pixmap *pixmap;
};

#endif // end MUPDF_PAGE_P_H
