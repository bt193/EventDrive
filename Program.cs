using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Microsoft.AspNetCore;
using Microsoft.AspNetCore.Hosting;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Logging;

namespace InvokeC
{
    public class Program
    {
#if false        
        public static void Main(string[] args)
        {
            EventStore.Instance();
            BuildWebHost(args).Run();
        }

        public static IWebHost BuildWebHost(string[] args) =>
            WebHost.CreateDefaultBuilder(args)
                .UseStartup<Startup>()
                .Build();
#else
        static void Main(string[] args)
        {
            var eventStore = EventStore.Instance();
            var watch = new Stopwatch();
            watch.Start();

            Task.Run(async () => Listen());

            var evt = new Event
            {
                EventId = Guid.NewGuid(),
                StreamId = "player/adam",
                EventType = "PlayerCreated",
                Version = 0,
                Metadata = Encoding.UTF8.GetBytes("{}"),
                Payload = Encoding.UTF8.GetBytes("{}"),
            };

            for (var i = 0; i < 25_000; ++i) //100M -> 2.4GiB
            {
                evt.EventId = Guid.NewGuid();
                eventStore.Append(evt);
            }

            var watch2 = new Stopwatch();
            var interations = 0;
            var position = Guid.Empty.ToByteArray();
            var chunk = new EventChunk(new byte[0], 0, false);
            var events = 0;

            watch2.Start();

            while (!chunk.Empty)
            {
                chunk = eventStore.GetFrom(position);
                events += chunk.Parse().ToList().Count;
                ++interations;
            }

            watch2.Stop();
            Console.WriteLine($"GetFrom: {watch2.ElapsedMilliseconds}, interations: {interations}, events: {events}, eps: {1000 * events / (1 + watch2.ElapsedMilliseconds)}");

            Console.WriteLine("Success!");
            Console.ReadKey();
        }

        private async static void Listen()
        {
            var listener = new TcpListener(IPAddress.Parse("0.0.0.0"), 4000);

            listener.Server.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReuseAddress, true);
            listener.Start();

            while (true)
            {
                var client = await listener.AcceptTcpClientAsync();

                Console.WriteLine("new client!");
                FeedClient(client);
            }
        }

        private static async Task FeedClient(TcpClient client)
        {
            var eventStore = EventStore.Instance();
            var stream = client.GetStream();
            EventChunk chunk;
            var events = 0L;

            var watch = new Stopwatch();
            var loop = 0L;
            var length = 0L;

            watch.Start();

            while (true)
            {
                var position = Guid.Empty.ToByteArray();

                do
                {
                    chunk = await Task.Run(() => eventStore.GetFrom(position));
                    await stream.WriteAsync(chunk.Buffer, 0, chunk.Length);
                    length += chunk.Length;
                } while (!chunk.Empty);
                events += 25_000L;
                if (++loop % 40 == 0)
                Console.WriteLine($"Push: {events}, eps: {1000L*events/watch.ElapsedMilliseconds}, data: {length / events}");
            }
        }
#endif
    }
}
