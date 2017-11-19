using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;

namespace InvokeC.Controllers
{
    [Route("api/[controller]")]
    public class QueryController : Controller
    {
        public class Result
        {
            public IEnumerable<object> Events { get; set; }
            public long LatencyMs { get; set; }
            public string Next { get; set; }
        }

        [HttpGet]
        [Route("{from}")]
        public Result Get(string from = "0000000000000000000000000000000000000000")
        {
            var position = StringToByteArray(from);
            var watch = new Stopwatch();
            watch.Start();

            var events = EventStore.Instance().GetFrom(position).Parse();

            return new Result
            {
                Events = events,
                LatencyMs = watch.ElapsedMilliseconds,
                Next = ByteArrayToString(position)
            };
        }

        [HttpPut]
        public object Put([FromBody] Event @event)
        {
            EventStore.Instance().Append(@event);
            return new { Message = "OK" };
        }

        public static byte[] StringToByteArray(string hex)
        {
            var result = Enumerable.Range(0, hex.Length)
                .Where(x => x % 2 == 0)
                .Select(x => Convert.ToByte(hex.Substring(x, 2), 16))
                .ToArray();

            return result;
        }

        public static string ByteArrayToString(byte[] ba)
        {
            StringBuilder hex = new StringBuilder(ba.Length * 2);

            foreach (byte b in ba)
                hex.AppendFormat("{0:x2}", b);
            return hex.ToString();
        }
    }
}
