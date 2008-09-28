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

	ObjectValueMap *getActorMap(KeyId actor);

	void getTopKSimilar(KeyId actor, int k, DistanceFunction df, FloatKeyVec &ret);
	void getTopKSimilar(ObjectValueMap *a, int k, DistanceFunction df, FloatKeyVec &ret);

	/*Update Functions */
	void addOrUpdateValue(KeyId actor, KeyId object, const float value);
	void removeValue(KeyId actor, KeyId object);
	void removeActor(KeyId actor);
};




#endif /* DATASET_H_ */
