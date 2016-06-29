#include "listener.h"

using namespace app;
using namespace utility;
using namespace web;
using std::move;
using std::pair;
using std::vector;

app::listener::listener(web::uri address)
	: _listener(address) {
	_listener.support([this](http::http_request req) {
		return this->dispatch(req);
	});
}

pplx::task<void> app::listener::open() {
	return _listener.open();
}

pplx::task<void> app::listener::close() {
	return _listener.close();
}

void app::listener::register_route(string_t route, callback_t callback) {
	_routes.emplace_back(move(route), move(callback));
}

pplx::task<void> app::listener::dispatch(web::http::http_request req) {
	auto path = req.request_uri().path();
	auto query = uri::decode(req.request_uri().query());
	json::value params;

	for (auto&& elem : uri::split_query(query))
		params[elem.first] = json::value(move(elem.second));

	for(auto&& route: _routes)
		if (path.find(route.first) == 0) {
			callback_t callback = route.second;

			return req.extract_json(true)
				.then([](pplx::task<json::value> perv_task) {
					auto data = json::value::null();
					try {
						data = perv_task.get();
					} 
					/* this is fine, since not all request need to send json data */
					catch (...)
						{ /* ignore parsing json data errors from request body */ }
					return data;
				})
				.then([callback, params](json::value body) {
					return callback(params, body);
				})
				/* reply is non-blocking, even if there is an exception */
				.then([req](pplx::task<json::value> perv_task) {
					auto code = web::http::status_codes::OK;
					json::value data;

					try {
						/* note: this is non-blocking because the perv_task is already completed. */
						data = perv_task.get();
					}
					catch (route_exception& e) {
						data = e.data;
						code = e.code;
					}
					catch (std::exception& e) {
						data = json::value(utility::conversions::to_string_t(e.what())); // this is just for debugging
						code = web::http::status_codes::InternalError;
					}
					catch (...) {
						data = json::value(U("ERROR: something went wrong"));
						code = web::http::status_codes::InternalError;
					}

					return req.reply(code, data);
				});
		}

	return req.reply(web::http::status_codes::NotFound, U("ERROR: Route not found"));
}

