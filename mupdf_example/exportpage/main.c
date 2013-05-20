/**
 * @file main.c
 * @brief export page(based on mupdf/doc/example.c)
 * @author xiangxw xiangxw5689@126.com
 * @date 2012-02-20
 */

#include "fitz.h"

int main(int argc, char **argv)
{
	/* get filename */
	char *filename = NULL;
	int index;
	if (argc != 3) {
		printf("usage: %s filename.pdf pagenum\n", argv[0]);
		return 1;
	}
	filename = argv[1];
	index = atoi(argv[2]);

	/* open document */
	fz_context *context = fz_new_context(NULL, NULL, FZ_STORE_UNLIMITED);
	fz_document *document = fz_open_document(context, filename);

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

	/* export to image file */
	char str[100];
	sprintf(str, "%d.png", index);
	fz_write_png(context, pixmap, str, 0);
	fz_drop_pixmap(context, pixmap);

	/* clean up */
	fz_free_page(document, page);
	fz_close_document(document);
	fz_free_context(context);
	return 0;
}
