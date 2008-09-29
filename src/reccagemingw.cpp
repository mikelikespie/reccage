//============================================================================
// Name        : reccagemingw.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <pthread.h>
#include <iostream>

#include <sys/time.h>

#include <math.h>

#include "mytypes.h"
#include "ConcurrentDataSet.h"
#include "ConcurrentDataSetPool.h"
#include "StringWrapper.h"

using namespace std;

float pearsonDistance(ObjectValueMap *p1, ObjectValueMap *p2) {
	float sum1 = 0.0f;
	float sum2 = 0.0f;
	float sumSq1 = 0.0f;
	float sumSq2 = 0.0f;

	float pSum = 0.0f;

	int n = 0; // Number of common matches

	for(ObjectValueMap::iterator i = p1->begin(); i != p1->end(); i++) {

		ObjectValueMap::iterator i2 = p2->find((*i).first);

		if(i2 != p2->end()) { //If we found it
			n++;

			float v1 = (*i).second;
			float v2 = (*i2).second;

			sum1 += v1;
			sumSq1 += v1 * v1;

			sum2 += v2;
			sumSq2 += v2 * v2;

			pSum += v1 * v2;
		}
	}

	if(n == 0) {
		return 0.0f;
	}

	float num = pSum - (sum1 * sum2 / (float)n);

	float den = sqrtf((sumSq1 - sum1 * sum1 / (float)n) * (sumSq2 - sum2 * sum2 / (float)n));

	if(den == 0.0) {
		return 0.0f;
	}

	return (float) (num/den);

}

#ifndef USE_HASH_MAPS
float pearsonDistanceOrdered(ObjectValueMap *p1, ObjectValueMap *p2) {
	float sum1 = 0.0f;
	float sum2 = 0.0f;
	float sumSq1 = 0.0f;
	float sumSq2 = 0.0f;

	float pSum = 0.0f;

	int n = 0; // Number of common matches

	ObjectValueMap::iterator i = p1->begin();
	ObjectValueMap::iterator i2 = p2->begin();


	while(true) {

		while(i != p1->end() && i2 != p2->end() && i->first != i2->first) {
			if(i->first < i2->first)
				++i;
			else
				++i2;
		}

		if(i == p1->end() || i2 == p2->end()) {
			break;
		}

		n++;

		float v1 = (*i).second;
		float v2 = (*i2).second;

		sum1 += v1;
		sumSq1 += v1 * v1;

		sum2 += v2;
		sumSq2 += v2 * v2;

		pSum += v1 * v2;

		++i;
		++i2;
	}

	if(n == 0) {
		return 0.0f;
	}

	float num = pSum - (sum1 * sum2 / (float)n);

	float den = sqrtf((sumSq1 - sum1 * sum1 / (float)n) * (sumSq2 - sum2 * sum2 / (float)n));

	if(den == 0.0) {
		return 0.0f;
	}

	return (float) (num/den);

}
#endif

template <class K>
static void populateDataSet(K &ds) {

	ds.addOrUpdateValue("Lisa Rose", "Lady in the Water", 2.5);
	ds.addOrUpdateValue("Lisa Rose", "Snakes on a Plane", 3.5);
	ds.addOrUpdateValue("Lisa Rose", "Just My Luck", 3.0);
	ds.addOrUpdateValue("Lisa Rose", "Superman Returns", 3.5);
	ds.addOrUpdateValue("Lisa Rose", "You, Me and Dupree", 2.5);
	ds.addOrUpdateValue("Lisa Rose", "The Night Listener", 3.0);

	ds.addOrUpdateValue("Gene Seymour", "Lady in the Water", 3.0);
	ds.addOrUpdateValue("Gene Seymour", "Snakes on a Plane", 3.5);
	ds.addOrUpdateValue("Gene Seymour", "Just My Luck", 1.5);
	ds.addOrUpdateValue("Gene Seymour", "Superman Returns", 5.0);
	ds.addOrUpdateValue("Gene Seymour", "The Night Listener", 3.0);
	ds.addOrUpdateValue("Gene Seymour", "You, Me and Dupree", 3.5);

	ds.addOrUpdateValue("Michael Phillips", "Lady in the Water", 2.5);
	ds.addOrUpdateValue("Michael Phillips", "Snakes on a Plane", 3.0);
	ds.addOrUpdateValue("Michael Phillips", "Superman Returns", 3.5);
	ds.addOrUpdateValue("Michael Phillips", "The Night Listener", 4.0);

	ds.addOrUpdateValue("Claudia Puig", "Snakes on a Plane", 3.5);
	ds.addOrUpdateValue("Claudia Puig", "Just My Luck", 3.0);
	ds.addOrUpdateValue("Claudia Puig", "The Night Listener", 4.5);
	ds.addOrUpdateValue("Claudia Puig", "Superman Returns", 4.0);
	ds.addOrUpdateValue("Claudia Puig", "You, Me and Dupree", 2.5);

	ds.addOrUpdateValue("Mick LaSalle", "Lady in the Water", 3.0);
	ds.addOrUpdateValue("Mick LaSalle", "Snakes on a Plane", 4.0);
	ds.addOrUpdateValue("Mick LaSalle", "Just My Luck", 2.0);
	ds.addOrUpdateValue("Mick LaSalle", "Superman Returns", 3.0);
	ds.addOrUpdateValue("Mick LaSalle", "The Night Listener", 3.0);
	ds.addOrUpdateValue("Mick LaSalle", "You, Me and Dupree", 2.0);

	ds.addOrUpdateValue("Jack Matthews", "Lady in the Water", 3.0);
	ds.addOrUpdateValue("Jack Matthews", "Snakes on a Plane", 4.0);
	ds.addOrUpdateValue("Jack Matthews", "The Night Listener", 3.0);
	ds.addOrUpdateValue("Jack Matthews", "Superman Returns", 5.0);
	ds.addOrUpdateValue("Jack Matthews", "You, Me and Dupree", 3.5);


	ds.addOrUpdateValue("Toby", "Snakes on a Plane", 4.5);
	ds.addOrUpdateValue("Toby", "You, Me and Dupree", 1.0);
	ds.addOrUpdateValue("Toby", "Superman Returns", 4.0);
}

template <class K>
void perform( K &ds) {


	timeval a,b,c;

	for(KeyId i = 0; i < 400000; i++) {
		for(KeyId j = 0; j < 5; j++) {
			ds.addOrUpdateValue(i, j + (i % 20), 1.4);
		}
		if(!((i + 1) % 100000)) {
			cout << "finished " << i << endl;
		}
	}

	gettimeofday(&a, NULL);
	for( int i = 0; i < 100; i++) {
		FloatKeyVec v = ds.getTopKSimilar(i, 100, pearsonDistanceOrdered);
	}
	gettimeofday(&b, NULL);
	timersub(&b,&a,&c);

	printf("%.15f\n",  ((double)c.tv_sec + (double)c.tv_usec / 1000000.0 )/ 100.0 );
}

template <class K>
static void testBookVals(K &ds) {
//	ds.startThread();
	populateDataSet<K>(ds);

#ifndef USE_HASH_MAPS
	//KeyFloatPairVec v = ds.getSimilarities("Lisa Rose", pearsonDistanceOrdered);
	FloatKeyVec v = ds.getTopKSimilar("Lisa Rose", 3, pearsonDistanceOrdered);
#else
	FloatKeyVec v = ds.getSimilarities("Lisa Rose", pearsonDistance);
#endif

	cout << "Lisa Rose:" << endl;

	for(FloatKeyVec::iterator i = v.begin(); i != v.end(); i++) {
		cout << "k\t" << ds.lookupActor((*i).second) << ":\t" << (*i).first << endl;
	}

}


int main() {

	DataSet ds;
	StringWrapper<DataSet> sw(ds);
	testBookVals<StringWrapper<DataSet> >(sw);

	ConcurrentDataSetPool dp(6);
	StringWrapper<ConcurrentDataSetPool> sws(dp);

	testBookVals<StringWrapper<ConcurrentDataSetPool> >(sws);

	{
	DataSet ds1;
	perform<DataSet>(ds1);
	}
//	perform<ConcurrentDataSet >();
	{
	ConcurrentDataSetPool ds2(4);
	perform<ConcurrentDataSetPool >(ds2);
	}

	return 0;
}
