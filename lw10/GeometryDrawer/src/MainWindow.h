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
	void OnSaveAsFile();
	void OnInsertImage();

private:
	void CreateMenu();
	void CreateToolbar();

	QMenu* m_menu;
	QToolBar* m_toolbar;
	QGraphicsView* m_view;
	QGraphicsScene* m_scene;
	std::optional<QString> m_saveFilePath;

	DocumentPresenter* m_documentPresenter;
};