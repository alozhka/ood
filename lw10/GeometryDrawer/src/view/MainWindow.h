#pragma once

#include "../presenter/CanvasPresenter.h"

#include <QGraphicsView>
#include <QMainWindow>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow();
	~MainWindow() = default;

private slots:
	void OnSelectRectangle();
	void OnSelectTriangle();
	void OnSelectEllipse();

	void OnOpenFile();
	void OnSaveFile();

private:
	void CreateMenu();
	void CreateToolbar();

	QMenu* m_menu;
	QToolBar* m_toolbar;
	QGraphicsView* m_view;
	QGraphicsScene* m_scene;

	std::unique_ptr<CanvasPresenter> m_canvasPresenter;
};