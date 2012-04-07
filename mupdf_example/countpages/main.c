/**
 * @file main.c
 * @brief print page count of a pdf file(based on mupdf/doc/example.c)
 * @author xiangxw xiangxw5689@126.com
 * @date 2011-12-15
 */

#include <stdlib.h>
#include <string.h>
#include "fitz.h"

int main(int argc, char **argv)
{
	/* get filename */
	char *filename = NULL;
	if (argc != 2) {
		printf("usage: %s filename.pdf\n", argv[0]);
		return 1;
	}
	if (strcmp(argv[1], "--help") == 0) {
		printf("usage: %s filename.pdf\n", argv[0]);
		return 0;
	}
	filename = argv[1];

	/* open document */
	fz_context *context = fz_new_context(NULL, NULL, FZ_STORE_UNLIMITED);
	fz_document *document = fz_open_document(context, filename);

	/* print page count */
	printf("page count: %d\n", fz_count_pages(document));

	fz_close_document(document);
	fz_free_context(context);
	return 0;
}
