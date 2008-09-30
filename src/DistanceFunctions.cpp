/*
 * DistanceFunctions.cpp
 *
 *  Created on: Sep 28, 2008
 *      Author: Mike
 */

#include <math.h>

#include <iostream>
#include "mytypes.h"
#include "DistanceFunctions.h"

float pearsonDistanceOrdered(ObjectValueMap *p1, ObjectValueMap *p2) {
	float sum1 = 0.0f;
	float sum2 = 0.0f;
	float sumSq1 = 0.0f;
	float sumSq2 = 0.0f;

	float pSum = 0.0f;

	int n = 0; // Number of common matches

	ObjectValueMap::iterator i = p1->begin();
	ObjectValueMap::iterator i2 = p2->begin();

//	return 1.0;
	while(true) {

		int n1=0, n2=0;

		while(i != p1->end() && i2 != p2->end() && i->first != i2->first) {
			if(i->first < i2->first) {
				++i;
				n1++;
			} else {
				++i2;
				n2++;
			}
		}
		if(i->first == i2->first) {
			n1++;
			n2++;
		}

		std::cout << "c=" << std::endl;

		if(i == p1->end() || i2 == p2->end()) {
			break;
		}

		std::cout << n1 << " - " << n2 << std::endl;

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
