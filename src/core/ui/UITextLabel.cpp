

#include "UITextLabel.h"

UITextLabel::UITextLabel(const std::string &text, const sf::Font &font, unsigned int fontSize,
                         const sf::Vector2f &position)
{
    m_text.setFont(font);
    m_text.setString(text);
    m_text.setCharacterSize(fontSize);
    m_text.setFillColor(sf::Color::White);
    m_text.setPosition(position);
    CenterOrigin();
}

void UITextLabel::SetText(const std::string &text)
{
    m_text.setString(text);
    CenterOrigin();
}

void UITextLabel::SetFont(const sf::Font &font)
{
    m_text.setFont(font);
    CenterOrigin();
}

void UITextLabel::SetFontSize(unsigned int size)
{
    m_text.setCharacterSize(size);
    CenterOrigin();
}

void UITextLabel::SetColor(const sf::Color &color)
{
    m_text.setFillColor(color);
}

void UITextLabel::SetOutline(float thickness, const sf::Color &color)
{
    m_text.setOutlineThickness(thickness);
    m_text.setOutlineColor(color);
}

void UITextLabel::SetPosition(const sf::Vector2f &position)
{
    m_text.setPosition(position);
}

sf::Vector2f UITextLabel::GetPosition() const
{
    return m_text.getPosition();
}

void UITextLabel::SetSize(const sf::Vector2f &)
{
}

sf::Vector2f UITextLabel::GetSize() const
{
    auto bounds = m_text.getLocalBounds();
    return {bounds.width, bounds.height};
}

void UITextLabel::CenterOrigin()
{
    auto bounds = m_text.getLocalBounds();
    m_text.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
}

void UITextLabel::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_text, states);
}
