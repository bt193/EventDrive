using System;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using Newtonsoft.Json;

namespace InvokeC
{
    public class Event
    {
        public Guid EventId { get; set; }
        public string EventType { get; set; }
        public string StreamId { get; set; }
        public int Version { get; set; }
        public byte[] Metadata { get; set; }
        public byte[] Payload { get; set; }
    }

    class Program
    {
        [DllImport("libc.so.6")]
        private static extern int getpid();

        [DllImport("./inc.so")]
        private static extern void Initialize();

        [DllImport("./inc.so")]
        private static extern void AppendEvent(byte[] @event, int length);

        [DllImport("./inc.so")]
        private static extern int ReadEventsFromFrom(byte[] position, byte[] buffer, int length);

        public static void Append(Event evt)
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

        static void Main(string[] args)
        {
            Initialize();

            var watch = new Stopwatch();
            watch.Start();

            var evt = new Event
            {
                EventId = Guid.NewGuid(),
                StreamId = "player/adam",
                EventType = "PlayerCreated",
                Version = 0,
                Metadata = Encoding.UTF8.GetBytes("{}"),
                Payload = Encoding.UTF8.GetBytes("{}"),
            };

            for (var i = 0; i < 1_000_000; ++i)
            {
                evt.EventId = new Guid(i, 0, 0, new byte[8]);
                Append(evt);
            }

            // Console.WriteLine($"Watch: {watch.ElapsedMilliseconds}");

            var position = StringToByteArray("0a910620ba1454a139beb417786415e1ce899afd");
            //var position = StringToByteArray("564914116c1860309f6dc8776cb9a15ac750c6e5");
            var buffer = new byte[4096];
            int len = -1;
            var watch2 = new Stopwatch();
            var interations = 0;

            watch2.Start();
            while (len != 0)
            {
                ++interations;
                // Console.WriteLine($"--- {len}");
                len = ReadEventsFromFrom(position, buffer, buffer.Length);
                Parse(buffer, len);
                //break;
            }
            Console.WriteLine($"watch2: {watch2.ElapsedMilliseconds}, interations: {interations}");
        }

        private static void Parse(byte[] buffer, int len)
        {
            using (var memoryStream = new MemoryStream())
            {
                memoryStream.Write(buffer, 0, len);
                memoryStream.Seek(0, SeekOrigin.Begin);

                using (var reader = new BinaryReader(memoryStream))
                {
                    while (reader.PeekChar() != -1)
                    {
                        var evt = new Event();

                        var length = reader.ReadInt32();
                        var op = reader.ReadInt32();
                        reader.ReadBytes(20); // hash
                        evt.EventId = new Guid(reader.ReadBytes(16)); // eventId
                        var eventTypeLength = reader.ReadInt32();
                        evt.EventType = Encoding.UTF8.GetString(reader.ReadBytes(eventTypeLength));
                        var streamIdLength = reader.ReadInt32();
                        evt.StreamId = Encoding.UTF8.GetString(reader.ReadBytes(streamIdLength));
                        evt.Version = reader.ReadInt32();
                        var metadataLength = reader.ReadInt32();
                        evt.Metadata = reader.ReadBytes(metadataLength);
                        var payloadLength = reader.ReadInt32();
                        evt.Payload = reader.ReadBytes(payloadLength);

                        // var json = JsonConvert.SerializeObject(evt);
                        // Console.WriteLine(json);
                    }
                }
            }
        }

        public static byte[] StringToByteArray(string hex)
        {
            var result = Enumerable.Range(0, hex.Length)
                .Where(x => x % 2 == 0)
                .Select(x => Convert.ToByte(hex.Substring(x, 2), 16))
                .ToArray();

            return result;
        }
    }
}
