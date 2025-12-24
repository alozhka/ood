#include "MainWindow.h"

#include "presenter/DocumentPresenter.h"

#include <QAction>
#include <QFileDialog>
#include <QMenuBar>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>
#include <QWidget>

namespace
{
QGraphicsScene* CreateScene(QObject* parent)
{
	auto* scene = new QGraphicsScene(parent);
	scene->setBackgroundBrush(Qt::white);
	scene->setSceneRect(QRectF(0, 0, 800, 600));
	return scene;
}
QGraphicsView* CreateView(QGraphicsScene* scene, QWidget* parent)
{
	auto* view = new QGraphicsView(scene, parent);
	view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	view->setRenderHint(QPainter::Antialiasing);
	view->setDragMode(QGraphicsView::NoDrag);
	return view;
}
} // namespace

MainWindow::MainWindow()
{
	setWindowTitle("Geometry Drawer");
	setStyleSheet("QMainWindow { background-color: black; }");

	m_scene = CreateScene(this);
	m_view = CreateView(m_scene, this);

	auto* centralWidget = new QWidget(this);
	auto* layout = new QHBoxLayout(centralWidget);
	layout->addStretch();
	layout->addWidget(m_view);
	layout->addStretch();
	layout->setContentsMargins(50, 50, 50, 50);

	setCentralWidget(centralWidget);

	auto* shapesDocument = new Document();
	m_documentPresenter = new DocumentPresenter(shapesDocument, m_scene, this);

	CreateMenu();
	CreateToolbar();
}

void MainWindow::OnOpenFile()
{
	QString filePath = QFileDialog::getOpenFileName(
		this,
		"Open composition",
		"",
		"JSON files (*.json);;All files (*)");

	if (filePath.isEmpty())
	{
		return;
	}

	if (!m_documentPresenter->LoadFromFile(filePath))
	{
		qWarning() << "Failed to load file:" << filePath;
	}
}

void MainWindow::OnSaveFile()
{
    // Программировать, не приходя в сознание
	QString filePath = QFileDialog::getSaveFileName(
		this,
		"Save composition",
		"",
		"JSON files (*.json);;All files (*)");

	if (filePath.isEmpty())
	{
		return;
	}

	if (!m_documentPresenter->SaveToFile(filePath))
	{
		qWarning() << "Failed to save file:" << filePath;
	}
}

void MainWindow::CreateMenu()
{
	m_menu = menuBar()->addMenu("File");

	QAction* openFile = new QAction("Open", this);
	openFile->setShortcut(QKeySequence::Open);
	connect(openFile, &QAction::triggered, this, &MainWindow::OnOpenFile);
	m_menu->addAction(openFile);

	QAction* saveFile = new QAction("Save as", this);
	saveFile->setShortcut(QKeySequence::SaveAs);
	connect(saveFile, &QAction::triggered, this, &MainWindow::OnSaveFile);
	m_menu->addAction(saveFile);
}

void MainWindow::CreateToolbar()
{
	m_toolbar = addToolBar("Tools");

	QToolButton* rectBtn = new QToolButton(this);
	rectBtn->setText("□ Rectangle");
	connect(rectBtn, &QToolButton::clicked, m_documentPresenter, &DocumentPresenter::AddRectangle);
	m_toolbar->addWidget(rectBtn);

	QToolButton* ellipseBtn = new QToolButton(this);
	ellipseBtn->setText("○ Ellipse");
	connect(ellipseBtn, &QToolButton::clicked, m_documentPresenter, &DocumentPresenter::AddEllipse);
	m_toolbar->addWidget(ellipseBtn);

	QToolButton* triangleBtn = new QToolButton(this);
	triangleBtn->setText("△ Triangle");
	connect(triangleBtn, &QToolButton::clicked, m_documentPresenter, &DocumentPresenter::AddTriangle);
	m_toolbar->addWidget(triangleBtn);

	m_toolbar->addSeparator();

	QToolButton* deleteBtn = new QToolButton(this);
	deleteBtn->setText("Delete selected");
	deleteBtn->setShortcut(QKeySequence::Delete);
	connect(deleteBtn, &QToolButton::clicked, m_documentPresenter, &DocumentPresenter::RemoveSelectedShapes);
	m_toolbar->addWidget(deleteBtn);
}