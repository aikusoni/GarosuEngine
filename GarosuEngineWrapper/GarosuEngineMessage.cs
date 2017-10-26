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

    public sealed class BaseMessage : IDisposable
    {
        private IntPtr msgPtr = IntPtr.Zero;
        public IntPtr MessagePtr
        {
            get { return msgPtr; }
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

        public bool SetParam(String paramName, bool value)
        {
            return SafeNativeMethods.SetParam_Bool(msgPtr, new StringBuilder(paramName), value);
        }

        public bool SetParam(String paramName, IntPtr handle)
        {
            return SafeNativeMethods.SetParam_VoidPtr(msgPtr, new StringBuilder(paramName), handle);
        }
    }
}
