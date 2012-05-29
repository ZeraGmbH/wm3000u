// implementierung cReanimate

#include "reanimate.h"

int cReanimate::PushAction(int action) // eine action ins fifo
{
    mActionList.push_back(action);
    return mActionList.count();
}


void cReanimate::ReanimateSlot()
{
    if ( !mActionList.isEmpty() ) {
	int action = mActionList.first();
	mActionList.pop_back();
	emit SendActionSignal(action);
    }
}
