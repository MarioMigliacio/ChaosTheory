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

/// TODO: 
void ChaosTheory::Game::Introduction()
{
}

/// TODO:
void ChaosTheory::Game::Menu()
{
}

void ChaosTheory::Game::Play()
{
	m_window.setTitle("Playing");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	while(m_window.isOpen())
	{
		sf::Event event;

		while(m_window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				m_state = CurrentState::s_quit;
				return;
			}
		}

		m_window.clear();
		m_window.draw(shape);
		m_window.display();
	}
}

bool ChaosTheory::Game::Quitting()
{
	return m_state == CurrentState::s_quit;
}
