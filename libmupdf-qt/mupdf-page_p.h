/**
 * @file mupdf-page_p.h
 * @brief class PagePrivate
 * @author xiangxw xiangxw5689@126.com
 * @date 2012-03-31
 */

#ifndef MUPDF_PAGE_P_H
#define MUPDF_PAGE_P_H

extern "C" {
#include "fitz.h"
#include "mupdf.h"
}

namespace Mupdf
{

class PagePrivate
{
public:
	PagePrivate():xref(NULL), page(NULL), pixmap(NULL) {}
	~PagePrivate()
	{
		if (page) {
			pdf_free_page(page);
			page = NULL;
		}
		if (pixmap) {
			fz_drop_pixmap(pixmap);
			pixmap = NULL;
		}
	}

	void rgba2bgra(unsigned char *data, int size);

	pdf_xref *xref;
	pdf_page *page;
	fz_pixmap *pixmap;
};

} // end namespace Mupdf

#endif // end MUPDF_PAGE_P_H
