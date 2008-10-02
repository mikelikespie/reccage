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

	FloatKeyVec getTopKSimilar(KeyId actor, int k, DistanceFunction df);
	FloatKeyVec getTopKSimilar(ObjectValueMap *a, int k, DistanceFunction df);

	ItemRecMap	getRecs(FloatKeyVec &v);
	FloatKeyVec	getRecs(KeyId actor, int top_k, DistanceFunction df);
	ItemRecMap	getRecs(ObjectValueMap *a, int top_k, DistanceFunction df);

	/*Update Functions */
	void addOrUpdateValue(KeyId actor, KeyId object, float value);
	void removeValue(KeyId actor, KeyId object);
	void removeActor(KeyId actor);
};




#endif /* DATASET_H_ */
