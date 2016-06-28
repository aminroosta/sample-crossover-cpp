#pragma once
#include <cpprest/filestream.h>
#include <memory>

namespace app {
	using namespace concurrency::streams;
	using namespace utility;
	using std::make_shared;

	struct utils {
		template<typename AS>
		static concurrency::task<AS> read_file(string_t filename) {
			auto all_lines = make_shared<container_buffer<AS>>();
			return fstream::open_istream(U("config.json")).then([=](istream file) {
				return file.read_to_end(*all_lines);
			}).then([=](size_t) {
				return move(all_lines->collection());
			});
		}

		static concurrency::task<void> write_file(const istream& stream, string_t filename) {
			auto pfile = make_shared<ostream>();
			return
				fstream::open_ostream(filename)
				.then([&stream, pfile](ostream outStream) {
				*pfile = outStream;
				return stream.read_to_end(pfile->streambuf());
			})
				.then([=](size_t) {
				pfile->close();
			});
		}
	};
}