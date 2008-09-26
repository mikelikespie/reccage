/*
 * StringDataSet.h
 *
 *  Created on: Sep 24, 2008
 *      Author: Mike
 */

#ifndef STRINGDATASET_H_
#define STRINGDATASET_H_


#include "DataSet.h"
#include "StringNumMapper.h"

class StringDataSet : public DataSet {
private:
	StringNumMapper mapper;
public:
	StringDataSet() {};
	virtual ~StringDataSet() {};


	inline const KeyFloatPairVec getSimilarities(const char * actor, DistanceFunction df) {
		return DataSet::getSimilarities(mapper.getKey(actor), df);
	}

	inline const KeyFloatPairVec getTopKSimilar(const char * actor, int k, DistanceFunction df) {
		return DataSet::getTopKSimilar(mapper.getKey(actor), k, df);
	}

	/*Update Functions */
	inline void addOrUpdateValue(const char * actor, const char * object, const float value) {
		DataSet::addOrUpdateValue(mapper.getKey(actor), mapper.getKey(object), value);
	}

	inline void removeValue(const char * actor, const char * object) {
		DataSet::removeValue(mapper.getKey(actor), mapper.getKey(object));
	}

	inline void removeActor(const char * actor) {
		DataSet::removeActor(mapper.getKey(actor));
	}

	inline const char * lookupName(KeyId id) {
		return mapper.getString(id);
	}

};

#endif /* STRINGDATASET_H_ */
