using System;
using System.Runtime.InteropServices;
using System.Text;

namespace GarosuEngineWrapper
{
    internal static class SafeNativeMethods
    {
        ///// Engine
        [DllImport(Utils.EngineDll, EntryPoint = "CreateEngine", CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr CreateEngine();

        [DllImport(Utils.EngineDll, EntryPoint = "DeleteEngine", CallingConvention = CallingConvention.Cdecl)]
        internal static extern bool DeleteEngine(IntPtr enginePtr);

        [DllImport(Utils.EngineDll, EntryPoint = "Initialize", CallingConvention = CallingConvention.Cdecl)]
        internal static extern bool InitializeEngine(IntPtr enginePtr);

        [DllImport(Utils.EngineDll, EntryPoint = "Finalize", CallingConvention = CallingConvention.Cdecl)]
        internal static extern bool FinalizeEngine(IntPtr enginePtr);

        [DllImport(Utils.EngineDll, EntryPoint = "SendMessage", CallingConvention = CallingConvention.Cdecl)]
        internal static extern bool SendMessage(IntPtr enginePtr, IntPtr garosuMessage);

        [DllImport(Utils.EngineDll, EntryPoint = "RegisterCallback", CallingConvention = CallingConvention.Cdecl)]
        internal static extern bool RegisterCallback(IntPtr enginePtr, GarosuEngineCallback callback);
        internal delegate bool GarosuEngineCallback(IntPtr engineEvent);
  
        ///// Message
        [DllImport(Utils.EngineDll, EntryPoint = "CreateMessage", CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr CreateMessage(UInt32 msgId);

        [DllImport(Utils.EngineDll, EntryPoint = "DeleteMessage", CallingConvention = CallingConvention.Cdecl)]
        internal static extern bool DeleteMessage(IntPtr msgPtr);
  

        ///// Parameter
        [DllImport(Utils.EngineDll, EntryPoint = "SetParam_Bool", CallingConvention = CallingConvention.Cdecl)]
        internal static extern bool SetParam_Bool(IntPtr msgPtr, StringBuilder paramName, bool paramValue);

        [DllImport(Utils.EngineDll, EntryPoint = "SetParam_VoidPtr", CallingConvention = CallingConvention.Cdecl)]
        internal static extern bool SetParam_VoidPtr(IntPtr msgPtr, StringBuilder paramName, IntPtr paramValue);
    }
}
