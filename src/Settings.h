#pragma once
#include "stdafx.h"

struct SSetting
{
#if !_DEBUG
   bool bComentar = false;
   bool bRandSuposition = true;
#else
   bool bComentar = true;
   bool bRandSuposition = false;
#endif
   bool bValidSpecial = false;
};
