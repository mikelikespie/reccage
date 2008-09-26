/*
 * mytypes.h
 *
 *  Created on: Sep 23, 2008
 *      Author: Mike
 */

#ifndef MYTYPES_H_
#define MYTYPES_H_

#include <hash_map.h>
#include <map>
#include <string>
#include <vector>

typedef unsigned long KeyId;

//#define USE_HASH_MAPS
struct keyidlt
{
  inline bool operator()(KeyId s1, KeyId s2) const
  {
    return s1 < s2;
  }
};

#ifdef USE_HASH_MAPS


//TODO look into making this a vector
typedef __gnu_cxx::hash_map<KeyId, ObjectValueMap*, __gnu_cxx::hash<KeyId>, keyideq> ActorObjectMap;

#else

struct keyideq
{
  inline bool operator()(KeyId s1, KeyId s2) const
  {
    return s1 == s2;
  }
};
typedef __gnu_cxx::hash_map<KeyId, float, __gnu_cxx::hash<KeyId>, keyideq> ObjectValueMap;


//TODO look into making this a vector
typedef std::map<KeyId, ObjectValueMap*, keyidlt> ActorObjectMap;
#endif

typedef std::pair<KeyId, float> KeyFloatPair;

typedef std::vector<KeyFloatPair> KeyFloatPairVec;
typedef float (*DistanceFunction) (ObjectValueMap *p1, ObjectValueMap *p2);

#endif /* MYTYPES_H_ */
