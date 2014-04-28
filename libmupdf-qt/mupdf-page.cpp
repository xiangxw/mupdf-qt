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
 * @brief Update page data.
 */
void PagePrivate::updatePageData()
{
	fz_matrix transform;
	fz_rect bounds;
	fz_device *list_device;
	fz_device *text_device;
	
	// build transform and bounds
	fz_rotate(&transform, rotation);
	fz_pre_scale(&transform, 1.0f, 1.0f);
	fz_bound_page(document, page, &bounds);
	fz_transform_rect(&bounds, &transform);

	// create display list
	if (display_list) {
		fz_drop_display_list(context, display_list);
	}
	display_list = fz_new_display_list(context);
	list_device = fz_new_list_device(context, display_list);

	// create text sheet and text page
	if (text_sheet) {
		fz_free_text_sheet(context, text_sheet);
	}
	if (text_page) {
		fz_free_text_page(context, text_page);
	}
	text_sheet = fz_new_text_sheet(context);
	text_page = fz_new_text_page(context);
	text_device = fz_new_text_device(context, text_sheet, text_page);

	// update display list
	fz_run_page(document, page, list_device, &transform, NULL);
	fz_free_device(list_device);

	// update text
	fz_run_display_list(display_list, text_device, &transform, &bounds, NULL);
	fz_free_device(text_device);
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

		// update page data
		d->updatePageData();
	}
	fz_catch(d->context)
	{
		d->deleteData();
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
 * @brief Check whether this page object is valid.
 */
bool Page::isValid() const
{
	return (d && d->page) ? true : false;
}
	
/**
 * @brief Render page to QImage
 *
 * @return QImage use implicit data share, so there is no deep copy here.
 *         This function will return a empty QImage if failed.
 *         The returned QImage should be deleted before Document object is deleted.
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

	d->updatePageData();
}

/**
 * @brief Return the text in a rect.
 *
 * @param x0 X coordinate of top left corner.
 * @param y0 Y coordinate of top left corner.
 * @param x1 X coordinate of bottom right corner.
 * @param y1 Y coordinate of bottom right corner.
 */
QString Page::text(float x0, float y0, float x1, float y1) const
{
	QString ret;
	char *str;
	fz_rect rect;

	// build fz_rect
	rect.x0 = x0;
	rect.y0 = y0;
	rect.x1 = x1;
	rect.y1 = y1;
	
	// get text
	if (!fz_is_infinite_rect(&rect)) {
		str = fz_copy_selection(d->context, d->text_page, rect);
		ret = QString::fromUtf8(str);
		free(str);
	}

	return ret;
}

} // end namespace MuPDF
