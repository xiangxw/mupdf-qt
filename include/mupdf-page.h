#ifndef MUPDF_PAGE_H
#define MUPDF_PAGE_H

#include <QList>

class QImage;
class QString;
class QSizeF;

namespace MuPDF
{
class Page;
class PagePrivate;
class Document;
class TextBox;

/**
 * @brief A page.
 *
 * @note When you are doing something with this page, make sure the Document
 * who generate this page is valid.
 */
class Page
{
public:
    ~Page();
    bool isValid() const;
    QImage renderImage() const;
    QSizeF size() const;
    void setTransparentRendering(bool enable);
    void setBackgroundColor(int r, int g, int b, int a = 255);
    void setTransform(float scaleX, float scaleY, float rotation = 0.0f);
    QString text(float x0, float y0, float x1, float y1) const;
    QList<TextBox *> textList() const;

private:
    Page(PagePrivate *pagep)
        : d(pagep)
    {

    }

    PagePrivate *d;

friend class Document;
};

} // end namespace MuPDF

#endif // end MUPDF_PAGE_H
