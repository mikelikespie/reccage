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
#include "ConcurrentDataSet.h"

class ConcurrentDataSetPool {
private:
	int nThreads;
	ConcurrentDataSet * dataSets;
public:
	ConcurrentDataSetPool(int threads) : nThreads(threads) {
		assert(nThreads > 0);
		dataSets = new ConcurrentDataSet[nThreads];
		for(int i = 0; i < nThreads; i++) {
			dataSets[i].startThread();
		}
	}


	KeyFloatPairVec getSimilarities(KeyId actor, DistanceFunction df) {
		KeyFloatPairVec v;
		ObjectValueMap *actorMap = dataSets[actor % nThreads].getActorMap(actor);
		for(int i = 0; i < nThreads; i++) {
			dataSets[i].startGetSimilarities(actorMap,df);
		}


		for(int i = 0; i < nThreads; i++) {
			KeyFloatPairVec v2  = dataSets[i].getSimilarities();
			v.insert(v.end(), v2.begin(), v2.end());
		}
		return v;
	}
//	void iterateThroughTest(KeyId actor, DistanceFunction df);
//	KeyFloatPairVec getTopKSimilar(KeyId actor, int k, DistanceFunction df);


	/*Update Functions */
	void addOrUpdateValue(KeyId actor, KeyId object, const float value) {
		dataSets[actor % nThreads].
			addOrUpdateValue(actor, object, value);
	}
	void removeValue(KeyId actor, KeyId object) {
		dataSets[actor % nThreads].
			removeValue(actor, object);

	}
	void removeActor(KeyId actor) {
		dataSets[actor % nThreads].
			removeActor(actor);
	}

	virtual ~ConcurrentDataSetPool() {
		delete[] dataSets;
	}
};

#endif /* CONCURRENTDATASETPOOL_H_ */
