/*
 * ConcurrentDataSet.h
 *
 *  Created on: Sep 25, 2008
 *      Author: Mike
 */

#ifndef CONCURRENTDATASETPOOL_H_
#define CONCURRENTDATASETPOOL_H_

#include <assert.h>
#include <iostream>
#include <vector>

#include "StringWrapper.h"
#include "ConcurrentDataSet.h"

class ConcurrentDataSetPool {
private:
	int nThreads;
	ConcurrentDataSet * dataSets;

public:
	ConcurrentDataSetPool(int threads);
	FloatKeyVec getTopKSimilar(KeyId actor, int k, DistanceFunction df);
	void mergeVecs(FloatKeyVec * retvec, int nVecs, int k, FloatKeyVec &ret);
	FloatKeyVec getTopKSimilar(ObjectValueMap *actorMap, int k, DistanceFunction df);
	void mergeRecs(ItemRecMap &a, ItemRecMap &b);
	ItemRecMap getRecs(FloatKeyVec & v);
	FloatKeyVec	getRecs(KeyId actor, int top_k, DistanceFunction df);
	ItemRecMap	getRecs(ObjectValueMap *a, int top_k, DistanceFunction df);

//	void iterateThroughTest(KeyId actor, DistanceFunction df);
//	KeyFloatPairVec getTopKSimilar(KeyId actor, int k, DistanceFunction df);

	/*Update Functions */
	void addOrUpdateValue(KeyId actor, KeyId object, float value);
	void removeValue(KeyId actor, KeyId object);
	void removeActor(KeyId actor);
	virtual ~ConcurrentDataSetPool();
};

typedef StringWrapper<ConcurrentDataSetPool> ConcurrentStringDataSetPool;
#endif /* CONCURRENTDATASETPOOL_H_ */
