/**
 * @file mupdf-page.cpp
 * @brief class Page
 * @author xiangxw xiangxw5689@126.com
 * @date 2012-04-03
 */

#include "mupdf-page.h"
#include "mupdf-page_p.h"
#include "mupdf-document.h"
#include "mupdf-document_p.h"
extern "C" {
#include "fitz.h"
}
#include <QtGui/QImage>

namespace MuPDF
{
	
/**
 * @brief Whether the page is successfully loaded
 */
bool Page::isLoaded() const
{
	return m_sharedData->d->page;
}

/**
 * @brief Render page to QImage
 *
 * @param scaleX Scale for X
 * @param scaleY Scale for Y
 * @param rotate Clockwise rotation of 0, 90, 180 or 270 degrees
 *
 * @return QImage use implicit data share, so there is no deep copy here
 */
QImage Page::renderImage(float scaleX, float scaleY, PDFRotateType rotate)
{
	PagePrivate *d = m_sharedData->d;
	fz_pixmap *pixmap = d->pixmap;

	fz_matrix transform = fz_scale(scaleX, scaleY);
//	transform = fz_concat(transform, fz_rotate(rotation));

	fz_rect rect = fz_bound_page(d->document, d->page);
	rect = fz_transform_rect(transform, rect);
	fz_bbox bbox = fz_round_rect(rect);

	pixmap = fz_new_pixmap_with_bbox(d->context, fz_device_rgb, bbox);
	fz_clear_pixmap_with_value(d->context, pixmap, 0xff);

	fz_device *dev = fz_new_draw_device(d->context, pixmap);
	fz_run_page(d->document, d->page, dev, transform, NULL);
	fz_free_device(dev);

	// render as QImage
	unsigned char *samples = fz_pixmap_samples(d->context, pixmap);
	int width = fz_pixmap_width(d->context, pixmap);
	int height = fz_pixmap_height(d->context, pixmap);
	d->rgba2bgra(samples, width * height * 4);
	QImage image(samples, // no deep copy here
			width, height, QImage::Format_ARGB32);
	return image;
}

/**
 * @brief Constructor
 */
Page::Page(const Document &document, int index)
{
	if (document.d->document == NULL || index < 0) {
		return;
	}

	m_sharedData = new PageData;
	PagePrivate *d = m_sharedData->d;
	d->context = document.d->context;
	d->document = document.d->document;
	d->page = fz_load_page(d->document, index);
}

/**
 * @brief Constructor
 */
PageData::PageData()
	:d(new PagePrivate)
{
}

/**
 * @brief Copy constructor, used for copy-on-write.
 */
PageData::PageData(const PageData &other)
	:QSharedData(other)
{
	qDebug("Should not come here! \
			Because there will be no copy-on-write in this library.\n");
}

/**
 * @brief Destructor
 */
PageData::~PageData()
{
	if (d->page) {
		fz_free_page(d->document, d->page);
		d->page = NULL;
	}
	if (d->pixmap) {
		fz_drop_pixmap(d->context, d->pixmap);
		d->pixmap = NULL;
	}
	if (d) {
		delete d;
		d = NULL;
	}
}

/**
 * @brief Transform rgba data to bgra data. fz_pixmap store data as rgba, but QImage store data as bgra.
 *
 * @param data Pointer to fz_pixmap data
 * @param size Size of fz_pixmap data
 */
void PagePrivate::rgba2bgra(unsigned char *data, int size)
{
	unsigned char r, g, b/* ,a */;
	for (int i = 0; i < size; i += 4) {
		r = *(data + i);
		/* g = *(data + i + 1); */
		b = *(data + i + 2);
		/* a = *(data + i + 3); */

		*(data + i) = b;
		/* *(data + i + 1) = g; */
		*(data + i + 2) = r;
		/* *(data + i + 3) = a */
	}
}

} // end namespace MuPDF
