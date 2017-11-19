using System.IO;
using System.Runtime.InteropServices;
using System.Text;
using System;

namespace InvokeC
{
    public class EventStore
    {
        [DllImport("./es.so")]
        private static extern void Initialize();

        [DllImport("./es.so")]
        private static extern void AppendEvent(byte[] @event, int length);

        [DllImport("./es.so")]
        private static extern int ReadEventsFrom(byte[] position, byte[] buffer, int length);

        [DllImport("./es.so")]
        private static extern int ReadEventsFromFast(byte[] position, byte[] buffer, int length, ref IntPtr ptr);

        protected EventStore()
        {
            Initialize();
        }

        private static EventStore _instance;
        private static object _mutex = new object();

        public static EventStore Instance()
        {
            if (_instance != null)
            {
                return _instance;
            }

            lock (_mutex)
            {
                if (_instance == null)
                {
                    _instance = new EventStore();
                }
            }
            return _instance;
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
            var buffer = new byte[1024 * 40];
            var len = ReadEventsFrom(position, buffer, buffer.Length);

            return new EventChunk(buffer, len);
        }
    }
}
