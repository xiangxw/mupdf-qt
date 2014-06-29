#ifndef MUPDF_PAGE_H
#define MUPDF_PAGE_H

class QImage;
class QString;
class QRect;

namespace MuPDF
{
class Page;
class PagePrivate;
class Document;

class Page
{
public:
	~Page();
	bool isValid() const;
	QImage renderImage() const;
	QRect size() const;
	void setTransparentRendering(bool enable);
	void setBackgroundColor(int r, int g, int b, int a = 255);
	void setTransform(float scaleX, float scaleY, float rotation = 0.0f);
	QString text(float x0, float y0, float x1, float y1) const;

private:
	Page(const Document &document, int index);
	PagePrivate *d;

friend class Document;
};

} // end namespace MuPDF

#endif // end MUPDF_PAGE_H
