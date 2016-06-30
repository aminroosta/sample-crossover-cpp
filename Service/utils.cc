#include "utils.h"

namespace app {
	using namespace concurrency::streams;
	using namespace concurrency;
	using namespace utility;
	using namespace std::chrono;
	using std::make_shared;
	using std::move;

	concurrency::task<utility::string_t> utils::read_file(const string_t filename)
	{
		return fstream::open_istream(filename).then([=](concurrency::streams::istream file) {
			return file.extract<utility::string_t>();
		});
	}

	task<void> utils::write_file(const utility::string_t content, const utility::string_t filename) {
		auto pfile = make_shared<ostream>();
		return
			fstream::open_ostream(filename)
			.then([content, pfile](ostream outStream) {
			*pfile = outStream;
			return outStream.print(content);
		}).then([=](size_t) {
			return pfile->close();
		});
	}

	task<void> utils::write_file(const istream& stream, const string_t filename) {
		auto pfile = make_shared<ostream>();
		return
			fstream::open_ostream(filename)
			.then([&stream, pfile](ostream outStream) {
			*pfile = outStream;
			return stream.read_to_end(pfile->streambuf());
		}).then([=](size_t) {
			return pfile->close();
		});
	}

	utility::string_t utils::time_point_to_string(const std::chrono::system_clock::time_point & tp) {
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
}
