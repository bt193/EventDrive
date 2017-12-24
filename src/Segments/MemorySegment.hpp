#ifndef __MEMORYSEGMENT__
#define __MEMORYSEGMENT__

enum OpCode : int
{
  None = 0,
  BeginTransaction = -1,
  Commit = -2
};

class MemorySegment
{
public:
  MemorySegment(char *memory, int length, int fullLength);
  virtual ~MemorySegment();
  virtual bool BeginTransaction();
  virtual bool Commit();
  virtual bool Put(char *memory, int length);
  char *Peek(int length);
  void Skip(int length);
  virtual void FlushAsync();
  virtual void FlushSync();

protected:
  char *_memory;
  int _length;
  int _fullLength;
  int _offset = 0;
  bool _dirty = false;

private:
  MemorySegment() = default;
  MemorySegment(MemorySegment &&) = default;
  MemorySegment(const MemorySegment &) = default;
  MemorySegment &operator=(MemorySegment &&) = default;
  MemorySegment &operator=(const MemorySegment &) = default;
};

#endif
