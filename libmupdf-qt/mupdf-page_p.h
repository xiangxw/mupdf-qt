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
		  text_sheet(NULL),
		  text_page(NULL),
		  text_device(NULL),
		  scaleX(1.0f),
		  scaleY(1.0f),
		  rotation(0.0f)
	{
	}
	~PagePrivate()
	{
		deleteData();
	}

	inline void deleteData()
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
		if (text_sheet) {
			fz_free_text_sheet(context, text_sheet);
			text_sheet = NULL;
		}
		if (text_page) {
			fz_free_text_page(context, text_page);
			text_page = NULL;
		}
		if (text_device) {
			fz_free_device(text_device);
			text_device = NULL;
		}
	}

	void updatePageData();

	fz_context *context;
	fz_document *document;
	fz_page *page;
	fz_display_list *display_list;
	fz_device *list_device;
	fz_text_sheet *text_sheet;
	fz_text_page *text_page;
	fz_device *text_device;
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
