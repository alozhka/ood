#pragma once

#include "../draw/ICanvas.h"
#include "../shapes/Picture.h"

#include <atomic>
#include <functional>
#include <map>
#include <mutex>
#include <queue>
#include <thread>

class ShapesController
{
public:
	ShapesController(
		shapes::Picture& picture,
		gfx::ICanvas& canvas,
		std::istream& input,
		std::ostream& output);
	void ProcessCommand();
	void StartInputThread();
	void StopInputThread();

	~ShapesController();

private:
	void ProcessAddShape(std::istream& input);
	void ProcessMoveShape(std::istream& input);
	void ProcessDeleteShape(std::istream& input);
	void ProcessMovePicture(std::istream& input);
	void ProcessList(std::istream& input) const;
	void ProcessChangeColor(std::istream& input);
	void ProcessChangeShape(std::istream& input);
	void ProcessDrawShape(std::istream& input);
	void ProcessDrawPicture(std::istream& input);
	// TODO:
	// void ProcessCloneShape(const std::vector<std::string>& tokens);

	void InputThreadFunction();

	using ActionMap = std::map<std::string, std::function<void(std::istream&)>>;

	ActionMap m_actions;
	shapes::Picture& m_picture;
	gfx::ICanvas& m_canvas;
	std::istream& m_input;
	std::ostream& m_output;

	// Отдельный поток под считывание
	std::queue<std::string> m_commandQueue{};
	std::mutex m_queueMutex{};
	std::atomic<bool> m_threadStopped{ false };
	std::thread m_inputThread;
};