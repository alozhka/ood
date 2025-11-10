#pragma once
#include "Menu.h"

#include <istream>

#include "../shape/factory/ShapesFactory.h"
#include "../slide/ISlide.h"
#include "../slide/Slide.h"

class CommandController
{
public:
    CommandController(std::istream& input, std::ostream& output)
        : m_menu{input, output}
          , m_input{input}
          , m_output{output}
    {
        m_menu.AddItem(
            "List",
            "Usage: List. Lists shapes",
            [this](std::istream&) { List(); });
        m_menu.AddItem(
            "InsertShape",
            "Usage: InsertShape <shapeType> <params>. Inserts shape into slide",
            std::bind_front(&CommandController::InsertShape, this));
    }

    void Run()
    {
        m_menu.Run();
    }

private:
    void InsertShape(std::istream& input)
    {
        std::string type;
        if (!(input >> type))
        {
            throw std::invalid_argument("Shape type is not specified");
        }

        std::shared_ptr<IShape> shape = m_shapesFactory->Create(type, input);
        m_slide->AddShape(shape);
    }

    void List() const
    {
        for (size_t i = 0; i < m_slide->GetShapesCount(); ++i)
        {
            std::shared_ptr<IShape> shape = m_slide->GetShapeAt(i);
            m_output << std::format("{}. Type: {}; Params: {}\n",
                                    i + 1,
                                    shape->GetType(),
                                    shape->ListParams());
        }
    }

    Menu m_menu;
    std::unique_ptr<ISlide> m_slide{std::make_unique<Slide>()};
    std::unique_ptr<IShapesFactory> m_shapesFactory{std::make_unique<ShapesFactory>()};

    std::istream& m_input;
    std::ostream& m_output;
};
