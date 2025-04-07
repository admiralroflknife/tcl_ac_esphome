# Integration of TCL-based air conditioners for Home Assistant
I can confirm this code works with another model as well the Rovsun/Zokorp Mini split's. Both 9k and 18k btu models are working fully.


I've adjusted the code here for farenheit compatibility (this may break celcius use a bit as I've directly mapped farenheit to celcius values to prevent rounding issue weirdness).
i've added in support as well for half degree steps on the mini split in celcius which the oriignal code lacked.
the Swing mode button in home assistant thermostat card now maps properly
It also mutes the beeper so any change in setting programically prevents a beep.
Added some additonal Logging statements


Thank you to the orginal for the code, really helped me get my mini splits going




ORIGINAL READ-ME
### Implemented:
- Split system modes (auto, cool, dry, fan only, heat)
- Fan modes (mute, min, min-mid, mid, mid-high, high, turbo)
- Indoor unit temperature
- Target temperature
- Swing mode (only h/v, and visualization only)

### Tested on:
- Royal Clima rci-pf40hn
- Lennox LI036CI-180P432
- SunWind SW-18
- Kesser Split 12000/BTU
- Veska VSK-12000BTU (likely TCL TAC-12chsa/xa73i, as the Kesser above)

### Unsuccessfully tested:
- TCL-12chsa/tpg

### Tuya Module 32001-000140
The [original WiFi-Module](https://github.com/user-attachments/assets/f1888a35-ba68-4869-9790-71ff8c572931) is an ESP8266 and it's original Tuya firmware can be replaced with Tasmota or esphome. It's case is easy to open and [solderpads for serial connection](https://github.com/user-attachments/assets/4515421f-4346-4248-aba7-d4db3886ac40) are available.
The wired UART for the connection to the AC's mainboard uses tx_pin: GPIO15 / rx_pin: GPIO13

### Donation: 
- [ЮMoney](https://yoomoney.ru/fundraise/XBIABgGlKEA.230703)
