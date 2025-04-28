#include "RadioButton.h"
#include "AssetManager.h"
#include "Macros.h"

namespace
{
const sf::Color DEFAULT_IDLE_COLOR(200, 200, 200);
const sf::Color DEFAULT_HOVER_COLOR(160, 160, 255);
const sf::Color DEFAULT_SELECTED_COLOR(100, 180, 255);
const sf::Color DEFAULT_TEXT_COLOR(0, 0, 0);
const sf::Color DEFAULT_SELECTED_TEXT_COLOR(255, 255, 255);
} // namespace

RadioButton::RadioButton(const sf::Vector2f &position, const sf::Vector2f &size, const std::string &label,
                         std::function<void()> onSelect)
    : m_onSelect(std::move(onSelect))
{
    m_shape.setPosition(position);
    m_shape.setSize(size);

    m_label.setFont(AssetManager::Instance().GetFont("Default.ttf"));
    m_label.setString(label);
    m_label.setCharacterSize(24);
    m_label.setFillColor(DEFAULT_TEXT_COLOR);

    m_currentFillColor = DEFAULT_IDLE_COLOR;
    m_currentTextColor = DEFAULT_TEXT_COLOR;

    CenterLabel();
}

void RadioButton::SetSelected(bool selected)
{
    m_isSelected = selected;
}

bool RadioButton::IsSelected() const
{
    return m_isSelected;
}

void RadioButton::Update(const sf::Vector2i &mousePosition, bool isMousePressed)
{
    sf::Vector2f mouse(mousePosition);
    bool wasHovered = m_isHovered;

    m_isHovered = m_shape.getGlobalBounds().contains(mouse);

    if (m_isHovered && !wasHovered)
    {
        CT_LOG_DEBUG("RadioButton hovered.");
    }

    else if (!m_isHovered && wasHovered)
    {
        CT_LOG_DEBUG("RadioButton unhovered.");
    }

    HandleClickLogic(isMousePressed);
    UpdateVisualState();
}

bool RadioButton::Contains(const sf::Vector2i &point) const
{
    return m_shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(point));
}

void RadioButton::SetPosition(const sf::Vector2f &position)
{
    m_shape.setPosition(position);
    CenterLabel();
}

void RadioButton::SetSize(const sf::Vector2f &size)
{
    m_shape.setSize(size);
    CenterLabel();
}

void RadioButton::CenterLabel()
{
    sf::FloatRect textRect = m_label.getLocalBounds();
    m_label.setOrigin(textRect.left + textRect.width / 2.f, textRect.top + textRect.height / 2.f);

    m_label.setPosition(m_shape.getPosition().x + m_shape.getSize().x / 2.f,
                        m_shape.getPosition().y + m_shape.getSize().y / 2.f);
}

void RadioButton::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::RectangleShape drawableShape = m_shape;
    drawableShape.setFillColor(m_currentFillColor);

    sf::Text drawableText = m_label;
    drawableText.setFillColor(m_currentTextColor);

    target.draw(drawableShape, states);
    target.draw(drawableText, states);
}

void RadioButton::HandleClickLogic(bool isMousePressed)
{
    if (m_isHovered && isMousePressed)
    {
        if (m_onSelect)
        {
            m_onSelect();
        }
    }
}

void RadioButton::UpdateVisualState()
{
    if (m_isSelected)
    {
        m_currentFillColor = DEFAULT_SELECTED_COLOR;
        m_currentTextColor = DEFAULT_SELECTED_TEXT_COLOR;
    }

    else if (m_isHovered)
    {
        m_currentFillColor = DEFAULT_HOVER_COLOR;
        m_currentTextColor = DEFAULT_TEXT_COLOR;
    }

    else
    {
        m_currentFillColor = DEFAULT_IDLE_COLOR;
        m_currentTextColor = DEFAULT_TEXT_COLOR;
    }
}