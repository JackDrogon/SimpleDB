#include <string>
#include <iostream>

#include "db.h"

using namespace std;

int main()
{
	auto db = DB::Open("hello");

	std::string key("hello");
	std::string value("world");

	for (int i = 0; i < 2000; ++i) {
		db->Put(key + to_string(i), value + to_string(i));
	}


	std::string get_value;
	if (db->Get(key + "1", get_value)) {
		cout << key << " => " << get_value << endl;
	} else {
		cout << "Not found" << endl;
	}

	return 0;
}
