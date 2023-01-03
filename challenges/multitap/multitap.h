#ifndef MULTITAP_H
#define MULTITAP_H

#include <map>
#include <vector>

int encode_character(const char ch, char *multitap);

void encode(const char* plaintext, char* multitap);

#endif