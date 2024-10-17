#pragma once

#include <string>
#include <vector>

#include <frs_utility_cpp/string_bitfield_map.h>

/* defined names for log channels
 *
 * Note: this does not define an enum,
 * just a set of functions that return the name as a string
 *
 */

struct LogChannels
{
	/* define channels */
	#define CHANNEL_LIST(FN) \
		FN(MAIN) \
		FN(TEST)

	/* these must be functions, not variables, so they're available in global static oontext */
	#define DEFINE_CHANNEL(NAME) static std::string NAME () { return #NAME; }

	CHANNEL_LIST(DEFINE_CHANNEL)

	#define CHANNEL_COUNT(x) count++;

	static unsigned ChannelCount()
	{
		unsigned count = 0;
		CHANNEL_LIST(CHANNEL_COUNT)
		return count;
	}

	#define LIST_OF_CHANNEL(NAME) result.push_back(#NAME);

	static std::vector<std::string> ChannelList()
	{
		std::vector<std::string> result;
		CHANNEL_LIST(LIST_OF_CHANNEL)
		return result;
	}

	#undef CHANNEL_LIST
	#undef DEFINE_CHANNEL
	#undef CHANNEL_COUNT
	#undef LIST_OF_CHANNEL
};


/* class to define bitfield values for channel string names */

class LogChannelIdMap : public FRS::utility::string_bitfield_map
{
public:

	static LogChannelIdMap& singleton ()
	{
		static LogChannelIdMap instance;

		if (instance.size() == 0)
		{
			for (auto& name : LogChannels::ChannelList())
				instance.id(name);
		}

		return instance;
	}
};

