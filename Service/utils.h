#pragma once
#include <cpprest/filestream.h>
#include <cpprest/http_client.h>
#include <memory>
#include <string>

namespace app {
	struct utils {
		static concurrency::task<std::string> read_file(utility::string_t filename);
		static concurrency::task<void> write_file(const utility::string_t content, const utility::string_t filename);
		static concurrency::task<void> write_file(const concurrency::streams::istream& stream, utility::string_t filename);
	};
}