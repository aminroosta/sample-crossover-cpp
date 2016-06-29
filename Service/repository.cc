#include "repository.h"
#include <mutex>
#include <fstream>

using namespace app;
using namespace pplx;
using namespace utility;
using namespace concurrency::streams;
using std::vector;

repository& repository::instance(bool clear_data /* = true */ ) {
	static repository repo(clear_data);
	return repo;
}

/* we will cache the result of users.json untill the content is invaliated, (another taks updates the file) */
task<std::vector<user>> app::repository::get_users() {
	static task<vector<user>> cached_task;
	static std::mutex mtx;
	if (invalidated == true) {
		std::lock_guard<std::mutex> guard(mtx);

		/* check to prevent other treads update an already updated task */
		if (invalidated == true) {
			/* tasks behave like shared_ptrs, so we are safe here */
			cached_task = utils::read_file(this->json_filename).then([](string_t content) {
				auto json_content = web::json::value::parse(content);
				return serializeable<vector<user>>::parse(json_content);
			});
			cached_task.wait();
			invalidated = false;
		}
	}
	return cached_task;
}

task<void> app::repository::write_dummy_users() {
	/* create dummy users */
	auto users = user::create_initial_users();
	auto users_json = serializeable<vector<user>>::serialize(users);
	return utils::write_file(users_json.to_string(), this->json_filename);
}

#define USERS_JSON "users.json" // default json file name to use 
repository::repository(bool clear_data) : invalidated(true), json_filename(U(USERS_JSON)) {

	if (clear_data) {
		remove(USERS_JSON); // remove the users.json file on the disk to make sure dummy users are loaded.
	}

	/* check if a file exists on disk or not(without boost::filesystem)*/
	std::ifstream file(USERS_JSON);
	if (!file) {
		write_dummy_users().wait();
	}

	get_users().wait(); // initialize the users list.
	bool b = invalidated;
}
