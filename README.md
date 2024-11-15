# ToolBoxFromProject

## 项目简介

`ToolBoxFromProject`中包含了一些简单的、通用的操作类，包括使用Windows环境下Cpp的socket通信类SimpleWinSocket,快速将的十六进制数据 `A0` 转换成 `0xA0` 格式的script.exe
有诸如此类的一些简单的小工具。

## 安装指南
Cpp工具均以`.hpp`的文件格式给出，直接`#include`至项目中就可以直接使用，python编写的工具会封装成`.exe`文件，并在终端提示使用用法。

### 系统要求

- Windows 操作系统
- Visual Studio 或其他支持 C++ 的编译器
- Winsock 2.2 库

### 安装步骤

1. 克隆项目仓库：
   ```sh
   git clone https://github.com/yourusername/SimpleWinSocket.git