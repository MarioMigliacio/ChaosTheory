// ============================================================================
//  File        : UIChatBox.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-06-28
//  Description : Represents a Chatbox UI element with typewriter effect
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "UIChatBox.h"
#include "AssetManager.h"
#include "Assets.h"
#include "AudioManager.h"
#include "Macros.h"
#include "ResolutionScaleManager.h"
#include "UIFactory.h"

/// @brief Constants that can be adjusted throughout the UIChatBox.
namespace
{
/// @brief Default size for speaker icon.
const sf::Vector2f ICON_SIZE = {128.f, 128.f};

/// @brief Default border size for the icon groupbox.
constexpr float ICON_BORDER_SIZE = 2.f;

/// @brief Default padding for icon centering.
constexpr float ICON_PADDING = 2.f;

/// @brief Constant to adjust for size of primary chatbox width.
constexpr int MAIN_GROUPBOX_MULTIPLIER_WIDTH = 6;

/// @brief Constant to adjust for size of primary chatbox height.
constexpr float MAIN_GROUPBOX_SCALAR_HEIGHT = 10.f;

/// @brief Constant to adjust for internal padding of primary chatbox.
constexpr float MAIN_GROUPBOX_SCALAR_INTERNAL_PADDING = 25.f;

/// @brief Constant to adjust for edge padding of primary chatbox.
constexpr float MAIN_GROUPBOX_SCALAR_EDGE_PADDING = 5.f;

} // namespace

/// @brief ChatBox constructor that initializes internal groupbox, optional speaker icon, and text label.
/// @param config ChatBoxConfig structure containing optional parameters.
UIChatBox::UIChatBox(const ChatBoxConfig &config) : m_typeSpeed(config.typeSpeed)
{
    auto &scaleMgr = ResolutionScaleManager::Instance();

    const float scaledBorderSize = scaleMgr.ScaleUniform(ICON_BORDER_SIZE);

    // Main container groupbox
    m_groupBox = UIFactory::Instance().CreateGroupBox(GroupBoxConfig{.position = config.position,
                                                                     .size = config.size,
                                                                     .useTitle = config.showTitle,
                                                                     .centerOrigin = false,
                                                                     .scheme = config.titleScheme});

    m_groupBox->SetSize(sf::Vector2f(
        scaleMgr.ScaleX((ICON_SIZE.x + scaledBorderSize * 2) * MAIN_GROUPBOX_MULTIPLIER_WIDTH),
        scaleMgr.ScaleY(ICON_SIZE.y + scaledBorderSize * 2) + scaleMgr.ScaleY(MAIN_GROUPBOX_SCALAR_HEIGHT)));
    m_groupBox->SetInternalPadding(scaleMgr.ScaleX(MAIN_GROUPBOX_SCALAR_INTERNAL_PADDING));
    m_groupBox->SetEdgePadding(scaleMgr.ScaleX(MAIN_GROUPBOX_SCALAR_EDGE_PADDING));
    m_groupBox->SetLayoutMode(LayoutMode::Horizontal);
    m_groupBox->SetCenterChildren(false);

    // optional speaker icon entity
    if (config.useSpeakerIcon && !config.iconTextureKey.empty())
    {
        GroupBoxConfig iconGbCfg{.position = sf::Vector2f(0.f, 0.f),
                                 .size = sf::Vector2f(0.f, 0.f),
                                 .useTitle = false,
                                 .title = std::string(),
                                 .centerOrigin = false,
                                 .scheme = UITextLabelScheme::DefaultScheme};

        auto iconGroupBox = UIFactory::Instance().CreateGroupBox(iconGbCfg);

        iconGroupBox->SetSize(sf::Vector2f(scaleMgr.ScaleX(ICON_SIZE.x + scaledBorderSize * 2),
                                           scaleMgr.ScaleY(ICON_SIZE.y + scaledBorderSize * 2)));
        iconGroupBox->SetInternalPadding(ICON_PADDING);
        iconGroupBox->SetEdgePadding(ICON_PADDING);
        iconGroupBox->SetLayoutMode(LayoutMode::Vertical);
        iconGroupBox->SetCenterChildren(true);
        iconGroupBox->SetOutlineColor(GAUGE_BORDER_COLOR_GOLD);
        iconGroupBox->SetOutlineThickness(scaleMgr.ScaleUniform(ICON_BORDER_SIZE));

        IconConfig iconCfg{.position = sf::Vector2f(0.f, 0.f),
                           .size = ICON_SIZE,
                           .textureKey = config.iconTextureKey,
                           .type = config.iconType};

        auto speakerIcon = UIFactory::Instance().CreateIcon(iconCfg);

        iconGroupBox->AddElement(speakerIcon);
        m_groupBox->AddElement(iconGroupBox);
    }

    // Create text label for dialog lines
    m_textLabel = UIFactory::Instance().CreateTextLabel(
        TextLabelConfig{.text = std::string(),
                        .position = sf::Vector2f(0.f, 0.f),
                        .fontSize = ResolutionScaleManager::Instance().ScaleFont(DEFAULT_CHATBOX_FONT_SIZE),
                        .centerOrigin = false,
                        .scheme = config.textScheme});

    m_groupBox->AddElement(m_textLabel);
}

/// @brief Adds a line of text to the queue. Starts next line immediately if idle.
/// @param text the text to add to the dialog.
void UIChatBox::AddLine(const std::string &text)
{
    m_lineQueue.push(text);

    if (m_typingComplete)
    {
        StartNextLine();
    }
}

/// @brief Starts revealing the next line in the queue or clears text if queue is empty.
void UIChatBox::StartNextLine()
{
    if (m_nextIndicatorIcon)
    {
        m_groupBox->RemoveElement(m_nextIndicatorIcon);
        m_nextIndicatorIcon.reset();
    }

    if (m_lineQueue.empty())
    {
        m_fullText.clear();
        m_currentText.clear();
        m_typingComplete = true;
        m_textLabel->SetText(std::string());

        return;
    }

    m_fullText = m_lineQueue.front();
    m_lineQueue.pop();
    m_currentText.clear();
    m_charIndex = 0;
    m_timer = 0.f;
    m_typingComplete = false;
}

/// @brief Immediately finish revealing current line if in progress.
void UIChatBox::SkipTyping()
{
    if (!m_typingComplete)
    {
        m_currentText = m_fullText;
        m_typingComplete = true;
        m_textLabel->SetText(m_currentText);
    }
}

/// @brief Returns true if there are more lines in the queue.
/// @returns True / False.
bool UIChatBox::HasMoreLines() const
{
    return !m_lineQueue.empty();
}

/// @brief Returns true if current line is fully revealed.
/// @returns True / False.
bool UIChatBox::IsTypingComplete() const
{
    return m_typingComplete;
}

/// @brief Returns true if there is no active or queued text remaining.
/// @returns True / False.
bool UIChatBox::IsEmpty() const
{
    return m_fullText.empty() && m_lineQueue.empty();
}

/// @brief Clears all queued and current text, resets state.
void UIChatBox::Clear()
{
    while (!m_lineQueue.empty())
    {
        m_lineQueue.pop();
    }

    m_fullText.clear();
    m_currentText.clear();
    m_charIndex = 0;
    m_typingComplete = true;
    m_textLabel->SetText(std::string());

    if (m_nextIndicatorIcon)
    {
        m_groupBox->RemoveElement(m_nextIndicatorIcon);
        m_nextIndicatorIcon.reset();
    }
}

/// @brief @brief Performs internal state management during a single frame.
/// @param mousePos current MousePosition.
/// @param isMousePressed IsMousePressed?
/// @param isJustClicked IsMouseJustPressed?
/// @param dt delta time since last update.
void UIChatBox::Update(const sf::Vector2i &mousePos, bool isMousePressed, bool isJustClicked, float dt)
{
    if (!m_typingComplete)
    {
        m_timer += dt;
        m_sfxCooldown += dt;

        while (m_timer >= m_typeSpeed && m_charIndex < m_fullText.size())
        {
            m_currentText += m_fullText[m_charIndex];
            m_charIndex++;
            m_timer -= m_typeSpeed;

            // Only play SFX if enough time has passed since last sound
            if (m_sfxCooldown >= m_sfxInterval)
            {
                PlayCharacterSound();
                m_sfxCooldown = 0.f;
            }
        }

        if (m_charIndex >= m_fullText.size())
        {
            m_typingComplete = true;
        }

        m_textLabel->SetText(m_currentText);
    }

    else if (!m_lineQueue.empty() && !m_nextIndicatorIcon)
    {
        IconConfig cfg{.position = sf::Vector2f(0.f, 0.f),
                       .size = sf::Vector2f(24.f, 24.f),
                       .textureKey = UIAssets::UIArrowDownTextureKey,
                       .type = IconType::DialogNextIcon};

        m_nextIndicatorIcon = UIFactory::Instance().CreateIcon(cfg);
    }

    m_groupBox->Update(mousePos, isMousePressed, isJustClicked, dt);

    if (m_nextIndicatorIcon)
    {
        // Set icon position relative to group box
        auto iconPos = m_groupBox->GetPosition();
        auto boxSize = m_groupBox->GetSize();

        iconPos.x += boxSize.x - m_nextIndicatorIcon->GetSize().x - 2.f; // move across near far edge
        iconPos.y += boxSize.y - m_nextIndicatorIcon->GetSize().y - 2.f; // move down near bottom edge

        m_nextIndicatorIcon->SetPosition(iconPos);

        m_nextIndicatorIcon->Update(mousePos, isMousePressed, isJustClicked, dt);
    }
}

/// @brief Sets the position of the chatbox container.
/// @param position position to set chatbox container.
void UIChatBox::SetPosition(const sf::Vector2f &position)
{
    m_groupBox->SetPosition(position);
}

/// @brief Gets the current position of the chatbox container.
/// @returns m_groupBox.Position.
sf::Vector2f UIChatBox::GetPosition() const
{
    return m_groupBox->GetPosition();
}

/// @brief Sets the size of the chatbox container.
/// @param size new size for the chatbox container.
void UIChatBox::SetSize(const sf::Vector2f &size)
{
    m_groupBox->SetSize(size);
}

/// @brief Gets the current size of the chatbox container.
/// @returns m_groupBox.Size.
sf::Vector2f UIChatBox::GetSize() const
{
    return m_groupBox->GetSize();
}

/// @brief Checks if a point is inside the chatbox container.
/// @param point X / Y coordinate to compare against us.
/// @returns True / False.
bool UIChatBox::Contains(const sf::Vector2i &point) const
{
    return m_groupBox->Contains(point);
}

/// @brief Draw this UIChatBox to the Renderable Target.
/// @param target render target.
/// @param states optional sf::RenderStates.
void UIChatBox::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(*m_groupBox, states);

    if (m_nextIndicatorIcon)
    {
        target.draw(*m_nextIndicatorIcon, states);
    }
}

/// @brief When typing during update, play a sound effect while characters become rendered in chatbox container.
void UIChatBox::PlayCharacterSound()
{
    AudioManager::Instance().PlaySFX(AudioAssets::ChatBlipKey);
}
