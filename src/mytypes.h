/*
 * mytypes.h
 *
 *  Created on: Sep 23, 2008
 *      Author: Mike
 */

#ifndef MYTYPES_H_
#define MYTYPES_H_

//#define USE_HASH_MAPS

#ifdef USE_HASH_MAPS
#include <hash_map.h>
#endif
#include <map>
#include <string>
#include <vector>

typedef unsigned long KeyId;


#ifdef USE_HASH_MAPS

struct keyideq
{
  inline bool operator()(KeyId s1, KeyId s2) const
  {
    return s1 == s2;
  }
};
typedef __gnu_cxx::hash_map<KeyId, float, __gnu_cxx::hash<KeyId>, keyideq> ObjectValueMap;

//TODO look into making this a vector
typedef __gnu_cxx::hash_map<KeyId, ObjectValueMap*, __gnu_cxx::hash<KeyId>, keyideq> ActorObjectMap;

#else

typedef std::map<KeyId, float> ObjectValueMap;

//TODO look into making this a vector
typedef std::map<KeyId, ObjectValueMap*> ActorObjectMap;
#endif

//typedef  std::multimap<float, KeyId>  FloatKeyMultiMap;
typedef  std::vector<std::pair<float, KeyId> >  FloatKeyVec;
typedef float (*DistanceFunction) (ObjectValueMap *p1, ObjectValueMap *p2);


//Sorts descending
inline bool PairSortPredicate(const std::pair<float, KeyId>& lhs, const std::pair<float, KeyId>& rhs)
{
  return lhs.first > rhs.first;
}

#endif /* MYTYPES_H_ */
