/**
 * @file main.c
 * @brief show infomation of a PDF file
 * @author xiangxw xiangxw5689@126.com
 * @date 2011-12-15
 */

#include <stdlib.h>
#include "fitz.h"
#include "mupdf.h"

int main(int argc, char **argv)
{
	pdf_xref *xref = NULL;
	fz_error error;
	const char *filename = "/home/a/document/PDF_Reference_1-7.pdf";

	/* open xref */
	error = pdf_open_xref(&xref, filename, NULL);
	if (error) {
		printf("can't open document: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	/* load page tree */
	error = pdf_load_page_tree(xref);
	if (error) {
		printf("can't load page tree: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	/* print page count */
	int pages = pdf_count_pages(xref);
	printf("page count: %d\n", pages);

	/* print infomation */
	fz_obj *info = fz_dict_gets(xref->trailer, "Info");
	if (info) {
		fz_obj *obj = NULL;
		obj = fz_dict_gets(info, "Title"); /* print title */
		if (obj) {
			printf("Title: %s\n", pdf_to_utf8(obj));
		}
		obj = fz_dict_gets(info, "Subject"); /* print subject */
		if (obj) {
			printf("Subject: %s\n", pdf_to_utf8(obj));
		}
		obj = fz_dict_gets(info, "Author"); /* print author */
		if (obj) {
			printf("Author: %s\n", pdf_to_utf8(obj));
		}
		obj = fz_dict_gets(info, "Keywords"); /* print keywords */
		if (obj) {
			printf("Keywords: %s\n", pdf_to_utf8(obj));
		}
		obj = fz_dict_gets(info, "Creator"); /* print creator */
		if (obj) {
			printf("Creator: %s\n", pdf_to_utf8(obj));
		}
		obj = fz_dict_gets(info, "Producer"); /* print producer */
		if (obj) {
			printf("Producer: %s\n", pdf_to_utf8(obj));
		}
		obj = fz_dict_gets(info, "CreationDate"); /* print creation date */
		if (obj) {
			printf("CreationDate: %s\n", pdf_to_utf8(obj));
		}
		obj = fz_dict_gets(info, "ModDate"); /* print mod date */
		if (obj) {
			printf("ModDate: %s\n", pdf_to_utf8(obj));
		}
	}

	/* clean */
	pdf_free_xref(xref);
	return 0;
}
