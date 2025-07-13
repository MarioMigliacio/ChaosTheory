// ============================================================================
//  File        : UIFillableGauge.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-06-18
//  Description : Visual bar UI element that fills based on a value
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "UIFillableGauge.h"
#include "UIFactory.h"

/// @brief Constants that can be adjusted throughout the UIFillableGauge.
namespace
{
/// @brief Constant for UI TextLabel font size.
constexpr auto TEXT_LABEL_INITIAL_VALUE = "100%";

/// @brief Very slight nudge to offset on horizontal layout mode for percentage label.
constexpr float PERCENTAGE_LABEL_HORIZONTAL_OFFSET_CORRECTION = 1.f;

/// @brief Very slight nudge to offset on vertical layout mode for percentage label.
constexpr float PERCENTAGE_LABEL_VERTICAL_OFFSET_CORRECTION = 2.f;
} // namespace

/// @brief Constructs a UIFillableGauge component.
/// @param position Coordinates to construct at.
/// @param size Size of the UI element.
UIFillableGauge::UIFillableGauge(const sf::Vector2f &position, const sf::Vector2f &size)
    : m_position(position), m_size(size)
{
    m_backgroundBar.setPosition(position);
    m_backgroundBar.setSize(size);
    m_backgroundBar.setFillColor(m_barBackgroundColor);

    m_fillBar.setPosition(position);
    m_fillBar.setSize({size.x * m_value, size.y});
    m_fillBar.setFillColor(m_barFillColor);
}

/// @brief Performs internal state management during a single frame.
/// @param mousePosition Currently traced mouse position.
/// @param isMousePressed IsMousePressed?
/// @param isMouseJustPressed IsMouseJustPressed?
/// @param dt delta time since last update.
void UIFillableGauge::Update(const sf::Vector2i &mousePosition, bool isMousePressed, bool isMouseJustPressed, float dt)
{
}

/// @brief Returns whether or not the point is bound inside this FillableGauge.
/// @param point Point to compare against us.
/// @return true / false
bool UIFillableGauge::Contains(const sf::Vector2i &point) const
{
    return m_backgroundBar.getGlobalBounds().contains(static_cast<sf::Vector2f>(point));
}

/// @brief Sets the position for this FillableGauge.
/// @param position Sets position for container and its children.
void UIFillableGauge::SetPosition(const sf::Vector2f &position)
{
    m_position = position;
    sf::Vector2f barPos = position;

    if (m_showTitle && m_titleLabel)
    {
        const auto labelSize = m_titleLabel->GetSize();

        if (m_titlePosition == GaugeTitlePosition::Above)
        {
            float labelY = position.y;
            float barY = labelY + labelSize.y + m_titlePadding;

            m_titleLabel->SetPosition({position.x + (m_size.x / 2.f), labelY});
            barPos.y = barY;
        }

        else if (m_titlePosition == GaugeTitlePosition::Left)
        {
            float labelX = position.x;
            float labelY = position.y + (m_size.y / 2.f) - (labelSize.y / 2.f);
            float barX = labelX + labelSize.x + m_titlePadding;

            m_titleLabel->SetPosition({labelX, labelY});
            barPos.x = barX;
        }
    }

    m_backgroundBar.setPosition(barPos);
    m_fillBar.setPosition(barPos);
    m_border.setPosition(barPos);

    if (m_percentageLabel)
    {
        float centerX = m_backgroundBar.getPosition().x + (m_size.x / 2.f);
        float centerY = m_backgroundBar.getPosition().y + (m_size.y / 2.f);

        if (m_orientation == LayoutMode::Horizontal)
        {
            centerY -= PERCENTAGE_LABEL_HORIZONTAL_OFFSET_CORRECTION; // Nudge up slightly for visual alignment
        }

        else // Vertical
        {
            centerX += PERCENTAGE_LABEL_VERTICAL_OFFSET_CORRECTION; // Nudge right slightly for better balance
        }

        m_percentageLabel->SetPosition({centerX, centerY});
    }
}

/// @brief Returns the position which this FillableGauge originates from.
/// @return m_position.
sf::Vector2f UIFillableGauge::GetPosition() const
{
    return m_position;
}

/// @brief Sets the internal size components for this FillableGauge.
/// @param size size to use for m_size, and starting point for m_totalSize.
void UIFillableGauge::SetSize(const sf::Vector2f &size)
{
    m_size = size;
    m_backgroundBar.setSize(size);
    m_fillBar.setSize({size.x * m_value, size.y});
    m_border.setSize(m_size);

    // Compute total size separately
    m_totalSize = size;

    if (m_showTitle && m_titleLabel)
    {
        const sf::Vector2f labelSize = m_titleLabel->GetSize();

        if (m_titlePosition == GaugeTitlePosition::Above)
        {
            m_totalSize.y += labelSize.y + m_titlePadding;
        }

        else if (m_titlePosition == GaugeTitlePosition::Left)
        {
            m_totalSize.x += labelSize.x + m_titlePadding;
        }
    }

    UpdateFillVisual();
}

/// @brief Returns the internally tracked size for this FillableGauge.
/// @return m_totalSize.
sf::Vector2f UIFillableGauge::GetSize() const
{
    return m_totalSize;
}

/// @brief Sets the value which tracks position of the FillableGauge bar.
/// @param percentage m_value to be clamped.
void UIFillableGauge::SetValue(float percentage)
{
    m_value = std::clamp(percentage, 0.f, 1.f);
    UpdateFillVisual();

    if (m_percentageLabel)
    {
        int pct = static_cast<int>(m_value * 100.f);
        m_percentageLabel->SetText(std::to_string(pct) + "%");

        float centerX = m_backgroundBar.getPosition().x + (m_size.x / 2.f);
        float centerY = m_backgroundBar.getPosition().y + (m_size.y / 2.f);

        if (m_orientation == LayoutMode::Horizontal)
        {
            centerY -= PERCENTAGE_LABEL_HORIZONTAL_OFFSET_CORRECTION; // Nudge up slightly for visual alignment
        }

        else // Vertical
        {
            centerX += PERCENTAGE_LABEL_VERTICAL_OFFSET_CORRECTION; // Nudge right slightly for better balance
        }

        m_percentageLabel->SetPosition({centerX, centerY});
    }
}

/// @brief Allows for customizable preset color schemes, such as health, gas, mana etc.
/// @param scheme Enum type which holds preset supported color schemes.
void UIFillableGauge::SetScheme(GaugeColorScheme scheme)
{
    switch (scheme)
    {
        case GaugeColorScheme::Health:
            m_barBackgroundColor = DEFAULT_GAUGE_HEALTH_BACKGROUND_COLOR;
            m_barFillColor = DEFAULT_GAUGE_HEALTH_FILL_COLOR;
            break;

        case GaugeColorScheme::Gas:
            m_barBackgroundColor = DEFAULT_GAUGE_GAS_BACKGROUND_COLOR;
            m_barFillColor = DEFAULT_GAUGE_GAS_FILL_COLOR;
            break;

        case GaugeColorScheme::Mana:
            m_barBackgroundColor = DEFAULT_GAUGE_MANA_BACKGROUND_COLOR;
            m_barFillColor = DEFAULT_GAUGE_MANA_FILL_COLOR;
            break;

        case GaugeColorScheme::Default:
        default:
            m_barBackgroundColor = DEFAULT_GAUGE_BACKGROUND_COLOR;
            m_barFillColor = DEFAULT_GAUGE_FILL_COLOR;
            break;
    }

    m_fillBar.setFillColor(m_barFillColor);
    m_backgroundBar.setFillColor(m_barBackgroundColor);
}

/// @brief Sets the Vertical or horizontal type orientation for this FillableGauge.
/// @param orientation Enum type from base Element which supports horizontal or vertical layouts.
void UIFillableGauge::SetOrientation(LayoutMode orientation)
{
    m_orientation = orientation;
    UpdateFillVisual();
}

/// @brief Sets the border thickness and color for this FillableGauge.
/// @param thickness Pixel thickness to track for the border.
/// @param color Color for the Border.
void UIFillableGauge::SetBorder(float thickness, const sf::Color &color)
{
    m_drawBorder = thickness > 0.f;
    m_border.setSize(m_size);
    m_border.setPosition(m_position);
    m_border.setFillColor(sf::Color::Transparent);
    m_border.setOutlineThickness(thickness);
    m_border.setOutlineColor(color);
}

/// @brief Customizable setting to allow % value of m_value to be centered in the FillableGauge bar.
/// @param show true / false.
void UIFillableGauge::SetShowPercentage(bool show)
{
    m_showPercentage = show;

    if (show && !m_percentageLabel)
    {
        m_percentageLabel = UIFactory::Instance().CreateTextLabel(TextLabelConfig{
            .text = TEXT_LABEL_INITIAL_VALUE, .position = m_position, .fontSize = DEFAULT_GAUGE_PERCENT_FONT_SIZE});
        SetValue(m_value);
        SetPosition(m_position);
    }
}

/// @brief Customizable setting to allow a title to be displayed alongside the FillableGauge bar.
/// @param text Text to be displayed.
/// @param fontSize Size of font.
/// @param padding Space away from the fillable bar.
/// @param scheme Color scheme to adhere to.
/// @param position Enumeration representing Left, or Above of the FillableGauge.
void UIFillableGauge::SetShowTitleLabel(const std::string &text, unsigned int fontSize, float padding,
                                        UITextLabelScheme scheme, GaugeTitlePosition position)
{
    m_showTitle = true;
    m_titlePosition = position;
    m_titlePadding = padding;
    bool shouldCenterOrigin = position == GaugeTitlePosition::Above ? true : false;

    m_titleLabel = UIFactory::Instance().CreateTextLabel(TextLabelConfig{.text = text,
                                                                         .position = m_position,
                                                                         .fontSize = fontSize,
                                                                         .centerOrigin = shouldCenterOrigin,
                                                                         .scheme = scheme});

    SetSize(m_size);         // Recalculate m_totalSize
    SetPosition(m_position); // Update all layout
}

/// @brief Allows the Title Label to be adjusted dynamically.
/// @param text Text to use for Title label.
void UIFillableGauge::SetTitleLabel(const std::string &text)
{
    if (m_showTitle)
    {
        m_titleLabel->SetText(text);
        SetPosition(m_position);
    }
}

/// @brief Draw this FillableGauge to the Renderable Target.
/// @param target render target.
/// @param states optional sf::RenderStates.
void UIFillableGauge::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_backgroundBar, states);
    target.draw(m_fillBar, states);

    if (m_drawBorder)
    {
        target.draw(m_border, states);
    }

    if (m_showPercentage && m_percentageLabel)
    {
        target.draw(*m_percentageLabel, states);
    }

    if (m_showTitle && m_titleLabel)
    {
        target.draw(*m_titleLabel, states);
    }
}

/// @brief Internal helper method to adjust the Fillbar against the Background bar using m_value.
void UIFillableGauge::UpdateFillVisual()
{
    sf::Vector2f fillSize = m_size;
    sf::Vector2f fillPos = m_backgroundBar.getPosition();

    if (m_orientation == LayoutMode::Horizontal)
    {
        fillSize.x *= m_value;
    }
    else // Vertical — fill from bottom upwards
    {
        fillSize.y *= m_value;
        fillPos.y += (m_size.y - fillSize.y); // shift fillBar upward
    }

    m_fillBar.setSize(fillSize);
    m_fillBar.setPosition(fillPos);
}
