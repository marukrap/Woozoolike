#include "Application.hpp"
#include "Strings.hpp"

int main()
{
	// initialize Strings
	Strings::load(Language::Korean);

	Application app;
	app.run();

	return 0;
}
