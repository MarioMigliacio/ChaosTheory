#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace ChaosTheory
{
	// Chaos Theory 
	// The Game class object represents the state
	// manager for the Chaos Theory game.
	// The initial state is initialized and we go
	// through the various states.
	//////////////////////////////////////////////////
	class Game
	{
	public:

		// Default game constructor
		//////////////////////////////////////////////////
		Game() {}

		// Game copy constructor 
		//////////////////////////////////////////////////
		Game(const Game& _ref) {}
 
		// Game destructor
		//////////////////////////////////////////////////
		~Game() {}
				
		// Runs the initialize logic for this game, 
		// includes setting screen resolution.
		//////////////////////////////////////////////////
		static void Initialize();

	private:
		
		// Runs the state logic for ChaosTheory
		//////////////////////////////////////////////////
		static void Run();

		/// TODO: (create issue for feature)
		// Runs the Introduction state for ChaosTheory
		//////////////////////////////////////////////////
		static void Introduction();

		/// TODO: (create issue for feature)
		// Runs the Menu state for ChaosTheory
		//////////////////////////////////////////////////
		static void Menu();
				
		// Runs the Play state for ChaosTheory
		//////////////////////////////////////////////////
		static void Play();
				
		// Returns whether or not the CurrentState = s_quit
		// True if we should quit, False otherwise.
		//////////////////////////////////////////////////
		static bool Quitting();
				
		// CurrentState enum holds the types of states that
		// ChaosTheory may be in.
		//////////////////////////////////////////////////
		enum CurrentState
		{
			s_uninitialized,
			s_intro,
			s_menu,
			s_playing,
			s_quit
		};

		// Static variable which maintains the SFML game window
		// from which you see the game rendered on.
		//////////////////////////////////////////////////
		static sf::RenderWindow m_window;

		// Static variable which maintains the state for
		// this Game objects CurrentState enum.
		//////////////////////////////////////////////////
		static CurrentState m_state;
	}; // Game
} // ChaosTheory