/**
 * @file mupdf-page.h
 * @brief class Page
 * @author xiangxw xiangxw5689@126.com
 * @date 2012-04-03
 */

#ifndef MUPDF_PAGE_H
#define MUPDF_PAGE_H

#include "mupdf-global.h"
class QImage;

namespace MuPDF
{
class Page;
class PagePrivate;
class Document;

class MUPDF_QT_EXPORT Page
{
public:
	enum PDFRotateType {
		PDFRotate0,
		PDFRotate90,
		PDFRotate180,
		PDFRotate270
	};
	~Page();
	QImage renderImage(float scaleX = 1.0f, float scaleY = 1.0f, PDFRotateType rotate = PDFRotate0);

private:
	Page(const Document &document, int index);
	PagePrivate *d;

friend class Document;
};

} // end namespace MuPDF

#endif // end MUPDF_PAGE_H
