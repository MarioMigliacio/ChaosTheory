// ============================================================================
//  File        : UIGroupBox.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-05-07
//  Description : Represents a UI rect containter for many use cases
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "UIGroupBox.h"
#include "Macros.h"

/// @brief Constructor for the UIGroupBox.
/// @param position Position to set this GroupBox.
/// @param size Size this GroupBox occupies.
UIGroupBox::UIGroupBox(const sf::Vector2f &position, const sf::Vector2f &size)
{
    m_background.setPosition(position);
    m_background.setSize(size);
    m_background.setFillColor(DEFAULT_GROUPBOX_FILL_COLOR);
    m_background.setOutlineColor(DEFAULT_GROUPBOX_OUTLINE_COLOR);
    m_background.setOutlineThickness(DEFAULT_GROUPBOX_OUTLINE_THICKNESS);

    CT_LOG_INFO("UIGroupBox created with position: {}x{}, size: {}x{}.", position.x, position.y, size.x, size.y);
}

/// @brief Set the title corresponding to this GroupBox.
/// @param title String title.
/// @param font Font loaded.
/// @param fontSize Font size.
void UIGroupBox::SetTitle(const std::string &title, const sf::Font &font, unsigned int fontSize)
{
    m_title.setFont(font);
    m_title.setString(title);
    m_title.setCharacterSize(fontSize);
    m_title.setFillColor(sf::Color::White);

    const auto bounds = m_title.getLocalBounds();
    m_title.setOrigin(bounds.left, bounds.top);
    m_title.setPosition(m_background.getPosition().x + 10.f, m_background.getPosition().y - bounds.height - 5.f);

    CT_LOG_INFO("UIGroupBox SetTitle: {}.", title);
}

/// @brief Add the UIElement to the container owned by this GroupBox.
/// @param element Any type of UIElement.
void UIGroupBox::AddElement(std::shared_ptr<UIElement> element)
{
    m_children.push_back(std::move(element));
    RealignChildren();
}

/// @brief Force the children entities to readjust, useful for resizing.
void UIGroupBox::RealignChildren()
{
    const sf::Vector2f basePos = m_background.getPosition();
    sf::Vector2f currentPos = {basePos.x + m_edgePadding, basePos.y + m_edgePadding + 20.f};

    for (auto &child : m_children)
    {
        sf::Vector2f childSize = child->GetSize();

        if (m_layoutMode == LayoutMode::Vertical)
        {
            float x = m_centerChildren ? basePos.x + (m_background.getSize().x - childSize.x) / 2.f
                                       : basePos.x + m_edgePadding;

            child->SetSize(childSize);
            child->SetPosition({x, currentPos.y});
            currentPos.y += childSize.y + m_internalPadding;
        }

        else
        {
            float y = m_centerChildren ? basePos.y + (m_background.getSize().y - childSize.y) / 2.f
                                       : basePos.y + m_edgePadding;

            child->SetPosition({currentPos.x, y});
            currentPos.x += childSize.x + m_internalPadding;
        }
    }
}

/// @brief Performs internal state management during a single frame.
/// @param mousePosition Currently traced mouse position.
/// @param isMousePressed IsMousePressed?
/// @param isMouseJustPressed IsMouseJustPressed?
/// @param dt delta time since last update.
void UIGroupBox::Update(const sf::Vector2i &mousePosition, bool isMousePressed, bool isMouseJustPressed, float dt)
{
    if (!IsEnabled())
    {
        return;
    }

    for (auto &child : m_children)
    {
        child->Update(mousePosition, isMousePressed, isMouseJustPressed, dt);
    }
}

/// @brief Returns whether or not the point is bound inside this GroupBox.
/// @param point Point to compare against us.
/// @return true / false
bool UIGroupBox::Contains(const sf::Vector2i &point) const
{
    return m_background.getGlobalBounds().contains(static_cast<sf::Vector2f>(point));
}

/// @brief Return a collection of UIElements tracked by this GroupBox.
/// @return m_children.
const std::vector<std::shared_ptr<UIElement>> &UIGroupBox::GetChildren() const
{
    return m_children;
}

/// @brief Sets the position for this GroupBox.
/// @param position Sets position for container and its children.
void UIGroupBox::SetPosition(const sf::Vector2f &position)
{
    sf::Vector2f offset = position - m_background.getPosition();
    m_background.setPosition(position);
    m_title.move(offset);

    for (auto &child : m_children)
    {
        child->SetPosition(child->GetPosition() + offset);
    }
}

/// @brief Gets the position for this UIGroupBox.
/// @return m_background.
sf::Vector2f UIGroupBox::GetPosition() const
{
    return m_background.getPosition();
}

/// @brief Sets the size for this GroupBox.
/// @param size new m_background.size.
void UIGroupBox::SetSize(const sf::Vector2f &size)
{
    m_background.setSize(size);
}

/// @brief Gets the size for this UIGroupBox.
/// @return m_background.size.
sf::Vector2f UIGroupBox::GetSize() const
{
    return m_background.getSize();
}

/// @brief Sets the Layout mode for this Groupbox.
/// @param mode Verticle / Horizontal
void UIGroupBox::SetLayoutMode(LayoutMode mode)
{
    m_layoutMode = mode;
    RealignChildren();
}

/// @brief Alignment to center for children.
/// @param center true / false
void UIGroupBox::SetCenterChildren(bool center)
{
    m_centerChildren = center;
    RealignChildren();
}

/// @brief Sets the fill color for this GroupBox.
/// @param color new m_background.FillColor
void UIGroupBox::SetFillColor(const sf::Color &color)
{
    m_background.setFillColor(color);
}

/// @brief Sets the outline color for this GroupBox.
/// @param color new m_background.OutlineColor
void UIGroupBox::SetOutlineColor(const sf::Color &color)
{
    m_background.setOutlineColor(color);
}

/// @brief Sets the outlinle thickness for this GroupBox.
/// @param thickness new m_background.OutlineThickness
void UIGroupBox::SetOutlineThickness(float thickness)
{
    m_background.setOutlineThickness(thickness);
}

/// @brief Sets the internal padding for this GroupBox.
/// @param padding new m_internalPadding.
void UIGroupBox::SetInternalPadding(float padding)
{
    m_internalPadding = padding;
    RealignChildren();
}

/// @brief Sets the Edge Padding for this GroupBox.
/// @param padding new m_edgePadding.
void UIGroupBox::SetEdgePadding(float padding)
{
    m_edgePadding = padding;
    RealignChildren();
}

/// @brief Draw this UIGroupBox to the Renderable Target.
/// @param target render target.
/// @param states optional sf::RenderStates.
void UIGroupBox::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_background, states);
    target.draw(m_title, states);

    for (const auto &child : m_children)
    {
        target.draw(*child, states);
    }
}
