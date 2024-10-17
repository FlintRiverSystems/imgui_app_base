
#include "app_example.h"
#include "boost_log.h"

using namespace std;

static logger_t lg (boost::log::keywords::channel = LogChannelIdMap::singleton().id(LogChannels::MAIN()));

int main (int argc, char** argv)
{
	setup_boost_log();

	BOOST_LOG_SEV(lg, lvl::info) << "Startup";

	app_example app;
	if (!app.init("imgui_app demo", 1024,768)) return -1;

	app.run();

	return 0;
}


