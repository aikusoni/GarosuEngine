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
    public partial class App : Application
    {
        EngineWrapper engine;

        App()
        {
        }

        public delegate bool CallbackSubscriber(int param1);
        public CallbackSubscriber subscribers;

        protected bool CallbackFromEngine(int param1)
        {
            if (subscribers != null) 
                subscribers(param1);

            return true;
        }

        protected override void OnStartup(StartupEventArgs e)
        {
            base.OnStartup(e);

            engine = new GarosuEngineWrapper.EngineWrapper();
            engine.SendMessage(new GarosuEngineWrapper.StringMessage(1, "d:\\"));
            engine.RegisterCallback(CallbackFromEngine);
            engine.InitializeEngine();
        }

        protected override void OnExit(ExitEventArgs e)
        {
            if (engine != null)
                engine.FinalizeEngine();

            base.OnExit(e);
        }
    }
}
