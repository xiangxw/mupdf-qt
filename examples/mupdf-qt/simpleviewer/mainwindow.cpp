#include "mainwindow.h"
#include <QScrollArea>
#include <QToolBar>
#include <QAction>
#include <QFileDialog>
#include <QImage>
#include <QPixmap>
#include <QScrollBar>
#include <QInputDialog>
#include <QMouseEvent>
#include <cmath>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent)
     , m_doc(NULL), m_page(NULL), m_index(0), m_scale(1.0f)
{
    scrollArea = new QScrollArea;
    scrollArea->setAlignment(Qt::AlignCenter);
    label = new Page;
    scrollArea->setWidget(label);
    this->setCentralWidget(scrollArea);
    this->setWindowState(Qt::WindowMaximized);

    createActions();
    createToolBars();
}

MainWindow::~MainWindow()
{
    if (m_page) {
        delete m_page;
        m_page = NULL;
    }
    if (m_doc) {
        delete m_doc;
        m_doc = NULL;
    }
}

void MainWindow::openDocument()
{
    QString file = QFileDialog::getOpenFileName(this,
            tr("Open PDF/XPS file"), ".", "PDF (*.pdf);;XPS (*.xps)");
    if (file.isEmpty()) {
        return;
    }

    if (m_page) {
        delete m_page;
        m_page = NULL;
    }
    if (m_doc) {
        delete m_doc;
        m_doc = NULL;
    }
    m_doc = MuPDF::loadDocument(file);
    if (NULL == m_doc) {
        return;
    }

    if (m_doc->needsPassword()) {
        bool ok;
        QString password;
        do {
            password = QInputDialog::getText(this, tr("Enter password"),
                                                 tr("Password:"), QLineEdit::Normal,
                                                 "", &ok);
            if (!ok)
                return;
        } while (!m_doc->authPassword(password));
    }

    m_title = m_doc->title();
    m_numPages = m_doc->numPages();

    m_index = 0;
    openPage(0);
}

void MainWindow::previousPage()
{
    if (NULL == m_doc) {
        return;
    }
    if (m_index > 0) {
        --m_index;
    }
    openPage(m_index);
}

void MainWindow::nextPage()
{
    if (NULL == m_doc) {
        return;
    }
    if (m_index < m_numPages - 1) {
        ++m_index;
    }
    openPage(m_index);
}

void MainWindow::zoomIn()
{
    if (NULL == m_doc) {
        return;
    }
    if (m_scale >= 10.0f) {
        return;
    }
    m_scale += 0.1f;
    label->setScale(m_scale);

    updateTitle();
}

void MainWindow::zoomOut()
{
    if (NULL == m_doc) {
        return;
    }
    if (m_scale < 0.15f) {
        return;
    }
    m_scale -= 0.1f;
    label->setScale(m_scale);

    updateTitle();
}

void MainWindow::createActions()
{
    openAction = new QAction(tr("Open"), this);
    connect(openAction, SIGNAL(triggered()), this, SLOT(openDocument()));

    previousPageAction = new QAction(tr("Previous"), this);
    connect(previousPageAction, SIGNAL(triggered()), this, SLOT(previousPage()));

    nextPageAction = new QAction(tr("Next"), this);
    connect(nextPageAction, SIGNAL(triggered()), this, SLOT(nextPage()));

    zoomInAction = new QAction(tr("ZoomIn"), this);
    connect(zoomInAction, SIGNAL(triggered()), this, SLOT(zoomIn()));

    zoomOutAction = new QAction(tr("ZoomOut"), this);
    connect(zoomOutAction, SIGNAL(triggered()), this, SLOT(zoomOut()));
}

void MainWindow::createToolBars()
{
    toolBar = this->addToolBar(tr("ToolBar"));
    toolBar->addAction(openAction);
    toolBar->addAction(previousPageAction);
    toolBar->addAction(nextPageAction);
    toolBar->addAction(zoomInAction);
    toolBar->addAction(zoomOutAction);
}

void MainWindow::openPage(int index)
{
    if (m_page) {
        delete m_page;
        m_page = NULL;
    }
    m_page = m_doc->page(index);
    if (NULL == m_page) {
        return;
    }

    label->setPage(m_page, m_scale);
    scrollArea->verticalScrollBar()->setValue(0);

    updateTitle();
}

void MainWindow::updateTitle()
{
    QString title;
    if (m_title.isEmpty()) {
        title = "No Title - ";
    } else {
        title = m_title + " - ";
    }
    title += "Page " + QString::number(m_index + 1)
        + "/" + QString::number(m_numPages);
    title += " - ";
    title += "Scale " + QString::number(qRound(m_scale * 100)) + "%";
    this->setWindowTitle(title);
}

Page::Page(QWidget *parent)
    : QLabel(parent),
      m_page(NULL),
      m_scale(1.0f)
{

}

Page::~Page()
{
}

void Page::setPage(MuPDF::Page *page, float scale)
{
    m_page = page;
    m_scale = scale;
    updatePage();
}

void Page::setScale(float scale)
{
    m_scale = scale;
    updatePage();
}

void Page::mousePressEvent(QMouseEvent *event)
{
    m_pressPoint = event->localPos();
}

void Page::mouseReleaseEvent(QMouseEvent *event)
{
    QPointF point;
    float x0, y0, x1, y1;

    if (!m_page) {
        return;
    }

    point = event->localPos();
    if (point != m_pressPoint) {
        if (m_pressPoint.x() > point.x()) {
            x0 = point.x();
            x1 = m_pressPoint.x();
        } else {
            x0 = m_pressPoint.x();
            x1 = point.x();
        }
        if (m_pressPoint.y() > point.y()) {
            y0 = point.y();
            y1 = m_pressPoint.y();
        } else {
            y0 = m_pressPoint.y();
            y1 = point.y();
        }
        QRectF rect(x0, y0, x1 - x0, y1 - y0);
        rect = MuPDF::mapToOrigin(rect, m_scale, m_scale);
        qDebug() << m_page->text(rect);
    }
}

void Page::updatePage()
{
    QImage image;

    if (!m_page) {
        return;
    }

    image = m_page->renderImage(m_scale, m_scale);
    this->setPixmap(QPixmap::fromImage(image));
    this->resize(this->sizeHint());
}
