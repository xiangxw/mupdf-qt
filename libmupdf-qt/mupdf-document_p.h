/**
 * @file mupdf-document_p.h
 * @brief class DocumentPrivate
 * @author xiangxw xiangxw5689@126.com
 * @date 2012-03-31
 */

#ifndef MUPDF_DOCUMENT_P_H
#define MUPDF_DOCUMENT_P_H

extern "C" {
#include "fitz.h"
#include "mupdf.h"
}

namespace Mupdf
{

class DocumentPrivate
{
public:
	DocumentPrivate():xref(NULL), numPages(0), info(NULL) {}
	~DocumentPrivate()
	{
		if (xref) {
			pdf_free_xref(xref);
			xref = NULL;
		}
	}

	pdf_xref *xref;
	int numPages;
	fz_obj *info;
};

} // end namespace Mupdf

#endif // end MUPDF_DOCUMENT_P_H
