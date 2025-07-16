// ============================================================================
//  File        : IntroductionScene.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-13
//  Description : Scene for intro cinematic with formation and dialog
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "IntroductionScene.h"
#include "AssetManager.h"
#include "Assets.h"
#include "DialogConstants.h"
#include "InputManager.h"
#include "Macros.h"
#include "MainMenuScene.h"
#include "ResolutionScaleManager.h"
#include "SandBoxScene.h"
#include "SceneTransitionManager.h"
#include "UIFactory.h"
#include "UIManager.h"
#include "WindowManager.h"
#include <cmath>

/// @brief Constants that can be adjusted throughout the IntroductionScene.
namespace
{
/// @brief adjustable; smaller = more precise
constexpr float SNAP_THRESHOLD = 5.f;

/// @brief Adjustable constant for Ship entry speed.
constexpr float ENTRY_SPEED = 300.f;

/// @brief Adjustable constant for Ship exit speed.
constexpr float EXIT_SPEED = 400.f;

/// @brief Adjustable constant for ship v formation offset x.
constexpr float SHIP_OFFSET_X = 80.f;

/// @brief Adjustable constant for ship v formation offset y.
constexpr float SHIP_OFFSET_Y = 50.f;

/// @brief Adjustable constant for ship being offscreen in y direction.
constexpr float SHIP_OFFSCREEN_Y = 100.f;

/// @brief Adjustable constant for ship being offscreen in x direction.
constexpr float SHIP_OFFSCREEN_X = 200.f;

/// @brief Constant for calculating exit positions for diagonal ships.
constexpr float OFFSCREEN_FACTOR = .8f;

/// @brief Fixed name constant to be used with BindActionKey to setup the skip chatbox dialog.
constexpr auto SKIP_CHAT_KEY = "Space";

/// @brief Fixed name constant to be used with BindActionKey to setup a termination key.
constexpr auto EXIT_BUTTON_KEY = "Exit";

/// @brief PI constant.
constexpr float PI = 3.14159f;
} // namespace

/// @brief Constructor for the IntroductionScene.
/// @param settings Internal settings to initialize with.
IntroductionScene::IntroductionScene(std::shared_ptr<Settings> settings) : m_settings(settings)
{
}

/// @brief Initializes the IntroductionScene.
void IntroductionScene::Init()
{
    CF_EXIT_EARLY_IF_ALREADY_INITIALIZED();

    UIManager::Instance().Clear();

    LoadRequiredAssets();
    BindInputKeys();
    LoadBackground();
    InitializeDialogContexts();
    SetupFormation();

    SceneTransitionManager::Instance().StartFadeIn();
    m_isInitialized = true;

    CT_LOG_INFO("IntroductionScene initialized.");
}

/// @brief Load any required assets relevant to the IntroductionScene.
void IntroductionScene::LoadRequiredAssets()
{
    for (const auto &[key, path] : FontAssets::Fonts)
    {
        if (!AssetManager::Instance().LoadFont(key, path))
        {
            CT_LOG_ERROR("IntroductionScene failed to load font asset: {} -> {}", key, path);
        }
    }

    for (const auto &[key, path] : IntroductionAssets::Backgrounds)
    {
        if (!AssetManager::Instance().LoadTexture(key, path))
        {
            CT_LOG_ERROR("IntroductionScene failed to load texture asset: {} -> {}", key, path);
        }
    }

    for (const auto &[key, path] : IntroductionAssets::Sprites)
    {
        if (!AssetManager::Instance().LoadTexture(key, path))
        {
            CT_LOG_ERROR("IntroductionScene failed to load texture asset: {} -> {}", key, path);
        }
    }

    for (const auto &[key, path] : IntroductionAssets::Sounds)
    {
        if (!AssetManager::Instance().LoadSound(key, path))
        {
            CT_LOG_ERROR("IntroductionScene failed to load sound asset: {} -> {}", key, path);
        }
    }

    CT_LOG_INFO("IntroductionScene finished LoadRequiredAssets.");
}

/// @brief Shuts down this scene and resets internal state.
void IntroductionScene::Shutdown()
{
    CT_WARN_IF_UNINITIALIZED("IntroductionScene", "Shutdown");

    m_ships.clear();
    m_chatBox.reset();
    m_dialogQueue.Clear();
    m_isInitialized = false;

    CT_LOG_INFO("IntroductionScene shutdown.");
}

/// @brief Handles the exit criteria for this scene.
void IntroductionScene::OnExit()
{
    InputManager::Instance().UnbindKey(SKIP_CHAT_KEY);
    InputManager::Instance().UnbindKey(EXIT_BUTTON_KEY);

    CT_LOG_INFO("IntroductionScene OnExit.");
}

/// @brief Performs internal state management during a single frame.
/// @param dt delta time since last update.
void IntroductionScene::Update(float dt)
{
    const auto mousePos = InputManager::Instance().GetMousePosition();
    const bool isPressed = InputManager::Instance().IsMouseButtonPressed(sf::Mouse::Left);
    const bool isJustPressed = InputManager::Instance().IsMouseButtonJustPressed(sf::Mouse::Left);

    if (m_background)
    {
        m_background->Update(dt);
    }

    UIManager::Instance().Update(mousePos, isPressed, isJustPressed, dt);

    switch (m_phase)
    {
        case IntroPhase::ShipEntry:
            UpdateFormationEntry(dt);
            break;

        case IntroPhase::Dialog:
            if (!m_dialogStarted)
            {
                if (m_dialogQueue.HasNext())
                {
                    DialogLine first = m_dialogQueue.Next();

                    m_chatBox = UIFactory::Instance().CreateChatBox(
                        ChatBoxConfig{.position = DEFAULT_CHATBOX_POSITION,
                                      .size = DEFAULT_CHATBOX_SIZE,
                                      .showTitle = first.showTitle,
                                      .title = first.speakerName,
                                      .typeSpeed = DEFAULT_CHATBOX_DIALOG_SPEED,
                                      .textScheme = UITextLabelScheme::DefaultScheme,
                                      .titleScheme = UITextLabelScheme::DefaultScheme,
                                      .useSpeakerIcon = !first.iconTextureKey.empty(),
                                      .iconTextureKey = first.iconTextureKey,
                                      .iconType = first.iconType});

                    m_chatBox->AddLine(first.text);
                    UIManager::Instance().AddElement(m_chatBox);
                    m_dialogStarted = true;
                }
            }

            AdvanceChatBox();
            break;

        case IntroPhase::ShipExit:
            UpdateShipExit(dt);
            break;

        case IntroPhase::Done:
            m_hasPendingTransition = true;
            m_requestedScene = SceneID::SandBox;
            break;
    }

    CheckActionsPressed();

    // Handle button scene request change
    if (m_hasPendingTransition)
    {
        if (m_requestedScene == SceneID::MainMenu)
        {
            CT_LOG_INFO("IntroductionScene Exit Event Requested. Transitioning to next scene: {}.",
                        SceneIDToString(m_requestedScene));

            m_hasPendingTransition = false;
            SceneManager::Instance().ReplaceScene(std::make_unique<MainMenuScene>(m_settings));
        }

        else if (m_requestedScene == SceneID::SandBox)
        {
            CT_LOG_INFO("IntroductionScene transitioning to next scene: {}.", SceneIDToString(m_requestedScene));

            m_hasPendingTransition = false;
            SceneManager::Instance().ReplaceScene(std::make_unique<SandBoxScene>(m_settings));
        }
    }
}

/// @brief Handle any relevent events at this scene level if needed.
/// @param event bubbled down from caller.
void IntroductionScene::HandleEvent(const sf::Event &event)
{
}

/// @brief Not used in IntroductionScene context.
/// @param newSize bubbled down from caller, not needed.
void IntroductionScene::OnResize(const sf::Vector2u &newSize)
{
}

/// @brief While this scene is active, render the necessary components.
void IntroductionScene::Render()
{
    auto &window = WindowManager::Instance().GetWindow();
    window.clear();

    if (m_background)
    {
        m_background->Draw(window);
    }

    for (const auto &ship : m_ships)
    {
        window.draw(ship.sprite);
    }

    UIManager::Instance().Render(window);
}

/// @brief Determines if any configured keyboard input has been pressed during scene update.
void IntroductionScene::CheckActionsPressed()
{
    auto &input = InputManager::Instance();

    if (input.IsKeyJustPressed(EXIT_BUTTON_KEY))
    {
        CT_LOG_INFO("IntroductionScene: Exit Button Pressed.");

        m_hasPendingTransition = true;
        m_requestedScene = SceneID::MainMenu;
    }
}

/// @brief Helper method to load the Background for this Scene.
void IntroductionScene::LoadBackground()
{
    m_background = std::make_unique<Background>();
    m_background->InitParallax(
        {{BackgroundAssets::GasPattern1BackgroundKey, 2.f}, {BackgroundAssets::PlainStarBackgroundKey, 1.f}});

    m_background->SetLayerMotion(BackgroundAssets::PlainStarBackgroundKey, {0.2f, -0.5f});
    m_background->SetLayerMotion(BackgroundAssets::GasPattern1BackgroundKey, {0.f, -0.1f});
}

/// @brief Sets up keyboard inputs that can be picked up during scene lifetime.
void IntroductionScene::BindInputKeys()
{
    InputManager::Instance().BindKey(SKIP_CHAT_KEY, sf::Keyboard::Key::Space);
    InputManager::Instance().BindKey(EXIT_BUTTON_KEY, sf::Keyboard::Key::Escape);
}

/// @brief Abstracted dialog contexts for scene ease of use.
void IntroductionScene::InitializeDialogContexts()
{
    for (const auto &line : DialogConstants::IntroDialog::INTRO_SEQUENCE)
    {
        m_dialogQueue.AddLine(line);
    }

    CT_LOG_DEBUG("IntroductionScene Queued up dialog contexts.");
}

/// @brief Update the chatbox internal to this IntroductionScene.
void IntroductionScene::AdvanceChatBox()
{
    auto &input = InputManager::Instance();

    if (!m_chatBox)
    {
        return;
    }

    if (input.IsKeyJustPressed(SKIP_CHAT_KEY) || InputManager::Instance().IsMouseButtonJustPressed(sf::Mouse::Left))
    {
        // Chatbox is not finished with its line; skip to line end.
        if (!m_chatBox->IsTypingComplete())
        {
            m_chatBox->SkipTyping();
        }

        // Chatbox is at the end of a line, and there's more dialog; skip to next line.
        else if (m_chatBox->HasMoreLines())
        {
            m_chatBox->StartNextLine();
        }

        // Chatbox is at the end of a dialog, but there may be another dialog in the queue; skip to next dialog.
        else if (m_dialogQueue.HasNext())
        {
            StartNextDialog();
        }

        // Fully done - remove chatbox
        else
        {
            m_chatBox->Clear();
            UIManager::Instance().RemoveElement(m_chatBox);
            m_chatBox.reset();

            m_phase = IntroPhase::ShipExit;
        }
    }
}

/// @brief Update the dialog queue to the next in line.
void IntroductionScene::StartNextDialog()
{
    DialogLine next = m_dialogQueue.Next();

    // Reconfigure title & icon first
    m_chatBox->SetSpeaker(next.speakerName, next.showTitle, next.iconTextureKey, next.iconType);

    // Then clear any previous lines
    m_chatBox->Clear();

    // Then add the new line text
    m_chatBox->AddLine(next.text);
}

/// @brief Helper method to stage vector of FormationShips, including enter, and exit positions.
void IntroductionScene::SetupFormation()
{
    const auto winSize = WindowManager::Instance().GetWindow().getSize();
    sf::Vector2f center(winSize.x / 2.f, winSize.y / 2.f);

    float offsetX = SHIP_OFFSET_X;
    float offsetY = SHIP_OFFSET_Y;

    std::vector<sf::Vector2f> targetPositions = {
        center + sf::Vector2f(-2 * offsetX, 2 * offsetY), // left outer
        center + sf::Vector2f(-offsetX, offsetY),         // left inner
        center,                                           // center (player)
        center + sf::Vector2f(offsetX, offsetY),          // right inner
        center + sf::Vector2f(2 * offsetX, 2 * offsetY)   // right outer
    };

    sf::Vector2f startPos(center.x, winSize.y + SHIP_OFFSCREEN_Y);

    for (size_t i = 0; i < targetPositions.size(); ++i)
    {
        FormationShip fs;
        sf::Texture *tex;

        switch (i)
        {
            case 0:
                tex = AssetManager::Instance().GetTexture(SpriteAssets::PlayerShipBlueKey);
                break;
            case 1:
                tex = AssetManager::Instance().GetTexture(SpriteAssets::PlayerShipBlackKey);
                break;
            case 2:
                tex = AssetManager::Instance().GetTexture(SpriteAssets::PlayerShipWhiteKey);
                break;
            case 3:
                tex = AssetManager::Instance().GetTexture(SpriteAssets::PlayerShipGreenKey);
                break;
            case 4:
                tex = AssetManager::Instance().GetTexture(SpriteAssets::PlayerShipRedKey);
                break;
        }

        if (!tex)
        {
            CT_LOG_ERROR("IntroductionScene: Missing texture for ship at index {}", i);
            continue;
        }

        fs.sprite.setTexture(*tex);
        fs.sprite.setOrigin(tex->getSize().x / 2.f, tex->getSize().y / 2.f);
        fs.sprite.setPosition(startPos);
        fs.targetPosition = targetPositions[i];

        // Scaling
        float baseSize = tex->getSize().x;
        float scale = ResolutionScaleManager::Instance().ScaleX(baseSize) / baseSize;
        fs.sprite.setScale(scale, scale);

        // Exit direction: away from center
        sf::Vector2f exitDir = targetPositions[i] - center;

        // Compute length (magnitude) of direction vector using Pythagorean theorem
        // sqrt(x² + y²)
        float dirLen = std::sqrt(exitDir.x * exitDir.x + exitDir.y * exitDir.y);

        // Normalize vector to have length = 1 (unit vector), so we can scale it by speed easily
        if (dirLen > 0.f)
        {
            exitDir /= dirLen;
        }

        // Compute screen diagonal, in higher resolutions this will always work.
        // The ships at index 1 and 3 will take fancy diagonals on their way out.
        float screenLength = std::sqrt(winSize.x * winSize.x + winSize.y * winSize.y);
        float exitDistance = screenLength * OFFSCREEN_FACTOR;

        // Default exit position
        sf::Vector2f exitPos = targetPositions[i] + exitDir * exitDistance;

        // Straight left
        if (i == 0)
        {
            exitPos = sf::Vector2f(-SHIP_OFFSCREEN_X, targetPositions[i].y);
        }

        // Straight right
        else if (i == 4)
        {
            exitPos = sf::Vector2f(winSize.x + SHIP_OFFSCREEN_X, targetPositions[i].y);
        }

        // Player ship, bottom center.
        else if (i == 2)
        {
            float scaledHeight = fs.sprite.getGlobalBounds().height;
            exitPos = sf::Vector2f(center.x, winSize.y - scaledHeight);
        }

        fs.exitPosition = exitPos;
        m_ships.push_back(fs);
    }

    CT_LOG_DEBUG("IntroductionScene SetupFormation complete. Ships assigned a destination.");
}

/// @brief Updates the vector of FormationShips based on time. Transition into dialog state after reached exit
/// conditions.
/// @param dt deltatime since last update.
void IntroductionScene::UpdateFormationEntry(float dt)
{
    bool allArrived = true;

    for (auto &fs : m_ships)
    {
        if (fs.arrived)
        {
            continue;
        }

        sf::Vector2f dir = fs.targetPosition - fs.sprite.getPosition();

        // Compute length (magnitude) of direction vector using Pythagorean theorem
        // sqrt(x² + y²)
        float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);

        if (len > SNAP_THRESHOLD)
        {
            dir /= len;
            float trueSpeed = ResolutionScaleManager::Instance().GetUniformScale() * ENTRY_SPEED;
            fs.sprite.move(dir * trueSpeed * dt);

            // Compute angle (in degrees) for sprite facing direction
            // atan2 returns angle in radians between x-axis and vector (dir.y, dir.x)
            // Convert from radians to degrees by multiplying with (180 / PI)
            float angle = std::atan2(dir.y, dir.x) * 180.f / PI + 90.f;
            fs.sprite.setRotation(angle);
            allArrived = false;
        }

        else
        {
            fs.sprite.setPosition(fs.targetPosition);
            fs.arrived = true;
            fs.sprite.setRotation(0.f); // Reset to face up or neutral after arriving
        }
    }

    if (allArrived)
    {
        m_phase = IntroPhase::Dialog;

        CT_LOG_DEBUG("IntroductionScene UpdateFormation complete. Ships arrived at destination.");
    }
}

/// @brief Updates the vector of FormationShips based on time. Transition into end dialog state after reached exit
/// conditions.
/// @param dt deltatime since last update.
void IntroductionScene::UpdateShipExit(float dt)
{
    bool allExited = true;

    for (size_t i = 0; i < m_ships.size(); ++i)
    {
        auto &fs = m_ships[i];
        sf::Vector2f dir = fs.exitPosition - fs.sprite.getPosition();

        // Compute length (magnitude) of direction vector using Pythagorean theorem
        // sqrt(x² + y²)
        float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);

        if (len > SNAP_THRESHOLD)
        {
            dir /= len;
            float trueSpeed = ResolutionScaleManager::Instance().GetUniformScale() * EXIT_SPEED;
            fs.sprite.move(dir * trueSpeed * dt);

            // Compute angle (in degrees) for sprite facing direction
            // atan2 returns angle in radians between x-axis and vector (dir.y, dir.x)
            // Convert from radians to degrees by multiplying with (180 / PI)
            float angle = std::atan2(dir.y, dir.x) * 180.f / PI + 90.f;
            fs.sprite.setRotation(angle);
            allExited = false;
        }

        else
        {
            fs.sprite.setPosition(fs.exitPosition);
            fs.sprite.setRotation(0.f); // Reset to face up or neutral after arriving
        }
    }

    if (allExited)
    {
        m_phase = IntroPhase::Done;

        CT_LOG_DEBUG("IntroductionScene UpdateShipExit complete. Ships have reached exit destinations.");
    }
}
