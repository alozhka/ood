#pragma once

#include <vector>

#include "../draw/ICanvas.h"
#include "../shapes/Picture.h"

#include <functional>
#include <map>

class ShapesController
{
public:
	ShapesController(
		shapes::Picture& picture,
		gfx::ICanvas& canvas,
		std::istream& input,
		std::ostream& output);
	void ProcessCommand();

private:
	void ProcessAddShape(std::istream& input);
	void ProcessMoveShape(std::istream& input);
	void ProcessDeleteShape(std::istream& input);
	void ProcessMovePicture(std::istream& input);
	void ProcessList() const;
	void ProcessChangeColor(std::istream& input);
	void ProcessChangeShape(std::istream& input);
	void ProcessDrawShape(std::istream& input);
	void ProcessDrawPicture(std::istream& input);
	// TODO:
	// void ProcessCloneShape(const std::vector<std::string>& tokens);

	using ActionMap = std::map<std::string, std::function<void(std::istream&)>>;

	ActionMap m_actions;
	shapes::Picture& m_picture;
	gfx::ICanvas& m_canvas;
	std::istream& m_input;
	std::ostream& m_output;
};