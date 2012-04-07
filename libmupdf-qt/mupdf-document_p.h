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
#include "mupdf.h"
#include "mupdf-internal.h"
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
	pdf_obj *info() const
	{
		pdf_document *xref = (pdf_document *)document;
		char *key = (char *)"Info";
		pdf_obj *obj = pdf_dict_gets(xref->trailer, key);
		return obj;
	}

	fz_context *context;
	fz_document *document;
	int numPages;
};

} // end namespace MuPDF

#endif // end MUPDF_DOCUMENT_P_H
