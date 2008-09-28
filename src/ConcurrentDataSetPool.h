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


	void getTopKSimilar(KeyId actor, int k, DistanceFunction df, FloatKeyMultiMap &ret) {
		ObjectValueMap *actorMap = dataSets[actor % nThreads].getActorMap(actor);
		if(actorMap) {
			return getTopKSimilar(actorMap, k, df, ret);
		} else {
			ret.clear();
		}
	}

	void getTopKSimilar(ObjectValueMap *actorMap, int k, DistanceFunction df, FloatKeyMultiMap &ret) {

		ret.clear();

		for(int i = 0; i < nThreads; i++) {
			dataSets[i].startGetTopKSimilar(actorMap, k, df);
		}


		//TODO implement a more efficient merge algo
		for(int i = 0; i < nThreads; i++) {
			FloatKeyMultiMap v2;
			dataSets[i].getTopKSimilar(v2);

			ret.insert(ret.end(), v2.begin(), v2.end());

			//find how many we need to delete
			int toDelete = k - ret.size();

			if(toDelete > 0) {
				FloatKeyMultiMap::iterator end = ret.begin();
				for(int j = 0; j < toDelete; j++) {++end;};
				ret.erase(ret.begin(), end);
			}
		}
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
