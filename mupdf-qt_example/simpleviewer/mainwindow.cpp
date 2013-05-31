/**
 * @file mainwindow.cpp
 * @brief class MainWindow
 * @author xiangxw xiangxw5689@126.com
 * @date 2012-04-01
 */

#include "mainwindow.h"
#include <QScrollArea>
#include <QLabel>
#include <QToolBar>
#include <QAction>
#include <QFileDialog>
#include <QImage>
#include <QPixmap>
#include <QScrollBar>
#include <QInputDialog>
#include <cmath>

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
            tr("Open PDF/XPS file"), ".", "PDF (*.pdf);;XPS (*.xps)");
	if (file.isEmpty()) {
		return;
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
	if (m_scale < 0.15f) {
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
	MuPDF::Page *page = m_doc->page(index);
	if (NULL == page) {
		return;
	}
	page->setTransparentRendering(true);
	QImage image = page->renderImage(m_scale, m_scale);
	label->setPixmap(QPixmap::fromImage(image));
	delete page;
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
	title += "Scale " + QString::number(round(m_scale * 100)) + "%";
	this->setWindowTitle(title);
}
