#ifndef __EVENTCOLLISIONINDEX_HPP__
#define __EVENTCOLLISIONINDEX_HPP__

class EventCollisionIndex
{
  public:
    EventCollisionIndex();
    ~EventCollisionIndex();

  private:
    EventCollisionIndex(EventCollisionIndex &&) = default;
    EventCollisionIndex(const EventCollisionIndex &) = default;
    EventCollisionIndex &operator=(EventCollisionIndex &&) = default;
    EventCollisionIndex &operator=(const EventCollisionIndex &) = default;
};

#endif
