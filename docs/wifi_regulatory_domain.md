# 关于
无线热点会定期向周围发射广播信号(beacon frame), 里面包含了这个设备的基本信息, 比如
SSID, BSSDI, 加密方式, 信道, MAC地址, 生产商信息, 以及Country Code.

其中, Country Code, 是 ISO 3166-1 标准定义的由两个字符表示的国家代码, 比如CN, US.
又称为 Regulatory Domain(regdomain). 不同国家定义了不同的无线电信号发射强度, 信道
被占用的时间, 以及可用信号. 我们可以基于此, 来识别当前终端所在的可能的国家.

但实际情况是, Country Code 是可以在无线路由器中设置/修改的, 也有可能是个海淘的无线
设备, 所以, 单单读取一个 Country Code 并不可靠.

# 抓包
这里, 使用 tshark (wireshark 的终端版) 来抓取网络流量.
```
$ tshark -c 1000 -a duration:30 -I -f 'wlan[0] == 0x80' -Tfields -e wlan.sa -e wlan_mgt.country_info.code
```

参数说明:
* `-c 1000`, 抓取1000个数据包后停止
* `-a duration:30`, 抓取30秒之后停止
* `-I`, 开启网卡的监听模式 (monitor mode), 不然, 无法抓取无线广播. 这个选项在已经
 接入无线热点时, 才有效果, 如果没有接入, 使用 `airmon-ng`.
* `-f 'wlan[0] == 0x80'`, 只抓取 beacon packet
* `-Tfields -e wlan.sa -e wlan_mgt.country_info.code `, 只显示包的
  sent address 和 country code

## airmon-ng
用于切换无线设备的监控模式的一个脚本. 比如 `$ sudo airmon-ng start wlan0`

目前, airmon-ng 会在 4.4 内核中出现问题, 可能导致内核卡死, 已经临时禁用它了. 现在直接使用
tshark 的监听模式来抓包的, 但后者的监听模式在一些无线网卡上是无法开启的, 就会导致无法抓取到
beacon packets.

# 参考
https://en.wikipedia.org/wiki/ISO_3166-1_alpha-2
https://en.wikipedia.org/wiki/Beacon_frame
http://wiki.prplfoundation.org/wiki/Linux_Wifi_Regulatory_Framework
https://www.wireshark.org/docs/wsug_html_chunked/AppToolstshark.html
https://www.wireshark.org/docs/man-pages/tshark.html
https://wiki.wireshark.org/CaptureSetup/WLAN
http://www.lovemytool.com/blog/2010/02/wireshark-wireless-display-and-capture-filters-samples-by-joke-snelders.html
https://developer.gnome.org/NetworkManager/stable/nmcli-examples.html