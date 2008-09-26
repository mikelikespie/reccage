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

class DataSet {
private:
	ActorObjectMap myMap;
public:
	DataSet();
	virtual ~DataSet();
	KeyFloatPairVec getSimilarities(KeyId actor, DistanceFunction df);
	void iterateThroughTest(KeyId actor, DistanceFunction df);
	KeyFloatPairVec getTopKSimilar(KeyId actor, int k, DistanceFunction df);


	/*Update Functions */
	void addOrUpdateValue(KeyId actor, KeyId object, const float value);
	void removeValue(KeyId actor, KeyId object);
	void removeActor(KeyId actor);

	void doubleSize() {
		//myMap.resize(myMap.bucket_count() * 2);
	}

	static void cleanUpObjectValueMap(ObjectValueMap * m);

};

#endif /* DATASET_H_ */
