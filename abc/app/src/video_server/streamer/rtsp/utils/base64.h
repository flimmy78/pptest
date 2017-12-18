#ifndef GW_TIME_UTIL_H
#define GW_TIME_UTIL_H

#include <string>

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int len);
std::string base64_decode(std::string const& s);


#endif