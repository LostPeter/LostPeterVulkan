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

#include "../include/FParameter.h"

namespace LostPeterFoundation
{
	////////////////////////// FParameter //////////////////////////
	FParameter::FParameter(const String& strName, const String& strValue, FParameterType eParameter)
		: m_strName(strName)
		, m_strValue(strValue)
		, m_eParamter(eParameter)
	{

	}
    

	////////////////////////// FParameterCommand ///////////////////
	FParameterCommand::FParameterCommand()
	{

	}

	FParameterCommand::~FParameterCommand()
	{

	}


	////////////////////////// FParameterDictionary ////////////////
	FParameterDictionary::FParameterDictionary() 
	{

	}

	FParameterDictionary::~FParameterDictionary()
	{
		Clear();
	}	


	////////////////////////// FParameterInterface /////////////////
	ParameterDictionaryMap FParameterInterface::ms_mapParameterDictionary;
	FParameterInterface::FParameterInterface()
	{

	}

	FParameterInterface::~FParameterInterface()
	{

	}

	void FParameterInterface::CleanupParameterDictionaryMap()
	{
		ms_mapParameterDictionary.clear();
	}

}; //LostPeterFoundation