#include "mupdf-qt.h"
#include <QImage>
#include <QDebug>

// Usage: ./bin/test_text a.pdf 1
int main(int argc, char **argv)
{
    // open document
    QString file = argv[1];
    if (file.isEmpty()) {
        return 1;
    }
    MuPDF::Document *document = MuPDF::loadDocument(file);
    if (NULL == document) {
        return -1;
    }

    // load page
    MuPDF::Page *page = document->page(atoi(argv[2]) - 1);
    if (NULL == page) {
        delete document;
        return -1;
    }

    // text
    QList<MuPDF::TextBox *> boxes = page->textList();
    foreach (MuPDF::TextBox *box, boxes) {
        qDebug() << box->text();
    }

    qDeleteAll(boxes);
    delete page;
    delete document;
    return 0;
}
