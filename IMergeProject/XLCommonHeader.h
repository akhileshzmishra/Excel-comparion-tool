#ifndef _XLCOMMONHEADER_H_
#define _XLCOMMONHEADER_H_



//#include "libxl.h"
#include <string>
//#include <wstring>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <map>
#include "InternalSmartPointer.h"
#include <math.h>
#include <cstdio>
using namespace std;
//using namespace libxl;

#include "BasicExcel.hpp"
#include "ExcelFormat.h"
using namespace ExcelFormat;

enum XLCellDataType
{
	Integer,
	Double,
	String,
	WString,
	Char,

	InvalidDataType
};



typedef std::wstring WStrType;







#endif