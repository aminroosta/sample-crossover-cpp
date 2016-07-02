#include "utils.h"
#include <cpprest/filestream.h>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <chrono>
#include <agents.h>

using namespace utility;
using namespace concurrency::streams;
using namespace concurrency;
using namespace std::chrono;
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

string_t time_point_to_string(const system_clock::time_point & tp) {
	auto ttime_t = system_clock::to_time_t(tp);
	auto tp_sec = system_clock::from_time_t(ttime_t);
	milliseconds ms = duration_cast<milliseconds>(tp - tp_sec);

	std::tm * ttm = localtime(&ttime_t);

	char date_time_format[] = "%Y.%m.%d-%H.%M.%S";

	char time_str[] = "yyyy.mm.dd.HH-MM.SS.fff";

	strftime(time_str, strlen(time_str), date_time_format, ttm);

	std::string result(time_str);
	result.append(".");
	result.append(std::to_string(ms.count()));

	return utility::conversions::to_string_t(result);
}

/*----------------------- use opencv to take a picture ------------------*/
pplx::task<string_t> try_take_picture() {
	return pplx::task<string_t>([]() {
		string_t wfilename = time_point_to_string(system_clock::now()) + U(".jpg");
		std::string filename(wfilename.begin(), wfilename.end());
		try {
			cv::VideoCapture cap(0);
			if (cap.isOpened()) {
				cv::Mat frame;
				cap >> frame;
				cv::imwrite(filename.c_str(), frame);
				cap.release();
			}
		}
		catch (std::exception& e) {
			auto msg = e.what();
			return string_t(U("Failed to take a picture from webcam!"));
		}
		return string_t(U("Webcam image saved (")) + wfilename + U(")");
	});
}
