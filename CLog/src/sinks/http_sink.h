#include "sink.h"
#include "details/stream_writer.h"
#include "../configuration.h"

namespace CLog::sinks
{
	template<typename FlushPolicy, typename ThreadingPolicy>
	class http_sink
		: public sink<true, ThreadingPolicy>
		, private details::stream_writer<FlushPolicy>
	{
	private:
		using sink_t = sink<true, ThreadingPolicy>;

	public:
		http_sink();
		void sink_it(const details::record_base& record) override;
	};

	template<typename FlushPolicy, typename ThreadingPolicy>
	http_sink<FlushPolicy, ThreadingPolicy>::http_sink()
		: details::stream_writer<FlushPolicy>(stdout)
	{}

	template<typename FlushPolicy, typename ThreadingPolicy>
	void http_sink<FlushPolicy, ThreadingPolicy>::sink_it(const details::record_base& record)
	{
		sink_t::m_current_level = record.log_level;
        std::string message = "Writing the message [" + std::string(record.message) + "] into a distributed log service at [https://mock-log-server.com]...";
        std::string_view new_message(message.c_str());
        auto new_record = details::record<default_config>(message, record.log_level, record.meta);
		details::stream_writer<FlushPolicy>::write(sink_t::m_pattern.compile(std::move(new_record)));
	}
}