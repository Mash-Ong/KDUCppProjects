#include <GLFW\glfw3.h>
#include <iostream>
#include "Application.h"
#include "MyPhoton.h"
#include "GameObject.h"
#include "Grid.h"
#include "Cell.h"
#include "Button.h"

Application::Application()
{
}

Application::~Application()
{
}

void Application::init()
{
	std::cout << "Application is started " << std::endl;
	
	// initialize the sprites
	m_waitText = new Sprite("../media/text_wait.bmp");
	m_drawText = new Sprite("../media/text_draw.bmp");
	m_yrTurnText = new Sprite("../media/text_yrTurn.bmp");
	m_winText = new Sprite("../media/text_win.bmp");
	m_loseText = new Sprite("../media/text_lose.bmp");
	m_player1Text = new Sprite("../media/text_p1.bmp");
	m_player2Text = new Sprite("../media/text_p2.bmp");
	m_crossSymbol = new Sprite("../media/cross.bmp");
	m_circleSymbol = new Sprite("../media/hollowCircle.bmp");
	m_rematchText = new Sprite("../media/btn_rematch.bmp");
	m_rematchHText = new Sprite("../media/btn_rematch_hovered.bmp");

	m_3x3Grid = new Grid(3, 3, 120, 120, 6);

	m_titleText = new GameObject(new Sprite("../media/title.bmp"));
	m_titleText->setPosition(Vector(400, 530, 0));

	m_instrucText = new GameObject(m_waitText);
	m_instrucText->setPosition(Vector(170, 200, 0));

	m_playerText = new GameObject(NULL); // sprite will be set when game is started.
	m_playerText->setPosition(Vector(170, 340, 0));
	m_playerText->setScale(Vector(1.1, 1.1, 1.1));

	m_rematchBtn = new Button(170, 100, 307, 94, m_rematchText);
	m_rematchBtn->setSprite(NULL);  // sprite will be set when game is won/lose.
}

void Application::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_3x3Grid->draw(); // draw grid, then grid draw cells

	// data title
	m_titleText->draw();
	m_instrucText->draw(); // tell the player what to do, whether it's waiting or making choice.
	m_playerText->draw();
	m_rematchBtn->draw();
}

void Application::setGameState(GameState newState)
{
	m_currentGState = newState;

	switch (m_currentGState)
	{
	case GAME_READY:
		m_rematchBtn->setSprite(NULL);
		m_playerText->setSprite(NULL);
		m_instrucText->setSprite(m_waitText);
		break;

	case GAME_START:
		if (MyPhoton::getInstance().isPlayerOne())
			m_playerText->setSprite(m_player1Text);
		else if (MyPhoton::getInstance().isPlayerTwo())
			m_playerText->setSprite(m_player2Text);
		m_3x3Grid->initCells();
		m_totalCell = m_3x3Grid->getRowCount() * m_3x3Grid->getColCount();
		if (MyPhoton::getInstance().isThisPlayerTurn())
			setGameState(GAME_MYTURN);
		else
			setGameState(GAME_WAITING);
		break;

	case GAME_WAITING:
		m_instrucText->setSprite(m_waitText);
		break;

	case GAME_MYTURN:
		m_instrucText->setSprite(m_yrTurnText);
		break;

	case GAME_WIN:
		m_instrucText->setSprite(m_winText);
		m_rematchBtn->setSprite(m_rematchText);
		break;

	case GAME_LOSE:
		m_instrucText->setSprite(m_loseText);
		m_rematchBtn->setSprite(m_rematchText);
		break;

	case GAME_DRAW:
		m_instrucText->setSprite(m_drawText);
		m_rematchBtn->setSprite(m_rematchText);
		break;

	default:
		break;
	}
}

Application::GameState Application::getGameState()
{
	return m_currentGState;
}

void Application::updateGrid(bool clicked, int cellIndex, PlayerTag pTag)
{
	if (clicked)
	{
		m_totalCell--;
		if (m_totalCell == 0)
		{
			setGameState(GAME_DRAW);
		}

		if (pTag == PLAYER1)
		{
			// place a cross sign on the grid
			m_3x3Grid->getCell(cellIndex)->setStatus(Cell::CROSSED, m_crossSymbol);
			if (isCrossWon())
			{
				if (MyPhoton::getInstance().isPlayerOne())
				{
					setGameState(GAME_WIN);
				}
				else
				{
					setGameState(GAME_LOSE);
				}
			}
		}
		else if (pTag == PLAYER2)
		{
			m_3x3Grid->getCell(cellIndex)->setStatus(Cell::CIRCLED, m_circleSymbol);
			if (isCircleWon())
			{
				if (MyPhoton::getInstance().isPlayerTwo())
					setGameState(GAME_WIN);
				else
					setGameState(GAME_LOSE);
			}
		}
	}
	else
	{
		m_3x3Grid->getCell(cellIndex)->setHovered(true);
	}
}

bool Application::isCrossWon()
{
	if (m_3x3Grid->getCell(0)->getStatus() == Cell::CROSSED &&
		m_3x3Grid->getCell(3)->getStatus() == Cell::CROSSED &&
		m_3x3Grid->getCell(6)->getStatus() == Cell::CROSSED)
		return true;
	if (m_3x3Grid->getCell(1)->getStatus() == Cell::CROSSED &&
		m_3x3Grid->getCell(4)->getStatus() == Cell::CROSSED &&
		m_3x3Grid->getCell(7)->getStatus() == Cell::CROSSED)
		return true;
	if (m_3x3Grid->getCell(2)->getStatus() == Cell::CROSSED &&
		m_3x3Grid->getCell(5)->getStatus() == Cell::CROSSED &&
		m_3x3Grid->getCell(8)->getStatus() == Cell::CROSSED)
		return true;
	if (m_3x3Grid->getCell(2)->getStatus() == Cell::CROSSED &&
		m_3x3Grid->getCell(1)->getStatus() == Cell::CROSSED &&
		m_3x3Grid->getCell(0)->getStatus() == Cell::CROSSED)
		return true;
	if (m_3x3Grid->getCell(5)->getStatus() == Cell::CROSSED &&
		m_3x3Grid->getCell(4)->getStatus() == Cell::CROSSED &&
		m_3x3Grid->getCell(3)->getStatus() == Cell::CROSSED)
		return true;
	if (m_3x3Grid->getCell(8)->getStatus() == Cell::CROSSED &&
		m_3x3Grid->getCell(7)->getStatus() == Cell::CROSSED &&
		m_3x3Grid->getCell(6)->getStatus() == Cell::CROSSED)
		return true;
	if (m_3x3Grid->getCell(5)->getStatus() == Cell::CROSSED &&
		m_3x3Grid->getCell(4)->getStatus() == Cell::CROSSED &&
		m_3x3Grid->getCell(3)->getStatus() == Cell::CROSSED)
		return true;
	if (m_3x3Grid->getCell(5)->getStatus() == Cell::CROSSED &&
		m_3x3Grid->getCell(4)->getStatus() == Cell::CROSSED &&
		m_3x3Grid->getCell(3)->getStatus() == Cell::CROSSED)
		return true;
	if (m_3x3Grid->getCell(6)->getStatus() == Cell::CROSSED &&
		m_3x3Grid->getCell(4)->getStatus() == Cell::CROSSED &&
		m_3x3Grid->getCell(2)->getStatus() == Cell::CROSSED)
		return true;
	if (m_3x3Grid->getCell(8)->getStatus() == Cell::CROSSED &&
		m_3x3Grid->getCell(4)->getStatus() == Cell::CROSSED &&
		m_3x3Grid->getCell(0)->getStatus() == Cell::CROSSED)
		return true;
	return false;
}

bool Application::isCircleWon()
{
	if (m_3x3Grid->getCell(0)->getStatus() == Cell::CIRCLED &&
		m_3x3Grid->getCell(3)->getStatus() == Cell::CIRCLED &&
		m_3x3Grid->getCell(6)->getStatus() == Cell::CIRCLED)
		return true;
	if (m_3x3Grid->getCell(1)->getStatus() == Cell::CIRCLED &&
		m_3x3Grid->getCell(4)->getStatus() == Cell::CIRCLED &&
		m_3x3Grid->getCell(7)->getStatus() == Cell::CIRCLED)
		return true;
	if (m_3x3Grid->getCell(2)->getStatus() == Cell::CIRCLED &&
		m_3x3Grid->getCell(5)->getStatus() == Cell::CIRCLED &&
		m_3x3Grid->getCell(8)->getStatus() == Cell::CIRCLED)
		return true;
	if (m_3x3Grid->getCell(2)->getStatus() == Cell::CIRCLED &&
		m_3x3Grid->getCell(1)->getStatus() == Cell::CIRCLED &&
		m_3x3Grid->getCell(0)->getStatus() == Cell::CIRCLED)
		return true;
	if (m_3x3Grid->getCell(5)->getStatus() == Cell::CIRCLED &&
		m_3x3Grid->getCell(4)->getStatus() == Cell::CIRCLED &&
		m_3x3Grid->getCell(3)->getStatus() == Cell::CIRCLED)
		return true;
	if (m_3x3Grid->getCell(8)->getStatus() == Cell::CIRCLED &&
		m_3x3Grid->getCell(7)->getStatus() == Cell::CIRCLED &&
		m_3x3Grid->getCell(6)->getStatus() == Cell::CIRCLED)
		return true;
	if (m_3x3Grid->getCell(5)->getStatus() == Cell::CIRCLED &&
		m_3x3Grid->getCell(4)->getStatus() == Cell::CIRCLED &&
		m_3x3Grid->getCell(3)->getStatus() == Cell::CIRCLED)
		return true;
	if (m_3x3Grid->getCell(5)->getStatus() == Cell::CIRCLED &&
		m_3x3Grid->getCell(4)->getStatus() == Cell::CIRCLED &&
		m_3x3Grid->getCell(3)->getStatus() == Cell::CIRCLED)
		return true;
	if (m_3x3Grid->getCell(6)->getStatus() == Cell::CIRCLED &&
		m_3x3Grid->getCell(4)->getStatus() == Cell::CIRCLED &&
		m_3x3Grid->getCell(2)->getStatus() == Cell::CIRCLED)
		return true;
	if (m_3x3Grid->getCell(8)->getStatus() == Cell::CIRCLED &&
		m_3x3Grid->getCell(4)->getStatus() == Cell::CIRCLED &&
		m_3x3Grid->getCell(0)->getStatus() == Cell::CIRCLED)
		return true;
	return false;
}

void Application::onMouseMoved(float mouseX, float mouseY)
{
	m_mousePos = Vector(mouseX, mouseY, 0);
	if (m_currentGState == GAME_MYTURN)
	{
		int cellHovered = m_3x3Grid->onMouseMoved(mouseX, mouseY);
		if (cellHovered != -1)
		{
			if (m_3x3Grid->getCell(cellHovered)->isClickable()) // send the current hovered cell data to the other player so he can see my mouse movement.
			{
				MyPhoton::getInstance().sendCellData(cellHovered, false);
				m_3x3Grid->getCell(cellHovered)->setHovered(true);
			}
		}
	}
	
	if (m_rematchBtn->getSprite()) // is the rematch button appeared?
	{
		if (m_rematchBtn->isHovered(mouseX, mouseY))
			m_rematchBtn->setSprite(m_rematchHText);
		else
			m_rematchBtn->setSprite(m_rematchText);
	}
}

void Application::onMouseButton(int btnIndex, int btnAction)
{
	if (btnIndex == GLFW_MOUSE_BUTTON_LEFT && btnAction == GLFW_PRESS)
	{
		if (m_currentGState == GAME_MYTURN)
		{

			int cellClicked = m_3x3Grid->onClicked(m_mousePos);
			if (cellClicked != -1)
			{
				// the other player's turn.
				MyPhoton::getInstance().switchPlayer();
				MyPhoton::getInstance().sendCellData(cellClicked, true);
				m_3x3Grid->getCell(cellClicked)->setHovered(false);
				setGameState(GAME_WAITING);

				if (MyPhoton::getInstance().isPlayerOne())
					updateGrid(true, cellClicked, PLAYER1);
				else if (MyPhoton::getInstance().isPlayerTwo())
					updateGrid(true, cellClicked, PLAYER2);
			}
		}

		if (m_currentGState == GAME_LOSE || m_currentGState == GAME_WIN)
		{
			if (m_rematchBtn->isHovered(m_mousePos.mVal[0], m_mousePos.mVal[1]))
			{
				MyPhoton::getInstance().sendCellData(10, true); // '10' is for rematch button
				MyPhoton::getInstance().beginMatch();
			}
		}
	}
}


void Application::update(float deltaTime)
{
	
	/* // Logger
	std::cout << "delta time: " << deltaTime << std::endl;
	std::cout << "fps: " << 1 / deltaTime << std::endl;
	*/
}