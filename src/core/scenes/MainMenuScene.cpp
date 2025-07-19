// ============================================================================
//  File        : MainMenuScene.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-13
//  Description : Hosts the definitions for Main Menu Scene Object
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "MainMenuScene.h"
#include "AssetManager.h"
#include "Assets.h"
#include "AudioManager.h"
#include "InputManager.h"
#include "Macros.h"
#include "ResolutionScaleManager.h"
#include "SceneManager.h"
#include "SceneTransitionManager.h"
#include "UIFactory.h"
#include "UIManager.h"
#include "UIPresets.h"
#include "WindowManager.h"

/// @brief Constants that can be adjusted throughout the MainMenuScene.
namespace
{
/// @brief Default Main Menu Title string.
constexpr auto DEFAULT_TITLE_STR = "Chaos Theory";

/// @brief Fixed name constant for the play button label.
constexpr auto PLAY_BTN_LABEL = "Play";

/// @brief Fixed name constant for the Settings button label.
constexpr auto SETTING_BTN_LABEL = "Settings";

/// @brief Fixed name constant for the Exit button label.
constexpr auto EXIT_BTN_LABEL = "Exit";

/// @brief Height of amplitude/trough for sinusoidal ship movement.
constexpr float SINE_WAVE_AMP = 50.f;

/// @brief Oscillations per second during ship update.
constexpr float WAVE_FREQ = 2.f;

/// @brief Wrap around padding for ship.
constexpr float SHIP_BOUNDARY = 50.f;

/// @brief Adjust the ship movement speed across the screen.
constexpr float SHIP_VELOCITY = 150.f;
} // namespace

/// @brief Constructor for the MainMenuScene.
/// @param settings Internal settings to initialize with.
MainMenuScene::MainMenuScene(std::shared_ptr<Settings> settings) : m_settings(settings)
{
}

/// @brief  Initializes the MainMenuScene.
void MainMenuScene::Init()
{
    CF_EXIT_EARLY_IF_ALREADY_INITIALIZED();

    auto &window = WindowManager::Instance().GetWindow();
    auto desiredSetting = m_settings->m_resolution;
    auto desiredSize = WindowManager::Instance().GetResolutionSize(desiredSetting);
    auto currentSize = window.getSize();

    // Only re-apply resolution if needed
    if (currentSize != desiredSize)
    {
        WindowManager::Instance().ApplyResolution(desiredSetting);
    }

    UIManager::Instance().Clear();
    SceneTransitionManager::Instance().StartFadeIn();

    LoadRequiredAssets();
    SetupSceneComponents();

    m_isInitialized = true;

    CT_LOG_INFO("MainMenuScene initialized.");
}

/// @brief Load any required assets relevant to the MainMenuScene.
void MainMenuScene::LoadRequiredAssets()
{
    auto &assets = AssetManager::Instance();

    for (const auto &[key, path] : MainMenuAssets::UI)
    {
        if (!AssetManager::Instance().LoadTexture(key, path))
        {
            CT_LOG_ERROR("MainMenuScene::LoadRequiredAssets::UI failed to load asset: {}, {}", key, path);
        }
    }

    for (const auto &[key, path] : MainMenuAssets::Backgrounds)
    {
        if (!assets.LoadTexture(key, path))
        {
            CT_LOG_ERROR("MainMenuScene::LoadRequiredAssets::Backgrounds failed to load Asset: {}, {}", key, path);
        }
    }

    for (const auto &[key, path] : MainMenuAssets::Sprites)
    {
        if (!assets.LoadTexture(key, path))
        {
            CT_LOG_ERROR("MainMenuScene::LoadRequiredAssets::Sprites failed to load Asset: {}, {}", key, path);
        }
    }

    for (const auto &[key, path] : FontAssets::Fonts)
    {
        if (!assets.LoadFont(key, path))
        {
            CT_LOG_ERROR("MainMenuScene::LoadRequiredAssets::LoadFont failed to load Asset: {}, {}", key, path);
        }
    }

    CT_LOG_INFO("MainMenuScene finished LoadRequiredAssets.");
}

/// @brief Shuts down this scene and resets internal state.
void MainMenuScene::Shutdown()
{
    CT_WARN_IF_UNINITIALIZED("MainMenuScene", "Shutdown");

    m_settings.reset();
    m_isInitialized = false;

    CT_LOG_INFO("MainMenuScene Shutdown.");
}

/// @brief Handles the exit criteria for this scene.
void MainMenuScene::OnExit()
{
    CT_LOG_INFO("MainMenuScene OnExit.");
}

/// @brief Performs internal state management during a single frame.
/// @param dt delta time since last update.
void MainMenuScene::Update(float dt)
{
    const auto mousePos = InputManager::Instance().GetMousePosition();
    const bool isPressed = InputManager::Instance().IsMouseButtonPressed(sf::Mouse::Left);
    const bool isJustPressed = InputManager::Instance().IsMouseButtonJustPressed(sf::Mouse::Left);

    UIManager::Instance().Update(mousePos, isPressed, isJustPressed, dt);

    if (m_background)
    {
        m_background->Update(dt);
    }

    if (m_shipActive)
    {
        UpdateShip(dt);
    }

    // Handle button scene request change
    if (m_hasPendingTransition)
    {
        CT_LOG_INFO("MainMenuScene Requesting Scene Change to '{}'", SceneIDToString(m_requestedScene));
        m_hasPendingTransition = false;

        // Game mode will have its own dedicated sound track, but the Settings page can share the track.
        if (m_requestedScene == SceneID::Game || m_requestedScene == SceneID::Introduction)
        {
            if (AudioManager::Instance().IsInitialized())
            {
                AudioManager::Instance().StopMusic();
            }
        }

        SceneTransitionManager::Instance().ForceFullyOpaque();
        SceneManager::Instance().RequestSceneChange(m_requestedScene);
    }

    // Handle exit request from the scene
    else if (m_shouldExit)
    {
        CT_LOG_INFO("MainMenuScene requested exit. Popping scene...");
        SceneManager::Instance().PopScene();
    }
}

/// @brief Handle any quick cancelation requests if present.
/// @param event bubbled down from caller, not needed.
void MainMenuScene::HandleEvent(const sf::Event &event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Escape)
        {
            m_shouldExit = true;

            CT_LOG_INFO("MainMenuScene: Esc event handled.");
        }
    }
}

/// @brief Not used in MainMenuScene context.
/// @param newSize bubbled down from caller, not needed.
void MainMenuScene::OnResize(const sf::Vector2u &newSize)
{
}

/// @brief While this scene is active, render the necessary components.
void MainMenuScene::Render()
{
    CT_WARN_IF_UNINITIALIZED("MainMenuScene", "Render");

    auto &window = WindowManager::Instance().GetWindow();
    window.clear();

    if (m_background)
    {
        m_background->Draw(window);
    }

    if (m_shipActive)
    {
        window.draw(m_shipSprite);
    }

    UIManager::Instance().Render(window);
}

/// @brief Helper method to clear up clutter from main Init.
void MainMenuScene::SetupSceneComponents()
{
    CreateTitleText();
    CreateButtons();
    LoadBackground();
    InitShip();
    PlayIntroMusic();
}

/// @brief Assists with the loading of the TitleText for this MainMenuScene.
void MainMenuScene::CreateTitleText()
{
    auto &scaleMgr = ResolutionScaleManager::Instance();

    const std::string titleText = DEFAULT_TITLE_STR;
    const unsigned int fontSize = scaleMgr.ScaleFont(DEFAULT_TEXT_LABEL_TITLE_FONT_SIZE);
    const sf::Vector2f centerPos = {WindowManager::Instance().GetWindow().getSize().x / 2.f,
                                    scaleMgr.ScaledReferenceY(DEFAULT_TEXT_LABEL_TITLE_HEIGHT_PERCENT)};

    m_titleLabel = UIFactory::Instance().CreateTextLabel(
        TextLabelConfig{.text = titleText, .position = centerPos, .fontSize = fontSize});

    UIManager::Instance().AddElement(m_titleLabel);
}

/// @brief Assists with creating the Buttons for this MainMenuScene.
void MainMenuScene::CreateButtons()
{
    const auto winSize = WindowManager::Instance().GetWindow().getSize();

    const float scaledButtonWidth = ResolutionScaleManager::Instance().ScaleX(BASE_BUTTON_WIDTH_PIXEL);
    const float scaledButtonHeight = ResolutionScaleManager::Instance().ScaleY(BASE_BUTTON_HEIGHT_PIXEL);
    const float scaledSpacing = scaledButtonHeight * BASE_BUTTON_SPACING_PERCENT;
    const float startY = winSize.y * 0.7f;
    const float centerX = (winSize.x - scaledButtonWidth) / 2.f;

    const sf::Vector2f btnSize = {BASE_BUTTON_WIDTH_PIXEL, BASE_BUTTON_HEIGHT_PIXEL};
    const sf::Vector2f playPos{centerX, startY};
    const sf::Vector2f settingsPos{centerX, playPos.y + scaledButtonHeight + scaledSpacing};
    const sf::Vector2f exitPos{centerX, settingsPos.y + scaledButtonHeight + scaledSpacing};

    // Play button
    UIManager::Instance().AddElement(UIFactory::Instance().CreateSkinnableButton(SkinnableButtonConfig{
        .position = playPos,
        .size = btnSize,
        .label = PLAY_BTN_LABEL,
        .idleTexture = UIAssets::UISkinButtonBlueIdleKey,
        .hoverTexture = UIAssets::UISkinButtonBlueHoverKey,
        .scheme = UISkinnableButtonColorScheme::Blue,
        .onClick = [this]()
        {
            CT_LOG_INFO("Play button clicked!");
            m_hasPendingTransition = true;
            m_requestedScene = SceneID::SandBox; // TODO: Revert to IntroductionScene when not rapid testing.
        }}));

    // Settings button
    UIManager::Instance().AddElement(UIFactory::Instance().CreateSkinnableButton(
        SkinnableButtonConfig{.position = settingsPos,
                              .size = btnSize,
                              .label = SETTING_BTN_LABEL,
                              .idleTexture = UIAssets::UISkinButtonBlueIdleKey,
                              .hoverTexture = UIAssets::UISkinButtonBlueHoverKey,
                              .scheme = UISkinnableButtonColorScheme::Blue,
                              .onClick = [this]()
                              {
                                  CT_LOG_INFO("Settings button clicked!");
                                  m_hasPendingTransition = true;
                                  m_requestedScene = SceneID::Settings;
                              }}));

    // Exit button
    UIManager::Instance().AddElement(UIFactory::Instance().CreateSkinnableButton(
        SkinnableButtonConfig{.position = exitPos,
                              .size = btnSize,
                              .label = EXIT_BTN_LABEL,
                              .idleTexture = UIAssets::UISkinButtonBlueIdleKey,
                              .hoverTexture = UIAssets::UISkinButtonBlueHoverKey,
                              .scheme = UISkinnableButtonColorScheme::Blue,
                              .onClick = [this]()
                              {
                                  CT_LOG_INFO("Exit button clicked!");
                                  m_shouldExit = true;
                              }}));
}

/// @brief Loads the main background images for this MainMenuScene.
void MainMenuScene::LoadBackground()
{
    m_background = std::make_unique<Background>();
    m_background->InitParallax({{BackgroundAssets::GasPattern1BackgroundKey, 2.f},
                                {BackgroundAssets::PlainStarBackgroundKey, 1.f},
                                {BackgroundAssets::GasPattern2BackgroundKey, 4.f}});

    m_background->SetLayerMotion(BackgroundAssets::GasPattern1BackgroundKey, {-1.f, 0.f});
    m_background->SetLayerMotion(BackgroundAssets::GasPattern2BackgroundKey, {1.f, 0.f});
    m_background->SetLayerMotion(BackgroundAssets::PlainStarBackgroundKey, {1.f, .33f});

    CT_LOG_INFO("Menu background loaded and scaled.");
}

/// @brief Plays the background music for this MainMenuScene.
void MainMenuScene::PlayIntroMusic()
{
    if (!AudioManager::Instance().IsMusicPlaying() ||
        AudioManager::Instance().GetCurrentMusicName() != MainMenuAssets::MenuSong)
    {
        CT_LOG_INFO("MainMenuScene: Starting or resuming menu music.");
        AudioManager::Instance().PlayMusic(MainMenuAssets::MenuSong, true);
    }

    else
    {
        CT_LOG_INFO("MainMenuScene: Menu music already playing, no action needed.");
    }
}

/// @brief Helper method for setting up the travelling spaceship.
void MainMenuScene::InitShip()
{
    auto winSize = WindowManager::Instance().GetWindow().getSize();
    m_yDist = std::uniform_real_distribution<float>(winSize.y * 0.2f, winSize.y * 0.8f); // avoid top/bottom edges
    m_rng.seed(static_cast<unsigned>(time(nullptr)));

    RandomizeShipTexture();

    const float startY = m_yDist(m_rng);
    m_shipSprite.setPosition(0.f, startY);
    m_shipSineTimer = 0.f;
    m_shipActive = true;
}

/// @brief Helper method to draw from a pool of available player ship color variants for spice.
void MainMenuScene::RandomizeShipTexture()
{
    auto shipTypeIndex = std::uniform_int_distribution<int>(0, 5);
    const int i = shipTypeIndex(m_rng);

    sf::Texture *tex;

    switch (i)
    {
        case 0:
        default:
            tex = AssetManager::Instance().GetTexture(SpriteAssets::PlayerAssets::PlayerShipWhiteKey);
            break;
        case 1:
            tex = AssetManager::Instance().GetTexture(SpriteAssets::PlayerAssets::PlayerShipBlackKey);
            break;
        case 2:
            tex = AssetManager::Instance().GetTexture(SpriteAssets::PlayerAssets::PlayerShipBlueKey);
            break;
        case 3:
            tex = AssetManager::Instance().GetTexture(SpriteAssets::PlayerAssets::PlayerShipGoldKey);
            break;
        case 4:
            tex = AssetManager::Instance().GetTexture(SpriteAssets::PlayerAssets::PlayerShipGreenKey);
            break;
        case 5:
            tex = AssetManager::Instance().GetTexture(SpriteAssets::PlayerAssets::PlayerShipRedKey);
            break;
    }

    if (!tex)
    {
        CT_LOG_ERROR("MainMenuScene: RandomizeShipTexture Failed to load PlayerShip texture.");
        return;
    }

    m_shipSprite.setTexture(*tex);
    m_shipSprite.setOrigin(tex->getSize().x / 2.f, tex->getSize().y / 2.f);
    m_shipSprite.setRotation(90.f);

    const float baseSize = tex->getSize().x; // the ship is even in width and height, just need one.
    float scale = ResolutionScaleManager::Instance().ScaleX(baseSize) / baseSize;
    m_shipSprite.setScale(scale, scale);
}

/// @brief Helper method to update the travelling spaceship.
/// @param dt delta time since last update.
void MainMenuScene::UpdateShip(float dt)
{
    m_shipSineTimer += dt;

    sf::Vector2f pos = m_shipSprite.getPosition();
    pos.x += ResolutionScaleManager::Instance().GetScaleX() * SHIP_VELOCITY * dt;

    // Wave motion: sine-based Y offset
    float waveAmplitude = SINE_WAVE_AMP; // pixels
    float waveFrequency = WAVE_FREQ;     // oscillations per second
    float waveOffset = std::sin(m_shipSineTimer * waveFrequency * 2 * PI) * waveAmplitude;

    pos.y += waveOffset * dt; // apply offset gradually

    float windowWidth = WindowManager::Instance().GetWindow().getSize().x;

    if (pos.x > windowWidth + SHIP_BOUNDARY) // Wrap around logic
    {
        RandomizeShipTexture();
        pos.x = -SHIP_BOUNDARY;
        pos.y = m_yDist(m_rng); // new randomized Y
        m_shipSineTimer = 0.f;  // reset wave cycle
    }

    m_shipSprite.setPosition(pos);
}
