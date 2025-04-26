// ============================================================================
//  File        : SettingsScene.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-23
//  Description : Hosts the definition for Settings Scene object
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "SettingsScene.h"
#include "AssetManager.h"
#include "AudioManager.h"
#include "InputManager.h"
#include "Macros.h"
#include "SceneFactory.h"
#include "SceneManager.h"
#include "SettingsManager.h"
#include "UIFactory.h"
#include "UIManager.h"
#include "WindowManager.h"

namespace
{
constexpr float SLIDER_WIDTH = 300.f;
constexpr float SLIDER_HEIGHT = 20.f;
constexpr float SLIDER_SPACING = 50.f;
constexpr float BUTTON_WIDTH = 100.f;
constexpr float BUTTON_HEIGHT = 30.f;
constexpr float BUTTON_SPACING = 30.f;
} // namespace

SettingsScene::SettingsScene(std::shared_ptr<Settings> settings) : m_settings(settings)
{
}

void SettingsScene::Init()
{
    CF_EXIT_EARLY_IF_ALREADY_INITIALIZED();

    auto windowSize = WindowManager::Instance().GetWindow().getSize();

    CreateTitleText();
    CreateSliders();
    CreateButtons();

    m_isInitialized = true;

    CT_LOG_INFO("SettingsScene initialized.");
}

void SettingsScene::Shutdown()
{
    CT_WARN_IF_UNINITIALIZED("SettingsScene", "Shutdown");

    m_settings.reset();
    m_isInitialized = false;

    CT_LOG_INFO("SettingsScene Shutdown.");
}

void SettingsScene::OnExit()
{
    SettingsManager::Instance().SaveToFile("config.json");

    CT_LOG_INFO("SettingsScene OnExit.");
}

// Handle any internal logic that should be done relevant to this scene.
void SettingsScene::HandleEvent(const sf::Event &event)
{
    // Only if you want to catch window resize, close, etc.
}

void SettingsScene::Update(float dt)
{
    const auto mousePos = InputManager::Instance().GetMousePosition();
    const bool isPressed = InputManager::Instance().IsMouseButtonPressed(sf::Mouse::Left);

    UIManager::Instance().Update(mousePos, isPressed);

    if (m_shouldExit)
    {
        SceneManager::Instance().RequestSceneChange(SceneID::MainMenu);
    }
}

void SettingsScene::OnResize(const sf::Vector2u &newSize)
{
}

void SettingsScene::Render()
{
    auto &window = WindowManager::Instance().GetWindow();
    window.clear();

    window.draw(m_title);
    UIManager::Instance().Render(window);
}

void SettingsScene::CreateTitleText()
{
    m_title.setFont(AssetManager::Instance().GetFont("Default.ttf"));
    m_title.setString("Settings");
    m_title.setCharacterSize(48);
    m_title.setFillColor(sf::Color(255, 255, 200));
    m_title.setOutlineColor(sf::Color::Black);
    m_title.setOutlineThickness(2.f);

    const auto bounds = m_title.getLocalBounds();
    m_title.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);

    const auto windowSize = WindowManager::Instance().GetWindow().getSize();
    m_title.setPosition(windowSize.x / 2.f, windowSize.y * 0.15f);
}

void SettingsScene::CreateSliders()
{
    auto settings = SettingsManager::Instance().GetSettings();
    auto windowSize = WindowManager::Instance().GetWindow().getSize();
    float startX = (windowSize.x - SLIDER_WIDTH) / 2.f;
    float startY = windowSize.y * 0.4f;

    UIManager::Instance().AddElement(
        MakeSlider("Master Volume", {startX, startY}, settings->m_masterVolume,
                   [](float value) { SettingsManager::Instance().GetSettings()->m_masterVolume = value; }));

    startY += SLIDER_SPACING;

    UIManager::Instance().AddElement(MakeSlider("Music Volume", {startX, startY + SLIDER_SPACING},
                                                settings->m_musicVolume, [](float value)
                                                { SettingsManager::Instance().GetSettings()->m_musicVolume = value; }));

    startY += SLIDER_SPACING;

    UIManager::Instance().AddElement(MakeSlider("SFX Volume", {startX, startY + SLIDER_SPACING}, settings->m_sfxVolume,
                                                [](float value)
                                                { SettingsManager::Instance().GetSettings()->m_sfxVolume = value; }));
}

void SettingsScene::CreateButtons()
{
    auto windowSize = WindowManager::Instance().GetWindow().getSize();
    float startX = (windowSize.x - BUTTON_WIDTH) / 2.f;
    float startY = windowSize.y * 0.7f;

    UIManager::Instance().AddElement(
        UIFactory::Instance().CreateButton(ButtonType::Classic, {startX, startY}, {BUTTON_WIDTH, BUTTON_HEIGHT}, "Back",
                                           [this]()
                                           {
                                               CT_LOG_INFO("SettingsScene: Back button clicked.");
                                               m_shouldExit = true;
                                           }));
}

std::shared_ptr<UIElement> SettingsScene::MakeSlider(const std::string &label, const sf::Vector2f &position,
                                                     float initial, std::function<void(float)> onChange)
{
    return UIFactory::Instance().CreateSlider(label, position, {SLIDER_WIDTH, SLIDER_HEIGHT}, 0.f, 100.f, initial,
                                              std::move(onChange));
}
