

#pragma once

#include "UIElement.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>

enum class LayoutMode
{
    Vertical,
    Horizontal
};

class GroupBox : public UIElement
{
  public:
    explicit GroupBox(const sf::Vector2f &position, const sf::Vector2f &size);
    ~GroupBox() override = default;

    void SetTitle(const std::string &title, const sf::Font &font, unsigned int fontSize = 18);
    void AddElement(std::shared_ptr<UIElement> element);
    void RealignChildren();

    void Update(const sf::Vector2i &mousePosition, bool isMousePressed) override;
    bool Contains(const sf::Vector2i &point) const override;
    const std::vector<std::shared_ptr<UIElement>> &GetChildren() const;

    void SetPosition(const sf::Vector2f &position) override;
    sf::Vector2f GetPosition() const override;
    void SetSize(const sf::Vector2f &size) override;
    sf::Vector2f GetSize() const override;

    void SetLayoutMode(LayoutMode mode);
    void SetCenterChildren(bool center);

    void SetFillColor(const sf::Color &color);
    void SetOutlineColor(const sf::Color &color);
    void SetOutlineThickness(float thickness);

    void SetInternalPadding(float padding);
    void SetEdgePadding(float padding);

  private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

  private:
    sf::RectangleShape m_background;
    sf::Text m_title;

    std::vector<std::shared_ptr<UIElement>> m_children;
    LayoutMode m_layoutMode = LayoutMode::Vertical;

    float m_internalPadding = 12.f;
    float m_edgePadding = 10.f;

    bool m_centerChildren = false;
};
