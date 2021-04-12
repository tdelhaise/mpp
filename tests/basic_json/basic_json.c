//
// Created by thierry on 11/04/2021.
//

#include <stdio.h>
#include <assert.h>
#include <memory.h>
#include <MPPLibrary.h>

const char jsonString[] = u8"{ \"message\": \"some text for this message\", \"anIntVariable\": 12567, \"aFloatVariable\": 12345.67, \"anArrayOfString\": [ \"string1\", \"string2\" ], \"anArrayOfInt\": [ 1234, 5678 ] }";

void mySessionCallbackFunction(const MPPSession* mppSession, const MPPNetworkFrame* mppNetworkFrame) {
    assert(mppSession != NULL);
    assert(mppNetworkFrame != NULL);
}

int main(int argc, char* argv[]) {
    MPPNetworkFrameHeader mppNetworkFrameHeader;

    const char destinationHostname[] = u8"localhost";
    const unsigned int destinationPort = 12567;

    printf("Hello world from basic_json.c\n");
    printf("Using compiler name %s\n",COMPILER_NAME);
    printf("Using compiler version %s\n",COMPILER_VERSION);

    printf("jsonString [%s] as a size of [%lu]\n",jsonString,sizeof(jsonString));

    if(!isMPPLibraryInitialized()) {
        bool initialized = MPPLibraryStart();
        if(initialized) {
            printf("MPPLibraryStart: [true]\n");
        } else {
            printf("MPPLibraryStart: [false]\n");
            MPPLibraryStop();
            return -1;
        }
    }

    MPPError mppError = { .errorCode = 0, .message[0] = 0 };
    MPPSession* mppSession = MPPSessionCreate(&mppError);
    if(mppSession == NULL) {
        assert(mppError.errorCode != 0);
        assert(strlen(mppError.message) != 0);
        printf("[ERROR] An error occurred when calling MPPSessionCreate => [%s]",mppError.message);
        MPPLibraryStop();
        return -2;
    }

    if(!MPPSessionSetJsonBuffer(mppSession, jsonString, sizeof(jsonString),&mppError)) {
        assert(mppError.errorCode != 0);
        assert(strlen(mppError.message) != 0);
        printf("[ERROR] An error occurred when calling MPPSessionSetJsonBuffer => [%s]",mppError.message);
        return -3;
    }

    if(!MPPSessionSetDestinationHost(mppSession,destinationHostname,destinationPort,&mppError)) {
        assert(mppError.errorCode != 0);
        assert(strlen(mppError.message) != 0);
        printf("[ERROR] An error occurred when calling MPPSessionSetDestinationHost => [%s]",mppError.message);
        return -4;
    }

    MPPSessionSetReceiveCallback(mppSession,&mySessionCallbackFunction);


    MPPSessionRelease(mppSession);
    mppSession = NULL;

    MPPLibraryStop();
    return 0;
}
