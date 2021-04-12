#include "../headers/MPPLibrary.h"

static bool mppLibraryInitialized = false;
static bool mppLibraryNetworkListenerStarted = false;


bool isMPPLibraryInitialized(void) {
    return mppLibraryInitialized;
}

bool isMPPLibraryNetworkListenerStarted(void) {
    return mppLibraryNetworkListenerStarted;
}

bool MPPLibraryStart(void) {
    mppLibraryInitialized = true;

    return mppLibraryInitialized;
}

bool MPPLibraryStop(void) {
    return true;
}

void MPPLibraryStartNetworkListener(MPPError* mppError) {
    assert(mppError != NULL);
    assert(mppError)

}

void MPPLibraryStopNetworkListener(MPPError* mppError) {
    assert(mppError != NULL);

}

