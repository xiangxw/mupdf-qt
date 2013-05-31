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

	fz_drop_pixmap(p->context, p->pixmap);
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
	fz_pixmap *pixmap = NULL;
	fz_device *dev = NULL;
	fz_try(d->context)
	{
        pixmap = fz_new_pixmap_with_bbox(d->context, fz_device_rgb, &bbox);
		if (d->transparent) {
			fz_clear_pixmap(d->context, pixmap);
		} else {
			fz_clear_pixmap_with_value(d->context, pixmap, 0xff);
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
	unsigned char *samples = fz_pixmap_samples(d->context, pixmap);
	int width = fz_pixmap_width(d->context, pixmap);
	int height = fz_pixmap_height(d->context, pixmap);
	d->rgba2bgra(samples, width * height * 4);
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
