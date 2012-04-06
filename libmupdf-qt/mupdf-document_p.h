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
	DocumentPrivate(const QString &filePath):document(NULL), numPages(-1)
	{
		// create context
		context = fz_new_context(NULL, NULL, FZ_STORE_UNLIMITED);

		// open document
		document = fz_open_document(context, filePath.toLocal8Bit().data());
		if (document == NULL) {
			return;
		}

		// count pages
		numPages = fz_count_pages(document);
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
