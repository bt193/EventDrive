#ifndef __POSITIONINDEX_HPP__
#define __POSITIONINDEX_HPP__

class PositionIndex
{
  public:
    PositionIndex();
    ~PositionIndex();

  private:
    PositionIndex(PositionIndex &&) = default;
    PositionIndex(const PositionIndex &) = default;
    PositionIndex &operator=(PositionIndex &&) = default;
    PositionIndex &operator=(const PositionIndex &) = default;
};

#endif
