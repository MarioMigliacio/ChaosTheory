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
#include "ResolutionScaleManager.h"
#include "UIFactory.h"

/// @brief Constants that can be adjusted throughout the UIGroupBox.
namespace
{
/// @brief Default alignment padding for UITextlabel.
constexpr float TITLE_PAD_X = 10.f;

/// @brief Default alignment padding for UITextLabel.
constexpr float TITLE_PAD_Y = 5.f;

/// @brief During alignment, account for Title if it exists.
constexpr float PADDING_OFFSET = 20.f;
} // namespace

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
/// @param centerOrigin Whether or not to center the label text around position.
/// @param scheme Supported Color Scheme for UITextLabel.
void UIGroupBox::SetTitle(const std::string &title, const sf::Font &font, unsigned int fontSize, bool centerOrigin,
                          UITextLabelScheme scheme)
{
    // Compute default title position just above box
    const sf::Vector2f groupBoxPos = m_background.getPosition();
    const float paddingY = ResolutionScaleManager::Instance().ScaleY(TITLE_PAD_Y);
    const sf::Vector2f pos = {0.f, 0.f};

    m_titleLabel = UIFactory::Instance().CreateTextLabel(TextLabelConfig{
        .text = title, .position = pos, .fontSize = fontSize, .centerOrigin = centerOrigin, .scheme = scheme});
    sf::Vector2f textSize = m_titleLabel->GetSize();

    // Compute anchor X
    float anchorX = groupBoxPos.x + m_edgePadding;
    if (centerOrigin)
    {
        anchorX = groupBoxPos.x + (m_background.getSize().x / 2);
    }

    const float anchorY = groupBoxPos.y - textSize.y - paddingY;

    m_titleLabel->SetPosition(sf::Vector2f{anchorX, anchorY});

    if (centerOrigin)
    {
        m_titleLabel->CenterOrigin();
    }

    CT_LOG_INFO("UIGroupBox SetTitle: {}.", title);
}

/// @brief Provides access to the UITextLabel for altering its color scheme.
/// @param scheme Enum field for the type of label layout.
void UIGroupBox::SetTitleScheme(UITextLabelScheme scheme)
{
    const float scaledOutline = ResolutionScaleManager::Instance().ScaleX(DEFAULT_TEXT_LABEL_BORDER_THICKNESS);

    m_titleLabel->ApplyTextLabelStyle(scheme, scaledOutline);
}

/// @brief Add the UIElement to the container owned by this GroupBox.
/// @param element Any type of UIElement.
void UIGroupBox::AddElement(std::shared_ptr<UIElement> element)
{
    m_children.push_back(std::move(element));
    RealignChildren();
}

/// @brief Remove the desired element from the collection of child elements.
/// @param element record to be removed from m_children.
void UIGroupBox::RemoveElement(const std::shared_ptr<UIElement> &element)
{
    auto it = std::find(m_children.begin(), m_children.end(), element);

    if (it != m_children.end())
    {
        m_children.erase(it);
        RealignChildren();
    }
}

/// @brief Force the children entities to readjust, useful for resizing.
void UIGroupBox::RealignChildren()
{
    const sf::Vector2f basePos = m_background.getPosition();
    sf::Vector2f currentPos = {basePos.x + m_edgePadding, basePos.y + m_edgePadding};

    if (m_titleLabel)
    {
        currentPos.y += PADDING_OFFSET;
    }

    for (auto &child : m_children)
    {
        sf::Vector2f childSize = child->GetSize();

        if (m_layoutMode == LayoutMode::Vertical)
        {
            float x = m_centerChildren ? basePos.x + (m_background.getSize().x - childSize.x) / 2.f
                                       : basePos.x + m_edgePadding;

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

/// @brief Remove all existing children in this GroupBox.
void UIGroupBox::ClearChildren()
{
    m_children.clear();
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

    if (m_titleLabel)
    {
        m_titleLabel->SetPosition(m_titleLabel->GetPosition() + offset);
    }

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
    RealignChildren();
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

    if (m_titleLabel)
    {
        target.draw(*m_titleLabel, states);
    }

    for (const auto &child : m_children)
    {
        target.draw(*child, states);
    }
}
