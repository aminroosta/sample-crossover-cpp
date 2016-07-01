#include "utils.h"
#include <cpprest/filestream.h>
#include <fstream>

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
