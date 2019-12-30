#ifndef  APPLICATION_H
#define APPLICATION_H

class Sprite;
class Grid;
class GameObject;
class Button;
class Vector;

//! The main object that exists in the core game loop, it is used to initialize, update and draw all relevent objects every frame.
class Application
{
public:
	enum GameState
	{
		GAME_READY = 0, // There are (at least) 2 players in the server.
		GAME_START,
		GAME_WAITING, // Waiting for the other player to make its turn.
		GAME_MYTURN,
		GAME_RUNNING,
		GAME_WIN,
		GAME_LOSE,
		GAME_DRAW
	};
	
	enum PlayerTag
	{
		PLAYER1 = 0,
		PLAYER2
	};

	Application();
	~Application();

	static Application& getInstance()
	{
		static Application instance;
		return instance;
	}

	void init();
	void update(float deltaTime);
	void draw();
	void onMouseMoved(float mouseX, float mouseY);
	void onMouseButton(int btnIndex, int btnAction);
	void setGameState(GameState newState);
	GameState getGameState();
	void updateGrid(bool clicked, int cellIndex, PlayerTag pTag);
	bool isCrossWon();
	bool isCircleWon();

private:
	int m_totalCell; // how many available cells are in the grid?
	Vector m_mousePos; //! save the mouse position on the screen.
	GameState m_currentGState;
	GameObject* m_titleText;
	GameObject* m_instrucText; //! text to show the result and instructions telling the player to wait or is it their turn.
	GameObject* m_playerText; //! text to show the player info.
	Button* m_rematchBtn;
	Grid* m_3x3Grid;

	// Sprites
	Sprite* m_waitText;
	Sprite* m_drawText;
	Sprite* m_yrTurnText;
	Sprite* m_winText;
	Sprite* m_loseText;
	Sprite* m_rematchText;
	Sprite* m_rematchHText; // hovered sprite
	Sprite* m_player1Text;
	Sprite* m_player2Text;
	Sprite* m_crossSymbol;
	Sprite* m_circleSymbol;
	
};
#endif // ! APPLICATION_H