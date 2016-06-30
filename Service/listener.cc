#include "listener.h"

using namespace app;
using namespace utility;
using namespace web;
using namespace web::http;
using std::move;
using std::pair;
using std::vector;

app::listener::listener(web::uri address)
	: _listener(address), auth(authorize::instance()) {
	_listener.support([this](http::http_request req) {
		return this->dispatch(req);
	});

	/* GET: /authorize?name=name&password=password */
	register_route(U("/authorize"), [this](json::value params, json::value) {
		if (!params[U("name")].is_string() || !params[U("password")].is_string())
			throw route_exception(U("bad api call, call it like this: /authorize?name=name&password=password"), status_codes::BadRequest);
		auto name = params[U("name")].as_string();
		auto password = params[U("password")].as_string();

		auto token = auth.login(name, password).get();
		if(token.empty())
			throw route_exception(U("username or password invalid"), status_codes::Unauthorized);
		json::value ret;
		ret[U("token")] = json::value(token);
		return ret;
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

	if (path.find(U("/unauthorized")) != 0 && path.find(U("/authorize")) != 0 ) {
		/* check if the user is authenticated or not */
		if (!params[U("token")].is_string())
			return req.reply(web::http::status_codes::Unauthorized, U("ERROR: Use /authorize api to login and use the given token"));

		auto token = params[U("token")].as_string();
		if(auth.validate_token(token) == false)
			return req.reply(web::http::status_codes::Unauthorized, U("ERROR: token is invalid or expired"));
	}

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

