#include "Game.h"

// Static variable declarations:
ChaosTheory::Game::CurrentState ChaosTheory::Game::m_state = s_uninitialized;
sf::RenderWindow ChaosTheory::Game::m_window;

void ChaosTheory::Game::Initialize()
{
	// Ensure that the initialization takes place correctly here and for the first time.
	if(m_state != CurrentState::s_uninitialized)
	{
		return;
	}

	// sf::Style::Fullscreen
	// Create the Rendering Window.
	sf::VideoMode mode = sf::VideoMode::getDesktopMode();

	// 720p resolution: 1280 x 720
	const int gameWidth = 1280;
	const int gameHeight = 720;
	sf::View gameView;
	gameView.reset(sf::FloatRect(0.f, 0.f, static_cast<float>(gameWidth), static_cast<float>(gameHeight)));
	gameView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));

	// use the sf::Style to change the window's options. Fullscreen makes debugging a nightmare, the Titlebar + close styles prevent resizing window.
	m_window.create(sf::VideoMode(gameWidth, gameHeight, mode.bitsPerPixel), "Chaos Theory", sf::Style::Titlebar | sf::Style::Close);
	m_window.setView(gameView);
	m_window.setFramerateLimit(60);

	m_state = CurrentState::s_playing;

	while(!Quitting())
	{
		Run();
	}

	m_window.close();
}

void ChaosTheory::Game::Run()
{
	switch(m_state)
	{
	case CurrentState::s_intro:
		break;
	case CurrentState::s_menu:
		break;
	case CurrentState::s_playing:
		Play();
		break;
	default:
		break;
	}
}

void ChaosTheory::Game::Introduction()
{
}

void ChaosTheory::Game::Menu()
{
}

void ChaosTheory::Game::Play()
{
	m_window.setTitle("Playing");
	//Playing playGame;

	//playGame.Show(m_window);

	/*if(playGame.getPlayState() == Playing::PlayState::s_gameover)
	{
		m_state = CurrentState::s_gameover;
	}
	else
	{
		m_state = CurrentState::s_quit;
	}*/
}

bool ChaosTheory::Game::Quitting()
{
	return m_state == CurrentState::s_quit;
}
