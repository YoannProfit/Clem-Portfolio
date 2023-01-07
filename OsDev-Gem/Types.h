#ifndef TYPES_H
#define TYPES_H

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

// ----------------------------------- //

typedef char int8;
typedef short int16;
typedef int int32;
typedef long long int64;

// ----------------------------------- //

typedef unsigned int size_t;
typedef uint8 uchar

// ----------------------------------- //

typedef Core::String<char> Core::String
typedef Core::String<wchar_t> Core::WString

typedef Line<Core::String> Line
typedef Line<Core::WString> WLine

typedef DataParameters<char, Core::String> DataParameters
typedef DataParameters<wchar_t, Core::WString> WDataParameters

#endif