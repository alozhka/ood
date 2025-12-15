#include "MainWindow.h"

#include "presenter/DocumentPresenter.h"

#include <QAction>
#include <QFileDialog>
#include <QMenuBar>
#include <QToolBar>
#include <QToolButton>

MainWindow::MainWindow()
{
	setWindowTitle("Geometry Drawer");

	m_scene = new QGraphicsScene(this);
	m_scene->setBackgroundBrush(Qt::white);
	QRectF sceneRect(0, 0, 800, 600);
	m_scene->setSceneRect(sceneRect);

	m_view = new QGraphicsView(m_scene, this);
	m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_view->setRenderHint(QPainter::Antialiasing);
	m_view->setDragMode(QGraphicsView::NoDrag);

	setCentralWidget(m_view);

	auto* shapesDocument = new Document();
	m_documentPresenter = new DocumentPresenter(shapesDocument, m_scene, this);

	CreateMenu();
	CreateToolbar();
}

void MainWindow::OnSelectRectangle()
{
	m_documentPresenter->AddShape(Shape::Type::Rectangle);
}
void MainWindow::OnSelectTriangle()
{
	m_documentPresenter->AddShape(Shape::Type::Triangle);
}
void MainWindow::OnSelectEllipse()
{
	m_documentPresenter->AddShape(Shape::Type::Ellipse);
}

void MainWindow::OnOpenFile()
{
	QFileDialog::getOpenFileName(this, "Open composition", "", "JSON files (*.json);; All files (*)");
	// TODO: реализовать
}
void MainWindow::OnSaveFile()
{
	// Программировать, не приходя в сознание
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
	connect(rectBtn, &QToolButton::clicked, this, &MainWindow::OnSelectRectangle);
	m_toolbar->addWidget(rectBtn);

	QToolButton* ellipseBtn = new QToolButton(this);
	ellipseBtn->setText("○ Ellipse");
	connect(ellipseBtn, &QToolButton::clicked, this, &MainWindow::OnSelectEllipse);
	m_toolbar->addWidget(ellipseBtn);

	QToolButton* triangleBtn = new QToolButton(this);
	triangleBtn->setText("△ Triangle");
	connect(triangleBtn, &QToolButton::clicked, this, &MainWindow::OnSelectTriangle);
	m_toolbar->addWidget(triangleBtn);

	m_toolbar->addSeparator();

	QToolButton* deleteBtn = new QToolButton(this);
	deleteBtn->setText("Delete selected");
	deleteBtn->setShortcut(QKeySequence::Delete);
	connect(deleteBtn, &QToolButton::clicked, m_documentPresenter, &DocumentPresenter::RemoveSelectedShapes);
	m_toolbar->addWidget(deleteBtn);
}