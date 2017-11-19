using System;

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
}
