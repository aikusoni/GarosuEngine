using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Interop;
using System.Runtime.InteropServices;

namespace GarosuEngineEditor
{
    /// <summary>
    /// Win32View.xaml에 대한 상호 작용 논리
    /// </summary>

    public class Win32ViewHwndHost : HwndHost
    { 
        [DllImport("GarosuHwndHost.dll", EntryPoint = "CreateWin32Window", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr CreateWin32Window(IntPtr applicationInstance, IntPtr hWndParent, int screenWidth, int screenHeight);
 
        [DllImport("GarosuHwndHost.dll", EntryPoint = "DestroyWin32Window", CallingConvention = CallingConvention.Cdecl)]
        private static extern void DestroyWin32Window();

        [DllImport("GarosuHwndHost.dll", EntryPoint = "TestVal", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int TestVal();

        protected override HandleRef BuildWindowCore(HandleRef hwndParent)
        {
            throw new System.NotImplementedException();
        }

        protected override void DestroyWindowCore(HandleRef hwnd)
        {
            throw new System.NotImplementedException();
        }

        protected override IntPtr WndProc(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, ref bool handled)
        {
            return base.WndProc(hwnd, msg, wParam, lParam, ref handled);
        }

        public static int GetTestVal()
        {
            return TestVal();
        }
    }

    public partial class Win32View : UserControl
    {   
        public Win32View()
        {
            InitializeComponent();
            testText.Text = Win32ViewHwndHost.GetTestVal().ToString();
        }

        public override void BeginInit()
        {
            base.BeginInit();
        }
    }
}
