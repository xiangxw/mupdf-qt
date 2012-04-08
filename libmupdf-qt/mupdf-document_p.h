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
#include "mupdf.h"
#include "mupdf-internal.h"
}

class QString;

namespace MuPDF
{

class DocumentPrivate
{
public:
	DocumentPrivate():context(NULL), document(NULL)
	{
	}
	~DocumentPrivate()
	{
		if (document) {
			fz_close_document(document);
			document = NULL;
		}
		if (context) {
			fz_free_context(context);
			context = NULL;
		}
	}
	QString info(const char * key);

	fz_context *context;
	fz_document *document;
};

} // end namespace MuPDF

#endif // end MUPDF_DOCUMENT_P_H
