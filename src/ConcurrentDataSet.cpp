/*
 * ConcurrentDataSet.cpp
 *
 *  Created on: Sep 25, 2008
 *      Author: Mike
 */

#include "ConcurrentDataSet.h"

/*
template <> void *ConcurrentDataSet<const char *>::startRoutine(void *me)
{
	((ConcurrentDataSet*)me)->loop();
	return NULL;
}
*/
void *ConcurrentDataSet::startRoutine(void *me)
{
	((ConcurrentDataSet*)me)->loop();
	return NULL;
}
