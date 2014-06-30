#include <mupdf/fitz.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    /* get filename and page number */
    char *filename = NULL;
    fz_context *context;
    fz_document *document;
    fz_outline *outline, *link;

    if (argc != 2) {
        printf("usage: %s filename.pdf\n", argv[0]);
        return 1;
    }
    filename = argv[1];

    /* open document */
    context = fz_new_context(NULL, NULL, FZ_STORE_UNLIMITED);
    fz_register_document_handlers(context);
    document = fz_open_document(context, filename);

    /* print outline */
    outline = fz_load_outline(document);
    link = outline;
    while (link) {
        printf("%s\n", link->title);
        link = link->next;
    }

    /* clean up */
    fz_free_outline(context, outline);
    fz_close_document(document);
    fz_free_context(context);
    return 0;
}
