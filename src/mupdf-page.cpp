#include "mupdf-page.h"
#include "mupdf-page_p.h"
#include "mupdf-document.h"
#include "mupdf-document_p.h"
#include "mupdf-textbox.h"
#include "mupdf-textbox_p.h"
extern "C" {
#include <mupdf/fitz.h>
}
#include <QImage>
#include <QRect>

static void clear_bgr_samples_with_value(
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

static void clear_rgb_samples_with_value(
        unsigned char *samples, int size,
        int b, int g, int r, int a)
{
    int i = 0;

    while (i < size) {
        *(samples + i++) = r;
        *(samples + i++) = g;
        *(samples + i++) = b;
        *(samples + i++) = a;
    }
}

/**
 * @brief Clean up image data when the last copy of the QImage is destoryed.
 */
static inline void imageCleanupHandler(void *data)
{
    unsigned char *samples = static_cast<unsigned char *>(data);

    if (samples) {
        delete []samples;
    }
}

namespace MuPDF
{

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
    d->transparent = document.d->transparent;
    d->b = document.d->b;
    d->g = document.d->g;
    d->r = document.d->r;
    d->a = document.d->a;

    fz_try(d->context)
    {
        fz_rect bounds;
        fz_device *list_device;
        fz_device *text_device;

        // load page
        d->page = fz_load_page(d->document, index);
        
        // display list
        d->display_list = fz_new_display_list(d->context);
        list_device = fz_new_list_device(d->context, d->display_list);
        fz_run_page(d->document, d->page, list_device, &fz_identity, NULL);
        fz_free_device(list_device);

        // create text sheet and text page
        d->text_sheet = fz_new_text_sheet(d->context);
        d->text_page = fz_new_text_page(d->context);
        text_device = fz_new_text_device(d->context, d->text_sheet, d->text_page);
        fz_bound_page(d->document, d->page, &bounds);
        fz_run_display_list(d->display_list, text_device, &fz_identity, &bounds, NULL);
        fz_free_device(text_device);
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
 * @return This function will return a empty QImage if failed.
 */
QImage Page::renderImage() const
{
    fz_pixmap *pixmap = NULL;
    unsigned char *samples = NULL;
    unsigned char *copyed_samples = NULL;
    int width = 0;
    int height = 0;
    int size = 0;

    // get transformed page size
    fz_rect bounds;
    fz_bound_page(d->document, d->page, &bounds);
    fz_transform_rect(&bounds, &(d->transform));
    fz_irect bbox;
    fz_round_rect(&bbox, &bounds);

    // render to pixmap
    fz_device *dev = NULL;
    fz_try(d->context)
    {
        // fz_pixmap will always include a separate alpha channel
#if QT_VERSION < 0x050200
        pixmap = fz_new_pixmap_with_bbox(d->context, fz_device_bgr(d->context), &bbox);
#else
        // use rgba for Qt5.2
        pixmap = fz_new_pixmap_with_bbox(d->context, fz_device_rgb(d->context), &bbox);
#endif
        samples = fz_pixmap_samples(d->context, pixmap);
        width = fz_pixmap_width(d->context, pixmap);
        height = fz_pixmap_height(d->context, pixmap);
        size = width * height * 4;
        if (!d->transparent) {
            if (d->b >= 0 && d->g >= 0 && d->r >= 0 && d->a >= 0) {
                // with user defined background color
#if QT_VERSION < 0x050200
                clear_bgr_samples_with_value(samples, size, d->b, d->g, d->r, d->a);
#else
                // use rgba for Qt5.2
                clear_rgb_samples_with_value(samples, size, d->b, d->g, d->r, d->a);
#endif
            } else {
                // with white background
                fz_clear_pixmap_with_value(d->context, pixmap, 0xff);
            }
        }
        dev = fz_new_draw_device(d->context, pixmap);
        fz_run_display_list(d->display_list, dev, &(d->transform), &bounds, NULL);
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
    copyed_samples = new unsigned char[size];
    memcpy(copyed_samples, samples, size);
    fz_drop_pixmap(d->context, pixmap);
#if QT_VERSION < 0x050200
    // most computers use little endian, so Format_ARGB32 means bgra order
    // note: this is not correct for computers with big endian architecture
    image = QImage(copyed_samples,
            width, height, QImage::Format_ARGB32, imageCleanupHandler, copyed_samples);
#else
    // with Qt 5.2, Format_RGBA8888 is correct for any architecture
    image = QImage(copyed_samples,
            width, height, QImage::Format_RGBA8888, imageCleanupHandler, copyed_samples);
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
    fz_rotate(&(d->transform), rotation);
    fz_pre_scale(&(d->transform), scaleX, scaleY);
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
    fz_matrix transform;

    // build fz_rect
    rect.x0 = x0;
    rect.y0 = y0;
    rect.x1 = x1;
    rect.y1 = y1;
    fz_invert_matrix(&transform, &(d->transform));
    fz_transform_rect(&rect, &transform);
    
    // get text
    if (!fz_is_infinite_rect(&rect)) {
        str = fz_copy_selection(d->context, d->text_page, rect);
        ret = QString::fromUtf8(str);
        free(str);
    }

    return ret;
}

/**
 * @brief Return all text boxes of the page.
 *
 * @note The returned text boxes should be deleted when they are no longer used.
 * @note Sizes of the returned text boxes are at 72 dpi, so they won't change when setTransform() is called.
 */
QList<TextBox *> Page::textList() const
{
    QList<TextBox *> ret;
    TextBox *box;
    TextBoxPrivate *boxp;
    fz_text_block *block;
    fz_text_line *line;
    fz_text_span *span;

    for (int block_num = 0; block_num < d->text_page->len; ++block_num) {
        // get block
        if (d->text_page->blocks[block_num].type != FZ_PAGE_BLOCK_TEXT) {
            continue;
        }
        block = d->text_page->blocks[block_num].u.text;

        for (line = block->lines; line < block->lines + block->len; ++line) { // lines
            for (span = line->first_span; span; span = span->next) { // spans
                boxp = new TextBoxPrivate(span);
                box = new TextBox(boxp);
                ret << box;
            }
        }
    }

    return ret;
}

} // end namespace MuPDF
