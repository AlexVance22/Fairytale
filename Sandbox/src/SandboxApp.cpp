#include "Fairytale.h"


class Sandbox : public ftale::Application
{
private:


public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};


ftale::Application* ftale::createApplication()
{
	return new Sandbox();
}