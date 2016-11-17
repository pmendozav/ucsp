#ifdef _WIN32 //Windows
//..
#else //Linux
#define MAKEWORD(a,b) ((WORD)(((BYTE)(a))|(((WORD)((BYTE)(b)))<<8)))
#endif

using p_word = unsigned short;
using p_dword = unsigned long;
using p_long = long;
using p_byte = unsigned char;
using p_char = char;

using p_int = int;
using p_double = double;
using p_bool = bool;
using p_uchar = uchar;

template<class T> using p_vector = std::vector<T>;


#include "bitmap.h"
#include "common.h"
#include "helper_functions.h"
#include "improc.h"

using UCImage = core::Image<p_uchar>;
