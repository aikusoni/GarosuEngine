using System;
using System.Runtime.InteropServices;

namespace GarosuEngineWrapper
{

    public sealed class EngineWrapper : IDisposable
    {
        // Garosu Engine Instance
        private IntPtr engine = IntPtr.Zero;

        public EngineWrapper()
        {
            engine = SafeNativeMethods.CreateEngine();
        }

        ~EngineWrapper()
        {
            if (engine != IntPtr.Zero)
            {
                FinalizeEngine();
                SafeNativeMethods.DeleteEngine(engine);
                engine = IntPtr.Zero;
            }
        }

        public void Dispose()
        {
            if (engine != IntPtr.Zero)
            {
                FinalizeEngine();
                SafeNativeMethods.DeleteEngine(engine);
                engine = IntPtr.Zero;
            }
        }

        public bool InitializeEngine()
        {
            SafeNativeMethods.RegisterCallback(engine, CallbackFnc);
            return SafeNativeMethods.InitializeEngine(engine);
        }

        public bool FinalizeEngine()
        {
            return SafeNativeMethods.FinalizeEngine(engine);
        }

        public bool SendMessage(BaseMessage msg)
        {
            return SafeNativeMethods.SendMessage(engine, msg.MessagePtr);
        }

        public bool CallbackFnc(IntPtr engineEvent)
        {

            return true;
        }

        public delegate bool handler(int param1);
        handler engineCallbackHandler = null;

        public bool RegisterHandler(handler engineCallbackHandler)
        {
            this.engineCallbackHandler = engineCallbackHandler;
            return true;
        }
    }
}
