# 目标
解决华为云OBS C SDK 编译不通过的问题，使用CMake管理依赖
# 原始地址
https://github.com/huaweicloud/huaweicloud-sdk-c-obs
## 安装指南
### 0 依赖的独立安装包
OpenSSL    
curl     
libXml2    
Iconv     
pcre    
### 1 安装 huaweisecurec
```
cd platform/huaweisecurec/src
make 
```
### 2 安装 log4cpp
```
cd platform/eSDK_LogAPI_V2.1.10/log4cpp 
mkdir build
cd build 
cmake ..
make 
sudo make install
```
### 3 安装 eSDKLogAPI

```
cd /platform/eSDK_LogAPI_V2.1.10/eSDKLogAPI
make 
```
### 3 安装 OBS-C-SDK
```
mkdir build
cd build
cmake ..
make
make install
```