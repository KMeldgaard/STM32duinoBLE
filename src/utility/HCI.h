/*
  This file is part of the ArduinoBLE library.
  Copyright (c) 2018 Arduino SA. All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _HCI_H_
#define _HCI_H_

#include <Arduino.h>
#include "HCITransport.h"

#if __has_include("app_conf_custom.h")
#include "app_conf_custom.h"
#endif

#define BLE_CMD_MAX_PARAM_LEN 255

struct hci_le_set_data_length_cp0{
    uint16_t Connection_Handle;
    uint16_t TxOctets;
    uint16_t TxTime;
};

class HCIClass {
public:
  HCIClass();
  virtual ~HCIClass();

  virtual int begin();
  virtual void end();

  virtual void poll();
  virtual void poll(unsigned long timeout);

  virtual int reset();
  virtual int readLocalVersion(uint8_t& hciVer, uint16_t& hciRev, uint8_t& lmpVer,
                       uint16_t& manufacturer, uint16_t& lmpSubVer);
  virtual int readBdAddr(uint8_t addr[6]);

  virtual int readRssi(uint16_t handle);

  virtual int setEventMask(uint64_t eventMask);

  virtual int readLeBufferSize(uint16_t& pktLen, uint8_t& maxPkt);
  virtual int leSetRandomAddress(uint8_t addr[6]);
  virtual int leSetAdvertisingParameters(uint16_t minInterval, uint16_t maxInterval,
                                 uint8_t advType, uint8_t ownBdaddrType,
                                 uint8_t directBdaddrType, uint8_t directBdaddr[6],
                                 uint8_t chanMap,
                                 uint8_t filter);
  virtual int leSetAdvertisingData(uint8_t length, uint8_t data[]);
  virtual int leSetScanResponseData(uint8_t length, uint8_t data[]);
  virtual int leSetAdvertiseEnable(uint8_t enable);
  virtual int leSetScanParameters(uint8_t type, uint16_t interval, uint16_t window, 
                          uint8_t ownBdaddrType, uint8_t filter);
  virtual int leSetScanEnable(uint8_t enabled, uint8_t duplicates);
  virtual int leCreateConn(uint16_t interval, uint16_t window, uint8_t initiatorFilter,
                  uint8_t peerBdaddrType, uint8_t peerBdaddr[6], uint8_t ownBdaddrType,
                  uint16_t minInterval, uint16_t maxInterval, uint16_t latency,
                  uint16_t supervisionTimeout, uint16_t minCeLength, uint16_t maxCeLength);
  virtual int leConnUpdate(uint16_t handle, uint16_t minInterval, uint16_t maxInterval, 
                  uint16_t latency, uint16_t supervisionTimeout);
  virtual int leCancelConn();


  virtual int sendAclPkt(uint16_t handle, uint8_t cid, uint8_t plen, void* data);

  virtual int disconnect(uint16_t handle);

  virtual void debug(Stream& stream);
  virtual void noDebug();

  void setTransport(HCITransportInterface *HCITransport);

  #ifdef CFG_BLE_ENABLE_SET_DATA_LENGTH
  //-----------------------------
  // @brief 
  // @param connectionHandle Connection_Handle Connection handle for which the command applies. 
  //          Values: 0x0000 ... 0x0EFF
  // @param txOctects TxOctets Preferred maximum number of payload octets that the local
  //          Controller should include in a single Link Layer packet on this
  //          connection.
  //          Values: 0x001B ... 0x00FB
  // @param txTime TxTime Preferred maximum number of microseconds that the local
  //         Controller should use to transmit a single Link Layer packet on this
  //         connection.
  //         Values: 0x0148 ... 0x4290
  // @return Value indicating success or error code.
  int hciSetDataLength(uint16_t connectionHandle, uint16_t txOctects, uint16_t txTime);
  #endif // CFG_BLE_ENABLE_SET_DATA_LENGTH

  void resetPendingPacket() { _pendingPkt = 0; }

private:
  virtual int sendCommand(uint16_t opcode, uint8_t plen = 0, void* parameters = NULL);

  virtual void handleAclDataPkt(uint8_t plen, uint8_t pdata[]);
  virtual void handleNumCompPkts(uint16_t handle, uint16_t numPkts);
  virtual void handleEventPkt(uint8_t plen, uint8_t pdata[]);

  virtual void dumpPkt(const char* prefix, uint8_t plen, uint8_t pdata[]);

  Stream* _debug;

  int _recvIndex;
  uint8_t _recvBuffer[3 + 255];

  uint16_t _cmdCompleteOpcode;
  int _cmdCompleteStatus;
  uint8_t _cmdResponseLen;
  uint8_t* _cmdResponse;

  uint8_t _maxPkt;
  uint8_t _pendingPkt;

  uint8_t _aclPktBuffer[255];

  HCITransportInterface *_HCITransport;
};

extern HCIClass& HCI;

#endif
