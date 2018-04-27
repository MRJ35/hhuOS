#include <kernel/Kernel.h>
#include <devices/graphics/text/TextDriver.h>
#include "GraphicsDeviceNameNode.h"

GraphicsDeviceNameNode::GraphicsDeviceNameNode(uint8_t mode) : VirtualNode("device", FsNode::REGULAR_FILE), mode(mode) {
    graphicsService = Kernel::getService<GraphicsService>();
}

uint64_t GraphicsDeviceNameNode::getLength() {
    switch(mode) {
        case TEXT :
            return graphicsService->getTextDriver()->getDeviceName().length() + 1;
        case LINEAR_FRAME_BUFFER :
            return graphicsService->getLinearFrameBuffer()->getDeviceName().length() + 1;
        default:
            return 0;
    }
}

uint64_t GraphicsDeviceNameNode::readData(char *buf, uint64_t pos, uint64_t numBytes) {
    String name;

    switch(mode) {
        case TEXT :
            name = graphicsService->getTextDriver()->getDeviceName() + "\n";
            break;
        case LINEAR_FRAME_BUFFER :
            name = graphicsService->getLinearFrameBuffer()->getDeviceName() + "\n";
            break;
        default:
            break;
    }

    uint64_t length = name.length();

    if (pos + numBytes > length) {
        numBytes = (uint32_t) (length - pos);
    }

    memcpy(buf, (char*) name + pos, numBytes);

    return numBytes;
}

uint64_t GraphicsDeviceNameNode::writeData(char *buf, uint64_t pos, uint64_t numBytes) {
    return 0;
}