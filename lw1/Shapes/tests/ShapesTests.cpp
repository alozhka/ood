#include "../src/commands/ShapesController.h"

#include <gtest/gtest.h>

class ShapesTests : public testing::Test
{
protected:
	shapes::Picture m_picture{};
	ShapesController m_controller;
};
