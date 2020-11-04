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
#include "LoggerMgr.h"
#include "FileMgr.h"
#include "eSDKLog.h"
#include "ConfigMgr.h"
#include "FileMgr.h"
#include "eSDKTool.h"
#include <algorithm> 

#if defined(ANDROID) || defined(TARGET_MAC_OS) || defined(TARGET_OS_IPHONE)
#include <pthread.h>
#include <string.h>
#define MAX_PRODUCT_LENGTH 128
#endif

namespace eSDK
{
	LoggerMgr::LoggerMgr(void)
	{
		m_mutex = VOS_CreateMutex();
	}

	LoggerMgr::~LoggerMgr(void)
	{
 		try{
			if (VOS_NULL != m_mutex)
			{
				(void)VOS_DestroyMutex(m_mutex);
				m_mutex = NULL;
			}
			while (m_ProductMap.size() > 1)
			{
				PRODUCT_MAP::iterator itor = m_ProductMap.begin();
				PRODUCT_DATA& data = itor->second;
				if (NULL != data.peSDKLog)
				{
					delete data.peSDKLog;
					data.peSDKLog = NULL;
				}
				m_ProductMap.erase(itor);
			}
			if (1 == m_ProductMap.size())
			{
				PRODUCT_MAP::iterator itor = m_ProductMap.begin();
				PRODUCT_DATA& data = itor->second;
				//�ر���־��ӡ
				if (NULL != data.peSDKLog)
				{
					delete data.peSDKLog;
					data.peSDKLog = NULL;
				}				
				m_ProductMap.erase(itor);
			}
		}
		catch(...)
		{

		}
	}

	LoggerMgr& LoggerMgr::Instance(void)
	{
		static LoggerMgr inst;		
		return inst;
	}

#if defined ANDROID
	int LoggerMgr::initAndroid(const std::string& product, const std::string& iniInfo, unsigned int logLevel[LOG_CATEGORY], const std::string& logPath)
	{
		(void)VOS_MutexLock(m_mutex);

		//�Ѿ��������˳�
		if (m_ProductMap.end() != m_ProductMap.find(product))
		{      
			(void)VOS_MutexUnlock(m_mutex);
			return RET_INVALID_PARA;
		}

		//��ȡ������
		if(!ConfigMgrInstance().Init(iniInfo, product)) //�����ȡ���������ļ������Զ���ȡ·��
		{
			(void)VOS_MutexUnlock(m_mutex);
			return RET_INVALID_PARA;
		}

		if (!iniInfo.empty())
		{
			PRODUCT_DATA data;
			data.peSDKLog = new eSDKLog;

			data.logPath = getLogPathByCondition(logPath);

			data.productname = ConfigMgrInstance().GetLogProduct();
			if (NULL == data.peSDKLog)
			{
				(void)VOS_MutexUnlock(m_mutex);
				return RET_INVALID_PARA;
			}

			//������־�ļ�
			if (!data.peSDKLog->InitLog4cpp(product, logLevel, data.logPath))
			{
				delete data.peSDKLog;
				(void)VOS_MutexUnlock(m_mutex);
				return RET_INVALID_PARA;
			}
			(void)m_ProductMap.insert(std::make_pair(product, data));

			(void)VOS_MutexUnlock(m_mutex);

			return RET_SUCCESS;
		}
		else
		{
			PRODUCT_DATA data;
			data.productname = ConfigMgrInstance().GetLogProduct();

			data.logPath = getLogPathByCondition(logPath);

			ConfigMgrInstance().SetLogLevel_Interface(logLevel[0]);
			ConfigMgrInstance().SetLogLevel_Operation(logLevel[1]);
			ConfigMgrInstance().SetLogLevel_Run(logLevel[2]);

			(void)m_ProductMap.insert(std::make_pair(product, data));
		}
		(void)VOS_MutexUnlock(m_mutex);

		return RET_SUCCESS;
	}

#else

	int LoggerMgr::init(const std::string& product, const std::string& iniFile, unsigned int logLevel[LOG_CATEGORY], const std::string& logPath)
	{
		(void)VOS_MutexLock(m_mutex);

		//�Ѿ��������˳�
		if (m_ProductMap.end() != m_ProductMap.find(product))
		{
			(void)VOS_MutexUnlock(m_mutex);
			return RET_INVALID_PARA;
		}

		//��ȡ������
		if(!ConfigMgrInstance().Init(iniFile, product)) //�����ȡ���������ļ������Զ���ȡ·��
		{
			(void)VOS_MutexUnlock(m_mutex);
			return RET_INVALID_PARA;
		}

		if (!iniFile.empty())
		{
			PRODUCT_DATA data;
			data.peSDKLog = new eSDKLog; 
			data.iniFile = iniFile;
			data.logPath = getLogPathByCondition(logPath);
			data.productname = ConfigMgrInstance().GetLogProduct();
			data.filePermission = ConfigMgrInstance().GetFilePermission();
			if (NULL == data.peSDKLog)
			{
				(void)VOS_MutexUnlock(m_mutex);
				return RET_INVALID_PARA;
			}

			//������־�ļ�
			if (!data.peSDKLog->InitLog4cpp(product, logLevel, data.logPath, data.filePermission))
			{
				delete data.peSDKLog;
				(void)VOS_MutexUnlock(m_mutex);
				return RET_INVALID_PARA;
			}
			(void)m_ProductMap.insert(std::make_pair(product, data));


		}
#if defined(TARGET_OS_IPHONE)||defined(TARGET_OS_MAC)
		else
		{
			PRODUCT_DATA data;
			data.productname = ConfigMgrInstance().GetLogProduct();

			data.logPath = getLogPathByCondition(logPath);

			ConfigMgrInstance().SetLogLevel_Interface(logLevel[0]);
			ConfigMgrInstance().SetLogLevel_Operation(logLevel[1]);
			ConfigMgrInstance().SetLogLevel_Run(logLevel[2]);

			(void)m_ProductMap.insert(std::make_pair(product, data));
		}
#endif
		(void)VOS_MutexUnlock(m_mutex);

		return RET_SUCCESS;
	}
#endif

	int LoggerMgr::uninit(const std::string& product)
	{
		(void)VOS_MutexLock(m_mutex);

		//�Ѿ��˳�
		PRODUCT_MAP::iterator itor = m_ProductMap.find(product);
		if (m_ProductMap.end() == itor)
		{
			(void)VOS_MutexUnlock(m_mutex);
			return RET_INVALID_PARA;
		}
		PRODUCT_DATA& data = itor->second;

		//���һ����Ʒ
		if (1 == m_ProductMap.size())
		{
			//�ر���־��ӡ
			if (NULL != data.peSDKLog)
			{
				data.peSDKLog->ShutDownLog4cpp();
				//�ͷŸò�Ʒ��־��ӡ
#if !defined(ANDROID) && !defined(TARGET_MAC_OS) && !defined(TARGET_OS_IPHONE)
				delete data.peSDKLog;
				data.peSDKLog = NULL;
#endif	
			}
			//�����ļ������˳�
			(void)ConfigMgrInstance().Uninit();
		}
		else
		{
			//�ͷŸò�Ʒ��־��ӡ
            data.peSDKLog->UninitLog4cpp();
			delete data.peSDKLog;
			data.peSDKLog = NULL;
		}
		
		//�ͷŲ�Ʒ����
		m_ProductMap.erase(itor);

		(void)VOS_MutexUnlock(m_mutex);

		return RET_SUCCESS;//lint -e438
	}

	int LoggerMgr::printInterfacelog(const std::string& product,int iLevel,const std::string& strcontent)
	{
		unsigned int nRet = RET_SUCCESS;
		if (NULL == m_mutex)
		{
			return RET_NULL_POINTER;
		}
		// ��ȡ��־��ӡʵ��
		(void)VOS_MutexLock(m_mutex);
		PRODUCT_MAP::iterator itor = m_ProductMap.find(product);
		if (m_ProductMap.end() == itor)
		{
			(void)VOS_MutexUnlock(m_mutex);
			return RET_INVALID_PARA;
		}
		PRODUCT_DATA& data = itor->second;
		if (NULL == data.peSDKLog)
		{
			(void)VOS_MutexUnlock(m_mutex);
			return RET_INVALID_PARA;
		}
		(void)VOS_MutexUnlock(m_mutex);

		switch (iLevel)
		{
		case INFO_LEVEL:
			{
				data.peSDKLog->printIntInfolog(strcontent);
                break;

			}
		case ERROR_LEVEL:
			{
				data.peSDKLog->printIntErrorlog(strcontent);
                break;

			}
		default:
			{
				nRet = RET_INVALID_PARA;
                break;
			}
		}

		return nRet;
	}

	int LoggerMgr::printOperationlog(const std::string& product,int iLevel,const std::string& strcontent)
	{
		if (NULL == m_mutex)
		{
			return RET_NULL_POINTER;
		}
		// ��ȡ��־��ӡʵ��
		(void)VOS_MutexLock(m_mutex);
		PRODUCT_MAP::iterator itor = m_ProductMap.find(product);
		if (m_ProductMap.end() == itor)
		{
			(void)VOS_MutexUnlock(m_mutex);
			return RET_INVALID_PARA;
		}
		PRODUCT_DATA& data = itor->second;
		if (NULL == data.peSDKLog)
		{
			(void)VOS_MutexUnlock(m_mutex);
			return RET_INVALID_PARA;
		}
		(void)VOS_MutexUnlock(m_mutex);

		switch (iLevel)
		{
		case DEBUG_LEVEL:
			{
				data.peSDKLog->printOptDebuglog(strcontent);
				return RET_SUCCESS;
			}
		case INFO_LEVEL:
			{
				data.peSDKLog->printOptInfolog(strcontent);
				return RET_SUCCESS;
			}
		case WARN_LEVEL:
			{
				data.peSDKLog->printOptWarnlog(strcontent);
				return RET_SUCCESS;
			}
		case ERROR_LEVEL:
			{
				data.peSDKLog->printOptErrorlog(strcontent);
				return RET_SUCCESS;
			}
		default:
			{
				return RET_INVALID_PARA;
			}
		}
	}
	int LoggerMgr::printRunlog(const std::string& product,int iLevel,const std::string& strcontent)
	{
		if (NULL == m_mutex)
		{
			return RET_NULL_POINTER;
		}
		// ��ȡ��־��ӡʵ��
		(void)VOS_MutexLock(m_mutex);
		PRODUCT_MAP::iterator itor = m_ProductMap.find(product);
		if (m_ProductMap.end() == itor)
		{
			(void)VOS_MutexUnlock(m_mutex);
			return RET_INVALID_PARA;
		}
		PRODUCT_DATA& data = itor->second;
		if (NULL == data.peSDKLog)
		{
			(void)VOS_MutexUnlock(m_mutex);
			return RET_INVALID_PARA;
		}
		(void)VOS_MutexUnlock(m_mutex);

		switch (iLevel)
		{
		case DEBUG_LEVEL:
			{
				data.peSDKLog->printRunDebuglog(strcontent);
				return RET_SUCCESS;
			}
		case INFO_LEVEL:
			{
				data.peSDKLog->printRunInfolog(strcontent);
				return RET_SUCCESS;
			}
		case WARN_LEVEL:
			{
				data.peSDKLog->printRunWarnlog(strcontent);
				return RET_SUCCESS;
			}
		case ERROR_LEVEL:
			{
				data.peSDKLog->printRunErrorlog(strcontent);
				return RET_SUCCESS;
			}
		default:
			{
				return RET_INVALID_PARA;
			}
		}
	}


	void LoggerMgr::rollover(const std::string& product)
	{
		// ��ȡ��־��ӡʵ��
		(void)VOS_MutexLock(m_mutex);
		PRODUCT_MAP::iterator itor = m_ProductMap.find(product);
		if (m_ProductMap.end() == itor)
		{
			(void)VOS_MutexUnlock(m_mutex);
			return;
		}
		PRODUCT_DATA& data = itor->second;
		if (NULL == data.peSDKLog)
		{
			(void)VOS_MutexUnlock(m_mutex);
			return;
		}
		(void)VOS_MutexUnlock(m_mutex);

		// ֻ�败��Interface��־���ƽ� modify by cwx298983 2015.12.08 Start
		data.peSDKLog->InvokeIntLogRolling();
		// ֻ�败��Interface��־���ƽ� modify by cwx298983 2015.12.08 End
	}

	// Get the path of the logs Add by cwx298983 2016.04.19 Start 
	std::string LoggerMgr::getLogPathByCondition(const std::string& strIniLogPath)
	{
		std::string strRetLogPath("");

		// �ж�LogInit�д����logPath�����Ƿ�Ϊ�գ���Ϊ�գ���ֱ��ʹ��logPathΪ��־����·����
		// Ϊ�����ж�ini�ļ���logPath�Ƿ�Ϊ�գ������Ϊ�գ���ʹ��Ĭ��·����������·���µ�log�ļ�����
		// ��Ϊ�������ini��logPath����ȷ����־����·��
		if (INVALID_LOG_PATH == strIniLogPath)
		{
			strRetLogPath = ConfigMgrInstance().GetLogPath();
			if (strRetLogPath.empty())
			{
#ifdef WIN32
				strRetLogPath = eSDKTool::GetDllPath()+"log\\";
#else
				strRetLogPath = eSDKTool::GetDllPath()+"log/";
#endif
			}
			else
			{
				// �ж��Ƿ�Ϊ���·��
				if (eSDKTool::IsRelativePath(strRetLogPath))
				{
					// ���صı����Ǿ���·��
					strRetLogPath = eSDKTool::GetDllPath() + strRetLogPath;
				}
			}
		}
		else
		{
			strRetLogPath = strIniLogPath;
		}

		return strRetLogPath;
	}
	// Get the path of the logs Add by cwx298983 2016.04.19 End


#if defined(ANDROID) || defined(TARGET_MAC_OS) || defined(TARGET_OS_IPHONE)
    int LoggerMgr::setLogPropertyEx(const char* product, unsigned int logSize[LOG_CATEGORY], unsigned int logNum[LOG_CATEGORY])
	{
		(void)VOS_MutexLock(m_mutex);

		PRODUCT_MAP::iterator itor = m_ProductMap.find(product);
		if (m_ProductMap.end() == itor)
		{
			return RET_NotInit;
		}
		PRODUCT_DATA& tmpData = itor->second;
        
        PRODUCT_DATA data;
        data.productname = tmpData.productname;
        data.logPath = tmpData.logPath;

		ConfigMgrInstance().SetLogSize_Interface(logSize[0]);
		ConfigMgrInstance().SetLogSize_Operation(logSize[1]);
		ConfigMgrInstance().SetLogSize_Run(logSize[2]);

		ConfigMgrInstance().SetLogNum_Interface(logNum[0]);
		ConfigMgrInstance().SetLogNum_Operation(logNum[1]);
		ConfigMgrInstance().SetLogNum_Run(logNum[2]);

		data.peSDKLog = new eSDKLog;
		if (NULL == data.peSDKLog)
		{
			(void)VOS_MutexUnlock(m_mutex);
			return RET_INVALID_PARA;
		}

		unsigned int logLevel[3] = {INVALID_LOG_LEVEL, INVALID_LOG_LEVEL, INVALID_LOG_LEVEL};
		//������־�ļ�
		if (!data.peSDKLog->InitLog4cpp(product, logLevel, data.logPath))
		{
			delete data.peSDKLog;
			(void)VOS_MutexUnlock(m_mutex);
			return RET_INVALID_PARA;
		}
        m_ProductMap.erase(itor);
        (void)m_ProductMap.insert(std::make_pair(product, data));

		(void)VOS_MutexUnlock(m_mutex);
		return RET_SUCCESS;
	}

#endif
}

