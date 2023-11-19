/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/SceneManagerEnumerator.h"
#include "../include/SceneManager.h"

template<> LostPeter::SceneManagerEnumerator* LostPeterFoundation::FSingleton<LostPeter::SceneManagerEnumerator>::ms_Singleton = nullptr;

namespace LostPeter
{
	SceneManagerEnumerator* SceneManagerEnumerator::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	SceneManagerEnumerator& SceneManagerEnumerator::GetSingleton()
	{  
		F_Assert(ms_Singleton && "SceneManagerEnumerator::GetSingleton")
		return (*ms_Singleton);     
	}

    SceneManagerEnumerator::SceneManagerEnumerator()
        : Base("SceneManagerEnumerator")
    {

    }
    SceneManagerEnumerator::~SceneManagerEnumerator()
    {
		Destroy();
    }

	void SceneManagerEnumerator::Destroy()
	{
		
	}

    bool SceneManagerEnumerator::Init()
	{	
		

		return true;
	}



}; //LostPeter