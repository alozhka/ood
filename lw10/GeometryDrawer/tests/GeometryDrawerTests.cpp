#include <QLoggingCategory>
#include <QSignalSpy>
#include <QTemporaryFile>
#include <QTest>

#include "../src/model/Document.h"
#include "../src/model/ImageStorage.h"
#include "../src/model/Shape.h"
#include "../src/presenter/Commands.h"
#include "../src/presenter/JsonDocumentRepository.h"

// ============================================================================
// Тесты для класса Document
// ============================================================================
class DocumentTests : public QObject
{
	Q_OBJECT
private:
	Document* m_document = nullptr;
	QSignalSpy* m_shapesAddedSpy = nullptr;
	QSignalSpy* m_shapesRemovedSpy = nullptr;

private slots:
	void init()
	{
		m_document = new Document(this);
		m_shapesAddedSpy = new QSignalSpy(m_document, &Document::ShapesAdded);
		m_shapesRemovedSpy = new QSignalSpy(m_document, &Document::ShapesRemoved);
	}

	void cleanup()
	{
		delete m_shapesAddedSpy;
		m_shapesAddedSpy = nullptr;
		delete m_shapesRemovedSpy;
		m_shapesRemovedSpy = nullptr;
		delete m_document;
		m_document = nullptr;
	}

	void testAddShapes()
	{
		auto ellipse = QSharedPointer<Shape>::create(Shape::Type::Ellipse, QRectF(50, 50, 80, 80));
		auto rect = QSharedPointer<Shape>::create(Shape::Type::Rectangle, QRectF(0, 0, 100, 100));
		auto triangle = QSharedPointer<Shape>::create(Shape::Type::Triangle, QRectF(100, 100, 60, 60));
		QSet expected = { rect, ellipse, triangle };

		m_document->AddShapes(expected.values());

		QList<QSharedPointer<Shape>> shapesList = m_document->ListShapes();
		QSet actual(shapesList.begin(), shapesList.end());
		QCOMPARE(expected, actual);
		QCOMPARE(m_shapesAddedSpy->count(), 1);
	}

	void testRemoveShape()
	{
		auto ellipse = QSharedPointer<Shape>::create(Shape::Type::Ellipse, QRectF(50, 50, 80, 80));
		auto rect = QSharedPointer<Shape>::create(Shape::Type::Rectangle, QRectF(0, 0, 100, 100));
		auto triangle = QSharedPointer<Shape>::create(Shape::Type::Triangle, QRectF(100, 100, 60, 60));
		m_document->AddShapes({ ellipse, rect, triangle });
		QSet expectedIds = { ellipse->GetId(), rect->GetId() };

		QList<QSharedPointer<Shape>> removedShapes = m_document->RemoveShapes(expectedIds.values());

		QCOMPARE(removedShapes.size(), 2);
		QSet<QUuid> actualIds;
		for (const auto& shape : removedShapes)
		{
			actualIds.insert(shape->GetId());
		}
		QCOMPARE(expectedIds, actualIds);
		QCOMPARE(m_document->ListShapes().size(), 1);
		QCOMPARE(m_shapesRemovedSpy->count(), 1);
	}

	void testRemoveNonExistentShape()
	{
		QSignalSpy signal(m_document, &Document::ShapesRemoved);
		QUuid fakeId = QUuid::createUuid();
		QList<QSharedPointer<Shape>> removed = m_document->RemoveShapes({ fakeId });

		QCOMPARE(removed.size(), 0);
	}

	void testUpdateShapeGeometry()
	{
		auto shape = QSharedPointer<Shape>::create(Shape::Type::Rectangle, QRectF(0, 0, 100, 100));
		m_document->AddShapes({ shape });

		QRectF newRect(50, 50, 200, 150);
		QHash<QUuid, QRectF> updates;
		updates.insert(shape->GetId(), newRect);

		m_document->UpdateShapesGeometry(updates);

		QCOMPARE(shape->GetRect(), newRect);
	}

	void testShapesGeometryChangedSignal()
	{
		auto shape = QSharedPointer<Shape>::create(Shape::Type::Rectangle, QRectF(0, 0, 100, 100));
		m_document->AddShapes({ shape });

		QSignalSpy spy(m_document, &Document::ShapesGeometryChanged);

		QHash<QUuid, QRectF> updates;
		updates.insert(shape->GetId(), QRectF(10, 10, 50, 50));
		m_document->UpdateShapesGeometry(updates);

		QCOMPARE(spy.count(), 1);
	}

	void testClearDocument()
	{
		auto shape1 = QSharedPointer<Shape>::create(Shape::Type::Rectangle, QRectF(0, 0, 100, 100));
		auto shape2 = QSharedPointer<Shape>::create(Shape::Type::Ellipse, QRectF(50, 50, 80, 80));
		m_document->AddShapes({ shape1, shape2 });

		QSignalSpy spy(m_document, &Document::Cleared);

		m_document->Clear();

		QCOMPARE(m_document->ListShapes().size(), 0);
		QCOMPARE(spy.count(), 1);
	}

	void testGetShapesGeometry()
	{
		auto shape1 = QSharedPointer<Shape>::create(Shape::Type::Rectangle, QRectF(10, 20, 100, 50));
		auto shape2 = QSharedPointer<Shape>::create(Shape::Type::Ellipse, QRectF(30, 40, 80, 60));
		m_document->AddShapes({ shape1, shape2 });

		QHash<QUuid, QRectF> geometry = m_document->GetShapesGeometry({ shape1->GetId(), shape2->GetId() });

		QCOMPARE(geometry.size(), 2);
		QCOMPARE(geometry[shape1->GetId()], QRectF(10, 20, 100, 50));
		QCOMPARE(geometry[shape2->GetId()], QRectF(30, 40, 80, 60));
	}

	void testImageShapePath()
	{
		QString testPath = "/path/to/test/image.png";
		auto imageShape = QSharedPointer<Shape>::create(Shape::Type::Image, QRectF(0, 0, 100, 100));
		imageShape->SetImagePath(testPath);
		m_document->AddShapes({ imageShape });

		QCOMPARE(imageShape->GetType(), Shape::Type::Image);
		QCOMPARE(imageShape->GetImagePath(), testPath);

		auto retrievedShape = m_document->ListShapes()[0];
		QCOMPARE(retrievedShape->GetType(), Shape::Type::Image);
		QCOMPARE(retrievedShape->GetImagePath(), testPath);
	}
};

// ============================================================================
// Интеграционные тесты для Commands + Document + QUndoStack
// ============================================================================
class CommandsIntegrationTests : public QObject
{
	Q_OBJECT

private:
	Document* m_document = nullptr;
	QUndoStack* m_undoStack = nullptr;

private slots:
	void init()
	{
		m_document = new Document(this);
		m_undoStack = new QUndoStack(this);
	}

	void cleanup()
	{
		delete m_undoStack;
		delete m_document;
		m_undoStack = nullptr;
		m_document = nullptr;
	}

	// Тест: добавление фигуры через команду
	void testAddShapeCommand()
	{
		auto shape = QSharedPointer<Shape>::create(Shape::Type::Rectangle, QRectF(0, 0, 100, 100));
		auto* command = new AddShapeCommand(m_document, shape);

		m_undoStack->push(command);

		QCOMPARE(m_document->ListShapes().size(), 1);
		QCOMPARE(m_document->ListShapes()[0], shape);
	}

	// Тест: Undo добавления фигуры
	void testAddShapeCommandUndo()
	{
		auto shape = QSharedPointer<Shape>::create(Shape::Type::Rectangle, QRectF(0, 0, 100, 100));
		auto* command = new AddShapeCommand(m_document, shape);

		m_undoStack->push(command);
		QCOMPARE(m_document->ListShapes().size(), 1);

		m_undoStack->undo();
		QCOMPARE(m_document->ListShapes().size(), 0);
	}

	// Тест: Redo добавления фигуры
	void testAddShapeCommandRedo()
	{
		auto shape = QSharedPointer<Shape>::create(Shape::Type::Rectangle, QRectF(0, 0, 100, 100));
		auto* command = new AddShapeCommand(m_document, shape);

		m_undoStack->push(command);
		m_undoStack->undo();
		QCOMPARE(m_document->ListShapes().size(), 0);

		m_undoStack->redo();
		QCOMPARE(m_document->ListShapes().size(), 1);
		QCOMPARE(m_document->ListShapes()[0], shape);
	}

	// Тест: удаление фигуры через команду
	void testRemoveShapeCommand()
	{
		auto shape = QSharedPointer<Shape>::create(Shape::Type::Rectangle, QRectF(0, 0, 100, 100));
		m_document->AddShapes({ shape });

		auto* command = new RemoveShapesCommand(m_document, { shape->GetId() });
		m_undoStack->push(command);

		QCOMPARE(m_document->ListShapes().size(), 0);
	}

	// Тест: Undo удаления фигуры (восстановление)
	void testRemoveShapeCommandUndo()
	{
		auto shape = QSharedPointer<Shape>::create(Shape::Type::Rectangle, QRectF(0, 0, 100, 100), 5);
		m_document->AddShapes({ shape });
		QUuid shapeId = shape->GetId();

		auto* command = new RemoveShapesCommand(m_document, { shapeId });
		m_undoStack->push(command);
		QCOMPARE(m_document->ListShapes().size(), 0);

		// Undo - фигура должна восстановиться
		m_undoStack->undo();
		QCOMPARE(m_document->ListShapes().size(), 1);

		// Проверяем, что восстановлена та же фигура с тем же ID и слоем
		auto restoredShape = m_document->ListShapes()[0];
		QCOMPARE(restoredShape->GetId(), shapeId);
		QCOMPARE(restoredShape->GetLayer(), 5);
		QCOMPARE(restoredShape->GetRect(), QRectF(0, 0, 100, 100));
	}

	// Тест: изменение геометрии через команду
	void testUpdateGeometryCommand()
	{
		auto shape = QSharedPointer<Shape>::create(Shape::Type::Rectangle, QRectF(0, 0, 100, 100));
		m_document->AddShapes({ shape });

		QHash<QUuid, QRectF> oldGeometry;
		oldGeometry.insert(shape->GetId(), QRectF(0, 0, 100, 100));

		QHash<QUuid, QRectF> newGeometry;
		newGeometry.insert(shape->GetId(), QRectF(50, 50, 200, 150));

		auto* command = new UpdateShapesGeometryCommand(m_document, oldGeometry, newGeometry);
		m_undoStack->push(command);

		QCOMPARE(shape->GetRect(), QRectF(50, 50, 200, 150));
	}

	// Тест: Undo изменения геометрии
	void testUpdateGeometryCommandUndo()
	{
		auto shape = QSharedPointer<Shape>::create(Shape::Type::Rectangle, QRectF(0, 0, 100, 100));
		m_document->AddShapes({ shape });

		QHash<QUuid, QRectF> oldGeometry;
		oldGeometry.insert(shape->GetId(), QRectF(0, 0, 100, 100));

		QHash<QUuid, QRectF> newGeometry;
		newGeometry.insert(shape->GetId(), QRectF(50, 50, 200, 150));

		auto* command = new UpdateShapesGeometryCommand(m_document, oldGeometry, newGeometry);
		m_undoStack->push(command);

		m_undoStack->undo();
		QCOMPARE(shape->GetRect(), QRectF(0, 0, 100, 100));
	}

	// Тест: множественный Undo/Redo
	void testMultipleUndoRedo()
	{
		auto shape1 = QSharedPointer<Shape>::create(Shape::Type::Rectangle, QRectF(0, 0, 100, 100));
		auto shape2 = QSharedPointer<Shape>::create(Shape::Type::Ellipse, QRectF(50, 50, 80, 80));

		m_undoStack->push(new AddShapeCommand(m_document, shape1));
		m_undoStack->push(new AddShapeCommand(m_document, shape2));

		QCOMPARE(m_document->ListShapes().size(), 2);

		m_undoStack->undo();
		QCOMPARE(m_document->ListShapes().size(), 1);

		m_undoStack->undo();
		QCOMPARE(m_document->ListShapes().size(), 0);

		m_undoStack->redo();
		QCOMPARE(m_document->ListShapes().size(), 1);

		m_undoStack->redo();
		QCOMPARE(m_document->ListShapes().size(), 2);
	}

	// Тест: цепочка операций (добавление -> изменение -> удаление -> undo)
	void testComplexOperationChain()
	{
		// Добавляем фигуру
		auto shape = QSharedPointer<Shape>::create(Shape::Type::Rectangle, QRectF(0, 0, 100, 100));
		m_undoStack->push(new AddShapeCommand(m_document, shape));
		QCOMPARE(m_document->ListShapes().size(), 1);

		// Изменяем геометрию
		QHash<QUuid, QRectF> oldGeometry;
		oldGeometry.insert(shape->GetId(), QRectF(0, 0, 100, 100));
		QHash<QUuid, QRectF> newGeometry;
		newGeometry.insert(shape->GetId(), QRectF(50, 50, 150, 150));

		m_undoStack->push(new UpdateShapesGeometryCommand(m_document, oldGeometry, newGeometry));
		QCOMPARE(shape->GetRect(), QRectF(50, 50, 150, 150));

		// Удаляем фигуру
		m_undoStack->push(new RemoveShapesCommand(m_document, { shape->GetId() }));
		QCOMPARE(m_document->ListShapes().size(), 0);

		// Отменяем удаление
		m_undoStack->undo();
		QCOMPARE(m_document->ListShapes().size(), 1);
		QCOMPARE(m_document->ListShapes()[0]->GetRect(), QRectF(50, 50, 150, 150));

		// Отменяем изменение геометрии
		m_undoStack->undo();
		QCOMPARE(m_document->ListShapes()[0]->GetRect(), QRectF(0, 0, 100, 100));

		// Отменяем добавление
		m_undoStack->undo();
		QCOMPARE(m_document->ListShapes().size(), 0);
	}

	// Тест: проверка сигналов при Undo/Redo
	void testSignalsDuringUndoRedo()
	{
		QSignalSpy addSpy(m_document, &Document::ShapesAdded);
		QSignalSpy removeSpy(m_document, &Document::ShapesRemoved);

		auto shape = QSharedPointer<Shape>::create(Shape::Type::Rectangle, QRectF(0, 0, 100, 100));
		m_undoStack->push(new AddShapeCommand(m_document, shape));

		QCOMPARE(addSpy.count(), 1);

		m_undoStack->undo();
		QCOMPARE(removeSpy.count(), 1);

		m_undoStack->redo();
		QCOMPARE(addSpy.count(), 2);
	}
};

class DocumentRepositoryTests : public QObject
{
	Q_OBJECT
private:
	Document* m_document = nullptr;

private slots:
	void init()
	{
		m_document = new Document(this);
	}

	void cleanup()
	{
		delete m_document;
		m_document = nullptr;
	}

	void testSaveEmptyDocument()
	{
		QTemporaryFile file;
		QVERIFY(file.open());
		QString filePath = file.fileName();

		JsonDocumentRepository::SaveToFile(m_document, filePath);

		QVERIFY(QFile::exists(filePath));
	}

	void testSaveAndLoadSingleShape()
	{
		auto originalShape = QSharedPointer<Shape>::create(Shape::Type::Rectangle, QRectF(10, 20, 100, 50), 3);
		m_document->AddShapes({ originalShape });

		QTemporaryFile file;
		QVERIFY(file.open());
		QString filePath = file.fileName();

		JsonDocumentRepository::SaveToFile(m_document, filePath);

		QList<QSharedPointer<Shape>> loadedShapes = JsonDocumentRepository::LoadFromFile(filePath);

		QCOMPARE(loadedShapes.size(), 1);
		QCOMPARE(loadedShapes[0]->GetType(), Shape::Type::Rectangle);
		QCOMPARE(loadedShapes[0]->GetRect(), QRectF(10, 20, 100, 50));
		QCOMPARE(loadedShapes[0]->GetLayer(), 3);
	}

	void testSaveAndLoadMultipleShapes()
	{
		auto rect = QSharedPointer<Shape>::create(Shape::Type::Rectangle, QRectF(0, 0, 100, 100), 1);
		auto ellipse = QSharedPointer<Shape>::create(Shape::Type::Ellipse, QRectF(50, 50, 80, 80), 2);
		auto triangle = QSharedPointer<Shape>::create(Shape::Type::Triangle, QRectF(100, 100, 60, 60), 3);

		m_document->AddShapes({ rect, ellipse, triangle });

		QTemporaryFile file;
		QVERIFY(file.open());
		QString filePath = file.fileName();

		JsonDocumentRepository::SaveToFile(m_document, filePath);
		QList<QSharedPointer<Shape>> loadedShapes = JsonDocumentRepository::LoadFromFile(filePath);

		QCOMPARE(loadedShapes.size(), 3);

		bool hasRect = false, hasEllipse = false, hasTriangle = false;

		for (const auto& shape : loadedShapes)
		{
			if (shape->GetType() == Shape::Type::Rectangle)
			{
				hasRect = true;
				QCOMPARE(shape->GetRect(), QRectF(0, 0, 100, 100));
				QCOMPARE(shape->GetLayer(), 1);
			}
			else if (shape->GetType() == Shape::Type::Ellipse)
			{
				hasEllipse = true;
				QCOMPARE(shape->GetRect(), QRectF(50, 50, 80, 80));
				QCOMPARE(shape->GetLayer(), 2);
			}
			else if (shape->GetType() == Shape::Type::Triangle)
			{
				hasTriangle = true;
				QCOMPARE(shape->GetRect(), QRectF(100, 100, 60, 60));
				QCOMPARE(shape->GetLayer(), 3);
			}
		}

		QVERIFY(hasRect && hasEllipse && hasTriangle);
	}

	void testLoadFromNonExistentFile()
	{
		QVERIFY_THROWS_EXCEPTION(
			std::runtime_error,
			JsonDocumentRepository::LoadFromFile("/nonexistent/path/file.json"));
	}

	void testLoadFromInvalidJson()
	{
		QTemporaryFile file;
		QVERIFY(file.open());

		file.write("{ invalid json }");
		file.close();

		QVERIFY_THROWS_EXCEPTION(
			std::runtime_error,
			JsonDocumentRepository::LoadFromFile(file.fileName()));
	}

	void testJsonFormat()
	{
		auto shape = QSharedPointer<Shape>::create(Shape::Type::Ellipse, QRectF(10, 20, 30, 40), 5);
		m_document->AddShapes({ shape });

		QTemporaryFile file;
		QVERIFY(file.open());
		QString filePath = file.fileName();

		JsonDocumentRepository::SaveToFile(m_document, filePath);

		QFile jsonFile(filePath);
		QVERIFY(jsonFile.open(QIODevice::ReadOnly));

		QByteArray data = jsonFile.readAll();
		QJsonDocument doc = QJsonDocument::fromJson(data);

		QVERIFY(doc.isObject());

		QJsonObject root = doc.object();
		QVERIFY(root.contains("shapes"));

		QJsonArray shapesArray = root["shapes"].toArray();
		QCOMPARE(shapesArray.size(), 1);

		QJsonObject shapeObj = shapesArray[0].toObject();
		QCOMPARE(shapeObj["type"].toInt(), static_cast<int>(Shape::Type::Ellipse));
		QCOMPARE(shapeObj["x"].toDouble(), 10.0);
		QCOMPARE(shapeObj["y"].toDouble(), 20.0);
		QCOMPARE(shapeObj["width"].toDouble(), 30.0);
		QCOMPARE(shapeObj["height"].toDouble(), 40.0);
		QCOMPARE(shapeObj["layer"].toInt(), 5);
	}

	void testSaveAndLoadImageShape()
	{
		QString originalImagePath = QDir::tempPath() + "/test_original.png";
		QImage testImage(100, 100, QImage::Format_RGB32);
		testImage.fill(Qt::red);
		QVERIFY(testImage.save(originalImagePath, "PNG"));
		QString tempStoragePath = ImageStorage::SaveTemporary(originalImagePath);
		QFile::remove(originalImagePath);

		auto imageShape = QSharedPointer<Shape>::create(Shape::Type::Image, QRectF(10, 20, 150, 120), 2);
		imageShape->SetImagePath(tempStoragePath);
		m_document->AddShapes({ imageShape });

		QTemporaryFile docFile;
		docFile.setFileTemplate(QDir::tempPath() + "/test_doc_XXXXXX.json");
		QVERIFY(docFile.open());
		QString docFilePath = docFile.fileName();
		docFile.close();

		JsonDocumentRepository::SaveToFile(m_document, docFilePath);

		QVERIFY(QFile::exists(docFilePath));

		QFileInfo docFileInfo(docFilePath);
		QString imagesDir = docFileInfo.absolutePath() + "/" + docFileInfo.completeBaseName() + "_images";
		QVERIFY(QDir(imagesDir).exists());

		QDir imagesDirObj(imagesDir);
		QStringList imageFiles = imagesDirObj.entryList(QDir::Files);
		QCOMPARE(imageFiles.size(), 1);

		QList<QSharedPointer<Shape>> loadedShapes = JsonDocumentRepository::LoadFromFile(docFilePath);

		QCOMPARE(loadedShapes.size(), 1);
		QCOMPARE(loadedShapes[0]->GetType(), Shape::Type::Image);
		QCOMPARE(loadedShapes[0]->GetRect(), QRectF(10, 20, 150, 120));
		QCOMPARE(loadedShapes[0]->GetLayer(), 2);

		QString loadedImagePath = loadedShapes[0]->GetImagePath();
		QVERIFY(loadedImagePath.contains("GeometryDrawer_Images"));
		QVERIFY(QFile::exists(loadedImagePath));

		QImage loadedImage(loadedImagePath);
		QVERIFY(!loadedImage.isNull());
		QCOMPARE(loadedImage.size(), QSize(100, 100));

		QFile::remove(docFilePath);
		QDir(imagesDir).removeRecursively();
	}
};

int main(int argc, char* argv[])
{
	QCoreApplication app(argc, argv);

	QLoggingCategory::setFilterRules("*.debug=false\n*.info=false");

	int status = 0;

	{
		DocumentTests tc;
		status |= QTest::qExec(&tc, argc, argv);
	}

	{
		CommandsIntegrationTests tc;
		status |= QTest::qExec(&tc, argc, argv);
	}

	{
		DocumentRepositoryTests tc;
		status |= QTest::qExec(&tc, argc, argv);
	}

	return status;
}

// Включаем moc файл для тестов
#include "GeometryDrawerTests.moc"