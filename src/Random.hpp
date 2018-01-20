#ifndef __RANDOM_HPP__
#define __RANDOM_HPP__
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

class Random
{
  public:
    Random();
    ~Random();
    void Scramble(char *dest, size_t length);

  private:
    Random(Random &&) = default;
    Random(const Random &) = default;
    Random &operator=(Random &&) = default;
    Random &operator=(const Random &) = default;
    int _fd;
};

Random::Random()
{
    _fd = open("/dev/urandom", O_RDONLY);
}

Random::~Random()
{
    close(_fd);
}

void Random::Scramble(char *dest, size_t length)
{
    read(_fd, dest, length);
}

#endif
