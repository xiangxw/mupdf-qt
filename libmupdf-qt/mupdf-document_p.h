/**
 * @file mupdf-document_p.h
 * @brief class DocumentPrivate
 * @author xiangxw xiangxw5689@126.com
 * @date 2012-04-03
 */

#ifndef MUPDF_DOCUMENT_P_H
#define MUPDF_DOCUMENT_P_H

#include <QtCore/QString>
extern "C" {
#include "fitz.h"
}

namespace MuPDF
{

class DocumentPrivate
{
public:
	DocumentPrivate():context(NULL), document(NULL), numPages(-1)
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

	fz_context *context;
	fz_document *document;
	int numPages;
};

} // end namespace MuPDF

#endif // end MUPDF_DOCUMENT_P_H
