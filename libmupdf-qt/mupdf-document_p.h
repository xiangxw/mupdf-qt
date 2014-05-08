#ifndef MUPDF_DOCUMENT_P_H
#define MUPDF_DOCUMENT_P_H

extern "C" {
#include <mupdf/fitz.h>
#include <mupdf/pdf.h>
}

class QString;

class DocumentPrivate
{
public:
	DocumentPrivate()
		: context(NULL),
		  document(NULL),
		  transparent(false),
		  b(-1),
		  g(-1),
		  r(-1),
		  a(-1)
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
	bool transparent;
	int b, g, r, a; // background color
};

#endif // end MUPDF_DOCUMENT_P_H
