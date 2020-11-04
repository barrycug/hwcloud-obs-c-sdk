/*********************************************************************************
* Copyright 2019 Huawei Technologies Co.,Ltd.
* Licensed under the Apache License, Version 2.0 (the "License"); you may not use
* this file except in compliance with the License.  You may obtain a copy of the
* License at
* 
* http://www.apache.org/licenses/LICENSE-2.0
* 
* Unless required by applicable law or agreed to in writing, software distributed
* under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
* CONDITIONS OF ANY KIND, either express or implied.  See the License for the
* specific language governing permissions and limitations under the License.
**********************************************************************************
*/
#ifndef _ESDKLOG_H
#define _ESDKLOG_H

// includes & warning suppress
#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4244) // conversion from 'size_t' to '...', possible loss of data
#pragma warning(disable:4267) // conversion from 'size_t' to '...', possible loss of data
#pragma warning(disable:4512) // assignment operator could not be generated
#endif
#ifdef WIN32
#include "./log4cpp/config-win32.h"
#endif
#include "./log4cpp/Category.hh"
#include "./log4cpp/RollingFileAppender.hh"
#include "./log4cpp/OstreamAppender.hh"
#include "./log4cpp/PatternLayout.hh"

#include "eSDKLogDataType.h"
#include "./vos/vos.h"
using namespace VPP;

#ifdef WIN32
#pragma warning(pop)
#endif

//��־�ļ�
#define LOG_INTERFACE_FILE			"interface.log"
#define LOG_OPERATE_FILE			"operation.log"
#define LOG_RUN_FILE				"run.log"
//���ݽӿ��ļ�
#define LOG_INTERFACE_BACKUP_FILE	"interface.backup.log"
//Ĭ����־Ŀ¼
#define LOG_DIRECTORY				"log"

// ȥ������|����
//interface��־��ʽ"%d���� %t�߳�id %p���ȼ� %m��Ϣ %n���з�"
#define LOG_INTERFACE_PATTERN			"%d{%Y-%m-%d %H:%M:%S %l}|%5p|%m%n"
//operation��־��ʽ"%d���� %t�߳�id %p���ȼ� %m��Ϣ %n���з�"
#define LOG_OPERATION_PATTERN			"%d{%Y-%m-%d %H:%M:%S %l}|%5p|%m%n"
//run��־��ʽ"%d���� %t�߳�id %p���ȼ� %m��Ϣ %n���з�"
#define LOG_RUN_PATTERN					"%d{%Y-%m-%d %H:%M:%S %l}|%5p|[%t]%m%n"
//interface������־��ʽ"%m��Ϣ"
#define LOG_INTERFACE_BACKUP_PATTERN	"%m"
// ȥ������|����

//��־ʵ����
#define LOG_INTERFACE_INSTANCE		"INTERFACE"
#define LOG_OPERATE_INSTANCE		"OPERATE"
#define LOG_RUN_INSTANCE			"RUN"
#define LOG_INTERFACE_BACKUP_INSTANCE	"INTERFACEBACKUP"

enum LOGTYPE
{
	INTERFACE_TYPE	=	0,
	OPERATION_TYPE	=	1,
	RUN_TYPE		=	2
};



//��־��
class eSDKLog
{
public:
	eSDKLog(void);
	~eSDKLog(void);

public:	
	bool InitLog4cpp(const std::string& product, unsigned int logLevel[LOG_CATEGORY], const std::string& logPath, int mode=00644);
	
	// solve the problem that the process ended without call function LogFini.
    // shut down one log4cpp object.
	void UninitLog4cpp(void);
    // shut down all log4cpp objects. It is called when the process is end or function LogFini is called.
    void ShutDownLog4cpp(void);
    // solve the problem that the process ended without call function LogFini.

	void InvokeIntLogRolling(void);//����interface��־�ƽ�
	void InvokeOptLogRolling(void);//����operation��־�ƽ�
	void InvokeRunLogRolling(void);//����run��־�ƽ�	
	void printIntInfolog(const std::string& strcontent);
	void printIntErrorlog(const std::string& strcontent);
	void printOptDebuglog(const std::string& strcontent);
	void printOptInfolog(const std::string& strcontent);
	void printOptWarnlog(const std::string& strcontent);
	void printOptErrorlog(const std::string& strcontent);
	void printRunDebuglog(const std::string& strcontent);
	void printRunInfolog(const std::string& strcontent);
	void printRunWarnlog(const std::string& strcontent);
	void printRunErrorlog(const std::string& strcontent);

private:
	std::string GetLog4cppPath(const std::string& logPath, const std::string& strLogType);
	log4cpp::Priority::PriorityLevel GetLog4cppLevel(unsigned int logLevel, const std::string& strLogType);

private:
	VPP::VOS_Mutex* m_IntMutex;//�ӿ�����־��
	VPP::VOS_Mutex* m_OptMutex;//��������־��
	VPP::VOS_Mutex* m_RunMutex;//��������־��
	VPP::VOS_Mutex* m_IntBackupMutex;//�ӿ��౸����־��

	std::string m_InstanceInterfaceName;
	std::string m_InstanceOperationName;
	std::string m_InstanceRunName;

	std::string m_logPath;
	unsigned int m_nInterfaceLevel;
};

#endif
