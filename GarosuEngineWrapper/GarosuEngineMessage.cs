using System;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace GarosuEngineWrapper
{
    public class DummyMessage
    {
        protected IntPtr msgPtr;
        public IntPtr MessagePtr
        {
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
}
