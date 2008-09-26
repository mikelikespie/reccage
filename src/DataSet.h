/*
 * DataSet.h
 *
 *  Created on: Sep 23, 2008
 *      Author: Mike
 */

#ifndef DATASET_H_
#define DATASET_H_

#include <vector>

#include "mytypes.h"
#include "StringNumMapper.h"

class DataSet {
private:
	ActorObjectMap myMap;
public:
	DataSet();
	virtual ~DataSet();
	KeyFloatPairVec getSimilarities(KeyId actor, DistanceFunction df);
	KeyFloatPairVec getSimilarities(ObjectValueMap *p1, DistanceFunction df);

	ObjectValueMap *getActorMap(KeyId actor);

	void iterateThroughTest(KeyId actor, DistanceFunction df);
	KeyFloatPairVec getTopKSimilar(KeyId actor, int k, DistanceFunction df);
	KeyFloatPairVec getTopKSimilar(ObjectValueMap *a, int k, DistanceFunction df);


	/*Update Functions */
	void addOrUpdateValue(KeyId actor, KeyId object, const float value);
	void removeValue(KeyId actor, KeyId object);
	void removeActor(KeyId actor);

	void doubleSize() {
		//myMap.resize(myMap.bucket_count() * 2);
	}

	static void cleanUpObjectValueMap(ObjectValueMap * m);

};



template <class K> class StringWrapper {
private:
	StringNumMapper actorMapper;
	StringNumMapper objectMapper;
	K &obj;
public:
	StringWrapper(K &robj) : obj(robj) {};
	virtual ~StringWrapper() {};


	inline KeyFloatPairVec getSimilarities(const char * actor, DistanceFunction df) {
		return obj.getSimilarities(actorMapper.getKey(actor), df);
	}

	inline KeyFloatPairVec getSimilarities(ObjectValueMap *p1, DistanceFunction df) {
		return obj.getSimilarities(p1, df);
	}

	inline KeyFloatPairVec getTopKSimilar(const char * actor, int k, DistanceFunction df) {
		return obj.getTopKSimilar(actorMapper.getKey(actor), k, df);
	}

	/*Update Functions */
	inline void addOrUpdateValue(const char * actor, const char * object, const float value) {
		obj.addOrUpdateValue(actorMapper.getKey(actor), objectMapper.getKey(object), value);
	}

	inline void removeValue(const char * actor, const char * object) {
		obj.removeValue(actorMapper.getKey(actor), objectMapper.getKey(object));
	}

	inline void removeActor(const char * actor) {
		obj.removeActor(actorMapper.getKey(actor));
	}

	inline const char * lookupActor(KeyId id) {
		return actorMapper.getString(id);
	}

	inline const char * lookupObject(KeyId id) {
		return objectMapper.getString(id);
	}

};

#endif /* DATASET_H_ */
