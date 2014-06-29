#ifndef MUPDF_DOCUMENT_P_H
#define MUPDF_DOCUMENT_P_H

extern "C" {
#include <mupdf/fitz.h>
#include <mupdf/pdf.h>
}
#include <QList>

class QString;

namespace MuPDF
{

class OutlinePrivate;

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
	~DocumentPrivate();

	QString info(const char * key);

	fz_context *context;
	fz_document *document;
	bool transparent;
	int b, g, r, a; // background color
	
	// children
	QList<OutlinePrivate *> outlines;
};

}

#endif // end MUPDF_DOCUMENT_P_H
