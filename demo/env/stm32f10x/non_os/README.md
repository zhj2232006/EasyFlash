# stm32f10x 裸机Demo

---

## 1、简介

在app中通过ymodem将固件下载到app备份区域，然后写入环境变量:iap_need_copy_app和iap_copy_app_size,non_os demo作为bootloader,在`main()`函数中判断是否需要复制新的固件数据。

### 1.1、使用方法

- 1、打开电脑的终端与Demo的串口1进行连接，串口配置 115200 8 1 N，此时可以在终端上看到Demo的打印日志
- 2、断电重启Demo
- 3、等待重启完成后，即可查看到打印信息中的启动次数有所增加

> 注意：对于无法连接终端的用户，也可以使用仿真器与Demo平台进行连接，来观察启动次数的变化

## 2、文件（夹）说明

`components\easyflash\port\ef_port.c` 移植参考文件

`RVMDK` 下为Keil工程文件

`EWARM` 下为IAR工程文件