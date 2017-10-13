using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;

namespace GarosuEngineEditor
{
    /// <summary>
    /// App.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class App : Application
    {
        GarosuEngineWrapper.EngineWrapper engine;

        App()
        {
            engine = new GarosuEngineWrapper.EngineWrapper();
            engine.SendMessage(new GarosuEngineWrapper.StringMessage(1, "d:\\"));
            engine.InitializeEngine();
        }
    }
}
