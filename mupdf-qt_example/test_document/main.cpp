#include <QDateTime>
#include <QDebug>
#include "mupdf-qt.h"

int main(int argc, char **argv)
{
    // open document
    QString file = argv[1];
    if (file.isEmpty()) {
        return 1;
    }
    MuPDF::Document *document = MuPDF::loadDocument(file);
    if (NULL == document) {
        return 1;
    }

    // print info
    qDebug() << "PDF version:" << document->pdfVersion();
    qDebug() << "Page count:" << document->numPages();
    qDebug() << "Title:" << document->title();
    qDebug() << "Subject:" << document->subject();
    qDebug() << "Author:" << document->author();
    qDebug() << "Keywords:" << document->keywords();
    qDebug() << "Creator:" << document->creator();
    qDebug() << "Producer:" << document->producer();
    qDebug() << "CreationDate:" << document->creationDate()
        .toString(Qt::SystemLocaleLongDate);
    qDebug() << "ModDate:" << document->modDate()
        .toString(Qt::SystemLocaleLongDate);

    delete document;
    return 0;
}
