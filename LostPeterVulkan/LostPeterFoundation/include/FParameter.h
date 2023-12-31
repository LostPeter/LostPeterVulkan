/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-01-01
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _F_PARAMETER_H_
#define _F_PARAMETER_H_

#include "FPreDefine.h"

namespace LostPeterFoundation
{
	////////////////////////// FParameter //////////////////////////
	class foundationExport FParameter
	{
	public:
        FParameter(const String& strName, const String& strValue, FParamterType eParamter);

	public:
		String m_strName;
		String m_strValue;
		FParamterType m_eParamter;

	};
	typedef std::vector<FParameter>	FParameterVector;
	typedef std::map<String, FParameter> FParameterMap;


	////////////////////////// FParameterCommand ///////////////////
	class foundationExport FParameterCommand
	{	
	public:
		FParameterCommand();
		virtual ~FParameterCommand();

	public:
		virtual String DoGet(const void* pTarget) const = 0;
		virtual void DoSet(void* pTarget, const String& strValue) = 0;
	};
	

	////////////////////////// FParameterDictionary ////////////////
	class foundationExport FParameterDictionary
	{
		friend class FParameterInterface;

	public:
		FParameterDictionary();
		~FParameterDictionary();

	protected:
		FParameterVector m_aParameter;				
		FParameterCommandPtrMap	m_mapParameterCommand;		

	public:
		F_FORCEINLINE  const FParameterVector& GetParameterVector() const { return m_aParameter; }
		F_FORCEINLINE  FParameterVector& GetParameterVector() { return m_aParameter; }
		F_FORCEINLINE  const FParameterCommandPtrMap& GetParameterCommandPtrMap() const { return m_mapParameterCommand; }
		F_FORCEINLINE  FParameterCommandPtrMap& GetParameterCommandPtrMap() { return m_mapParameterCommand; }

		F_FORCEINLINE void AddParameter(const FParameter& parameter, FParameterCommand* pParameterCmd)
		{
			m_aParameter.push_back(parameter);
			m_mapParameterCommand[parameter.m_strName] = pParameterCmd;
		}

	public:
		FParameterCommand* GetParameterCommand(const String& strName)
		{
			FParameterCommandPtrMap::iterator itFind = m_mapParameterCommand.find(strName);
			if (itFind != m_mapParameterCommand.end())
			{
				return itFind->second;
			}
			return nullptr;
		}

		const FParameterCommand* GetParameterCommand(const String& strName) const
		{
			FParameterCommandPtrMap::const_iterator itFind = m_mapParameterCommand.find(strName);
			if (itFind != m_mapParameterCommand.end())
			{
				return itFind->second;
			}
			return nullptr;
		}

		void Clear()
		{
			m_aParameter.clear();
			m_mapParameterCommand.clear();
		}
	};
	typedef std::map<String, FParameterDictionary> ParameterDictionaryMap;


	////////////////////////// FParameterInterface /////////////////
	class foundationExport FParameterInterface 
	{
	public:
		FParameterInterface();
		virtual ~FParameterInterface();

	public:
	protected:
		static ParameterDictionaryMap ms_mapParameterDictionary;		
		String m_strNameParameterDictionary;	

	public:
		static void	CleanupParameterDictionaryMap();

	public:
		FParameterDictionary* GetParameterDictionary()
		{
			ParameterDictionaryMap::iterator itFind = ms_mapParameterDictionary.find(m_strNameParameterDictionary);
			if (itFind != ms_mapParameterDictionary.end())
			{
				return &(itFind->second);
			}
			return nullptr;
		}

		const FParameterDictionary*	GetParameterDictionary() const
		{
			ParameterDictionaryMap::const_iterator itFind = ms_mapParameterDictionary.find(m_strNameParameterDictionary);
			if (itFind != ms_mapParameterDictionary.end())
			{
				return &(itFind->second);
			}
			return nullptr;
		}


		const FParameterVector&	GetParameterVector() const
		{
			static FParameterVector aParameter;
			const FParameterDictionary* pParameterDictionary = GetParameterDictionary();
			if (pParameterDictionary)
				return pParameterDictionary->GetParameterVector();
			return aParameter;
		}

		virtual String GetParameter(const String& strName) const
		{
			const FParameterDictionary* pParameterDictionary = GetParameterDictionary();
			if (pParameterDictionary)
			{
				const FParameterCommand* pCmd = pParameterDictionary->GetParameterCommand(strName);
				if (pCmd)
				{
					return pCmd->DoGet(this);
				}
			}
			return "";
		}

		virtual bool SetParameter(const String& strName, const String& strValue)
		{
			FParameterDictionary* pParameterDictionary = GetParameterDictionary();
			if (pParameterDictionary)
			{
				FParameterCommand* pCmd = pParameterDictionary->GetParameterCommand(strName);
				if (pCmd)
				{
					pCmd->DoSet(this, strValue);
					return true;
				}
			}
			return false;
		}

		virtual void SetParameterMap(const NameValuePairMap& mapParameter)
		{
			NameValuePairMap::const_iterator it, itEnd;
			itEnd = mapParameter.end();
			for (it = mapParameter.begin(); it != itEnd; ++it)
			{
				SetParameter(it->first, it->second);
			}
		}

		virtual void CopyParameterTo(FParameterInterface* pDest) const
		{
			const FParameterDictionary* pParameterDictionary = GetParameterDictionary();
			if (pParameterDictionary)
			{
				FParameterVector::const_iterator it;
				for (it = pParameterDictionary->m_aParameter.begin();
					 it != pParameterDictionary->m_aParameter.end(); ++it)
				{
					pDest->SetParameter(it->m_strName, GetParameter(it->m_strName));
				}
			}
		}		

	protected:
		bool CreateParameterDictionary(const String& nameClass)
		{
			m_strNameParameterDictionary = nameClass;
			if (ms_mapParameterDictionary.find(nameClass) == ms_mapParameterDictionary.end())
			{
				ms_mapParameterDictionary[nameClass] = FParameterDictionary();
				return true;
			}
			return false;
		}
	};


}; //LostPeterFoundation

#endif