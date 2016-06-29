#include "utils.h"

namespace app {
	using namespace concurrency::streams;
	using namespace concurrency;
	using namespace utility;
	using std::make_shared;
	using std::move;

	concurrency::task<std::string> utils::read_file(const string_t filename)
	{
		typedef container_buffer<std::string> string_buffer;
		auto all_lines = std::make_shared<string_buffer>();

		return fstream::open_istream(filename)
			.then([=](concurrency::streams::istream file) {
			return file.read_to_end(*all_lines);
		}).then([=](size_t) {
			return std::move(all_lines->collection());
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
}
