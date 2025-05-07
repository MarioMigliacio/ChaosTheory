

#pragma once

#include "UIElement.h"
#include <SFML/Graphics.hpp>
#include <string>

class UITextLabel : public UIElement
{
  public:
    UITextLabel(const std::string &text, const sf::Font &font, unsigned int fontSize,
                const sf::Vector2f &position = {0.f, 0.f});
    ~UITextLabel() = default;

    // Disable copy
    UITextLabel(const UITextLabel &) = delete;
    UITextLabel &operator=(const UITextLabel &) = delete;

    // Allow move
    UITextLabel(UITextLabel &&) noexcept = default;
    UITextLabel &operator=(UITextLabel &&) noexcept = default;

    void SetText(const std::string &text);
    void SetFont(const sf::Font &font);
    void SetFontSize(unsigned int size);
    void SetColor(const sf::Color &color);
    void SetOutline(float thickness, const sf::Color &color);

    void SetPosition(const sf::Vector2f &position) override;
    sf::Vector2f GetPosition() const override;

    void SetSize(const sf::Vector2f &size) override; // unused, for interface
    sf::Vector2f GetSize() const override;

    void Update(const sf::Vector2i &, bool) override
    {
    }
    bool Contains(const sf::Vector2i &) const override
    {
        return false;
    }

  private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

  private:
    void CenterOrigin();

    sf::Text m_text;
};
