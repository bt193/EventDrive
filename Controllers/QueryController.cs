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
            public Guid Next { get; set; }
        }

        [HttpGet]
        [Route("{from}")]
        public Result Get(Guid from)
        {
            var position = from.ToByteArray();
            var watch = new Stopwatch();
            watch.Start();

            var events = EventStore.Instance().GetFrom(position).Parse();

            return new Result
            {
                Events = events,
                LatencyMs = watch.ElapsedMilliseconds,
                Next = new Guid(position)
            };
        }

        [HttpPut]
        public object Put([FromBody] List<Event> @event)
        {
            //EventStore.Instance().Append(@event);
            return new { Message = "OK" };
        }
    }
}
