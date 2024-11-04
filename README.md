# gta5_ol_timer

## 中文

### 简介

本项目为MSVC项目（由于兼容问题暂不考虑支持gcc）目前只有命令行界面，利用GTAOL战局中一分钟稳定等于现实中两秒钟，基于RefTime.json文件中的基准时间对与系统当前时间换算，从而可以离线显示目前GTA线上战局的全局时间

![image-20241104200517461](https://raw.githubusercontent.com/Goplush/gta5_ol_timer/refs/heads/main/assets/image-20241104200517461.png)





### 开发环境

Microsoft Visual Studio Professional 2022
版本 17.9.4
VisualStudio.17.Release/17.9.4+34714.143
Microsoft .NET Framework
版本 4.8.09037

已安装的版本: Professional

Visual C++ 2022   00483-00000-00004-AA043
Microsoft Visual C++ 2022

### 使用到的第三方库

github.com/nlohmann/json



### 项目架构

```
/Project
│
├── Peoject.cpp             // 主程序
├── time_manager.h          // 时间管理模块的头文件
├── time_manager.cpp        // 时间管理模块的实现文件
├── json_handler.h          // JSON 处理模块的头文件
├── json_handler.cpp        // JSON 处理模块的实现文件
└── Reftime.json            // 存储游戏时间-现实时间对的 JSON 文件
```

其中json处理代码由ChatGPT生成





### 使用方法

#### 基础

把生成的`Project.exe`和`RefTime.json`放在同一个文件夹，然后命令行窗口运行即可

#### 如果不准

如果gtaol更新，时间可能刷新，可以用任何一个时区的时间（你的本地时间或者标准UTF时间）和线上战局手机时间来更新`Reftime.json `文件，就像下面这张图一样

![image-20241104201432225](https://raw.githubusercontent.com/Goplush/gta5_ol_timer/refs/heads/main/assets/image-20241104201432225.png)

