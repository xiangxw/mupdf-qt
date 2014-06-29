// render as QImage(based on mupdf/doc/examples)

extern "C" {
#include <mupdf/fitz.h>
}
#include <QImage>

int main(int argc, char **argv)
{
    int index = 1;

    /* get filename */
    QString str = argv[1];
    if (str.isEmpty()) {
        return 1;
    }

    /* open document */
    fz_context *context = fz_new_context(NULL, NULL, FZ_STORE_UNLIMITED);
    fz_register_document_handlers(context);
    fz_document *document = fz_open_document(context, str.toLocal8Bit().data());

    /* load page*/
    fz_page *page = fz_load_page(document, index - 1);

    /* render page to pixmap */
    fz_matrix transform;
    fz_rotate(&transform, 0.0f);
    fz_pre_scale(&transform, 1.0f, 1.0f);
    fz_rect bounds;
    fz_bound_page(document, page, &bounds);
    fz_transform_rect(&bounds, &transform);
    fz_irect bbox;
    fz_round_rect(&bbox, &bounds);
    // fz_pixmap will always include a separate alpha channel
#if QT_VERSION < 0x050200
    fz_pixmap *pixmap = fz_new_pixmap_with_bbox(context, fz_device_bgr(context), &bbox);
#else
    fz_pixmap *pixmap = fz_new_pixmap_with_bbox(context, fz_device_rgb(context), &bbox);
#endif
    fz_clear_pixmap_with_value(context, pixmap, 0xff); // 0xff = 255
    fz_device *device = fz_new_draw_device(context, pixmap);
    fz_run_page(document, page, device, &transform, NULL);
    fz_free_device(device);

    /* render as QImage */
    unsigned char *samples = fz_pixmap_samples(context, pixmap);
    int width = fz_pixmap_width(context, pixmap);
    int height = fz_pixmap_height(context, pixmap);
#if QT_VERSION < 0x050200
    // most computers use little endian, so Format_ARGB32 means bgra order
    // note: this is not correct for computers with big endian architecture
    QImage image(samples, width, height, QImage::Format_ARGB32);
#else
    // with Qt 5.2, Format_RGBA8888 is correct for any architecture
    QImage image(samples, width, height, QImage::Format_RGBA8888);
#endif
    if (!image.save("a.png")) {
        return 1;
    }

    /* clean up */
    fz_drop_pixmap(context, pixmap);
    fz_free_page(document, page);
    fz_close_document(document);
    fz_free_context(context);

    return 0;
}
