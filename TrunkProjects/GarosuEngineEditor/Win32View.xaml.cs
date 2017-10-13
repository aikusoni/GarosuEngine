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
        private static extern IntPtr CreateWin32Window(IntPtr applicationInstance, IntPtr hwndParent, int screenWidth, int screenHeight);
 
        [DllImport("GarosuHwndHost.dll", EntryPoint = "DestroyWin32Window", CallingConvention = CallingConvention.Cdecl)]
        private static extern void DestroyWin32Window(IntPtr hWnd);

        [DllImport("GarosuHwndHost.dll")]//, EntryPoint = "TestVal", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int TestVal();

        protected override HandleRef BuildWindowCore(HandleRef hwndParent)
        {
            IntPtr instanceHandle = System.Runtime.InteropServices.Marshal.GetHINSTANCE(System.Reflection.Assembly.GetExecutingAssembly().GetModules()[0]);
            IntPtr win32Hwnd = CreateWin32Window(instanceHandle, hwndParent.Handle, 100, 100);
            return new HandleRef(this, win32Hwnd);
        }

        protected override void DestroyWindowCore(HandleRef hwnd)
        {
            DestroyWin32Window(hwnd.Handle);
        }

        protected override IntPtr WndProc(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, ref bool handled)
        {
            switch (msg)
            {
                case 0x05: // WM_SIZE
                    Console.WriteLine("WM_SIZE " + msg);
                    break;

                default:
                    Console.WriteLine(":" + msg);
                    break;
            }
            return base.WndProc(hwnd, msg, wParam, lParam, ref handled);
        }

        public static int GetTestVal()
        {
            return TestVal();
        }
    }

    public partial class Win32View : UserControl
    {
        private Win32ViewHwndHost hwndHost;

        public Win32View()
        {
            InitializeComponent();
            hwndHost = new Win32ViewHwndHost();
            win32Border.Child = hwndHost;
        }

        public override void BeginInit()
        {
            base.BeginInit();
        }
    }
}
