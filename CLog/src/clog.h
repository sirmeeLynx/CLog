#pragma once

#include "logger.h"
#include "sinks/file_sink.h"
#include "sinks/stdout_sink.h"
#include "sinks/stderr_sink.h"
#include "sinks/http_sink.h"
#include "details/topic.h"

namespace CLog
{
	template<template<typename, typename> typename... Sinks>
	using basic_logger = logger<default_config, synchronous_policy, immediate_flush_policy, 
		single_threaded_policy, Sinks...>;

	template<template<typename, typename> typename... Sinks>
	using basic_logger_mt = logger<default_config, synchronous_policy, immediate_flush_policy, 
		multi_threaded_policy, Sinks...>;

	template<template<typename, typename> typename... Sinks>
	using async_logger = logger<default_config, asynchronous_policy<>, immediate_flush_policy, 
		single_threaded_policy, Sinks...>;

	template<template<typename, typename> typename... Sinks>
	using async_logger_mt = logger<default_config, asynchronous_policy<>, immediate_flush_policy, 
		multi_threaded_policy, Sinks...>;

	using console_logger			= basic_logger<sinks::stdout_sink>;
	using console_logger_mt			= basic_logger_mt<sinks::stdout_sink>;
	using file_logger				= basic_logger<sinks::file_sink>;
	using file_logger_mt			= basic_logger_mt<sinks::file_sink>;
	using http_logger_mt			= basic_logger_mt<sinks::http_sink>;

	using async_console_logger		= async_logger<sinks::stdout_sink>;
	using async_console_logger_mt	= async_logger_mt<sinks::stdout_sink>;
	using async_file_logger			= async_logger<sinks::file_sink>;
	using async_file_logger_mt		= async_logger_mt<sinks::file_sink>;
	using async_http_logger_mt		= async_logger_mt<sinks::http_sink>;

	using default_overflow_policy	= block_overflow_policy;
	using default_flush_policy		= immediate_flush_policy;
}

namespace CLog
{
	void init_default_logger();
	void register_logger(logger_ptr logger);
	void set_automatic_registry(bool is_registry_automatic);
	void drop(std::string_view logger_name);
	void drop_all();
	void apply_to_all(const std::function<void(logger_ptr)>& function);
	bool is_registry_automatic();
	const std::unique_ptr<interface::logger>& default_logger();
	logger_ptr get(std::string_view logger_name);

	void set_pattern(std::string_view pattern);
	void set_level_filter(level log_level);

	void start_topic(std::string_view name);
	void end_topic();

	template<typename... Args> void info(const details::log_message& log_msg, Args&&... args);
	template<typename... Args> void warning(const details::log_message& log_msg, Args&&... args);
	template<typename... Args> void error(const details::log_message& log_msg, Args&&... args);
	template<typename... Args> void critical(const details::log_message& log_msg, Args&&... args);
	template<typename... Args> void debug(const details::log_message& log_msg, Args&&... args);
}

#ifndef CLog_DISABLE
#define CLog_INIT_DEFAULT_LOGGER() CLog::init_default_logger()
#else
#define CLog_INIT_DEFAULT_LOGGER()
#endif

#ifndef CLog_DISABLE
#define CLog_SET_PATTERN(pattern) CLog::set_pattern(pattern)
#else
#define CLog_SET_PATTERN()
#endif

#ifndef CLog_DISABLE
#define CLog_SET_LEVEL_FILTER(level) CLog::set_level_filter(level)
#else
#define CLog_SET_LEVEL_FILTER()
#endif

#ifndef CLog_DISABLE
#define CLog_START_TOPIC(topic) CLog::start_topic(topic)
#else
#define CLog_START_TOPIC()
#endif

#ifndef CLog_DISABLE
#define CLog_END_TOPIC() CLog::end_topic()
#else
#define CLog_END_TOPIC()
#endif

#if !defined(CLog_DISABLE) && !defined(CLog_INFO_OFF)
#define CLog_INFO(...) CLog::info(__VA_ARGS__)
#else
#define CLog_INFO()
#endif

#if !defined(CLog_DISABLE) && !defined(CLog_WARNING_OFF)
#define CLog_WARNING(...) CLog::warning(__VA_ARGS__)
#else
#define CLog_WARNING()
#endif

#if !defined(CLog_DISABLE) && !defined(CLog_ERROR_OFF)
#define CLog_ERROR(...) CLog::error(__VA_ARGS__)
#else
#define CLog_ERROR()
#endif

#if !defined(CLog_DISABLE) && !defined(CLog_CRITICAL_OFF)
#define CLog_CRITICAL(...) CLog::critical(__VA_ARGS__)
#else
#define CLog_CRITICAL()
#endif

#if !defined(CLog_DISABLE) && !defined(CLog_DEBUG_OFF)
#define CLog_DEBUG(...) CLog::debug(__VA_ARGS__)
#else
#define CLog_DEBUG()
#endif

#include "clog_impl.h"