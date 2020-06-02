// Semester2.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "Semester2.h"
#include "../Exports/GLib/GLib.h"
#include "../Engine/Timing.h"
#include "../Engine/Physics.h"
#include "../Engine/SystemFunctions.h"
#include <Windows.h>
#include "../Engine/Collider.h"
#include "../Engine/RenderingSystem.h"
#include "../Engine/ConsolePrint.h"
#include "../Engine/InputManager.h"


#if defined _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif // _DEBUG

bool gameOver = false;

const float X_BOUNDS = 1024;
const float Y_BOUNDS = 770;
bool bQuit;

SmartPointer<GameObject> player;
std::vector<Collider> colliderList;
std::vector<SmartPointer<GameObject>> projectileList;
std::vector<SmartPointer<GameObject>> objectList;
std::vector<SmartPointer<GameObject>> envObjectList;
std::vector<GLib::Sprites::Sprite*> spriteList;

GLib::Sprites::Sprite * pBG;
GLib::Sprites::Sprite * pWin;
GLib::Sprites::Sprite * pLose;

void LoadObjects();
void LoadSprites();
void ReleaseSprites();
void RenderUI();
SmartPointer<GameObject> ReturnFirstDisabled(std::vector<SmartPointer<GameObject>> objectList);
void UpdateControls();
void ReleaseMemomry();
InputManager inputManager;


int WINAPI wWinMain(HINSTANCE i_hInstance, HINSTANCE i_hPrevInstance, LPWSTR i_lpCmdLine, int i_nCmdShow)
{

	//Test collisons
	//CollisionsSys::CollisionPoolCheck();

	// IMPORTANT: first we need to initialize GLib
	bool bSuccess = GLib::Initialize(i_hInstance, i_nCmdShow, "Semester2Game", -1, 1024, 770);


	if (bSuccess)
	{

		//new input using delagates
		//Creates clock and initializes dT
		Timing time = Timing::Timing();
		double dT = 0;

		//stablizes dT;
		time.RunTimer(dT);
		while (dT > 1.0) {
			time.RunTimer(dT);
		}

		LoadObjects();		
		// Creates objectList and colliderList
		Engine::Input::Init();
		Engine::Input::ButtonChangeReceivers.AddDelegate(
			Engine::Input::ButtonChangeReceiver_t::Create<InputManager, &InputManager::TestKeyCallback>(&inputManager)
		);

		// Create a couple of sprites for UI that are not associated with a gameobject
		LoadSprites();

		bQuit = false;
	
		do
		{
			//Runs the clock
			time.RunTimer(dT);
			// IMPORTANT: We need to let GLib do it's thing. 
			GLib::Service(bQuit);

			if (!bQuit)
			{			
				//Updates forces on the player based on input
				UpdateControls();

				//Updates position of the gameobjects and their colliders
				Physics::UpdatePhysics(objectList, .78f, dT, X_BOUNDS, Y_BOUNDS);
				Physics::UpdatePhysics(projectileList, .50f, dT, X_BOUNDS, Y_BOUNDS);
				CollisionsSys::Update(colliderList, dT);			

				// IMPORTANT: Tell GLib that we want to start rendering
				GLib::BeginRendering();
				// Tell GLib that we want to render some sprites
				GLib::Sprites::BeginRendering();	

				RenderingSystem::RenderSprites(objectList);
				RenderingSystem::RenderSprites(envObjectList);
				RenderingSystem::RenderSprites(projectileList);
				RenderUI();

				// Tell GLib we're done rendering sprites
				GLib::Sprites::EndRendering();
				// IMPORTANT: Tell GLib we're done rendering
				GLib::EndRendering();


				if (!player->GetState())
				{
					gameOver = true;
				}
				else
				{
					bool check = true;
					for (auto itA = objectList.begin() + 1; itA != objectList.end(); itA++)
					{
						if ((*itA)->GetState())
						{
							check = false;
						}
					}
					gameOver = check;
				}

				bQuit = inputManager.esc;
			}
		} while (bQuit == false);

		ReleaseSprites();

		// IMPORTANT:  Tell GLib to shutdown, releasing resources.
		GLib::Shutdown();
	}

	ReleaseMemomry();

#if defined _DEBUG
	_CrtDumpMemoryLeaks();
#endif // _DEBUG

}

//Loads objects into colliderlist
void LoadObjects()
{
	player = SystemFunctions::CreateGameObjectSmartPointer("Player.lua");
	objectList = SystemFunctions::CreateMulitpleGameObjectSmartPointers("Enemy.lua");
	envObjectList = SystemFunctions::CreateMulitpleGameObjectSmartPointers("Maze.lua");
	projectileList = SystemFunctions::CreateMulitpleGameObjectSmartPointers("Projectiles.lua");

	objectList.insert(objectList.begin(), player);

	for (auto itA = projectileList.begin(); itA != projectileList.end(); itA++)
	{
		if ((*itA))
		{
			colliderList.push_back(Collider((*itA), 14.0f, 14.0f));
		}
	}

	for (auto itA = objectList.begin(); itA != objectList.end(); itA++)
	{
		if ((*itA))
		{
			colliderList.push_back(Collider((*itA), (*itA)->GetDimensions().x(), (*itA)->GetDimensions().y()));
		}
	}

	for (auto itA = envObjectList.begin(); itA != envObjectList.end(); itA++)
	{
		if ((*itA))
		{
			colliderList.push_back(Collider((*itA), (*itA)->GetDimensions().x(), (*itA)->GetDimensions().y()));
		}
	}
}

//loads sprites not attached to gameobjects
void LoadSprites()
{
	pBG = RenderingSystem::CreateSprite("Assets\\Sprites\\Background.dds");
	pWin = RenderingSystem::CreateSprite("Assets\\Sprites\\Youwin.dds");
	pLose = RenderingSystem::CreateSprite("Assets\\Sprites\\Youlose.dds");

	spriteList.push_back(pBG);	
	spriteList.push_back(pWin);
	spriteList.push_back(pLose);
}

//Releases all sprites
void ReleaseSprites()
{
	for (auto itA = spriteList.begin(); itA != spriteList.end(); itA++)
	{
		if (*(itA))
		{
				GLib::Sprites::Release(*(itA));
		}
	}

	for (auto itA = objectList.begin(); itA != objectList.end(); itA++)
	{
		if ((*itA))
		{
			GLib::Sprites::Release((*itA)->GetSprite());
		}
	}

	for (auto itA = envObjectList.begin(); itA != envObjectList.end(); itA++)
	{
		if ((*itA))
		{
			GLib::Sprites::Release((*itA)->GetSprite());
		}
	}

	for (auto itA = projectileList.begin(); itA != projectileList.end(); itA++)
	{
		if ((*itA))
		{
			GLib::Sprites::Release((*itA)->GetSprite());
		}
	}
}

//renders the sprites not attached to gameobjects
void RenderUI()
{
	if (gameOver && player->GetState())
	{
		GLib::Sprites::RenderSprite(*pWin, GLib::Point2D{ 0.0f, -370.0f }, 0.0f);
	}
	else if (gameOver && !player->GetState())
	{
		GLib::Sprites::RenderSprite(*pLose, GLib::Point2D{ 0.0f, -370.0f }, 0.0f);
	}

	GLib::Sprites::RenderSprite(*pBG, GLib::Point2D{ 0.0f, -320.0f }, 0.0f);

}

//returns the first disabled object in the list
SmartPointer<GameObject> ReturnFirstDisabled(std::vector<SmartPointer<GameObject>> objectList)
{
	for (auto itA = objectList.begin(); itA != objectList.end(); itA++)
	{
		if (!(*itA)->GetState())
		{
			return *itA;
		}
	}
	return nullptr;
}

//updates the controls of the game based on the inputmanager
void UpdateControls()
{
	float xForce = player->GetAcceleration().x()*player->GetMass();
	float yForce = player->GetAcceleration().y()*player->GetMass();

	if (inputManager.a)
	{
		xForce += -150000.0f;
	}

	if (inputManager.s)
	{
		yForce += -150000.0f;		
	}

	if (inputManager.d)
	{
		xForce += 150000.0f;	
	}

	if (inputManager.w)
	{
		yForce += 150000.0f;	
	}

	Physics::ApplyForce(player, Point2D(xForce, yForce));

	if (inputManager.space && player->GetState())
	{
		SmartPointer<GameObject> temp = ReturnFirstDisabled(projectileList);
		if (temp)
		{
			temp->UpdateVelocity(player->GetVelocity());

			float xForce = 0.0f;
			float yForce = 0.0f;

			temp->UpdatePosition(player->GetPosition());

			if (inputManager.a)
			{
				xForce = -600;
			}

			if (inputManager.w)
			{
				yForce = 600;
			}

			if (inputManager.d)
			{
				xForce = 600;
			}

			if (inputManager.s)
			{
				yForce = -600;
			}

			temp->SetState(true);
			Physics::ApplyForce(temp, Point2D(xForce, yForce));
		}
	}
	inputManager.SetFireTrigger();
}

//releases all memory used
void ReleaseMemomry()
{
	player.~SmartPointer();
	colliderList.clear();
	colliderList.~vector();
	objectList.clear();
	objectList.~vector();
	envObjectList.clear();
	envObjectList.~vector();
	spriteList.clear();
	spriteList.~vector();
	projectileList.clear();
	projectileList.~vector();
	inputManager.~InputManager();
	Engine::Input::ButtonChangeReceivers.~MultiCastDelegate();
}

