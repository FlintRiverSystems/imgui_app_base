#pragma once

#include "LogChannels.h"

#include <boost/dynamic_bitset.hpp>

/* define a logger that takes a channel and a severity level */
#include <boost/log/trivial.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>

//using logger_t = boost::log::sources::severity_channel_logger_mt<boost::log::trivial::severity_level, unsigned int>;
using logger_t = boost::log::sources::severity_channel_logger_mt<boost::log::trivial::severity_level, boost::dynamic_bitset<>>;
using lvl = boost::log::trivial::severity_level;

extern void setup_boost_log();

//static logger_t lg (boost::log::keywords::channel = LogChannelIdMap::singleton().id(LogChannels::MAIN()));
//BOOST_LOG_SEV(lg, lvl::info) << "message";

