// ============================================================================
//  File        : UIChatBox.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-06-28
//  Description : Represents a Chatbox UI element with typewriter effect
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "UIElement.h"
#include "UIGroupBox.h"
#include "UIIcon.h"
#include "UITextLabel.h"
#include <memory>
#include <queue>
#include <string>

/// @brief Data structure holding internal configurations useful for ButtonConfig construction.
/// @param position Vector2f position for ChatBox.
/// @param size Vector2f size for ChatBox.
/// @param showTitle Bool to toggle title [default false (title disabled)].
/// @param title Optional string for title label [default empty string].
/// @param typeSpeed float Seconds per character for typewriter effect [default 0.05f].
/// @param textScheme Scheme for chat text [default default color scheme].
/// @param titleScheme Scheme for title text [default default color scheme].
/// @param useSpeakerIcon bool to toggle using an icon sprite indicating a speaker.
/// @param iconTextureKey string key to path for texture of speaker icon if applicable.
/// @param iconType IconType enum for type of icon.
struct ChatBoxConfig
{
    sf::Vector2f position;
    sf::Vector2f size;
    bool showTitle = false;
    std::string title = std::string();
    float typeSpeed = 0.05f; // seconds per char
    UITextLabelScheme textScheme = UITextLabelScheme::DefaultScheme;
    UITextLabelScheme titleScheme = UITextLabelScheme::DefaultScheme;

    bool useSpeakerIcon = false;
    std::string iconTextureKey = std::string();
    IconType iconType = IconType::None;
};

// ============================================================================
//  Class       : UIChatBox
//  Purpose     : Manages this UIChatBox logic at the ui level.
//
//  Responsibilities:
//      - Reveal text lines, characters and whole text contained in groupbox
//      - Sound effects while characters are revealed.
//      - Skippable lines of text.
//      - Sprite Icon for designated speaker.
//      - Internally managed GroupBoxes for organization and appearance.
//
// ============================================================================
class UIChatBox : public UIElement
{
  public:
    UIChatBox(const ChatBoxConfig &config);
    ~UIChatBox() override = default;

    // Disable copy
    UIChatBox(const UIChatBox &) = delete;
    UIChatBox &operator=(const UIChatBox &) = delete;

    // Allow move
    UIChatBox(UIChatBox &&) noexcept = default;
    UIChatBox &operator=(UIChatBox &&) noexcept = default;

    void AddLine(const std::string &text);
    void SetSpeaker(const std::string &speakerName, bool showTitle, const std::string &iconTextureKey, IconType type);
    void StartNextLine();
    void SkipTyping();
    bool HasMoreLines() const;
    bool IsTypingComplete() const;
    bool IsEmpty() const;
    void Clear();

    void Update(const sf::Vector2i &mousePos, bool isMousePressed, bool isJustClicked, float dt) override;
    void SetPosition(const sf::Vector2f &position) override;
    sf::Vector2f GetPosition() const override;
    void SetSize(const sf::Vector2f &size) override;
    sf::Vector2f GetSize() const override;
    bool Contains(const sf::Vector2i &point) const override;

  protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    void PlayCharacterSound();

  private:
    std::queue<std::string> m_lineQueue;
    std::shared_ptr<UIGroupBox> m_groupBox;
    std::shared_ptr<UITextLabel> m_textLabel;
    std::shared_ptr<UIIcon> m_nextIndicatorIcon;
    std::shared_ptr<UIGroupBox> m_speakerIconGroupBox;

    std::string m_fullText;
    std::string m_currentText;

    float m_typeSpeed = 0.05f;
    float m_sfxInterval = 0.05f;
    float m_timer = 0.f;
    float m_sfxCooldown = 0.f;
    size_t m_charIndex = 0;
    bool m_typingComplete = true;
};
