using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GarosuEngineWrapper
{
    public abstract class Parameter
    {
        protected abstract IntPtr GetParamPtr();

        public bool SetParam(String paramName, bool value)
        {
            return SafeNativeMethods.SetParam_Bool(GetParamPtr(), new StringBuilder(paramName), value);
        }

        public bool SetParam(String paramName, IntPtr handle)
        {
            return SafeNativeMethods.SetParam_VoidPtr(GetParamPtr(), new StringBuilder(paramName), handle);
        }

        public bool SetParam(String paramName, Int64 value)
        {
            return SafeNativeMethods.SetParam_LongLongInt(GetParamPtr(), new StringBuilder(paramName), value);
        }

        public bool SetParam(String paramName, double value)
        {
            return SafeNativeMethods.SetParam_Double(GetParamPtr(), new StringBuilder(paramName), value);
        }

        public bool SetParam(String paramName, String str)
        {
            return SafeNativeMethods.SetParam_String(GetParamPtr(), new StringBuilder(paramName), new StringBuilder(str));
        }
    }
}
