using System;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;

namespace InvokeC
{
    class Program
    {
        static void Main(string[] args)
        {
            var eventStore = new EventStore();
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
                eventStore.Append(evt);
            }

            // Console.WriteLine($"Watch: {watch.ElapsedMilliseconds}");

            //var position = StringToByteArray("564914116c1860309f6dc8776cb9a15ac750c6e5");

            var watch2 = new Stopwatch();
            var interations = 0;
            var position = StringToByteArray("0000000000000000000000000000000000000000");
            // Task<EventChunk> task;
            var chunk = new EventChunk(new byte[0], 0, false);
            var events = 0;

            watch2.Start();

            while (!chunk.Empty)
            {
                // task = Task.FromResult(eventStore.GetFrom(position));
                // chunk.Parse().ToList();
                // chunk = task.Result;
                chunk = eventStore.GetFrom(position);
                events += chunk.Parse().ToList().Count;
                ++interations;
            }

            Console.WriteLine($"watch2: {watch2.ElapsedMilliseconds}, interations: {interations}, events: {events}");
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
