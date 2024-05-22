#include "Benchmark.h"

#include "CLog.h"

int main()
{
	using logger_config = CLog::configuration<
		CLog::enable_time,
		CLog::disable_local_time,
		CLog::disable_precise_units,
		CLog::disable_thread_id, 
		CLog::disable_process_id
	>;

	auto console = std::make_shared<
		CLog::logger<
			logger_config,
			CLog::asynchronous_policy<
				CLog::default_async_queue_size,
				CLog::default_overflow_policy
			>,
			CLog::buffered_flush_policy<>,
			CLog::single_threaded_policy,
			CLog::sinks::stdout_sink
		>
	>("CONSOLE");

	console->set_level_filter(CLog::level::info | CLog::level::debug | CLog::level::critical);
	console->set_pattern(".red([%T] [%n]) .dark_green([%l]): .cyan(%v) TEXT");

	{
		Timer timer("timer");
		console->critical("First critical message");
	}

	return 0;
}
