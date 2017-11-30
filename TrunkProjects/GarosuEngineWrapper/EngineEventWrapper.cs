using System;

namespace GarosuEngineWrapper
{
    public enum EngineEventId : UInt32
    {
        None = 0u,
        InitializeComplete,
    }

    public class BaseEvent : Parameter
    {
        IntPtr evtPtr = IntPtr.Zero;
        protected override IntPtr GetParamPtr()
        {
            return evtPtr;
        }

        public BaseEvent(IntPtr evtPtr)
        {
            this.evtPtr = evtPtr;
        }

        public EngineEventId GetEventId()
        {
            return (EngineEventId)SafeNativeMethods.GetEventId(evtPtr);
        }
    }
}
