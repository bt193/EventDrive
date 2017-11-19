using System.IO;
using System.Runtime.InteropServices;
using System.Text;

namespace InvokeC
{
    public class EventStore
    {
        [DllImport("./inc.so")]
        private static extern void Initialize();

        [DllImport("./inc.so")]
        private static extern void AppendEvent(byte[] @event, int length);

        [DllImport("./inc.so")]
        private static extern int ReadEventsFromFrom(byte[] position, byte[] buffer, int length);

        public EventStore()
        {
            Initialize();
        }

        public void Append(Event evt)
        {
            using (var memoryStream = new MemoryStream())
            {
                using (var writer = new BinaryWriter(memoryStream))
                {
                    writer.Write(evt.EventId.ToByteArray());
                    writer.Write(evt.EventType.Length);
                    writer.Write(Encoding.UTF8.GetBytes(evt.EventType));
                    writer.Write(evt.StreamId.Length);
                    writer.Write(Encoding.UTF8.GetBytes(evt.StreamId));
                    writer.Write(evt.Version);
                    writer.Write(evt.Metadata.Length);
                    writer.Write(evt.Metadata);
                    writer.Write(evt.Payload.Length);
                    writer.Write(evt.Payload);
                    writer.Flush();

                    var buffer = new byte[memoryStream.Length];

                    memoryStream.Seek(0, SeekOrigin.Begin);
                    memoryStream.Read(buffer, 0, buffer.Length);
                    AppendEvent(buffer, buffer.Length);
                }
            }
        }

        public EventChunk GetFrom(byte[] position)
        {
            var buffer = new byte[1024*64];
            var len = ReadEventsFromFrom(position, buffer, buffer.Length);

            return new EventChunk(buffer, len);
        }
    }
}
