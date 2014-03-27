/**
 * @file mupdf-page.cpp
 * @brief class Page
 * @author xiangxw xiangxw5689@126.com
 * @date 2012-04-03
 */

#include "mupdf-page.h"
#include "mupdf-page_p.h"
extern "C" {
#include <mupdf/fitz.h>
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

/**
 * @brief Clean up image data when the last copy of the QImage is destoryed.
 *
 * @param info Image data.
 */
static void imageCleanupHandler(void *info)
{
	info_s *p = static_cast<info_s *>(info);

	if (p) {
		fz_drop_pixmap(p->context, p->pixmap);
		delete p;
	}
}

/**
 * @brief Update display list.
 */
void PagePrivate::updateDisplayList()
{
	fz_matrix transform;

	fz_rotate(&transform, rotation);
	fz_pre_scale(&transform, 1.0f, 1.0f);
	fz_run_page(document, page, list_device, &transform, NULL);
}

namespace MuPDF
{

/**
 * @brief Constructor
 */
Page::Page(const Document &document, int index, float scaleX, float scaleY, float rotation)
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
	d->transparent = document.d->transparent;
	d->b = document.d->b;
	d->g = document.d->g;
	d->r = document.d->r;
	d->a = document.d->a;
	d->scaleX = scaleX;
	d->scaleY = scaleY;
	d->rotation = rotation;

	fz_try(d->context)
	{
		// load page
		d->page = fz_load_page(d->document, index);

		// run display list
		d->display_list = fz_new_display_list(d->context);
		d->list_device = fz_new_list_device(d->context, d->display_list);
		d->updateDisplayList();
	}
	fz_catch(d->context)
	{
		if (d->page) {
			fz_free_page(d->document, d->page);
			d->page = NULL;
		}
		if (d->display_list) {
			fz_drop_display_list(d->context, d->display_list);
			d->display_list = NULL;
		}
		if (d->list_device) {
			fz_free_device(d->list_device);
			d->list_device = NULL;
		}
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
 * @return QImage use implicit data share, so there is no deep copy here.
 *         This function will return a empty QImage if failed.
 */
QImage Page::renderImage() const
{
	fz_pixmap *pixmap = NULL;
	unsigned char *samples = NULL;
	int width = 0;
	int height = 0;
	int size = 0;

    // apply scale and rotate
    fz_matrix transform;
    fz_rotate(&transform, d->rotation);
    fz_pre_scale(&transform, d->scaleX, d->scaleY);

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
		pixmap = fz_new_pixmap_with_bbox(d->context, fz_device_bgr(d->context), &bbox);
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
		dev = fz_new_draw_device(d->context, pixmap);
		fz_run_display_list(d->display_list, dev, &transform, &bounds, NULL);
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
	// No copy before return
	info_s *info = new info_s;
	info->context = d->context;
	info->pixmap = pixmap;
	image = QImage(samples, // no deep copy here
			width, height, QImage::Format_ARGB32, imageCleanupHandler, info);
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
 * This function modify setting of current page only.
 * For global setting, use Document::setTransparentRendering() instead.
 *
 * @param enable True: transparent; False: not transparent(default).
 */
void Page::setTransparentRendering(bool enable)
{
	d->transparent = enable;
}

/**
 * @brief Set background color.
 * This function modify setting of current page only.
 * For global setting, use Document::setBackgroundColor() instead.
 *
 * @note This function will only work when page is not transparent.
 * This function won't change content of your file, it only change
 * the way you render the page.
 *
 * @param r red channel
 * @param g green channel
 * @param b blue channel
 * @param a alpha channel(default with non transparent)
 */
void Page::setBackgroundColor(int r, int g, int b, int a)
{
	d->r = r;
	d->g = g;
	d->b = b;
	d->a = a;
}

/**
 * @brief Set scale and rotation.
 *
 * @param scaleX Scale for X direction.
 *               Default value: 1.0f; >1.0f: zoom in; <1.0f: zoom out.
 * @param scaleY Scale for Y direction.
 *               Default value: 1.0f; >1.0f: zoom in; <1.0f: zoom out.
 * @param rotation Degree of clockwise rotation. Range: [0.0f, 360.0f).
 */
void Page::setTransform(float scaleX, float scaleY, float rotation)
{
	d->scaleX = scaleX;
	d->scaleY = scaleY;
	d->rotation = rotation;

	d->updateDisplayList();
}

} // end namespace MuPDF
