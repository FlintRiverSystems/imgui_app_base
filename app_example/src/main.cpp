
#include "app_example.h"

using namespace std;

int main (int argc, char** argv)
{
	app_example app;
	if (!app.init("imgui_app demo", 1024,768)) return -1;

	app.run();

	return 0;
}


