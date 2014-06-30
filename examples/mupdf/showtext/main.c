#include <mupdf/fitz.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    /* get filename and page number */
    char *filename = NULL;
    int index;
    fz_context *context;
    fz_document *document;
    fz_page *page;
    fz_matrix transform;
    fz_rect bounds;
    fz_display_list *display_list;
    fz_device *list_device;
    fz_text_sheet *text_sheet;
    fz_text_page *text_page;
    fz_device *text_device;
    FILE *file;
    fz_output *output;

    if (argc != 3) {
        printf("usage: %s filename.pdf pagenum\n", argv[0]);
        return 1;
    }
    filename = argv[1];
    index = atoi(argv[2]);

    /* open document */
    context = fz_new_context(NULL, NULL, FZ_STORE_UNLIMITED);
    fz_register_document_handlers(context);
    document = fz_open_document(context, filename);

    /* load page*/
    page = fz_load_page(document, index - 1);

    /* create transform and bounds*/
    fz_rotate(&transform, 0.0f);
    fz_pre_scale(&transform, 1.0f, 1.0f);
    fz_bound_page(document, page, &bounds);
    fz_transform_rect(&bounds, &transform);

    /* run display list */
    display_list = fz_new_display_list(context);
    list_device = fz_new_list_device(context, display_list);
    fz_run_page(document, page, list_device, &transform, NULL);
    fz_free_device(list_device);

    /* run text page */
    text_sheet = fz_new_text_sheet(context);
    text_page = fz_new_text_page(context);
    text_device = fz_new_text_device(context, text_sheet, text_page);
    fz_run_display_list(display_list, text_device, &transform, &bounds, NULL);
    fz_free_device(text_device);

    /* show text */
    file = fopen("text.txt", "w");
    output = fz_new_output_with_file(context, file);
    fz_print_text_page(context, output, text_page);
    fz_print_text_sheet(context, output, text_sheet);
    fz_close_output(output);

    /* clean up */
    fz_free_text_sheet(context, text_sheet);
    fz_free_text_page(context, text_page);
    fz_drop_display_list(context, display_list);
    fz_free_page(document, page);
    fz_close_document(document);
    fz_free_context(context);
    return 0;
}
