/**
 * @file mupdf-page.cpp
 * @brief class Page
 * @author xiangxw xiangxw5689@126.com
 * @date 2012-04-03
 */

#include "mupdf-page.h"
#include "mupdf-page_p.h"
extern "C" {
#include "fitz.h"
}
#include <QImage>
#include <QRect>

static void clear_samples_with_value(
		unsigned char *samples, int size,
		int b, int g, int r, int a)
{
	int i = 0;

	while (i < size) {
		*(samples + i++) = b;
		*(samples + i++) = g;
		*(samples + i++) = r;
		*(samples + i++) = a;
	}
}

namespace MuPDF
{

struct info_s
{
	fz_context *context;
	fz_pixmap *pixmap;
};

/**
 * @brief Clean up image data when the last copy of the QImage is destoryed.
 *
 * @param info Image data.
 */
void imageCleanupHandler(void *info)
{
	info_s *p = static_cast<info_s *>(info);

	if (p) {
		fz_drop_pixmap(p->context, p->pixmap);
		delete p;
	}
}

/**
 * @brief Constructor
 */
Page::Page(const Document &document, int index)
{
	if (document.d->document == NULL || index < 0) {
		return;
	}

	d = new PagePrivate();
	if (NULL == d) {
		return;
	}
	d->context = document.d->context;
	d->document = document.d->document;

	// load page
	fz_try(d->context)
	{
		d->page = fz_load_page(d->document, index);
	}
	fz_catch(d->context)
	{
		fz_free_page(d->document, d->page);
		d->page = NULL;
	}
}

Page::~Page()
{
	if (d) {
		delete d;
		d = NULL;
	}
}
	
/**
 * @brief Render page to QImage
 *
 * @param scaleX Scale for X
 * @param scaleY Scale for Y
 * @param rotate Degrees of clockwise rotation. Values less than zero and greater than 360 are handled as expected.
 *
 * @return QImage use implicit data share, so there is no deep copy here
 */
QImage Page::renderImage(float scaleX, float scaleY, float rotate)
{
	fz_pixmap *pixmap = NULL;
	unsigned char *samples = NULL;
	int width = 0;
	int height = 0;
	int size = 0;

    // apply scale and rotat
    fz_matrix transform;
    fz_rotate(&transform, rotate);
    fz_pre_scale(&transform, scaleX, scaleY);

    // get transformed page size
    fz_rect bounds;
    fz_bound_page(d->document, d->page, &bounds);
    fz_transform_rect(&bounds, &transform);
    fz_irect bbox;
    fz_round_rect(&bbox, &bounds);

	// render to pixmap
	fz_device *dev = NULL;
	fz_try(d->context)
	{
		pixmap = fz_new_pixmap_with_bbox(d->context, fz_device_bgr, &bbox);
		samples = fz_pixmap_samples(d->context, pixmap);
		width = fz_pixmap_width(d->context, pixmap);
		height = fz_pixmap_height(d->context, pixmap);
		size = width * height * 4;
		if (!d->transparent) {
			if (d->b >= 0 && d->g >= 0 && d->r >= 0 && d->a >= 0) {
				// with user defined background color
				clear_samples_with_value(samples, size, d->b, d->g, d->r, d->a);
			} else {
				// with white background
				fz_clear_pixmap_with_value(d->context, pixmap, 0xff);
			}
		}
        fz_device *dev = fz_new_draw_device(d->context, pixmap);
        fz_run_page(d->document, d->page, dev, &transform, NULL);
	}
	fz_always(d->context)
	{
		if (dev) {
			fz_free_device(dev);
		}
		dev = NULL;
	}
	fz_catch(d->context)
	{
		if (pixmap) {
			fz_drop_pixmap(d->context, pixmap);
		}
		pixmap = NULL;
	}

	// render to QImage
	QImage image;
	if (NULL == pixmap) {
		return image;
	}
#if (QT_VERSION >= 0x050000)
	// No copy before return when Qt5 is used
	info_s *info = new info_s;
	info->context = d->context;
	info->pixmap = pixmap;
	image = QImage(samples, // no deep copy here
			width, height, QImage::Format_ARGB32, imageCleanupHandler, info);
#else
	// Copy before return when Qt4 is used
	image = QImage(samples, // no deep copy here
			width, height, QImage::Format_ARGB32).copy();
	fz_drop_pixmap(d->context, pixmap);
#endif
	return image;
}

/**
 * @brief Page size at 72 dpi
 */
QRect Page::size() const
{
    //fz_rect rect = fz_bound_page(d->document, d->page);
    fz_rect rect;
    fz_bound_page(d->document, d->page, &rect);
    //fz_bbox bbox = fz_round_rect(rect);
    fz_irect bbox;
    fz_round_rect(&bbox, &rect);
    return QRect(bbox.x0, bbox.y0,
			bbox.x1 - bbox.x0, bbox.y1 - bbox.y0);
}

/**
 * @brief Whether to do transparent page rendering.
 *
 * @param enable True: transparent; False: not transparent.
 */
void Page::setTransparentRendering(bool enable)
{
	d->transparent = enable;
}

/**
 * @brief Set background color.
 *
 * @param r red channel
 * @param g green channel
 * @param b blue channel
 * @param a alpha channel
 */
void Page::setBackgroundColor(int r, int g, int b, int a)
{
	d->r = r;
	d->g = g;
	d->b = b;
	d->a = a;
}

} // end namespace MuPDF
