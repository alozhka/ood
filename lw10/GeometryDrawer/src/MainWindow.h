#pragma once

#include "presenter/DocumentPresenter.h"

#include <QGraphicsView>
#include <QMainWindow>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow();
	~MainWindow() = default;

private slots:
	void OnOpenFile();
	void OnSaveFile();
	void OnInsertImage();

private:
	void CreateMenu();
	void CreateToolbar();

	QMenu* m_menu;
	QToolBar* m_toolbar;
	QGraphicsView* m_view;
	QGraphicsScene* m_scene;

	DocumentPresenter* m_documentPresenter;
};