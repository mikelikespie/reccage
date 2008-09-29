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

using namespace std; // THIS IS BAD< but I am lazy
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
	ConcurrentDataSet() {
		pthread_mutex_init(&mutex_run, NULL);
		pthread_cond_init(&cond_run, NULL);
		finished = false;
		exit = false;
		startRun = false;

	}

	virtual ~ConcurrentDataSet() {
		//Signal for us to exit
		pthread_mutex_lock(&mutex_run);
		exit = true;
		pthread_mutex_unlock(&mutex_run);
		pthread_cond_signal(&cond_run);
	}

	void startThread() {
		pthread_create(&thread, NULL, startRoutine, (void*) this);
	}

	void startGetTopKSimilar(ObjectValueMap *actorMap, int k, DistanceFunction df) {
		functionParams.actorMap = actorMap;
		functionParams.df = df;
		functionParams.k = k;


		pthread_mutex_lock(&mutex_run);
		startRun=true;
		pthread_mutex_unlock(&mutex_run);
		pthread_cond_signal(&cond_run);

	}

	FloatKeyVec getTopKSimilar() {
		//Now we have to wait for the mutex to unlock
		pthread_mutex_lock(&mutex_run);
		while (!finished) {
			pthread_cond_wait(&cond_run, &mutex_run);
		}
		finished = false;
		pthread_mutex_unlock(&mutex_run);

		return functionParams.similaritiesRet;
	}


private:
	inline void loop() {
		while (true) {
			//Now we have to wait for the mutex to unlock
			pthread_mutex_lock(&mutex_run);
			while (!startRun) {
				pthread_cond_wait(&cond_run, &mutex_run);
			}
			startRun = false;

			if (exit) {
				pthread_mutex_unlock(&mutex_run);
				pthread_exit(NULL);
			}
			pthread_mutex_unlock(&mutex_run);

			functionParams.similaritiesRet = DataSet::getTopKSimilar(functionParams.actorMap,
							functionParams.k, functionParams.df);

			pthread_mutex_lock(&mutex_run);
			finished = true;
			pthread_mutex_unlock(&mutex_run);
			pthread_cond_signal(&cond_run);
		}
	}
};

#endif /* CONCURRENTDATASET_H_ */
