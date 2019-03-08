// API for raw access to a block device.
// For the class, the block device is going to be represented by a
// file on the host operating system. With very minor tweaks, it can
// be directed to a real block device on a Unix system.

// Methods:
// open - opens a block device
// close - closes the device
// read_block - takes a block number and a buffer - reads that block into the buffer
// write_block - takes a block number and a buffer - writes the block

// Public attributes:
// block_size - number of bytes per block on this device
// block_count - number of blocks on this device

#include <string>
#include <iostream>

#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "header_files/block_device.hh"

// This method creates a new block device, possibly creating a new
// file, truncating or exending an existing one.
// deviceName is the name of the file being created.
// blockSize is the bytes per block
// blockCount is the number of blocks for the device
BlockDevice::BlockDevice(std::string deviceName, int blockSize, int blockCount) {
    m_bytesPerBlock = blockSize;
    m_blockCount = blockCount;
    m_deviceHandle = open(deviceName.c_str(), O_RDWR | O_CREAT, 0644);
    // octcal 644 is owner read/write, everyone else read-only - see 'man chmod'
    if (m_deviceHandle < 0) {
        char *desc = strerror(errno);
        std::cout << "error creating device file " << deviceName << " due to " << desc << "\n";
    }
    int truncResult = ftruncate(m_deviceHandle, blockSize * blockCount);
    if (truncResult != 0) {
        std::cout << "error setting file size\n";
    }
}

void BlockDevice::closeDevice() {
    if (m_deviceHandle) {
        int ok = ::close(m_deviceHandle);
        if (ok != 0) {
            std::cout << "error closing device " << strerror(errno) << "\n";
        } else {
            m_deviceHandle = 0;
        }
    }
}

// object destructor closes the file, if the handle != 0
BlockDevice::~BlockDevice() {
    closeDevice();
}

void BlockDevice::readBlock(int blockNum, char *buff) {
    int byteOffset = blockNum * m_bytesPerBlock;
    int seekRet = lseek(m_deviceHandle, byteOffset, SEEK_SET);
    if (seekRet != byteOffset) {
        std::cout << "seek error in readBlock\n";
    }
    int numRead = read(m_deviceHandle, buff, m_bytesPerBlock);
    if (numRead != m_bytesPerBlock) {
        // raise some heck here - should never happen!
        std::cout << "read error in readBlock\n";
    }
}

void BlockDevice::writeBlock(int blockNum, const char *buff) {
    int byteOffset = blockNum * m_bytesPerBlock;
    int seekRet = lseek(m_deviceHandle, byteOffset, SEEK_SET);
    if (seekRet != byteOffset) {
        std::cout << "seek error in readBlock\n";
        std::cout << blockNum << std::endl;
        std::cout << m_bytesPerBlock << std::endl;
        std::cout << m_deviceHandle << std::endl;
        std::cout << byteOffset << std::endl;
        std::cout << seekRet << std::endl;
        std::cout << SEEK_SET << std::endl;
    }
    int numWritten = write(m_deviceHandle, buff, m_bytesPerBlock);
    if (numWritten != m_bytesPerBlock) {
        std::cout << "writeBlock write failure\n";
    }
}
