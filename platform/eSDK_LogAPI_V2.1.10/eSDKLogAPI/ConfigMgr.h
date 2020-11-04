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
#ifndef CONFIG_MGR_H_
#define CONFIG_MGR_H_


#include <string>
#include "eSDKLogDataType.h"


// ��־ʵ��
#define ConfigMgrInstance()		(eSDK::ConfigMgr::Instance())

// ��־�����ļ�
#define CLIENT_CONFIG_FILE		"eSDKClientLogCfg.ini"

// �����ļ��ڵ�
#define SECTION_NAME_LOG		"LogConfig"
#define SECTION_NAME_UPLOAD		"UploadConfig"
#define SECTION_NAME_PRODUCT	"ProductConfig"
#define SECTION_NAME_LOGPATH	"LogPath"


namespace eSDK
{
	class ConfigMgr
	{
	private:
		ConfigMgr(void);
		~ConfigMgr(void);

	public:
		static ConfigMgr& Instance(void);

	public:
        //û���ļ�Ҳ���Գ�ʼ��
        void InitWithDefaultVal();
		// ��ʼ��
#if defined ANDROID
		bool Init(const std::string& iniInfo, const std::string& product);
#else
		bool Init(const std::string& iniFile, const std::string& product);
#endif
		// ȥ��ʼ��
		bool Uninit(void);

		// ��ȡ��־����
		unsigned int GetLogSize_Interface(void) const;
		unsigned int GetLogSize_Operation(void) const;
		unsigned int GetLogSize_Run(void) const;
		unsigned int GetLogNum_Interface(void) const;
		unsigned int GetLogNum_Operation(void) const;
		unsigned int GetLogNum_Run(void) const;
		unsigned int GetLogLevel_Interface(void) const;
		unsigned int GetLogLevel_Operation(void) const;
		unsigned int GetLogLevel_Run(void) const;
		const std::string& GetLogProduct(void) const;
		std::string GetLogPath(void);
		// ��־�ļ���ȡȨ�� add by cwx298983 2016.06.29 Start
		unsigned int GetFilePermission(void) const;
		// ��־�ļ���ȡȨ�� add by cwx298983 2016.06.29 End

		void SetLogSize_Interface(const unsigned int nSize);
		void SetLogSize_Operation(const unsigned int nSize);
		void SetLogSize_Run(const unsigned int nSize);
		void SetLogNum_Interface(const unsigned int nNum);
		void SetLogNum_Operation(const unsigned int nNum);
		void SetLogNum_Run(const unsigned int nNum);
		void SetLogLevel_Interface(const unsigned int nLevel);
		void SetLogLevel_Operation(const unsigned int nLevel);
		void SetLogLevel_Run(const unsigned int nLevel);
		void SetLogPath(const std::string& strLogPath);

	private:
		unsigned int m_uiLogSize_Interface;		// ��־��С
		unsigned int m_uiLogSize_Operation;		// ��־��С
		unsigned int m_uiLogSize_Run;			// ��־��С
		unsigned int m_uiLogNum_Interface;		// ��־����
		unsigned int m_uiLogNum_Operation;		// ��־����
		unsigned int m_uiLogNum_Run;			// ��־����
		unsigned int m_uiLogLevel_Interface;	// ��־����
		unsigned int m_uiLogLevel_Operation;	// ��־����
		unsigned int m_uiLogLevel_Run;			// ��־����
		std::string m_strProductName;			// ��־����ģ��
		std::string m_strLogPath;				// ��־����·��
		// ��־�ļ���ȡȨ�� add by cwx298983 2016.06.29 Start
		unsigned int m_uiLogFilePermission;		// ��־�ļ���ȡȨ��
		// ��־�ļ���ȡȨ�� add by cwx298983 2016.06.29 End
	};
}

#endif

