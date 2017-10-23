using System;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

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
}
