using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace InvokeC
{
    public class EventChunk
    {
        public byte[] Buffer { get; private set; }
        public int Length { get; private set; }
        public bool Empty { get; private set; }

        public EventChunk(byte[] buffer, int length, bool? empty = null)
        {
            Length = length;
            Buffer = buffer;
            Empty = empty.HasValue ? empty.Value : (length <= 0);
        }

        public IEnumerable<object> Parse()
        {
            if (Empty)
            {
                yield break;
            }

            using (var memoryStream = new MemoryStream())
            {
                memoryStream.Write(Buffer, 0, Length);
                memoryStream.Seek(0, SeekOrigin.Begin);

                using (var reader = new BinaryReader(memoryStream))
                {
                    while (reader.PeekChar() != -1)
                    {
                        var evt = new Event();

                        var length = reader.ReadInt32();
                        var op = reader.ReadInt32();
                        memoryStream.Seek(20, SeekOrigin.Current);
                        evt.EventId = new Guid(reader.ReadBytes(16));
                        var eventTypeLength = reader.ReadInt32();
                        evt.EventType = Encoding.UTF8.GetString(reader.ReadBytes(eventTypeLength));
                        var streamIdLength = reader.ReadInt32();
                        evt.StreamId = Encoding.UTF8.GetString(reader.ReadBytes(streamIdLength));
                        evt.Version = reader.ReadInt32();
                        var metadataLength = reader.ReadInt32();
                        evt.Metadata = reader.ReadBytes(metadataLength);
                        var payloadLength = reader.ReadInt32();
                        evt.Payload = reader.ReadBytes(payloadLength);

                        yield return evt;
                    }
                }
            }
        }
    }
}
