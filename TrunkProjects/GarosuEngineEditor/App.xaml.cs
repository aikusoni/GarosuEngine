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

        public delegate bool CallbackSubscriber(BaseEvent evt);
        public CallbackSubscriber subscribers;

        protected bool HandleEngineCallback(BaseEvent evt)
        {
            if (subscribers != null)
            {
                if (subscribers.Invoke(evt)) return true;
            }

            var evtId = evt.GetEventId();
            switch (evtId)
            {
                case EngineEventId.None:
                    break;

                case EngineEventId.InitializeComplete:
                    {
                        Console.Out.WriteLineAsync("* EngineEvent : " + evtId.ToString() + " Occured");
                        String VersionNo = "";
                        String BuildNo = "";
                        String AuthorName = "";
                        Int64 Year = 0;
                        bool TrueOr = true;
                        double PI = 3.1;
                        double e = 2.7;
                        if (evt.GetParam("Version", ref VersionNo)) Console.Out.WriteLineAsync("** Version : " + VersionNo);
                        if (evt.GetParam("BuildNo", ref BuildNo)) Console.Out.WriteLineAsync("** BuildNo : " + BuildNo);
                        if (evt.GetParam("AuthorName", ref AuthorName)) Console.Out.WriteLineAsync("** AuthorName : " + AuthorName);
                        if (evt.GetParam("Year", ref Year)) Console.Out.WriteLineAsync("** Year : " + Year);
                        if (evt.GetParam("True or ", ref TrueOr)) Console.Out.WriteLineAsync("** TrueOr : " + false);
                        if (evt.GetParam("Pi", ref PI)) Console.Out.WriteLineAsync("** PI : " + PI);
                        if (evt.GetParam("e", ref e)) Console.Out.WriteLineAsync("** e : " + e);
                    }
                    return true;

                default:
                    Console.Out.WriteLineAsync("Unhandled Event : " + evtId);
                    break;
            }

            return false;
        }

        protected override void OnStartup(StartupEventArgs e)
        {
            base.OnStartup(e);

            engine = new GarosuEngineWrapper.EngineWrapper();

            engine.RegisterHandler(HandleEngineCallback);
            engine.InitializeEngine();

            // message test
            {
                var testMsg = new BaseMessage(EngineMessageId.TestMessage);
                testMsg.SetParam("bool", true);
                testMsg.SetParam("voidptr", IntPtr.Zero);
                testMsg.SetParam("integer", 1234567891234567);
                testMsg.SetParam("double", 3.1415926535897);
                testMsg.SetParam("string", "Hello, I am c# program. 혹시 한글도 읽을 수 있나요?");

                engine.SendMessage(testMsg);

                bool boolVal = false;
                IntPtr ptrVal = IntPtr.Zero;
                Int64 integerVal = 0;
                double doubleVal = 0.0;
                String strVal = "";

                if (testMsg.GetParam("bool", ref boolVal)) Console.Out.WriteLineAsync("bool " + boolVal);
                if (testMsg.GetParam("voidptr", ref ptrVal)) Console.Out.WriteLineAsync("voidptr " + (UInt64)ptrVal);
                if (testMsg.GetParam("integer", ref integerVal)) Console.Out.WriteLineAsync("integer " + integerVal);
                if (testMsg.GetParam("double", ref doubleVal)) Console.Out.WriteLineAsync("double " + doubleVal);
                if (testMsg.GetParam("string", ref strVal)) Console.Out.WriteLineAsync("string " + strVal);
            }
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
