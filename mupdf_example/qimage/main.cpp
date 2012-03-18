/**
 * @file main.cpp
 * @brief render as QImage(based on mupdf-0.9/apps/pdfdraw.c)
 * @author xiangxw xiangxw5689@126.com
 * @date 2012-02-20
 */

extern "C" {
#include "fitz.h"
#include "mupdf.h"
}
#include <QtGui/QApplication>
#include <QtGui/QImage>
#include <QtGui/QPixmap>
#include <QtGui/QLabel>

void rgba2bgra(unsigned char *data, int size);

int main(int argc, char **argv)
{
	QApplication app(argc, argv);

	pdf_xref *xref = NULL;
	pdf_page *page = NULL;
	fz_error error;
	char *filename = NULL;
	int index;

	/* get filename */
	if (argc != 3) {
		printf("usage: %s filename.pdf pagenum\n", argv[0]);
		return 1;
	}
	filename = argv[1];
	index = atoi(argv[2]) - 1;

	/* open xref */
	error = pdf_open_xref(&xref, filename, NULL);
	if (error) {
		printf("pdf_open_xref error: %s\n", filename);
		return 1;
	}

	/* load page tree */
	error = pdf_load_page_tree(xref);
	if (error) {
		printf("pdf_load_page_tree error: %s\n", filename);
		return 1;
	}

	/* load page */
	error = pdf_load_page(&page, xref, index);
	if (error) {
		printf("can not load page %d in file %s\n", index, filename);
		return 1;
	}

	/* render page to pixmap */
	fz_matrix ctm = fz_translate(0, -page->mediabox.y1);
	ctm = fz_concat(ctm, fz_scale(1.0f, -1.0f));
	ctm = fz_concat(ctm, fz_rotate(page->rotate));
	fz_bbox bbox = fz_round_rect(fz_transform_rect(ctm, page->mediabox));
	fz_pixmap *pixmap = fz_new_pixmap_with_rect(fz_device_rgb, bbox);
//	fz_clear_pixmap(pixmap); /* don't draw anything and save alpha */
	fz_clear_pixmap_with_color(pixmap, 255); /* draw white background and don't save alpha */
	fz_glyph_cache *glyphcache = fz_new_glyph_cache();
	fz_device *device = fz_new_draw_device(glyphcache, pixmap);
	pdf_run_page(xref, page, device, ctm);
	fz_free_glyph_cache(glyphcache);
	fz_free_device(device);

	/* render as QImage */
	rgba2bgra(pixmap->samples, pixmap->w * pixmap->h * 4);
	QImage image(pixmap->samples, pixmap->w, pixmap->h, QImage::Format_ARGB32);
	QLabel label;
	label.setPixmap(QPixmap::fromImage(image));
	label.show();
	fz_drop_pixmap(pixmap);

	/* free xref and page*/
	pdf_free_xref(xref);
	pdf_free_page(page);

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
