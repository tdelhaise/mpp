//
// Created by thierry on 10/04/2021.
//


#include <stdio.h>
#include <assert.h>
#include <memory.h>
#include <MPPLibrary.h>

const char testString[] = u8"net.diver6ty.application.module.variableName";

int main(int argc, char* argv[]) {
    MPPNetworkFrameHeader mppNetworkFrameHeader;

    printf("Hello world from basic_linking.c\n");

    printf("string %s as a size of %lu\n",testString,sizeof(testString));

    uint16_t maxInt = UINT16_MAX;
    uint32_t result = maxInt * maxInt;
    printf("maxInt: %u\n",result);

    if(!isMPPLibraryInitialized()) {
        bool initialized = MPPLibraryStart();
        assert(initialized == true);
        const char* initializedString = (initialized == true) ? "true" : "false" ;
        printf("isMPPLibraryInitialized: %s\n", initializedString);
    }

    if(MPPNetworkFrameHeaderInit(&mppNetworkFrameHeader)) {
        assert(mppNetworkFrameHeader.signature == MPP_NETWORK_FRAME_HEADER_SIGNATURE);
        assert(mppNetworkFrameHeader.version == MPP_NETWORK_FRAME_HEADER_VERSION);
        assert(mppNetworkFrameHeader.type == MPP_NETWORK_FRAME_TYPE_BUFFER_SIZE_NEGOTIATION);
        printf("signature: %d\n",mppNetworkFrameHeader.signature);
        printf("version: %d\n",mppNetworkFrameHeader.version);
        char uuidString[37];
        memset(uuidString,0,sizeof(uuidString));
        uuid_unparse_lower(mppNetworkFrameHeader.uuid,uuidString);
        printf("uuid: %s\n",uuidString);
    } else {
        printf("ERROR: Something get weird in MPPFrameInit function.\n");
    }

    MPPLibraryStop();

    return 0;
}

