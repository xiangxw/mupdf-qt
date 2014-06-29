#include "mupdf-qt.h"
#include <QDebug>

void printOutlineItem(const MuPDF::OutlineItem &item, int space = 0)
{
    MuPDF::OutlineItem next = item;

    if (!item.isValid()) {
        return;
    }

    // next
    while (next.isValid()) {
        {
            QDebug db(QtDebugMsg);
            db.nospace();
            for (int i = 0; i < space; ++i) {
                db << " ";
            }
            db << next.title();
        }
        printOutlineItem(next.down(), space + 4); // down
        next = next.next();
    }
}

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

    // load outline
    MuPDF::Outline *outline = document->outline();
    MuPDF::OutlineItem item;
    if (outline) {
        item = outline->topLevelItem();
        printOutlineItem(item);
    } else {
        qDebug() << "No outline!";
    }

    delete outline;
    delete document;
    return 0;
}
