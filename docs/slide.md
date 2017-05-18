# 关于
这个文档介绍了定制安装过程中的幻灯片的方法.

## 图片
不同的平台及语言, 要显示不同的图片.

### 语言
语言是通过目录来区分的, 包括 default, zh_CN, zh_TW:
* 如果是简体中文的话, 就使用 zh_CN 中的设置;
* 如果是繁体中文, 就使用 zh_TW 目录;
* 其它语言里, 都使用 default, 显示英文图片.

### 平台相关
不同的平台, 要显示的图片, 是在幻灯片目录iamges.json 文件中定义的.
* 如果幻灯片目录里不包含 images.json 文件, 就会使用这个目录里面的所有 png 图片.
* 如果在 images.json 文件中, 没有定义这个平台的图片列表, 就会使用这个目录里面的所有 png 图片.
* 图片显示顺序是在 images.json 中定义的顺序.

### 格式要求
幻灯片中的图片应该 png 格式的图片, 分辨率不大于 960*640, 有半透明效果.

## 选项
* "install_progress_page_disable_slide", 启用/禁用幻灯片切换.
* "install_progress_page_disable_slide_animation", 启用/禁用幻灯片切换时的过渡动画.
* "install_progress_page_animation_duration", 定义幻灯片切换时间间隔, 默认是 8000 毫秒.

## oem
如果需要定制幻灯片动画, 只需要把 slide 目录, 放置到 ISO_ROOT/oem/ 目录里面就可以了. 其中,
slide 目录的结果应该与 resources/slide 目录相同.
比如, 简体中文界面, 幻灯片的查找顺序依次是:
* ISO_ROOT/oem/slide/zh_CN
* /usr/share/deepin-installer/resources/slide/zh_CN
* ISO_ROOT/oem/slide/default
* /usr/share/deepin-installer/resources/slide/default
