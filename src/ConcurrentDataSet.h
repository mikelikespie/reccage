/*
 * ConcurrentDataSet.h
 *
 *  Created on: Sep 25, 2008
 *      Author: Mike
 */

#ifndef CONCURRENTDATASET_H_
#define CONCURRENTDATASET_H_

#include <pthread.h>
#include <iostream>
#include <assert.h>
#include "DataSet.h"

/**
 * These functions can only be called from one thread at a time
 *
 * CLASS IS NOT SYNCHRONIZED
 */

class ConcurrentDataSet: public DataSet {

private:
	pthread_t thread;
	pthread_mutex_t mutex_run;
	pthread_cond_t cond_run;

	struct {
		ObjectValueMap *actorMap;
		DistanceFunction df;
		int k; //-1 = return all

		FloatKeyVec similaritiesRet;
	} functionParams;

	volatile bool finished;
	volatile bool exit;

	volatile bool startRun;

	// ConcurrentDataSet::startRoutine(void *me);
	// ConcurrentDataSet<const char *>::startRoutine(void *me);
	static void *startRoutine(void *me);

public:
	ConcurrentDataSet();

	virtual ~ConcurrentDataSet();

	void startThread();

	void startGetTopKSimilar(ObjectValueMap *actorMap, int k, DistanceFunction df);

	FloatKeyVec getTopKSimilar();
private:
	void loop();
};

#endif /* CONCURRENTDATASET_H_ */
