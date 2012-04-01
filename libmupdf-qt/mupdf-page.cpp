/**
 * @file mupdf-page.cpp
 * @brief class Page
 * @author xiangxw xiangxw5689@126.com
 * @date 2012-03-31
 */

#include "mupdf-page.h"
#include "mupdf-page_p.h"
#include "mupdf-document.h"
#include "mupdf-document_p.h"
#include <QtGui/QImage>
extern "C" {
#include "fitz.h"
#include "mupdf.h"
}

namespace Mupdf
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
 * @param scale Default scale = 1.0f
 * @param rotate Clockwise rotation of 0, 90, 180 or 270 degrees
 *
 * @return QImage use implicit data share, so there is no deep copy here
 */
QImage Page::renderImage(float scale, PDFRotateType rotate)
{
	fz_matrix ctm = fz_translate(0, -(m_sharedData->d->page->mediabox.y1));
	ctm = fz_concat(ctm, fz_scale(scale, -scale));
	ctm = fz_concat(ctm, fz_rotate(m_sharedData->d->page->rotate));
	fz_bbox bbox = fz_round_rect(fz_transform_rect(ctm, m_sharedData->d->page->mediabox));
	if (m_sharedData->d->pixmap) {
		fz_drop_pixmap(m_sharedData->d->pixmap);
	}
	m_sharedData->d->pixmap = fz_new_pixmap_with_rect(fz_device_rgb, bbox);
	fz_pixmap *pixmap = m_sharedData->d->pixmap;
//	fz_clear_pixmap(pixmap); /* draw nothing and save alpha */
	fz_clear_pixmap_with_color(pixmap, 255); /* draw white background and don't save alpha */
	fz_glyph_cache *glyphcache = fz_new_glyph_cache();
	fz_device *device = fz_new_draw_device(glyphcache, pixmap);
	pdf_run_page(m_sharedData->d->xref, m_sharedData->d->page, device, ctm);
	fz_free_glyph_cache(glyphcache);
	fz_free_device(device);

	/* render as QImage */
	m_sharedData->d->rgba2bgra(pixmap->samples, pixmap->w * pixmap->h * 4);
	QImage image(pixmap->samples, /* no deep copy here */
			pixmap->w, pixmap->h, QImage::Format_ARGB32);
	return image;
}

/**
 * @brief Constructor
 */
Page::Page(const Document &document, int index)
	:m_sharedData(new PageData)
{
	m_sharedData->d->xref = document.d->xref;
	pdf_load_page(&(m_sharedData->d->page), document.d->xref, index);
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
	:QSharedData(other), d(new PagePrivate)
{
	d->xref = other.d->xref;
	d->page = other.d->page;
	d->pixmap = other.d->pixmap;
	qDebug("should not come here!\n");
}

/**
 * @brief Destructor
 */
PageData::~PageData()
{
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

} // end namespace Mupdf
