#pragma once

#include <list>
#include <unordered_map>
#include <utility>
#include <iostream>

// TODO: Add comments
// TODO: hash_map && list malloc memery when construct
// TODO: Add size_type
// TODO: Add noncopyable with ref&&
// TODO: std::move, double lru, lfu
// TODO: concurrent

template <typename Key, typename Value,
	  typename Container = std::unordered_map<
	      Key, std::pair<Value, typename std::list<Key>::iterator>>>
class LRU
{
public:
	using list = std::list<Key>;
	using map = Container;

	explicit LRU(unsigned int capacity) : capacity_(capacity) {}

public:
	bool Get(const Key &key, Value &value)
	{
		auto value_iter = values_.find(key);
		if (value_iter != values_.end()) {
			use(value_iter);
			value = value_iter->second.first;
			return true;
		}

		return false;
	}

	void Put(const Key &key, const Value &value)
	{
		auto value_iter = values_.find(key);
		if (value_iter == values_.end()) {
			insert(key, value);
		} else {
			value_iter->second.first = value;
			use(value_iter);
		}
	}

	void Remove(const Key &key)
	{
		auto value_iter = values_.find(key);
		if (value_iter == values_.end())
			return;

		values_.erase(value_iter);
		keys_.erase(value_iter->second.second);
	}

	unsigned int Capacity() { return capacity_; }

	void Resize(unsigned int capacity)
	{
		for (auto i = capacity_; i < capacity; ++i) {
			evict();
		}
		capacity_ = capacity;
	}

private:
	// Not copyable, not assignable.
	LRU(const LRU &);
	LRU &operator=(const LRU &);

	void evict()
	{
		auto value_iter = values_.find(keys_.front());
		values_.erase(value_iter);
		keys_.pop_front();
	}

	void insert(const Key &key, const Value &value)
	{
		if (keys_.size() == capacity_) {
			evict();
		}

		auto key_iter = keys_.insert(keys_.end(), key);
		values_[key] = std::make_pair(value, key_iter);
	}

	void use(typename map::iterator value_iter)
	{
		// Move the "pointer" to the end of the lru list.
		keys_.splice(keys_.end(), keys_, value_iter->second.second);

		// Now update the "pointer" so we can do this again.
		value_iter->second.second = --keys_.end();
	}

	// Give the operator access to our internals.
	friend std::ostream &operator<<(std::ostream &stream,
					const LRU<Key, Value> &c);

	unsigned int capacity_;
	list keys_;
	map values_;
};

template <typename Key, typename Value>
std::ostream &operator<<(std::ostream &stream, const LRU<Key, Value> &c)
{
	for (auto i1 = c.keys.begin(); i1 != c.keys.end(); i1++) {
		stream << *i1 << ": ";
		auto i2 = c.values.find(*i1);
		stream << *i2 << std::endl;
	}
	return stream;
}
