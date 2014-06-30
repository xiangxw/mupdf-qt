/* show infomation of a PDF file(based on mupdf-0.9/apps/win_main.c) */

#include <stdlib.h>
#include <mupdf/fitz.h>
#include <mupdf/pdf.h>

int main(int argc, char **argv)
{
    char *filename = argv[1];
    fz_context *context;
    pdf_document *xref;
    pdf_obj *info;
    char *str;

    /* open xref */
    context = fz_new_context(NULL, NULL, FZ_STORE_UNLIMITED);
    fz_register_document_handlers(context);
    xref = (pdf_document *)fz_open_document(context, filename);
    if (NULL == xref) {
        printf("can't open document: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    /* print infomation */
    info = pdf_dict_gets(pdf_trailer(xref), "Info");
    if (info) {
        pdf_obj *obj = NULL;
        obj = pdf_dict_gets(info, (char *)"Title"); /* print title */
        if (obj) {
            str = pdf_to_utf8(xref, obj);
            printf("Title: %s\n", str);
            free(str);
        }
        obj = pdf_dict_gets(info, (char *)"Subject"); /* print subject */
        if (obj) {
            str = pdf_to_utf8(xref, obj);
            printf("Subject: %s\n", str);
            free(str);
        }
        obj = pdf_dict_gets(info, (char *)"Author"); /* print author */
        if (obj) {
            str = pdf_to_utf8(xref, obj);
            printf("Author: %s\n", str);
            free(str);
        }
        obj = pdf_dict_gets(info, (char *)"Keywords"); /* print keywords */
        if (obj) {
            str = pdf_to_utf8(xref, obj);
            printf("Keywords: %s\n", str);
            free(str);
        }
        obj = pdf_dict_gets(info, (char *)"Creator"); /* print creator */
        if (obj) {
            str = pdf_to_utf8(xref, obj);
            printf("Creator: %s\n", str);
            free(str);
        }
        obj = pdf_dict_gets(info, (char *)"Producer"); /* print producer */
        if (obj) {
            str = pdf_to_utf8(xref, obj);
            printf("Producer: %s\n", str);
            free(str);
        }
        obj = pdf_dict_gets(info, (char *)"CreationDate"); /* print creation date */
        if (obj) {
            str = pdf_to_utf8(xref, obj);
            printf("CreationDate: %s\n", str);
            free(str);
        }
        obj = pdf_dict_gets(info, (char *)"ModDate"); /* print mod date */
        if (obj) {
            str = pdf_to_utf8(xref, obj);
            printf("ModDate: %s\n", str);
            free(str);
        }
    }

    /* clean */
    fz_close_document((fz_document *)xref);
    fz_free_context(context);
    return 0;
}
