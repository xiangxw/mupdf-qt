/**
 * @file mainwindow.cpp
 * @brief class MainWindow
 * @author xiangxw xiangxw5689@126.com
 * @date 2012-04-01
 */

#include "mainwindow.h"
#include <QtGui/QScrollArea>
#include <QtGui/QLabel>
#include <QtGui/QToolBar>
#include <QtGui/QAction>
#include <QtGui/QFileDialog>
#include <QtGui/QImage>
#include <QtGui/QPixmap>
#include <QtGui/QScrollBar>

MainWindow::MainWindow(QWidget *parent)
	:QMainWindow(parent)
	 , m_doc(NULL), m_index(0), m_scale(1.0f)
{
	scrollArea = new QScrollArea;
	scrollArea->setAlignment(Qt::AlignCenter);
	label = new QLabel;
	scrollArea->setWidget(label);
	this->setCentralWidget(scrollArea);
	this->setWindowState(Qt::WindowMaximized);

	createActions();
	createToolBars();
}

MainWindow::~MainWindow()
{
	if (m_doc) {
		delete m_doc;
	}
}

void MainWindow::open()
{
	QString file = QFileDialog::getOpenFileName(this,
			tr("Open PDF file"), ".", "PDF (*.pdf)");
	if (file.isEmpty()) {
		return;
	}

	if (m_doc) {
		delete m_doc;
	}
	m_doc = new Mupdf::Document(file);
	m_title = m_doc->getInfo(Mupdf::Document::PDFInfoTitle);
	m_numPages = m_doc->numPages();

	m_index = 0;
	showPage(0);
}

void MainWindow::previousPage()
{
	if (NULL == m_doc) {
		return;
	}
	if (m_index > 0) {
		--m_index;
	}
	showPage(m_index);
}

void MainWindow::nextPage()
{
	if (NULL == m_doc) {
		return;
	}
	if (m_index < m_numPages - 1) {
		++m_index;
	}
	showPage(m_index);
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
	showPage(m_index);
}

void MainWindow::zoomOut()
{
	if (NULL == m_doc) {
		return;
	}
	if (m_scale <= 0.1f) {
		return;
	}
	m_scale -= 0.1f;
	showPage(m_index);
}

void MainWindow::createActions()
{
	openAction = new QAction(tr("Open"), this);
	connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

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

void MainWindow::showPage(int index)
{
	Mupdf::Page page = m_doc->page(index);
	QImage image = page.renderImage(m_scale);
	label->setPixmap(QPixmap::fromImage(image));
	label->resize(label->sizeHint());
	scrollArea->verticalScrollBar()->setValue(0);

	QString title;
	if (m_title.isEmpty()) {
		title = "No Title - ";
	} else {
		title = m_title + " - ";
	}
	title += "Page " + QString::number(m_index + 1)
		+ "/" + QString::number(m_numPages);
	title += " - ";
	title += "Scale " + QString::number(m_scale * 100) + "%";
	this->setWindowTitle(title);
}
