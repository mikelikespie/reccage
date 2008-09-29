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


	FloatKeyVec getTopKSimilar(KeyId actor, int k, DistanceFunction df) {
		ObjectValueMap *actorMap = dataSets[actor % nThreads].getActorMap(actor);
		if(actorMap) {
			return getTopKSimilar(actorMap, k, df);
		} else {
			return FloatKeyVec();
		}
	}

	void mergeVecs(FloatKeyVec * retvec, int nVecs, int k, FloatKeyVec &ret) {
		FloatKeyVec::iterator *its = new FloatKeyVec::iterator[nThreads];
		for(int i = 0; i < nThreads; i++ ) {
			its[i] = retvec[i].begin();
		}

		ret.clear();

		if(k != -1)
			ret.reserve(k);

		while(k == -1 || (int)ret.size() < k) {
			FloatKeyVec::iterator *max = NULL;

			for(int i = 0; i < nVecs; i++) {
				if(its[i] != retvec[i].end() &&
					(max == NULL || its[i]->first > (*max)->first )) {
					max = &(its[i]);
				}
			}

			if(max == NULL) {
				break;
			} else {
				ret.push_back(*(*max));
				++(*max);
			}
		}
		delete[] its;
	}

	FloatKeyVec getTopKSimilar(ObjectValueMap *actorMap, int k, DistanceFunction df) {

		FloatKeyVec ret;

		for(int i = 0; i < nThreads; i++) {
			dataSets[i].startGetTopKSimilar(actorMap, k, df);
		}


		FloatKeyVec *retvec = new FloatKeyVec[nThreads];
		//TODO implement a more efficient merge algo
		for(int i = 0; i < nThreads; i++) {
			FloatKeyVec v2;
			retvec[i] = dataSets[i].getTopKSimilar();
		}

		mergeVecs(retvec, nThreads, k, ret);

		delete[] retvec;

		return ret;
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
