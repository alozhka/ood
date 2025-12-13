#include "MainWindow.h"
#include <QAction>
#include <QFileDialog>
#include <QMenuBar>
#include <QToolBar>
#include <QToolButton>

MainWindow::MainWindow()
{
	setWindowTitle("Geometry Drawer");

	CreateMenu();
	CreateToolbar();

	m_scene = new QGraphicsScene(this);
	m_scene->setBackgroundBrush(Qt::white);
	m_view = new QGraphicsView(m_scene, this);
	m_view->setRenderHint(QPainter::Antialiasing);
	m_view->setDragMode(QGraphicsView::RubberBandDrag);
	setCentralWidget(m_view);
	m_canvasPresenter = std::make_unique<CanvasPresenter>(m_scene, this);
}

void MainWindow::OnSelectRectangle()
{
	m_canvasPresenter->AddRectangle();
	// TODO: добавить в модель
}
void MainWindow::OnSelectTriangle()
{
	m_canvasPresenter->AddTriangle();
	// TODO: добавить в модель
}
void MainWindow::OnSelectEllipse()
{
	m_canvasPresenter->AddEllipse();
	// TODO: добавить в модель
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
}