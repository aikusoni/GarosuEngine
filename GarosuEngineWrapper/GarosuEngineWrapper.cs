using System;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;
using System.Windows.Forms;

namespace GarosuEngineWrapper
{
    class Utils
    {
        public const string EngineDll = "GarosuEngine.dll";

        [MethodImpl(MethodImplOptions.NoInlining)]
        public static string GetCurrentMethod()
        {
            var st = new StackTrace();
            var sf = st.GetFrame(1);
            var m = sf.GetMethod();

            return m.ReflectedType.Name + m.Name;
        }
    }

    public class DummyMessage
    {
        protected IntPtr msgPtr;
        public IntPtr MessagePtr {
            get { return msgPtr; }
        }
    }

    public class BaseMessage : DummyMessage
    {
        [DllImport(Utils.EngineDll, EntryPoint = "MakeBaseMessage", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr MakeBaseMessage(uint msgId);
        
        [DllImport(Utils.EngineDll, EntryPoint = "DeleteMessage", CallingConvention = CallingConvention.Cdecl)]
        private static extern bool DeleteMessage(IntPtr msg);

        public BaseMessage(uint msgId)
        {
            try
            {
                msgPtr = MakeBaseMessage(msgId);
            }
            catch (OutOfMemoryException ex)
            {
                MessageBox.Show(Utils.GetCurrentMethod() + "(" + msgId + ") allocation failed. " + ex.ToString());
            }
        }

        ~BaseMessage()
        {
            if (msgPtr != null)
                DeleteMessage(msgPtr);
        }
    }

    public class StringMessage : DummyMessage
    {
        [DllImport(Utils.EngineDll, EntryPoint = "MakeStringMessage", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr MakeStringMessage(uint msgId, [MarshalAs(UnmanagedType.LPStr)] String str);

        [DllImport(Utils.EngineDll, EntryPoint = "DeleteMessage", CallingConvention = CallingConvention.Cdecl)]
        private static extern bool DeleteMessage(IntPtr msg);

        public StringMessage(uint msgId, String str)
        {
            try
            {
                msgPtr = MakeStringMessage(msgId, str);
            }
            catch (OutOfMemoryException ex)
            {
                MessageBox.Show(Utils.GetCurrentMethod() + "(" + msgId + ") allocation failed. " + ex.ToString());
            }
        }

        ~StringMessage()
        {
            if (msgPtr != null)
                DeleteMessage(msgPtr);
        }
    }

    public class EngineWrapper
    {
        [DllImport(Utils.EngineDll, EntryPoint = "MakeGarosuEngine", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr MakeGarosuEngine();

        [DllImport(Utils.EngineDll, EntryPoint = "DeleteGarosuEngine", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr DeleteGarosuEngine(IntPtr enginePtr);

        [DllImport(Utils.EngineDll, EntryPoint = "Initialize", CallingConvention = CallingConvention.Cdecl)]
        private static extern bool InitializeEngine(IntPtr enginePtr);

        [DllImport(Utils.EngineDll, EntryPoint = "Finalize", CallingConvention = CallingConvention.Cdecl)]
        private static extern bool FinalizeEngine(IntPtr enginePtr);

        [DllImport(Utils.EngineDll, EntryPoint = "SendMessage", CallingConvention = CallingConvention.Cdecl)]
        private static extern bool SendMessage(IntPtr enginePtr, IntPtr garosuMessage);

        // Garosu Engine Instance
        private IntPtr engine;

        public EngineWrapper()
        {
            engine = MakeGarosuEngine();
        }

        ~EngineWrapper()
        {
            if (engine != null)
                DeleteGarosuEngine(engine);
        }

        public bool InitializeEngine()
        {
            return InitializeEngine(engine);
        }

        public bool FinalizeEngine()
        {
            return FinalizeEngine(engine);
        }

        public bool SendMessage(DummyMessage msg)
        {
            return SendMessage(engine, msg.MessagePtr);
        }
    }
}
