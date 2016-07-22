
# 关于

hooks 里面的任务, 是按照排序, 串行运行的. 如果某个hook里一个重要的步骤执行失败了, 
之后的所有任务都不会被执行, 这时, 安装器会直接转到错误提醒界面. 所以, hook脚本内部
最好做好各自的清理工作.

## 文件名命名方式
* 以 `.job` 结尾
* 以两位数字开头, 比如 `01-print-info.job` , 因为hooks的执行顺序是根据文件名
升序来排序的
* 以连字符 `-` 来连接文件名中的各个段, 不要使用下划线
* 支持指定特定的平台, 比如 `04-check-part-alpha.job` 只会在 Alpha 架构的机器里
执行, 目前支持的平台架构有 `alpha`, `arm`, `mips`, `x86` 这四种, 并不区分32位
与64位. 当然, 直接在hook脚本内部判断平台构架也是可以的

## oem
目录`ISO_ROOT/oem/hooks` 用来存放自定义的 hooks, 其写法与目录结构需要与安装器
自带的hooks一致.
 
下面列举一些oem hook的可能写法:

* 安装额外的deb包, 只需要把deb放到 `oem/hooks/in_chroot/` 目录, 然后写一个hook
脚本, 比如 `08-install-wps.job`, 在这个脚本里调用 `dpkg -i xxx.deb` 来安装
deb包
* 修改桌面的默认壁纸, 创建 `oem/hooks/in_chroot/44-update-wallpaper.job` 并
在这里脚本里, 把指定的壁纸复制到新系统即可