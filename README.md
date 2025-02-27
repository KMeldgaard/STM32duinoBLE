# STM32duinoBLE

This library is a fork of ArduinoBLE library to add the support of SPBTLE-RF and SPBTLE-1S BLE modules.
It was successfully tested with the X-NUCLEO-IDB05A2 or X-NUCLEO-IDB05A1 or X-NUCLEO-BNRG2A1 expansion board and a NUCLEO-F401RE
or NUCLEO-L476RG or NUCLEO-L053R8, with B-L475E-IOT01A and with STEVAL-MKSBOX1V1.
In order to use this library with STEVAL-MKSBOX1V1, you need to update the firmware of the SPBTLE-1S BLE module
mounted on that board as described in the following wiki page:

https://github.com/stm32duino/wiki/wiki/STM32duinoBLE#stm32duinoble-with-steval_mksbox1v1

In order to use this library with X-NUCLEO-BNRG2A1, you need to update the firmware of the BLUENRG-M2SP BLE module
mounted on that expansion board as described in the following wiki page:

https://github.com/stm32duino/wiki/wiki/STM32duinoBLE#stm32duinoble-with-x-nucleo-bnrg2a1

For more information about ArduinoBLE library please visit the official web page at:
https://github.com/arduino-libraries/ArduinoBLE
## License

```
Copyright (c) 2019 STMicroelectronics. All rights reserved.
Copyright (c) 2019 Arduino SA. All rights reserved.

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
```

# Configuration
STM32Cube_WPAN has several configuration options, which are set in the `app_conf.h`.
This package has a default configuration named `app_conf_default.h`.
The user can include the file `app_conf_custom.h` to customize the ble application. Options wrapped in `#ifndef, #endif` in `app_conf_default.h` can be overwritten. Additional options can be added.
## HCI data length
By default the data length (max payload per BLE packet) is set to 27 bytes. This can cause fragmentation when transmitting large characteristics using a large ATT_MTU.
To increase the data length user must define `CFG_BLE_ENABLE_SET_DATA_LENGTH` (in `app_conf_default.h`). Further more, the wanted data length must bbe set in same define - eg: `#define CFG_BLE_ENABLE_SET_DATA_LENGTH 251`. Valid range: 27 --> 251.
**Note: if this is enabled the pheripheral will attempt to increase the HCI data length with every connected device! There is no guarantee all BLE devices support all sizes!**
### HCI pending packet hack
An error that occurs frequently is HCI._pendingPkt not being set to 0 before a new connection, which results in tanking the BLE.
To hack around this the function `HCI::resetPendingPkt()` is added as public.
**THIS FUNCTION SHOULD ONLY BE CALLED IN DISCONNECT CALLBACK!!!**
