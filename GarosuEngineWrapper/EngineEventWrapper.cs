using System;

namespace GarosuEngineWrapper
{
    public enum EngineEventId : UInt32
    {

    }

    class BaseEvent : Parameter
    {
        IntPtr evtPtr = IntPtr.Zero;
        protected override IntPtr GetParamPtr()
        {
            return evtPtr;
        }

        BaseEvent(IntPtr evtPtr)
        {
            this.evtPtr = evtPtr;
        }
    }
}
