/*
 * util.h
 *
 *  Created on: Oct 1, 2008
 *      Author: Mike
 */

#ifndef UTIL_H_
#define UTIL_H_
#include <vector>

#include "mytypes.h"
inline FloatKeyVec recMapToVecItemRecMap(ItemRecMap m) {
	FloatKeyVec ret;
	ret.reserve(m.size());

	for(ItemRecMap::iterator i = m.begin(); i!= m.end(); ++i) {
		ret.push_back(std::pair<float, KeyId>(i->second.first / i->second.second, i->first));
	}
	std::sort(ret.begin(), ret.end(), PairSortPredicate);
	return ret;
}


#endif /* UTIL_H_ */
