/* print page count of a pdf file(based on mupdf/doc/example.c) */

#include <stdlib.h>
#include <string.h>
#include <mupdf/fitz.h>

int main(int argc, char **argv)
{
    /* get filename */
    char *filename = NULL;
    fz_context *context;
    fz_document *document;

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
    context = fz_new_context(NULL, NULL, FZ_STORE_UNLIMITED);
    fz_register_document_handlers(context);
    document = fz_open_document(context, filename);

    /* print page count */
    printf("page count: %d\n", fz_count_pages(document));

    fz_close_document(document);
    fz_free_context(context);
    return 0;
}
