/*
 * ConcurrentDataSet.cpp
 *
 *  Created on: Sep 25, 2008
 *      Author: Mike
 */

#include "ConcurrentDataSet.h"

using namespace std;

void *ConcurrentDataSet::startRoutine(void *me)
{
	((ConcurrentDataSet*)me)->loop();
	return NULL;
}

ConcurrentDataSet::ConcurrentDataSet() {
	pthread_mutex_init(&mutex_run, NULL);
	pthread_cond_init(&cond_run, NULL);
	finished = false;
	exit = false;
	startRun = false;

}

ConcurrentDataSet::~ConcurrentDataSet() {
	//Signal for us to exit
	pthread_mutex_lock(&mutex_run);
	exit = true;
	pthread_mutex_unlock(&mutex_run);
	pthread_cond_signal(&cond_run);
}

void ConcurrentDataSet::startThread() {
	pthread_create(&thread, NULL, startRoutine, (void*) this);
}

void ConcurrentDataSet::startGetTopKSimilar(ObjectValueMap *actorMap, int k, DistanceFunction df) {
	functionParams.actorMap = actorMap;
	functionParams.df = df;
	functionParams.k = k;


	pthread_mutex_lock(&mutex_run);
	startRun=true;
	pthread_mutex_unlock(&mutex_run);
	pthread_cond_signal(&cond_run);

}

FloatKeyVec ConcurrentDataSet::getTopKSimilar() {
	//Now we have to wait for the mutex to unlock
	pthread_mutex_lock(&mutex_run);
	while (!finished) {
		pthread_cond_wait(&cond_run, &mutex_run);
	}
	finished = false;
	pthread_mutex_unlock(&mutex_run);

	return functionParams.similaritiesRet;
}


void ConcurrentDataSet::loop() {
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
