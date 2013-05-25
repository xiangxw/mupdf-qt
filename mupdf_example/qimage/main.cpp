/**
 * @file main.cpp
 * @brief render as QImage(based on mupdf/doc/examples)
 * @author xiangxw xiangxw5689@126.com
 * @date 2012-02-20
 */

extern "C" {
#include "fitz.h"
}
#include <QApplication>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <QFileDialog>

void rgba2bgra(unsigned char *data, int size);

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	int index = 1;

	/* get filename */
	QString str = QFileDialog::getOpenFileName(0, "Select PDF file", ".", "PDF (*.pdf)");
	if (str.isEmpty()) {
		return 0;
	}

	/* open document */
	fz_context *context = fz_new_context(NULL, NULL, FZ_STORE_UNLIMITED);
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
    fz_pixmap *pixmap = fz_new_pixmap_with_bbox(context, fz_device_rgb, &bbox);
	fz_clear_pixmap_with_value(context, pixmap, 0xff); // 0xff = 255
	fz_device *device = fz_new_draw_device(context, pixmap);
    fz_run_page(document, page, device, &transform, NULL);
    fz_free_device(device);

	/* render as QImage */
	unsigned char *samples = fz_pixmap_samples(context, pixmap);
	int width = fz_pixmap_width(context, pixmap);
	int height = fz_pixmap_height(context, pixmap);
	rgba2bgra(samples, width * height * 4);
	QImage image(samples, width, height, QImage::Format_ARGB32);
	QLabel label;
	label.setPixmap(QPixmap::fromImage(image));
	label.show();
	fz_drop_pixmap(context, pixmap);

	/* clean up */
	fz_free_page(document, page);
	fz_close_document(document);
	fz_free_context(context);

	return app.exec();
}

/**
 * @brief transform rgba data to bgra data. fz_pixmap store data as rgba, but QImage store data as bgra.
 *
 * @param data pointer to fz_pixmap data
 * @param size size of fz_pixmap data
 */
void rgba2bgra(unsigned char *data, int size)
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
