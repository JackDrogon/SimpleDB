#include <string>
#include <iostream>

#include "db.h"

using namespace std;

int main()
{
	DB db("hello");

	std::string key("hello");
	std::string value("world");

	db.Put(key, value);

	std::string get_value;
	if (db.Get(key, get_value)) {
		cout << key << " => " << get_value << endl;
	} else {
		cout << "Not found" << endl;
	}



	db.Delete(key);

	if (db.Get(key, get_value)) {
		cout << key << " => " << get_value << endl;
	} else {
		cout << "Not found" << endl;
	}

	return 0;
}
