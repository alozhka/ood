#pragma once
#include <QObject>
#include <memory>

class TextViewModel : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString inputText READ GetInputText WRITE SetInputText NOTIFY InputTextChanged)
	Q_PROPERTY(QString displayText READ GetDisplayText NOTIFY DisplayTextChanged)

public:
	explicit TextViewModel(QObject* parent = nullptr)
		: QObject(parent)
		, m_model(std::make_unique<TextModel>(this))
	{
		connect(
			m_model.get(),
			&TextModel::ContentChanged,
			this,
			&TextViewModel::DisplayTextChanged);
	}

	QString GetInputText() const
	{
		return m_inputText;
	}

	void SetInputText(const QString& text)
	{
		m_inputText = text;
		emit InputTextChanged();
	}

	QString GetDisplayText() const
	{
		QString content = m_model->GetContent();
		return content.isEmpty() ? "Ничего не сохранено" : "Сохранено: " + content;
	}

public slots:
	void SaveText()
	{
		m_model->SetContent(m_inputText);
	}

signals:
	void InputTextChanged();
	void DisplayTextChanged();

private:
	QString m_inputText{ "" };
	std::unique_ptr<TextModel> m_model;
};