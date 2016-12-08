# 手动分区操作
手动分区分为两部分, 后端部分位于partman/目录, 它负责读取磁盘信息并执行实际的分区
操作, 通过 PartitionManager来调用; 前端部分位于ui/, 用于处理UI相关的操作, 与
后端的所有通信都是通过PartitionDelegate来完成.
 
初始时, PartitionManager在 工作线程中先扫描当前系统的所有磁盘信息,
并将结果(DeviceList)发送给PartitionDelegate, 这时, 用户就可以开发做分区操作了.
用户分区完成后, PartitionDelegate 将用户的分区操作(OperationList)发送给
PartitionManager, 后者在工作线程中执行实际的分区过程, 并将结果返回给UI线程.


# Tips
后台开始扫描磁盘之前, 会尽可能卸载所有可卸载的分区 (Debug 模式不会卸载交换分区). 如果卸载失败,
在分区页面里, 会隐藏正在被使用的分区.