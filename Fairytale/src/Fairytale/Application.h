#pragma once

#include "Core.h"


namespace ftale
{

	class FTALE_API Application
	{
	private:


	public:
		Application();
		virtual ~Application();

		void run();
	};


	Application* createApplication();

}