#ifndef DB_H_
#define DB_H_

#include <string>
#include "lru.h"

class DB {
public:
	DB(std::string name);
	~DB();
	bool Put(std::string &key, std::string &value);
	bool Get(std::string &key, std::string &value);
	bool Delete(std::string &key);

private:
	typedef LRU<std::string, std::string> Cache;

	std::string name_;
	FILE *write_file_;
	Cache cache_;
};

#endif // DB_H_
