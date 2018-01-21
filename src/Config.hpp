#ifndef __CONFIG_HPP
#define __CONFIG_HPP

enum Direction
{
    Forward,
    Backward
};

enum ExpectedVersion
{
    Any = -1, // Append As-is
    NoStream = -2, // No Events
    EmptyStream = -3 // Has Never Existed
};

typedef char sha256_t[32];
typedef char eventid_t[16];
typedef sha256_t position_t;

struct FixedString
{
    int Length;
    char String[];
};

#endif

