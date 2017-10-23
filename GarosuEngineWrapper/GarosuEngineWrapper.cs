using System;
using System.Runtime.InteropServices;

namespace GarosuEngineWrapper
{

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

        [DllImport(Utils.EngineDll, EntryPoint = "RegisterCallback", CallingConvention = CallingConvention.Cdecl)]
        private static extern bool RegisterCallback(IntPtr enginePtr, GarosuEngineCallback callback);
        delegate bool GarosuEngineCallback(int param1);

        // Garosu Engine Instance
        private IntPtr engine;

        public EngineWrapper()
        {
            engine = MakeGarosuEngine();
        }

        ~EngineWrapper()
        {
            if (engine != null)
            {
                FinalizeEngine();
                DeleteGarosuEngine(engine);
            }
        }

        public bool InitializeEngine()
        {
            RegisterCallback(engine, CallbackFnc);
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

        public bool CallbackFnc(int param1)
        {
            if (userCallbackFnc != null)
                return userCallbackFnc(param1);

            return true;
        }

        public delegate bool UserCallbackFnc(int param1);
        UserCallbackFnc userCallbackFnc = null;

        public bool RegisterCallback(UserCallbackFnc callbackFunction)
        {
            userCallbackFnc = callbackFunction;
            return true;
        }
    }
}
