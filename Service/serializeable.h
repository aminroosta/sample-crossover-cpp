#pragma once
#include <cpprest/json.h>

template<typename T>
struct serializeable; 
/*
 * Serializeable interface, each class specializes this interface.
 * And must implement the following interface:
 * {
 *	static web::json::value serialize(const T&);
 *	static T parse(web::json::value);
 * };
 */