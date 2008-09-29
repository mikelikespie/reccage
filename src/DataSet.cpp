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

FloatKeyVec DataSet::getTopKSimilar(KeyId actor, int k, DistanceFunction df)
{
	ActorObjectMap::iterator f = myMap.find(actor);
	if(f != myMap.end()) {
		ObjectValueMap *a = f->second;
		return getTopKSimilar(a, k, df);
	} else {
		return FloatKeyVec();
	}
}

FloatKeyVec DataSet::getTopKSimilar(ObjectValueMap *a, int k, DistanceFunction df)
{

	FloatKeyVec ret;

	ret.reserve(myMap.size());

	float min = 0.0;
	bool used_min = false;

	for(ActorObjectMap::iterator i = myMap.begin(); i != myMap.end(); i++) {
		float val = df(a, i->second);

		if(!used_min)
			min = val;
#if 0
		if(k == -1 || (int)ret.size() < k || min < val) {
			ret.push_back(pair<float, KeyId>(val, (*i).first));
			min = std::min<float>(min, val);
		}
#else
		ret.push_back(pair<float, KeyId>(val, (*i).first));
#endif
	}

	sort(ret.begin(), ret.end(), PairSortPredicate);

	if(k >= 0) {
		ret.resize(std::min<int>(k, ret.size()));
	}

	return ret;

}

ObjectValueMap *DataSet::getActorMap(KeyId actor) {
	ActorObjectMap::iterator f = myMap.find(actor);
	if(f != myMap.end()) {
		return f->second;
	} else {
		return NULL;
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
