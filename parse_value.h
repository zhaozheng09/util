#include <map>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <strings.h>
#include <vector>
#include <stdint.h>
#include <boost/utility/string_ref.hpp>
#include <sstream>
#include "timer.h"
using namespace std;


enum Type {
	Double = 1, 
	Int, 
	String, 
	Unknown
};

union Val {
    float f; 
	int64_t i;
};

void Error(const char *p) {
	std::cout << p << std::endl;
}

class Elem {
public:
	Type type_;
	vector<Val> vec_;
};

void SplitToLine(const string& file, vector<string> &res) {
	ifstream ifstr(file.c_str());
	uint32_t len = 4096 * 10;
	while(!ifstr.eof()) {
		char p[len];
		memset(p, 0x00, len);
		ifstr.getline(p, len);
		res.push_back(string(p, strlen(p)));
	}
	ifstr.close();
}

void SplitStrsToKV(const vector<string> &strs, vector<pair<boost::string_ref, boost::string_ref> > &pairs, const string& spec) {
	for (auto& i : strs) {
		size_t pos = i.find(spec);
		if (string::npos == pos) {
			pos = i.size();
		}
		pairs.push_back(pair<boost::string_ref, boost::string_ref>(boost::string_ref(i.c_str(), pos), boost::string_ref(i.c_str() + pos + 1, i.size() - pos - 1)));
		if (string::npos == pos) {
			break;
		}
	}
}

void ShowMap(const map<string, Elem> &m) {
	for (auto &i : m) {
		std::cout << i.first << "|";
		if (i.second.type_ == Type::Double) {
			for (auto &j : i.second.vec_) {
				std::cout << j.f << " ";
			}
		}
		else {
			for (auto &j : i.second.vec_) {
				std::cout << j.i << " ";
			}
		}
		std::cout << endl;
	}
}

template<class T>
void SplitBy(const char*p, const size_t len, const char spec, Elem &elemIn) {
	const char *next = p;
	while (next && next <= p + len) {
		const char* end = strchr(next, spec);
		if (!end) end = p + len;
		istringstream iss(string(next, end - next + 1));
		Val v;
		if (elemIn.type_ == Type::Double) {
			iss >> v.f;
		}
		else {
			iss >> v.i;
		}
		elemIn.vec_.push_back(v);
		next = end + 1;
	}
}

void GetMaps(vector<map<string, Elem> > &ms) {
	
	vector<string> res;
	SplitToLine("../values_rel.txt", res);
	vector<pair<boost::string_ref, boost::string_ref> > kvpairs;
	SplitStrsToKV(res, kvpairs, ",");
	map<string, Elem> m;
	for (auto& kv : kvpairs) {
		auto ret = m.insert(pair<string, Elem>(string(kv.first), Elem()));
		if (!ret.second) {
			ms.push_back(m);
			m.clear();
			m.insert(pair<string, Elem>(string(kv.first), Elem()));
		}
		auto iter = m.find(string(kv.first));
		if (string((kv.first)) == "dense") {
			iter->second.type_ = Type::Double;
			SplitBy<double>(kv.second.data(), kv.second.size(), ' ', iter->second);
		}
		else {
			iter->second.type_ = Type::Int;
			SplitBy<int64_t>(kv.second.data(), kv.second.size(), ' ', iter->second);
		}
	}
	ms.push_back(m);
	//ShowMap(m);
}

