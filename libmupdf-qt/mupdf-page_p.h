#ifndef MUPDF_PAGE_P_H
#define MUPDF_PAGE_P_H

#include "mupdf-document.h"
#include "mupdf-document_p.h"
extern "C" {
#include <mupdf/fitz.h>
}

namespace MuPDF
{

class PagePrivate
{
public:
	PagePrivate()
		: context(NULL),
		  document(NULL),
		  page(NULL),
		  display_list(NULL),
		  text_sheet(NULL),
		  text_page(NULL),
		  scaleX(1.0f),
		  scaleY(1.0f),
		  rotation(0.0f),
		  transform(fz_identity)
	{
	}
	~PagePrivate()
	{
		deleteData();
	}

	inline void deleteData()
	{
		if (display_list) {
			fz_drop_display_list(context, display_list);
			display_list = NULL;
		}
		if (text_sheet) {
			fz_free_text_sheet(context, text_sheet);
			text_sheet = NULL;
		}
		if (text_page) {
			fz_free_text_page(context, text_page);
			text_page = NULL;
		}
		if (page) {
			fz_free_page(document, page);
			page = NULL;
		}
	}

	fz_context *context;
	fz_document *document;
	fz_page *page;
	fz_display_list *display_list;
	fz_text_sheet *text_sheet;
	fz_text_page *text_page;
	float scaleX, scaleY, rotation;
	fz_matrix transform;
	bool transparent;
	int b, g, r, a; // background color
};

}

#endif // end MUPDF_PAGE_P_H
