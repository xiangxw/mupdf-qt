/**
 * @file mupdf-page.h
 * @brief class Page
 * @author xiangxw xiangxw5689@126.com
 * @date 2012-04-03
 */

#ifndef MUPDF_PAGE_H
#define MUPDF_PAGE_H

#include "mupdf-global.h"
#include <QtCore/QSharedDataPointer>
#include <QtCore/QSharedData>
class QImage;

namespace MuPDF
{
class Page;
class PageData;
class PagePrivate;
class Document;

// class Page. Implicitly share, see document for class QSharedDataPointer
class MUPDF_QT_EXPORT Page
{
public:
	enum PDFRotateType {
		PDFRotate0,
		PDFRotate90,
		PDFRotate180,
		PDFRotate270
	};
	Page(const Page &other):m_sharedData(other.m_sharedData) {}
	bool isLoaded() const;
	QImage renderImage(float scaleX = 1.0f, float scaleY = 1.0f, PDFRotateType rotate = PDFRotate0);

private:
	Page(const Document &document, int index);
	QSharedDataPointer<PageData> m_sharedData;

friend class Document;
};

// class PageData
class  PageData : public QSharedData
{
public:
	PageData();
	PageData(const PageData &other);
	~PageData();

	PagePrivate *d;
};

} // end namespace MuPDF

#endif // end MUPDF_PAGE_H
