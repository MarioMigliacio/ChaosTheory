#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace ChaosTheory
{
	/*
	/	The Game class object represents the state manager for the basic DancingPlutonium game.
	/	The initial state is initialized and we go through the various states:
	/	Run -> Introduction -> Menu -> Play
	*/
	class Game
	{
	public:
		#pragma region Ctor/Dtors

		/* Default game constructor*/
		Game() {}
		/* Game copy constructor */
		Game(const Game& _ref) {}
		/* Game destructor*/
		~Game() {}

		#pragma endregion
		#pragma region Initialize

		/* Run the initialize logic for this game, includes setting screen resolution */
		static void Initialize();

		#pragma endregion
	private:
		#pragma region State Methods

		/* Runs the state logic for Dancing Plutonium */
		static void Run();
		/* Runs the Introduction state for Dancing Plutonium */
		static void Introduction();
		/* Runs the Menu state for Dancing Plutonium */
		static void Menu();
		/* Runs the Play state for Dancing Plutonium */
		static void Play();
		/* Runs the Quitting state for Dancing Plutonium */
		static bool Quitting();

		#pragma endregion
		#pragma region CurrentState Enum

		/* CurrentState enum holds the types of states that Dancing Plutonium may be in */
		enum CurrentState
		{
			s_uninitialized,
			s_intro,
			s_menu,
			s_playing,
			s_quit
		};

		#pragma endregion
		#pragma region Static State Members

		static sf::RenderWindow m_window;	/* Static variable which maintains the SFML game window from which you see the game rendered on */
		static CurrentState m_state;		/* Static variable which maintains the state for this Game objects CurrentState enum */

		#pragma endregion
	};
}