using System;
using System.Runtime.InteropServices;
using System.Text;

namespace GarosuEngineWrapper
{
    public enum EngineMessageId : uint
    {
        None,
        TestMessage,
        SetApplicationStoragePath,
        SetVideoOutputHandle, // "output_target" : (void*)
    }

    public sealed class BaseMessage : Parameter, IDisposable
    {
        private IntPtr msgPtr = IntPtr.Zero;
        public IntPtr MessagePtr
        {
            get { return msgPtr; }
        }

        protected override IntPtr GetParamPtr()
        {
            return msgPtr;
        }

        public BaseMessage(EngineMessageId msgId)
        {
            msgPtr = SafeNativeMethods.CreateMessage((UInt32)msgId);
        }

        ~BaseMessage()
        {
            if (msgPtr != IntPtr.Zero)
            {
                SafeNativeMethods.DeleteMessage(msgPtr);
                msgPtr = IntPtr.Zero;
            }
        }

        public void Dispose()
        {
            if (msgPtr != IntPtr.Zero)
            {
                SafeNativeMethods.DeleteMessage(msgPtr);
                msgPtr = IntPtr.Zero;
            }
        }
    }
}
