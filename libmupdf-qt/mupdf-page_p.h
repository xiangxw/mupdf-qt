/**
 * @file mupdf-page_p.h
 * @brief class PagePrivate
 * @author xiangxw xiangxw5689@126.com
 * @date 2012-04-03
 */

#ifndef MUPDF_PAGE_P_H
#define MUPDF_PAGE_P_H

extern "C" {
#include "fitz.h"
}

namespace Mupdf
{

class PagePrivate
{
public:
	PagePrivate():
		context(NULL),
		document(NULL),
		page(NULL),
		pixmap(NULL)
	{}
	~PagePrivate()
	{
	}

	void rgba2bgra(unsigned char *data, int size);

	fz_context *context;
	fz_document *document;
	fz_page *page;
	fz_pixmap *pixmap; // page image
};

} // end namespace Mupdf

#endif // end MUPDF_PAGE_P_H
