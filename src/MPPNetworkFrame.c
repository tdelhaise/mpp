//
// Created by thierry on 11/04/2021.
//

#include "../headers/MPPLibrary.h"

bool MPPNetworkFrameHeaderInit(MPPNetworkFrameHeader* mppNetworkFrameHeader) {

    mppNetworkFrameHeader->signature = MPP_NETWORK_FRAME_HEADER_SIGNATURE;
    mppNetworkFrameHeader->version = MPP_NETWORK_FRAME_HEADER_VERSION;
    mppNetworkFrameHeader->frameSize = 0;
    mppNetworkFrameHeader->type = MPP_NETWORK_FRAME_TYPE_BUFFER_SIZE_NEGOTIATION;
    mppNetworkFrameHeader->kind.negotiation.bufferSize = 0;
    uuid_generate(mppNetworkFrameHeader->uuid);
    // check that the generate uuid is not null
    if(uuid_is_null(mppNetworkFrameHeader->uuid))
        return false;
    else
        return true;
}
