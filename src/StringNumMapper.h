/*
 * StringNumMapper.h
 *
 *  Created on: Sep 24, 2008
 *      Author: Mike
 */

#ifndef STRINGNUMMAPPER_H_
#define STRINGNUMMAPPER_H_

#include <limits>
#include <hash_map.h>
#include <vector>
#include <string.h>

struct streq
{
  inline bool operator()(const char* s1, const char* s2) const
  {
    return strcmp(s1, s2) == 0;
  }
};

typedef __gnu_cxx::hash_map<const char *, KeyId, __gnu_cxx::hash<char *>, streq> StringNumMapperMap;

//TODO Add support for deletion eventually
class StringNumMapper {

private:
	StringNumMapperMap stringNumMap;
	std::vector<char*> numStringVec;

public:
	inline StringNumMapper() {

	}
	inline virtual ~StringNumMapper() {
		for(std::vector<char*>::iterator i = numStringVec.begin();
			i != numStringVec.end();
			i++)
		{
			delete[] *i;
		}
	}

	// TODO THIS MIGHT BE THREAD UNSAFE
	// SPLIT BETWEEN ADD AND UPDATES
	//Adds or updates
	inline KeyId getKey(const char * str) {

		StringNumMapperMap::iterator f = stringNumMap.find(str);

		if(f == stringNumMap.end()) { // Found
			char * newstr = new char[strlen(str)+1];
			strcpy(newstr, str);
			numStringVec.push_back(newstr);

			KeyId k = numStringVec.size() - 1;
			stringNumMap[newstr] = k;
			return k;
		} else {
			return f->second;
		}
	}

	inline const char * getString(KeyId id) {
		if(id >= numStringVec.size()) {
			return NULL;
		} else {
			return numStringVec[id];
		}
	}
};

#endif /* STRINGNUMMAPPER_H_ */
