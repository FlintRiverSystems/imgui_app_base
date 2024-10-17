
#include <boost/phoenix/bind.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>

#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>

#include "boost_log.h"

using namespace std;
using boost::dynamic_bitset;
using namespace boost::log;
namespace expr = boost::log::expressions;

/* needed for filter, defines tag::channel_attr */
BOOST_LOG_ATTRIBUTE_KEYWORD(channel_attr, "Channel", boost::dynamic_bitset<>)

static dynamic_bitset<> enabled_channels;
static dynamic_bitset<> defined_channels;
static dynamic_bitset<> undefined_channels;

bool console_filter(
	boost::log::value_ref< boost::log::trivial::severity_level, boost::log::trivial::tag::severity > const& level,
	boost::log::value_ref< boost::dynamic_bitset<>, tag::channel_attr > const& channel)
{
	return true;
	return level >= lvl::info; /* only show info or higher on console */
}

bool log_file_filter(
	boost::log::value_ref< boost::log::trivial::severity_level, boost::log::trivial::tag::severity > const& level,
	boost::log::value_ref< boost::dynamic_bitset<>, tag::channel_attr > const& channel)
{
	return true;

	if (channel.empty() || channel.get().none() || channel.get().intersects(enabled_channels))
		return true;

	/* allow unknown channels */
	if (channel.get().intersects(undefined_channels)) return true;

	return false;
}

inline std::string ChannelLookup(const value_ref<boost::dynamic_bitset<>>& id)
{
	if (id.empty()) return "UNDEF";

	if (id.get().none()) return "NONE";

	return LogChannelIdMap::singleton().to_string(id.get());
}

void setup_boost_log()
{
	add_common_attributes();
	register_simple_formatter_factory< trivial::severity_level, char>("Severity");

	LogChannelIdMap::singleton().set_max_bitfield_size(128);

	/* global filter */
	core::get()->set_filter (trivial::severity >= trivial::trace);
		/* for example - this currently does no filtering  because everything is at least trace */

	defined_channels = LogChannelIdMap::singleton().full_bitmask();
	undefined_channels = ~defined_channels;

	enabled_channels = LogChannelIdMap::singleton().generate_bitmask({ "-LOG3" });

	/*** setup write to console ***/
	/******************************/
	auto cl = add_console_log
	(
		std::cout,
		keywords::auto_flush = true
	);

	cl->set_filter(boost::phoenix::bind(&console_filter, trivial::severity.or_none(), channel_attr.or_none()));

	const auto CHANNEL_NAME_WIDTH = LogChannelIdMap::singleton().max_string_width();

	constexpr auto SEVERITY_NAME_WIDTH = 7;

	auto formatter = expr::stream 
			<< expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f")
			<< " : " 
			<< "[P:" << expr::attr<attributes::current_process_id::value_type>("ProcessID") << "] "
			<< "[T:" << expr::attr<attributes::current_thread_id::value_type>("ThreadID") << "] "
			<< "[" << std::setw(CHANNEL_NAME_WIDTH) << left << boost::phoenix::bind(&ChannelLookup, expr::attr<boost::dynamic_bitset<>>("Channel")) << "] "
			<< "<" << std::setw(SEVERITY_NAME_WIDTH) << left << trivial::severity << "> "
			<< expr::message;

	cl->set_formatter(formatter);

	/***  setup write to rolling log file ***/
	/****************************************/

	auto fl = add_file_log 
	(
		keywords::file_name = "log_%Y%m%d.log",
		keywords::open_mode = ios_base::app,
		keywords::auto_flush = true,
		keywords::time_based_rotation = sinks::file::rotation_at_time_point(0,0,0)
	);

	fl->set_filter(boost::phoenix::bind(&log_file_filter, trivial::severity.or_none(), channel_attr.or_none()));

	auto formatter2 = expr::stream 
			<< expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f")
			<< " : " 
			<< "[P:" << expr::attr<attributes::current_process_id::value_type>("ProcessID") << "] "
			<< "[T:" << expr::attr<attributes::current_thread_id::value_type>("ThreadID") << "] "
			<< "[" << std::setw(CHANNEL_NAME_WIDTH) << left << boost::phoenix::bind(&ChannelLookup, expr::attr<boost::dynamic_bitset<>>("Channel")) << "] "
			<< "<" << std::setw(SEVERITY_NAME_WIDTH) << left << trivial::severity << "> "
			<< expr::message;

	fl->set_formatter(formatter2);
}
