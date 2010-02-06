/*
 * ConcurrentDataSet.h
 *
 *  Created on: Sep 25, 2008
 *      Author: Mike
 */

#include "mytypes.h"
#include "ConcurrentDataSetPool.h"
#include "futil.h"

ConcurrentDataSetPool::ConcurrentDataSetPool(int threads) : nThreads(threads) {
	assert(nThreads > 0);
	dataSets = new ConcurrentDataSet[nThreads];
	for(int i = 0; i < nThreads; i++) {
		dataSets[i].startThread();
	}
}

FloatKeyVec ConcurrentDataSetPool::getTopKSimilar(KeyId actor, int k, DistanceFunction df) {
	ObjectValueMap *actorMap = dataSets[actor % nThreads].getActorMap(actor);
	if(actorMap) {
		return getTopKSimilar(actorMap, k, df);
	} else {
		return FloatKeyVec();
	}
}

void ConcurrentDataSetPool::mergeVecs(FloatKeyVec * retvec, int nVecs, int k, FloatKeyVec &ret) {
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



FloatKeyVec ConcurrentDataSetPool::getTopKSimilar(ObjectValueMap *actorMap, int k, DistanceFunction df) {

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

void ConcurrentDataSetPool::mergeRecs(ItemRecMap &a, ItemRecMap &b) {
	ItemRecMap::iterator i  = a.begin();
	ItemRecMap::iterator i2 = b.begin();

	if(i2 == b.end()) {
		return;
	}

//	return 1.0;
	while(true) {

		while(i2 != b.end() && i != a.end() && i->first < i2->first) {
			++i;
		}

		//if they're not equal
		if(i != a.end() && i->first == i2->first) {
			i->second.first  += i2->second.first;
			i->second.second += i2->second.second;

			++i;
			++i2;
		} else {
			a.insert(i, *i2);

			++i2;
		}

		if(i2 == b.end()) {
			return;
		}

	}
}

//TODO parallelize this
ItemRecMap ConcurrentDataSetPool::getRecs(FloatKeyVec & v)
{
	ItemRecMap ret = dataSets[0].getRecs(v);

	for(int i = 1; i < nThreads; i++) {
		ItemRecMap r2 = dataSets[i].getRecs(v);
		mergeRecs(ret, r2);
	}

	return ret;
}

FloatKeyVec	ConcurrentDataSetPool::getRecs(KeyId actor, int top_k, DistanceFunction df) {
	FloatKeyVec v = getTopKSimilar(actor, top_k, df);
	return recMapToVecItemRecMap(getRecs(v));
}

ItemRecMap	ConcurrentDataSetPool::getRecs(ObjectValueMap *a, int top_k, DistanceFunction df) {
	FloatKeyVec v = getTopKSimilar(a, top_k, df);
	return getRecs(v);
}

//	void iterateThroughTest(KeyId actor, DistanceFunction df);
//	KeyFloatPairVec getTopKSimilar(KeyId actor, int k, DistanceFunction df);


/*Update Functions */
void ConcurrentDataSetPool::addOrUpdateValue(KeyId actor, KeyId object, float value) {
	dataSets[actor % nThreads].
		addOrUpdateValue(actor, object, value);
}
void ConcurrentDataSetPool::removeValue(KeyId actor, KeyId object) {
	dataSets[actor % nThreads].
		removeValue(actor, object);

}
void ConcurrentDataSetPool::removeActor(KeyId actor) {
	dataSets[actor % nThreads].
		removeActor(actor);
}

ConcurrentDataSetPool::~ConcurrentDataSetPool() {
	delete[] dataSets;
}

