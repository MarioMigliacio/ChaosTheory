// ============================================================================
//  File        : UIHUDPanel.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-06-12
//  Description : Represents a UI containter holding Heads Up Display entities
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "UIHUDPanel.h"

UIHUDPanel::UIHUDPanel(const sf::Vector2f &position, const sf::Vector2f &size) : m_position(position), m_size(size)
{
    m_background.setPosition(position);
    m_background.setSize(size);
    m_background.setFillColor(sf::Color(20, 20, 20, 180)); // TODO: theme
    m_background.setOutlineColor(sf::Color::Transparent);  // TODO:
    m_background.setOutlineThickness(0.f);                 // TODO:
}

void UIHUDPanel::Update(const sf::Vector2i &mousePosition, bool isMousePressed, bool isMouseJustPressed, float dt)
{
    for (auto &element : m_children)
    {
        if (element->IsEnabled())
        {
            element->Update(mousePosition, isMousePressed, isMouseJustPressed, dt);
        }
    }
}

bool UIHUDPanel::Contains(const sf::Vector2i &point) const
{
    return m_background.getGlobalBounds().contains(static_cast<sf::Vector2f>(point));
}

void UIHUDPanel::SetPosition(const sf::Vector2f &position)
{
    m_position = position;
    m_background.setPosition(position);
    RealignChildren();
}

sf::Vector2f UIHUDPanel::GetPosition() const
{
    return m_position;
}

void UIHUDPanel::SetSize(const sf::Vector2f &size)
{
    m_size = size;
    m_background.setSize(size);
    RealignChildren();
}

sf::Vector2f UIHUDPanel::GetSize() const
{
    return m_size;
}

void UIHUDPanel::SetFillColor(const sf::Color &color)
{
    m_background.setFillColor(color);
}

void UIHUDPanel::SetOutlineColor(const sf::Color &color)
{
    m_background.setOutlineColor(color);
}

void UIHUDPanel::SetOutlineThickness(float thickness)
{
    m_background.setOutlineThickness(thickness);
}

void UIHUDPanel::AddElement(std::shared_ptr<UIElement> element, HUDSlotAlignment alignment)
{
    if (!element)
    {
        return;
    }

    m_children.push_back(element);

    if (alignment == HUDSlotAlignment::Left)
    {
        m_leftAnchored.push_back(element);
    }

    else
    {
        m_rightAnchored.push_back(element);
    }

    RealignChildren();
}

const std::vector<std::shared_ptr<UIElement>> &UIHUDPanel::GetChildren() const
{
    return m_children;
}

void UIHUDPanel::SetLayoutMode(LayoutMode mode)
{
    m_layoutMode = mode;
    RealignChildren();
}

void UIHUDPanel::SetCenterChildren(bool center)
{
    m_centerChildren = center;
    RealignChildren();
}

void UIHUDPanel::SetInternalPadding(float padding)
{
    m_internalPadding = padding;
    RealignChildren();
}

void UIHUDPanel::SetEdgePadding(float padding)
{
    m_edgePadding = padding;
    RealignChildren();
}

void UIHUDPanel::RealignChildren()
{
    float leftCursor = m_position.x + m_edgePadding;
    float rightCursor = m_position.x + m_size.x - m_edgePadding;

    float centerY = m_position.y + (m_size.y / 2.f);

    // Left-aligned elements (render left-to-right)
    for (const auto &element : m_leftAnchored)
    {
        const auto size = element->GetSize();
        sf::Vector2f pos{leftCursor, centerY - (size.y / 2.f)};
        element->SetPosition(pos);
        leftCursor += size.x + m_internalPadding;
    }

    // Right-aligned elements (render right-to-left)
    for (auto it = m_rightAnchored.rbegin(); it != m_rightAnchored.rend(); ++it)
    {
        const auto &element = *it;
        const auto size = element->GetSize();
        rightCursor -= size.x;
        sf::Vector2f pos{rightCursor, centerY - (size.y / 2.f)};
        element->SetPosition(pos);
        rightCursor -= m_internalPadding;
    }

    // Center logic is optional, skipped for now
}

void UIHUDPanel::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_background, states);

    for (const auto &child : m_children)
    {
        if (child && child->IsEnabled())
        {
            target.draw(*child, states);
        }
    }
}