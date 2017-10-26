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

        public bool GetParam(String paramName, ref bool value)
        {
            bool ret = false;
            unsafe
            {
                bool tmp = false;
                ret = SafeNativeMethods.GetParam_Bool(GetParamPtr(), new StringBuilder(paramName), &tmp);
                if (ret) value = tmp;
            }
            return ret;
        }

        public bool GetParam(String paramName, ref IntPtr handle)
        {
            bool ret = false;
            unsafe
            {
                IntPtr tmp = IntPtr.Zero;
                ret = SafeNativeMethods.GetParam_VoidPtr(GetParamPtr(), new StringBuilder(paramName), &tmp);
                if (ret) handle = tmp;
            }
            return ret;
        }

        public bool GetParam(String paramName, ref Int64 value)
        {
            bool ret = false;
            unsafe
            {
                Int64 tmp = 0;
                ret = SafeNativeMethods.GetParam_LongLongInt(GetParamPtr(), new StringBuilder(paramName), &tmp);
                if (ret) value = tmp;
            }
            return ret;
        }

        public bool GetParam(String paramName, ref double value)
        {
            bool ret = false;
            unsafe
            {
                double tmp = 0;
                ret = SafeNativeMethods.GetParam_Double(GetParamPtr(), new StringBuilder(paramName), &tmp);
                if (ret) value = tmp;
            }
            return ret;
        }

        public bool GetParam(String paramName, ref String value)
        {
            bool ret = false;
            StringBuilder tmp = new StringBuilder();
            ret = SafeNativeMethods.GetParam_String(GetParamPtr(), new StringBuilder(paramName), tmp);
            if (ret) value = tmp.ToString();
            return ret;
        }
    }
}
