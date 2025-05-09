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

UIGroupBox::UIGroupBox(const sf::Vector2f &position, const sf::Vector2f &size)
{
    m_background.setPosition(position);
    m_background.setSize(size);
    m_background.setFillColor(sf::Color(25, 25, 25, 160));       // dark charcoal, translucent
    m_background.setOutlineColor(sf::Color(150, 255, 150, 200)); // greenish white glow
    m_background.setOutlineThickness(1.f);
}

void UIGroupBox::SetTitle(const std::string &title, const sf::Font &font, unsigned int fontSize)
{
    m_title.setFont(font);
    m_title.setString(title);
    m_title.setCharacterSize(fontSize);
    m_title.setFillColor(sf::Color::White);

    const auto bounds = m_title.getLocalBounds();
    m_title.setOrigin(bounds.left, bounds.top);
    m_title.setPosition(m_background.getPosition().x + 10.f, m_background.getPosition().y - bounds.height - 5.f);
}

void UIGroupBox::AddElement(std::shared_ptr<UIElement> element)
{
    m_children.push_back(std::move(element));
    RealignChildren();
}

// Force the children entities to readjust, useful for resizing.
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
        else // Horizontal layout
        {
            float y = m_centerChildren ? basePos.y + (m_background.getSize().y - childSize.y) / 2.f
                                       : basePos.y + m_edgePadding;

            child->SetPosition({currentPos.x, y});
            currentPos.x += childSize.x + m_internalPadding;
        }
    }
}

void UIGroupBox::Update(const sf::Vector2i &mousePosition, bool isMousePressed, float dt)
{
    if (!IsEnabled())
        return;

    for (auto &child : m_children)
        child->Update(mousePosition, isMousePressed, dt);
}

bool UIGroupBox::Contains(const sf::Vector2i &point) const
{
    return m_background.getGlobalBounds().contains(static_cast<sf::Vector2f>(point));
}

const std::vector<std::shared_ptr<UIElement>> &UIGroupBox::GetChildren() const
{
    return m_children;
}

void UIGroupBox::SetPosition(const sf::Vector2f &position)
{
    sf::Vector2f offset = position - m_background.getPosition();
    m_background.setPosition(position);
    m_title.move(offset);
    for (auto &child : m_children)
        child->SetPosition(child->GetPosition() + offset);
}

// Returns the position for this UIGroupBox.
sf::Vector2f UIGroupBox::GetPosition() const
{
    return m_background.getPosition();
}

void UIGroupBox::SetSize(const sf::Vector2f &size)
{
    m_background.setSize(size);
}

// Returns the size for this UIGroupBox.
sf::Vector2f UIGroupBox::GetSize() const
{
    return m_background.getSize();
}

void UIGroupBox::SetLayoutMode(LayoutMode mode)
{
    m_layoutMode = mode;
    RealignChildren();
}

void UIGroupBox::SetCenterChildren(bool center)
{
    m_centerChildren = center;
    RealignChildren();
}

void UIGroupBox::SetFillColor(const sf::Color &color)
{
    m_background.setFillColor(color);
}

void UIGroupBox::SetOutlineColor(const sf::Color &color)
{
    m_background.setOutlineColor(color);
}

void UIGroupBox::SetOutlineThickness(float thickness)
{
    m_background.setOutlineThickness(thickness);
}

void UIGroupBox::SetInternalPadding(float padding)
{
    m_internalPadding = padding;
    RealignChildren();
}

void UIGroupBox::SetEdgePadding(float padding)
{
    m_edgePadding = padding;
    RealignChildren();
}

void UIGroupBox::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_background, states);
    target.draw(m_title, states);
    for (const auto &child : m_children)
        target.draw(*child, states);
}
