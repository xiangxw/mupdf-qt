/**
 * @file mupdf-document_p.h
 * @brief class DocumentPrivate
 * @author xiangxw xiangxw5689@126.com
 * @date 2012-04-03
 */

#ifndef MUPDF_DOCUMENT_P_H
#define MUPDF_DOCUMENT_P_H

extern "C" {
#include "fitz.h"
}

namespace Mupdf
{

class DocumentPrivate
{
public:
	DocumentPrivate():document(NULL), numPages(-1)
	{
		context = fz_new_context(NULL, NULL, FZ_STORE_UNLIMITED);
	}
	~DocumentPrivate()
	{
		if (context) {
			fz_free_context(context);
			context = NULL;
		}
	}

	fz_context *context;
	fz_document *document;
	int numPages;
};

} // end namespace Mupdf

#endif // end MUPDF_DOCUMENT_P_H
