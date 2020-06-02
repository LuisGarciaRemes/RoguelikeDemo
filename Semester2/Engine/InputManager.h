#pragma once
#include "ConsolePrint.h"
#include "Delegates.h"
#include "SmartPointer.h"
#include "GameObject.h"
#include "../Exports/GLib/GLib.h"
#include "Physics.h"

#define SPACEKEY 0x0020
#define ESCKEY 0x001B
class InputManager
{
public:
	InputManager();
	~InputManager();
	bool a, s, d, w, esc, space = false;

	//Callback method
	void TestKeyCallback(unsigned int i_VKeyID, bool bWentDown)
	{
		if (i_VKeyID == 'A' && bWentDown)
		{
			a = true;
		}
		else if (i_VKeyID == 'A' && !bWentDown)
		{
			a = false;
		}

		if (i_VKeyID == 'S'&& bWentDown)
		{
			s = true;
		}
		else if(i_VKeyID == 'S'&& !bWentDown)
		{
			s = false;
		}

		if (i_VKeyID == 'D' && bWentDown)
		{
			d = true;
		}
		else if (i_VKeyID == 'D' && !bWentDown)
		{
			d = false;
		}

		if (i_VKeyID == 'W'&& bWentDown)
		{
			w = true;
		}
		else if (i_VKeyID == 'W'&& !bWentDown)
		{
			w = false;
		}

		if (i_VKeyID == SPACEKEY && bWentDown)
		{
			space = true;
		}

		if (i_VKeyID == ESCKEY && bWentDown)
		{
			DEBUG_PRINT("yup");
			esc = true;
		}
	}

	void SetFireTrigger()
	{
		space = false;
	}
};


namespace Engine {

	namespace Input {
		typedef unsigned int button_t;
		typedef Delegate<button_t, bool>	ButtonChangeReceiver_t;

		extern MultiCastDelegate<button_t, bool>	ButtonChangeReceivers;

		bool Init();
	} // namespace Input
} // namespace Engine
