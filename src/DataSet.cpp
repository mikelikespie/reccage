/*
 * DataSet.cpp
 *
 *  Created on: Sep 23, 2008
 *      Author: Mike
 */


#include <iostream>

#include "DataSet.h"

using namespace std;

DataSet::DataSet()  {

}

DataSet::~DataSet() {
	for(ActorObjectMap::iterator i = myMap.begin(); i != myMap.end(); i++) {
		delete i->second;
	}
}

void DataSet::removeActor(KeyId actor)
{
	ActorObjectMap::iterator f = myMap.find(actor);
	if(f != myMap.end()) {
		delete f->second;
		myMap.erase(f);
	}
}

KeyFloatPairVec DataSet::getSimilarities(KeyId actor, DistanceFunction df)
{

	ActorObjectMap::iterator f = myMap.find(actor);
	if(f != myMap.end()) {
		ObjectValueMap *a = f->second;
		return getSimilarities(a, df);
	} else {
		return KeyFloatPairVec();
	}
}


KeyFloatPairVec DataSet::getSimilarities(ObjectValueMap *p1, DistanceFunction df) {
	KeyFloatPairVec v;
	v.reserve(myMap.size());


	for(ActorObjectMap::iterator i = myMap.begin(); i != myMap.end(); i++) {
		//v.push_back(KeyFloatPair((*i).first, df(p1, i->second)));
		df(p1, i->second);
	}

	return v;
}


KeyFloatPairVec DataSet::getTopKSimilar(KeyId actor, int k, DistanceFunction df)
{
	KeyFloatPairVec v = KeyFloatPairVec();


	return v;
}
KeyFloatPairVec DataSet::getTopKSimilar(ObjectValueMap *a, int k, DistanceFunction df)
{
	KeyFloatPairVec v = KeyFloatPairVec();


	return v;
}

ObjectValueMap *DataSet::getActorMap(KeyId actor) {
	ActorObjectMap::iterator f = myMap.find(actor);
	if(f != myMap.end()) {
		return f->second;
	} else {
		return NULL;
	}
}

void DataSet::iterateThroughTest(KeyId actor, DistanceFunction df)
{
	ActorObjectMap::iterator f = myMap.find(actor);
	if(f != myMap.end()) {
		ObjectValueMap *a = f->second;

		for(ActorObjectMap::iterator i = myMap.begin(); i != myMap.end(); i++) {
			df(a, i->second);
		}

	}
}


void DataSet::addOrUpdateValue(KeyId actor, KeyId object, const float value)
{
	ObjectValueMap *a;


	//FIXME turn the count/insert into one operation

	ActorObjectMap::iterator f = myMap.find(actor);
	if(f == myMap.end()) {
		a = new ObjectValueMap();
		myMap[actor] = a;
	} else {
		a = (*f).second;
	}

	(*a)[object] = value;
}

void DataSet::removeValue(KeyId actor, KeyId object)
{
	if(myMap.find(actor) != myMap.end()) {
		ObjectValueMap *a = myMap[actor];

		if(a->count(object) > 0) {
			a->erase(object);

			if(a->size() == 0) {
				delete a;
				myMap.erase(actor);
			}
		}
	}
}
