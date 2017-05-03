# 关于
这里说明一下时区选择的问题.

## windows 时间
当检测到硬盘中已经有 windows 系统时, 就需要保持 linux 中的时间与 windows 中的时间一致.
这时, 会开启 "system_info_found_windows_system" 选项. 同时, 如果开启了
"system_info_windows_disable_timezone_page" 选项, SystemInfoFrame 页面中的
时区按钮会被隐去, 用户无法手动选择时区.

如果 "partition_enable_os_prober" 选项被关闭了, 就不会再检测分区中的系统类型, 也就无法
确定是否有 windows 系统存在. 如果 "system_info_use_windows_time" 选项关闭了, 就会忽略
windows 系统的时间, SystemInfoFrame 中的时区按钮是显示出来的, 用户也可以手动选择时区.

## 自动检测时区
当 "system_info_timezone_use_geoip" 选项开启时, 会尝试通过 http 请求的方式
(系统已经通过有线网络连接到了互联网), 根据机器当前的 IP 确定它所在的时区. 当然了, 这个方法并不
完全准确.

当 "system_info_timezone_use_regdomain" 选项开启时, 如果机接上存在无线网卡时, 会尝试
使用它监听附近的无线热点发送的广播信号, 根据里面的 country code 来确定相应的时区.

## 根据用户选择的语言设置时区
在 resources/languages.json 文件里面, 还定义了一种语言相对应的默认时区.
如果用户选定了这种语言, 也会在时区页面, 选中相应的时区名.
它的优先级要比自动检测的时区低.
