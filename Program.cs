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

            Task.Factory.StartNew(async () => Listen());

            var evt = new Event
            {
                EventId = Guid.NewGuid(),
                StreamId = "player/adam",
                EventType = "PlayerCreated",
                Version = 0,
                Metadata = Encoding.UTF8.GetBytes("{}"),
                Payload = Encoding.UTF8.GetBytes("{}"),
            };

            for (var i = 0; i < 1000_000; ++i) //100M -> 2.4GiB
            {
                evt.EventId = Guid.NewGuid();
                eventStore.Append(evt);
            }

            {
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
            }

            {
                var watch2 = new Stopwatch();
                var interations = 0;
                var position = Guid.Empty.ToByteArray();
                var chunk = new EventChunk(new byte[0], 0, false);
                var events = 0;
                IntPtr marker = IntPtr.Zero;

                watch2.Start();

                while (!chunk.Empty)
                {
                    chunk = eventStore.GetFromFast(position, ref marker);
                    events += chunk.Parse().ToList().Count;
                    ++interations;
                }

                watch2.Stop();
                Console.WriteLine($"GetFromFast: {watch2.ElapsedMilliseconds}, interations: {interations}, events: {events}, eps: {1000 * events / (1 + watch2.ElapsedMilliseconds)}");
            }


            Console.ReadKey();
        }

        private async static void Listen()
        {
            var listener = new TcpListener(IPAddress.Parse("127.0.0.1"), 4000);

            listener.Server.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReuseAddress, true);
            listener.Start();

            while (true)
            {
                var client = await listener.AcceptTcpClientAsync();

                await FeedClient(client);
            }
        }

        private static async Task FeedClient(TcpClient client)
        {
            var eventStore = EventStore.Instance();
            var stream = client.GetStream();
            EventChunk chunk;

            for (var i = 0; i < 1000; ++i)
            {
                var position = Guid.Empty.ToByteArray();
                IntPtr ptr = IntPtr.Zero;

                do
                {
                    chunk = await Task.FromResult(eventStore.GetFromFast(position, ref ptr));
                    await stream.WriteAsync(chunk.Buffer, 0, chunk.Length);
                } while (!chunk.Empty);
            }
        }
#endif
    }
}
//evt.EventId = new Guid(i, 0, 0, new byte[8]);
//1000000/552;10000000/5759;100000000/52384