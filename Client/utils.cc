#include "utils.h"
#include <cpprest/filestream.h>
#include <fstream>
#include <agents.h>

using namespace utility;
using namespace concurrency::streams;
using namespace concurrency;
using std::move;
using web::json::value;

#define CONFIG_JSON "config.json"

pplx::task<value> get_app_config() {

	auto file = std::ifstream(CONFIG_JSON);
	if (!file) {
		value ret;
		ret[U("url")] = value(U("http://127.0.0.1:3000"));
		ret[U("name")] = value(U("jack"));
		ret[U("password")] = value(U("jackpassword"));
		auto content = ret.to_string();

		/* write to disk and return default user - pass*/
		auto pfile = std::make_shared<ostream>();
		return fstream::open_ostream(U(CONFIG_JSON))
			.then([content, pfile](ostream stream) {
				*pfile = stream;
				return stream.print(content);
			}).then([=](size_t) {
				return pfile->close();
			}).then([ret] {
				return ret;
			});
	}

	/* read the config from disk */
	return fstream::open_istream(U(CONFIG_JSON))
		.then([](istream stream) {
			return stream.extract<string_t>();
		})
		.then([](string_t content) {
			return web::json::value::parse(content);
		});
}

pplx::task<void> complete_after(unsigned int timeout)
{
	task_completion_event<void> tce;

	auto fire_once = new timer<int>(timeout, 0, nullptr, false);
	auto callback = new call<int>([tce](int) {
		tce.set();
	});

	// Connect the timer to the callback and start the timer.
	fire_once->link_target(callback);
	fire_once->start();

	task<void> event_set(tce);

	return event_set.then([callback, fire_once]() {
		delete callback;
		delete fire_once;
	});
}
