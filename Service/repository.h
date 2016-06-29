#pragma once
#include<atomic>
#include "utils.h"
#include "users.h"

namespace app {

	/* singleton repository to manage users */
	struct repository {
		static repository& instance(bool clear_data = true);
		
	// returned task is cahced and only updates when the data is invalidated.
	pplx::task<std::vector<user>> get_users();

	utility::string_t json_filename; // users.json
	private:
		repository(bool clear_data);
		repository(const repository&) = delete;
		repository(repository&&) = delete;

		std::atomic<bool> invalidated; // indicate whether cached users are invalidated or not.
		pplx::task<void> write_dummy_users();
	};
}
