#include "MyPhoton.h"
#include "Application.h"
#include <iostream>

static const ExitGames::Common::JString appId = L"510523b0-ac46-427a-b0c1-87aa5d18b800"; // set your app id here
static const ExitGames::Common::JString appVersion = L"1.0";
static const ExitGames::Common::JString PLAYER_NAME = L"Windows";

//LoadBalancing::Listener& listener, const Common::JString& applicationID, 
//const Common::JString& appVersion, nByte connectionProtocol=Photon::ConnectionProtocol::DEFAULT, bool autoLobbyStats=false, nByte regionSelectionMode=RegionSelectionMode::DEFAULT

// functions
MyPhoton::MyPhoton() : mLoadBalancingClient(*this, appId, appVersion, ExitGames::Photon::ConnectionProtocol::TCP, false, ExitGames::LoadBalancing::RegionSelectionMode::SELECT)
{
}

void MyPhoton::connect(void)
{
	std::wcout<<"Connecting..."<<std::endl;
	mLoadBalancingClient.connect(ExitGames::LoadBalancing::AuthenticationValues(), PLAYER_NAME);
}

void MyPhoton::disconnect(void)
{
	mLoadBalancingClient.disconnect();
}

void MyPhoton::sendMyData(void)
{
	float data[6];
	data[0] = 08.0f;
	data[1] = 31.0f;

	mLoadBalancingClient.opRaiseEvent(true, data, 2, 1);
	std::cout << mLoadBalancingClient.getRoundTripTime() << std::endl;
}

void MyPhoton::sendCellData(int cellHovered, bool clicked)
{
	float data[6];
	data[0] = m_currentPlayerNr;
	data[1] = cellHovered; // '10' is for rematch button
	data[2] = clicked;

	mLoadBalancingClient.opRaiseEvent(true, data, 3, 1);
}

bool MyPhoton::isThisPlayerTurn()
{
	return m_currentPlayerNr == m_thisPlayerNr;
}

bool MyPhoton::isAPlayer()
{
	return m_thisPlayerNr == m_player1Nr || m_thisPlayerNr == m_player2Nr;
}

bool MyPhoton::isPlayerOne()
{
	return m_thisPlayerNr == m_player1Nr;
}

bool MyPhoton::isPlayerTwo()
{
	return m_thisPlayerNr == m_player2Nr;
}

void MyPhoton::beginMatch()
{
	// "Throw dice"
	m_player1Nr = rand() % 12; // reusing one of the not-in-use variable as value container.
	sendCellData(m_player1Nr, false);
	Application::getInstance().setGameState(Application::GAME_READY);
	
	std::cout << "Dice thrown by " << m_thisPlayerNr << " and the value is " << m_player1Nr << std::endl;
}

void MyPhoton::switchPlayer()
{
	if (m_currentPlayerNr == m_player1Nr)
		m_currentPlayerNr = m_player2Nr;
	else if (m_currentPlayerNr == m_player2Nr)
		m_currentPlayerNr = m_player1Nr;
}

void MyPhoton::run(void)
{
	mLoadBalancingClient.service();
}

// protocol implementations

void MyPhoton::debugReturn(int debugLevel, const ExitGames::Common::JString& string)
{
}

void MyPhoton::connectionErrorReturn(int errorCode)
{
	std::wcout<<"connectionErrorReturn : "<<errorCode<<std::endl;
}

void MyPhoton::clientErrorReturn(int errorCode)
{
	std::wcout<<"clientErrorReturn : "<<errorCode<<std::endl;
}

void MyPhoton::warningReturn(int warningCode)
{
	std::wcout<<"warningReturn : "<<warningCode<<std::endl;
}

void MyPhoton::serverErrorReturn(int errorCode)
{
	std::wcout<<"serverErrorReturn : "<<errorCode<<std::endl;
}

void MyPhoton::joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player)
{
	std::wcout << "joinRoomEventAction" << std::endl;
	// std::cout << "Current total players in the server: " << playernrs.getSize() << std::endl;

	if (m_thisPlayerNr == -1) // this user just joined the room.
	{
		m_thisPlayerNr = playerNr; 
		std::wcout << "Welcome, Your unique player ID in this server is " << m_thisPlayerNr << std::endl;
		Application::getInstance().setGameState(Application::GAME_WAITING);
	}

	if (playernrs.getSize() == 2)
		beginMatch();
}

void MyPhoton::leaveRoomEventAction(int playerNr, bool isInactive)
{
	std::wcout<<"leaveRoomEventAction"<<std::endl;
}

void MyPhoton::customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent)
{
	float* data = ExitGames::Common::ValueObject<float*>(eventContent).getDataCopy();
	if (data)
	{
		if (Application::getInstance().getGameState() == Application::GAME_READY)
		{
			if (m_player1Nr == data[1])
			{
				// "Throw dice" again.
				m_player1Nr = rand() % 12;
				sendCellData(m_player1Nr, false);
				return;
			}

			if (m_player1Nr > data[1])
			{
				m_player1Nr = m_thisPlayerNr;
				m_player2Nr = data[0];
			}
			else
			{
				m_player1Nr = data[0];
				m_player2Nr = m_thisPlayerNr;
			}
			m_currentPlayerNr = m_player1Nr; // Player 1 starts the round first and uses cross sign.
			Application::getInstance().setGameState(Application::GAME_START);
		}
		else if (Application::getInstance().getGameState() == Application::GAME_MYTURN || Application::getInstance().getGameState() == Application::GAME_WAITING)
		{
			// the game is running
			if ((int)data[2] == 1) // a valid cell is clicked
			{
				switchPlayer();
				if (isThisPlayerTurn())
					Application::getInstance().setGameState(Application::GAME_MYTURN);
				else
					Application::getInstance().setGameState(Application::GAME_WAITING);

				if (data[0] == m_player1Nr)
					Application::getInstance().updateGrid(true, data[1], Application::PLAYER1);
				else if (data[0] == m_player2Nr)
					Application::getInstance().updateGrid(true, data[1], Application::PLAYER2);
			}
			else
			{
				//std::cout << data[0] << ", " << m_currentPlayerNr << ", " << m_player1Nr << ", " << m_player2Nr << std::endl;
				// a valid cell is hovered (the following code is bugged)
				if (data[0] == m_player1Nr)
				{
					Application::getInstance().updateGrid(false, data[1], Application::PLAYER1);
				}
				else if (data[0] == m_player2Nr)
				{
					Application::getInstance().updateGrid(false, data[1], Application::PLAYER2);
				}
			}
		}
		else
		{
			if (data[1] = 10 && ((int)data[2] == 1)) // rematch button is clicked by the other player, get ready.
			{
				beginMatch();
			}
		}
	}
	else
		std::cout << "invalid data" << std::endl;
}

void MyPhoton::connectReturn(int errorCode, const ExitGames::Common::JString& errorString, const ExitGames::Common::JString& cluster)
{
	if(errorCode)
		std::wcout<<"Failed to connect : "<<errorString.cstr()<<std::endl;
	else
	{
		std::wcout<<"Connected to Photon Server."<<std::endl;
		
		std::wcout<<"Trying to join a random room..."<<std::endl;
		mLoadBalancingClient.opJoinRandomRoom();
	}
}

void MyPhoton::disconnectReturn(void)
{
	std::wcout<<"disconnected"<<std::endl;
}

void MyPhoton::createRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	std::wcout<<"createRoomReturn"<<std::endl;
}

void MyPhoton::joinOrCreateRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	std::wcout<<"joinOrCreateRoomReturn"<<std::endl;
}

void MyPhoton::joinRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	std::wcout<<"joinRoomReturn"<<std::endl;
}

void MyPhoton::joinRandomRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	std::wcout<<"joinRandomRoomReturn"<<std::endl;

	if(errorCode)
	{
		std::wcout<<"Failed to join a random room : "<<errorCode<<", "<<errorString.cstr()<<std::endl;

		if(errorCode == 32760) //no match found error code
		{
			std::wcout<<"Creating a room..."<<std::endl;
			//try to create a room and wait;
			ExitGames::Common::JString name;
			name += GETTIMEMS();
			mLoadBalancingClient.opCreateRoom(name, ExitGames::LoadBalancing::RoomOptions().setMaxPlayers(20));
		}
		
		return;
	}
}

void MyPhoton::leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString)
{
	std::wcout<<"leaveRoomReturn"<<std::endl;
}

void MyPhoton::joinLobbyReturn(void)
{
	std::wcout<<"joinLobbyReturn"<<std::endl;
}

void MyPhoton::leaveLobbyReturn(void)
{
	std::wcout<<"leaveLobbyReturn"<<std::endl;
}

void MyPhoton::onLobbyStatsResponse(const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStatsResponse>& lobbyStats)
{
	std::wcout<<"onLobbyStatsResponse"<<std::endl;
}

void MyPhoton::onLobbyStatsUpdate(const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStatsResponse>& lobbyStats)
{
	std::wcout<<"onLobbyStatsUpdate"<<std::endl;
}

void MyPhoton::onAvailableRegions(const ExitGames::Common::JVector<ExitGames::Common::JString>& availableRegions, const ExitGames::Common::JVector<ExitGames::Common::JString>& availableRegionServers)
{
	//std::wcout<<availableRegions[0]<<std::endl;
	//std::wcout<<availableRegions[1]<<std::endl;
	//mLoadBalancingClient.selectRegion(availableRegions[0]);
	mLoadBalancingClient.selectRegion("asia");
}
