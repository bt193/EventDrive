#ifndef __EVENTSTORE_HPP__
#define __EVENTSTORE_HPP__

class Index;

class EventStore
{
public:
  EventStore();
  ~EventStore();
  void Initialize();
  void Put(char *memory, int length);

private:
  EventStore(EventStore &&) = default;
  EventStore(const EventStore &) = default;
  EventStore &operator=(EventStore &&) = default;
  EventStore &operator=(const EventStore &) = default;
  Index *_index;
};

#endif