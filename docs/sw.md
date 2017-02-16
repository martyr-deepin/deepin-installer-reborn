这个文件用于记录申威平台相关的一些问题

## 安装器背景图
申威平台的 ImageMagick 在调用 blur 滤镜时, 会出现数学计算问题, 生成模糊的背景图会失败.
在把安装器的deb包装到系统里之前, 可以预先在其它平台生成好背景图, 然后放到申威的
 `/tmp/default_background.jpg`, 就可以不需要再用 ImageMagick 生成了. 
生成模糊图片的脚本位于 `tools/blur_background_image.sh`