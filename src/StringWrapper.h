/*
 * StringWrapper.h
 *
 *  Created on: Sep 27, 2008
 *      Author: Mike
 */

#ifndef STRINGWRAPPER_H_
#define STRINGWRAPPER_H_

template <class K> class StringWrapper {
private:
	StringNumMapper actorMapper;
	StringNumMapper objectMapper;
	K &obj;
public:
	StringWrapper(K &robj) : obj(robj) {};
	virtual ~StringWrapper() {};


	inline FloatKeyVec getTopKSimilar(const char * actor, int k, DistanceFunction df) {
		return obj.getTopKSimilar(actorMapper.getKey(actor), k, df);
	}

	inline FloatKeyVec getTopKSimilar(ObjectValueMap *objmap, int k, DistanceFunction df) {
		return obj.getTopKSimilar(objmap, k, df);
	}

	/*Update Functions */
	inline void addOrUpdateValue(const char * actor, const char * object, const float value) {
		obj.addOrUpdateValue(actorMapper.getKey(actor), objectMapper.getKey(object), value);
	}

	inline void removeValue(const char * actor, const char * object) {
		obj.removeValue(actorMapper.getKey(actor), objectMapper.getKey(object));
	}

	inline void removeActor(const char * actor) {
		obj.removeActor(actorMapper.getKey(actor));
	}

	inline const char * lookupActor(KeyId id) {
		return actorMapper.getString(id);
	}

	inline const char * lookupObject(KeyId id) {
		return objectMapper.getString(id);
	}

};


#endif /* STRINGWRAPPER_H_ */
