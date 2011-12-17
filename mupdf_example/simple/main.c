/**
 * @file main.c
 * @brief print page count of a pdf file
 * @author xiangxw xiangxw5689@126.com
 * @date 2011-12-15
 */

#include <stdlib.h>
#include <string.h>
#include "fitz.h"
#include "mupdf.h"

int main(int argc, char **argv)
{
	pdf_xref *xref = NULL;
	fz_error error;
	char *filename = NULL;

	/* get filename */
	if (argc != 2) {
		printf("usage: %s filename.pdf\n", argv[0]);
		return 1;
	}
	if (strcmp(argv[1], "--help") == 0) {
		printf("usage: %s filename.pdf\n", argv[0]);
		return 0;
	}
	filename = argv[1];

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

	/* print page count */
	printf("page count: %d\n", pdf_count_pages(xref));

	return 0;
}
