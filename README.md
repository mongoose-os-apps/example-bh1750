# Sample app for the BH1750 driver

Driver library is [here](https://github.com/mongoose-os-libs/bh1750)

## Build

```
$ mos build --platform esp8266
Connecting to https://mongoose.cloud, user test
Uploading sources (2052 bytes)
Firmware saved to /home/rojer/cesanta/mongoose-os-apps/example-bh1750/build/fw.zip
```

## Flash and run

```
$ mos flash && mos console
Loaded example-bh1750/esp8266 version 1.0.0 (20200823-222725)
...
Booting firmware...
All done!
Using port /dev/ttyUSB0
...
[Aug 23 23:41:12.143] esp_main.c:178          example-bh1750 1.0.0 (20200823-224013)
[Aug 23 23:41:12.149] esp_main.c:180          Mongoose OS 6571430 (20200823-224013/g6571430-master)
[Aug 23 23:41:12.157] esp_main.c:181          CPU: ESP8266EX, 160 MHz, RAM: 53792 total, 50732 free
[Aug 23 23:41:12.163] esp_main.c:185          SDK 2.2.2-dev(a58da79); flash: 4M
...
[Aug 23 23:41:12.351] mgos_i2c_gpio_maste:248 I2C GPIO init ok (SDA: 4, SCL: 5, freq: 100000)
...
[Aug 23 23:41:13.352] main.c:47               ++ Found BH1750 sensor at 0x23
[Aug 23 23:41:13.356] mgos_init.c:33          Init done, RAM: 53792 total, 46004 free, 46016 min free
[Aug 23 23:41:13.367] mgos_mdns.c:82          Listening on udp://:5353
[Aug 23 23:41:13.374] mgos_mongoose.c:66      New heap free LWM: 44568
[Aug 23 23:41:13.408] main.c:15               -- Inv, mt 180 wt 129
[Aug 23 23:41:13.458] main.c:15               -- Inv, mt 180 wt 79
[Aug 23 23:41:13.508] main.c:15               -- Inv, mt 180 wt 29
[Aug 23 23:41:13.559] main.c:13               ** 303.33 lux (raw 364) mt 180 wt 0
[Aug 23 23:41:13.608] main.c:13               ** 303.33 lux (raw 364) mt 180 wt 130
[Aug 23 23:41:13.658] main.c:13               ** 303.33 lux (raw 364) mt 180 wt 130
[Aug 23 23:41:13.708] main.c:13               ** 303.33 lux (raw 364) mt 180 wt 130
...
```

## Experiment

Change configuration at runtime via RPC.

Setup wifi
```
$ mos wifi Over_9000_Internets MY_PASSWORD && mos console
Using port /dev/ttyUSB0
Getting configuration...
Setting new configuration...
Using port /dev/ttyUSB0
...
[Aug 23 23:41:21.609] mgos_wifi.c:270         WiFi STA: Connecting to Over_9000_Internets
[Aug 23 23:41:21.616] mgos_dns_sd.c:505       DNS-SD initialized, host BH1750-test.local, ttl 120
[Aug 23 23:41:21.625] mgos_http_server.c:336  HTTP server started on [80]
[Aug 23 23:41:21.629] mgos_rpc_channel_ua:313 0x3fff031c UART0
[Aug 23 23:41:21.633] main.c:47               ++ Found BH1750 sensor at 0x23
[Aug 23 23:41:21.640] mgos_init.c:33          Init done, RAM: 53792 total, 47408 free, 47064 min free
...
[Aug 23 23:41:25.394] esp_main.c:158          SDK: connected with Over_9000_Internets, channel 1
[Aug 23 23:41:25.402] esp_main.c:158          SDK: dhcp client start...
[Aug 23 23:41:25.409] main.c:13               ** 304.17 lux (raw 365) mt 180 wt 128
[Aug 23 23:41:25.412] mgos_mongoose.c:66      New heap free LWM: 45576
[Aug 23 23:41:25.421] mgos_wifi.c:133         WiFi STA: Connected, BSSID 98:da:c4:bf:ce:97 ch 1 RSSI -48
[Aug 23 23:41:25.427] mgos_net.c:90           WiFi STA: connected
[Aug 23 23:41:25.431] esp_main.c:158          SDK: ip:192.168.11.224,mask:255.255.255.0,gw:192.168.11.1
[Aug 23 23:41:25.441] mgos_net.c:101          WiFi STA: ready, IP 192.168.11.224, GW 192.168.11.1, DNS 192.168.11.1
[Aug 23 23:41:25.457] main.c:13               ** 303.33 lux (raw 364) mt 180 wt 131
...
```

Now, in a different window:

```
$ mos --port http://BH1750-test.local/rpc call BH1750.SetConfig '{"mode": 17, "mtime": 254}'
null
```

And observe the change on the console:

```
...
[Aug 23 23:41:35.658] main.c:13               ** 304.17 lux (raw 365) mt 180 wt 130
[Aug 23 23:41:35.696] mgos_http_server.c:180  0x3fff0c4c HTTP connection from 192.168.11.45:52104
[Aug 23 23:41:35.707] mg_rpc.c:292            BH1750.SetConfig via HTTP 192.168.11.45:52104
[Aug 23 23:41:35.716] mgos_mongoose.c:66      New heap free LWM: 43488
[Aug 23 23:41:35.720] main.c:15               -- Inv, mt 501 wt 493
[Aug 23 23:41:35.757] main.c:15               -- Inv, mt 501 wt 457
[Aug 23 23:41:35.807] main.c:15               -- Inv, mt 501 wt 407
[Aug 23 23:41:35.857] main.c:15               -- Inv, mt 501 wt 357
[Aug 23 23:41:35.907] main.c:15               -- Inv, mt 501 wt 307
[Aug 23 23:41:35.957] main.c:15               -- Inv, mt 501 wt 257
[Aug 23 23:41:36.007] main.c:15               -- Inv, mt 501 wt 207
[Aug 23 23:41:36.057] main.c:15               -- Inv, mt 501 wt 157
[Aug 23 23:41:36.107] main.c:15               -- Inv, mt 501 wt 107
[Aug 23 23:41:36.157] main.c:15               -- Inv, mt 501 wt 57
[Aug 23 23:41:36.207] main.c:15               -- Inv, mt 501 wt 7
[Aug 23 23:41:36.258] main.c:13               ** 304.25 lux (raw 2688) mt 501 wt 0
[Aug 23 23:41:36.308] main.c:13               ** 304.25 lux (raw 2688) mt 501 wt 451
[Aug 23 23:41:36.358] main.c:13               ** 304.25 lux (raw 2688) mt 501 wt 451
...
```
