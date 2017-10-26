using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;
using GarosuEngineWrapper;

namespace GarosuEngineEditor
{
    /// <summary>
    /// App.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class App : Application, IDisposable
    {
        EngineWrapper engine = null;

        App()
        {
        }

        ~App()
        {
            this.Dispose(false);
        }

        public delegate bool CallbackSubscriber(int param1);
        public CallbackSubscriber subscribers;

        protected bool HandleEngineCallback(int param1)
        {
            if (subscribers != null) 
                subscribers(param1);

            return true;
        }

        protected override void OnStartup(StartupEventArgs e)
        {
            base.OnStartup(e);

            engine = new GarosuEngineWrapper.EngineWrapper();

            engine.RegisterHandler(HandleEngineCallback);
            engine.InitializeEngine();

            var testMsg = new BaseMessage(EngineMessageId.TestMessage);
            testMsg.SetParam("bool", true);
            testMsg.SetParam("voidptr", IntPtr.Zero);
            testMsg.SetParam("integer", 1234567891234567);
            testMsg.SetParam("double", 3.1415926535897);
            testMsg.SetParam("string", "Hello, I am c# program. 혹시 한글도 읽을 수 있나요?");

            engine.SendMessage(testMsg);
        }

        protected override void OnExit(ExitEventArgs e)
        {
            Dispose();

            base.OnExit(e);
        }

        private bool disposed;
        public void Dispose()
        {
            this.Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (this.disposed) return;
            if (disposing)
            {
                if (engine != null)
                {
                    engine.FinalizeEngine();
                    engine.Dispose();
                }
            }

            this.disposed = true;
        }
    }
}
