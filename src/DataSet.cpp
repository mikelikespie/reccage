/*
 * DataSet.cpp
 *
 *  Created on: Sep 23, 2008
 *      Author: Mike
 */


#include <iostream>


#include "DataSet.h"
#include "futil.h"


using namespace std;

DataSet::DataSet()  {

}

DataSet::~DataSet() {
	for(ActorObjectMap::iterator i = myMap.begin(); i != myMap.end(); i++) {
		delete i->second;
	}
}

ItemRecMap DataSet::getRecs(FloatKeyVec & v)
{
	ItemRecMap ret;

	for(FloatKeyVec::iterator u = v.begin(); u != v.end(); ++u) {
		KeyId user = u->second;
		float similarity = u->first;
		// TODO: Decide if we want to use this later
		if(similarity < 0.0) {
			continue;
		}
		ActorObjectMap::iterator vals = myMap.find(user);
		if(vals != myMap.end()) {
			ObjectValueMap *m = vals->second;
			for(ObjectValueMap::iterator objv = m->begin();
				objv != m->end();
				++objv) {

				KeyId item = objv->first;
				float val = objv->second;

				ItemRecMap::iterator f = ret.find(item);
				TotalSimSum *ss = NULL;
				if(f != ret.end()) {
					ss = &f->second;
				} else {
					pair<ItemRecMap::iterator, bool> r = ret.insert(ItemRecMap::value_type(item, TotalSimSum(0.0f, 0.0f)));
					ss = &(r.first->second);
				}

				ss->first += similarity * val;
				ss->second += similarity;
			}
		}
	}

	return ret;
}

FloatKeyVec DataSet::getRecs(KeyId actor, int top_k, DistanceFunction df)
{
	FloatKeyVec v = getTopKSimilar(actor, top_k, df);
	return recMapToVecItemRecMap(getRecs(v));
}

ItemRecMap DataSet::getRecs(ObjectValueMap *a, int top_k, DistanceFunction df)
{
	FloatKeyVec v = getTopKSimilar(a, top_k, df);
	return getRecs(v);
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
		//std::cout << "returned " << val << std::endl;
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


void DataSet::addOrUpdateValue(KeyId actor, KeyId object, float value)
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
