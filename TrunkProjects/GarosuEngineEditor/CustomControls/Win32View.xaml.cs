﻿using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Interop;
using System.Runtime.InteropServices;

namespace GarosuEngineEditor
{
    /// <summary>
    /// Win32View.xaml에 대한 상호 작용 논리
    /// </summary>
   
    internal class SafeNativeMethods
    {
        [DllImport("GarosuHwndHost.dll", EntryPoint = "CreateWin32Window", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr CreateWin32Window(IntPtr applicationInstance, IntPtr hwndParent, int screenWidth, int screenHeight);

        [DllImport("GarosuHwndHost.dll", EntryPoint = "DestroyWin32Window", CallingConvention = CallingConvention.Cdecl)]
        public static extern void DestroyWin32Window(IntPtr hWnd);
    }

    public class Win32ViewHwndHost : HwndHost
    {
        protected override HandleRef BuildWindowCore(HandleRef hwndParent)
        {
            IntPtr instanceHandle = System.Runtime.InteropServices.Marshal.GetHINSTANCE(System.Reflection.Assembly.GetExecutingAssembly().GetModules()[0]);
            IntPtr win32Hwnd = SafeNativeMethods.CreateWin32Window(instanceHandle, hwndParent.Handle, 100, 100);
            return new HandleRef(this, win32Hwnd);
        }

        protected override void DestroyWindowCore(HandleRef hwnd)
        {
            SafeNativeMethods.DestroyWin32Window(hwnd.Handle);
        }

        protected override IntPtr WndProc(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, ref bool handled)
        {
            switch ((WindowMessage)msg)
            {
                case WindowMessage.WM_ACTIVATE:
                    break;

                case WindowMessage.WM_SIZE:
                    break;

                default:
                    break;
            }

            return base.WndProc(hwnd, msg, wParam, lParam, ref handled);
        }
    }

    public partial class Win32View : UserControl, IDisposable
    {
        private Win32ViewHwndHost hwndHost;

        public Win32View()
        {
            InitializeComponent();
            hwndHost = new Win32ViewHwndHost();
            win32Border.Child = hwndHost;
        }

        private bool disposed;
        public void Dispose()
        {
            this.Dispose(true);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (this.disposed) return;
            if (disposing)
            {
                if (hwndHost != null)
                    hwndHost.Dispose();
            }

            this.disposed = true;
        }

        public override void BeginInit()
        {
            base.BeginInit();
        }

        public IntPtr GetHandle()
        {
            return hwndHost.Handle;
        }
    }

    public enum WindowMessage : uint
    {
        WM_NULL = 0x0000,
        WM_CREATE = 0x0001,
        WM_DESTROY = 0x0002,
        WM_MOVE = 0x0003,
        WM_SIZE = 0x0005,
        WM_ACTIVATE = 0x0006,
        WM_SETFOCUS = 0x0007,
        WM_KILLFOCUS = 0x0008,
        WM_ENABLE = 0x000a,
        WM_SETREDRAW = 0x000b,
        WM_SETTEXT = 0x000c,
        WM_GETTEXT = 0x000d,
        WM_GETTEXTLENGTH = 0x000e,
        WM_PAINT = 0x000f,
        WM_CLOSE = 0x0010,
        WM_QUERYENDSESSION = 0x0011,
        WM_QUIT = 0x0012,
        WM_QUERYOPEN = 0x0013,
        WM_ERASEBKGND = 0x0014,
        WM_SYSCOLORCHANGE = 0x0015,
        WM_ENDSESSION = 0x0016,
        WM_SHOWWINDOW = 0x0018,
        WM_CTLCOLOR = 0x0019,
        WM_WININICHANGE = 0x001a,
        WM_DEVMODECHANGE = 0x001b,
        WM_ACTIVATEAPP = 0x001c,
        WM_FONTCHANGE = 0x001d,
        WM_TIMECHANGE = 0x001e,
        WM_CANCELMODE = 0x001f,
        WM_SETCURSOR = 0x0020,
        WM_MOUSEACTIVATE = 0x0021,
        WM_CHILDACTIVATE = 0x0022,
        WM_QUEUESYNC = 0x0023,
        WM_GETMINMAXINFO = 0x0024,
        WM_PAINTICON = 0x0026,
        WM_ICONERASEBKGND = 0x0027,
        WM_NEXTDLGCTL = 0x0028,
        WM_SPOOLERSTATUS = 0x002a,
        WM_DRAWITEM = 0x002b,
        WM_MEASUREITEM = 0x002c,
        WM_DELETEITEM = 0x002d,
        WM_VKEYTOITEM = 0x002e,
        WM_CHARTOITEM = 0x002f,
        WM_SETFONT = 0x0030,
        WM_GETFONT = 0x0031,
        WM_SETHOTKEY = 0x0032,
        WM_GETHOTKEY = 0x0033,
        WM_QUERYDRAGICON = 0x0037,
        WM_COMPAREITEM = 0x0039,
        WM_GETOBJECT = 0x003d,
        WM_COMPACTING = 0x0041,
        WM_COMMNOTIFY = 0x0044,
        WM_WINDOWPOSCHANGING = 0x0046,
        WM_WINDOWPOSCHANGED = 0x0047,
        WM_POWER = 0x0048,
        WM_COPYGLOBALDATA = 0x0049,
        WM_COPYDATA = 0x004a,
        WM_CANCELJOURNAL = 0x004b,
        WM_NOTIFY = 0x004e,
        WM_INPUTLANGCHANGEREQUEST = 0x0050,
        WM_INPUTLANGCHANGE = 0x0051,
        WM_TCARD = 0x0052,
        WM_HELP = 0x0053,
        WM_USERCHANGED = 0x0054,
        WM_NOTIFYFORMAT = 0x0055,
        WM_CONTEXTMENU = 0x007b,
        WM_STYLECHANGING = 0x007c,
        WM_STYLECHANGED = 0x007d,
        WM_DISPLAYCHANGE = 0x007e,
        WM_GETICON = 0x007f,
        WM_SETICON = 0x0080,
        WM_NCCREATE = 0x0081,
        WM_NCDESTROY = 0x0082,
        WM_NCCALCSIZE = 0x0083,
        WM_NCHITTEST = 0x0084,
        WM_NCPAINT = 0x0085,
        WM_NCACTIVATE = 0x0086,
        WM_GETDLGCODE = 0x0087,
        WM_SYNCPAINT = 0x0088,
        WM_NCMOUSEMOVE = 0x00a0,
        WM_NCLBUTTONDOWN = 0x00a1,
        WM_NCLBUTTONUP = 0x00a2,
        WM_NCLBUTTONDBLCLK = 0x00a3,
        WM_NCRBUTTONDOWN = 0x00a4,
        WM_NCRBUTTONUP = 0x00a5,
        WM_NCRBUTTONDBLCLK = 0x00a6,
        WM_NCMBUTTONDOWN = 0x00a7,
        WM_NCMBUTTONUP = 0x00a8,
        WM_NCMBUTTONDBLCLK = 0x00a9,
        WM_NCXBUTTONDOWN = 0x00ab,
        WM_NCXBUTTONUP = 0x00ac,
        WM_NCXBUTTONDBLCLK = 0x00ad,
        SBM_SETPOS = 0x00e0,
        SBM_GETPOS = 0x00e1,
        SBM_SETRANGE = 0x00e2,
        SBM_GETRANGE = 0x00e3,
        SBM_ENABLE_ARROWS = 0x00e4,
        SBM_SETRANGEREDRAW = 0x00e6,
        SBM_SETSCROLLINFO = 0x00e9,
        SBM_GETSCROLLINFO = 0x00ea,
        SBM_GETSCROLLBARINFO = 0x00eb,
        WM_INPUT = 0x00ff,
        WM_KEYDOWN = 0x0100,
        WM_KEYFIRST = 0x0100,
        WM_KEYUP = 0x0101,
        WM_CHAR = 0x0102,
        WM_DEADCHAR = 0x0103,
        WM_SYSKEYDOWN = 0x0104,
        WM_SYSKEYUP = 0x0105,
        WM_SYSCHAR = 0x0106,
        WM_SYSDEADCHAR = 0x0107,
        WM_KEYLAST = 0x0108,
        WM_WNT_CONVERTREQUESTEX = 0x0109,
        WM_CONVERTREQUEST = 0x010a,
        WM_CONVERTRESULT = 0x010b,
        WM_INTERIM = 0x010c,
        WM_IME_STARTCOMPOSITION = 0x010d,
        WM_IME_ENDCOMPOSITION = 0x010e,
        WM_IME_COMPOSITION = 0x010f,
        WM_IME_KEYLAST = 0x010f,
        WM_INITDIALOG = 0x0110,
        WM_COMMAND = 0x0111,
        WM_SYSCOMMAND = 0x0112,
        WM_TIMER = 0x0113,
        WM_HSCROLL = 0x0114,
        WM_VSCROLL = 0x0115,
        WM_INITMENU = 0x0116,
        WM_INITMENUPOPUP = 0x0117,
        WM_SYSTIMER = 0x0118,
        WM_MENUSELECT = 0x011f,
        WM_MENUCHAR = 0x0120,
        WM_ENTERIDLE = 0x0121,
        WM_MENURBUTTONUP = 0x0122,
        WM_MENUDRAG = 0x0123,
        WM_MENUGETOBJECT = 0x0124,
        WM_UNINITMENUPOPUP = 0x0125,
        WM_MENUCOMMAND = 0x0126,
        WM_CHANGEUISTATE = 0x0127,
        WM_UPDATEUISTATE = 0x0128,
        WM_QUERYUISTATE = 0x0129,
        WM_CTLCOLORMSGBOX = 0x0132,
        WM_CTLCOLOREDIT = 0x0133,
        WM_CTLCOLORLISTBOX = 0x0134,
        WM_CTLCOLORBTN = 0x0135,
        WM_CTLCOLORDLG = 0x0136,
        WM_CTLCOLORSCROLLBAR = 0x0137,
        WM_CTLCOLORSTATIC = 0x0138,
        WM_MOUSEFIRST = 0x0200,
        WM_MOUSEMOVE = 0x0200,
        WM_LBUTTONDOWN = 0x0201,
        WM_LBUTTONUP = 0x0202,
        WM_LBUTTONDBLCLK = 0x0203,
        WM_RBUTTONDOWN = 0x0204,
        WM_RBUTTONUP = 0x0205,
        WM_RBUTTONDBLCLK = 0x0206,
        WM_MBUTTONDOWN = 0x0207,
        WM_MBUTTONUP = 0x0208,
        WM_MBUTTONDBLCLK = 0x0209,
        WM_MOUSELAST = 0x0209,
        WM_MOUSEWHEEL = 0x020a,
        WM_XBUTTONDOWN = 0x020b,
        WM_XBUTTONUP = 0x020c,
        WM_XBUTTONDBLCLK = 0x020d,
        WM_PARENTNOTIFY = 0x0210,
        WM_ENTERMENULOOP = 0x0211,
        WM_EXITMENULOOP = 0x0212,
        WM_NEXTMENU = 0x0213,
        WM_SIZING = 0x0214,
        WM_CAPTURECHANGED = 0x0215,
        WM_MOVING = 0x0216,
        WM_POWERBROADCAST = 0x0218,
        WM_DEVICECHANGE = 0x0219,
        WM_MDICREATE = 0x0220,
        WM_MDIDESTROY = 0x0221,
        WM_MDIACTIVATE = 0x0222,
        WM_MDIRESTORE = 0x0223,
        WM_MDINEXT = 0x0224,
        WM_MDIMAXIMIZE = 0x0225,
        WM_MDITILE = 0x0226,
        WM_MDICASCADE = 0x0227,
        WM_MDIICONARRANGE = 0x0228,
        WM_MDIGETACTIVE = 0x0229,
        WM_MDISETMENU = 0x0230,
        WM_ENTERSIZEMOVE = 0x0231,
        WM_EXITSIZEMOVE = 0x0232,
        WM_DROPFILES = 0x0233,
        WM_MDIREFRESHMENU = 0x0234,
        WM_IME_REPORT = 0x0280,
        WM_IME_SETCONTEXT = 0x0281,
        WM_IME_NOTIFY = 0x0282,
        WM_IME_CONTROL = 0x0283,
        WM_IME_COMPOSITIONFULL = 0x0284,
        WM_IME_SELECT = 0x0285,
        WM_IME_CHAR = 0x0286,
        WM_IME_REQUEST = 0x0288,
        WM_IMEKEYDOWN = 0x0290,
        WM_IME_KEYDOWN = 0x0290,
        WM_IMEKEYUP = 0x0291,
        WM_IME_KEYUP = 0x0291,
        WM_NCMOUSEHOVER = 0x02a0,
        WM_MOUSEHOVER = 0x02a1,
        WM_NCMOUSELEAVE = 0x02a2,
        WM_MOUSELEAVE = 0x02a3,
        WM_CUT = 0x0300,
        WM_COPY = 0x0301,
        WM_PASTE = 0x0302,
        WM_CLEAR = 0x0303,
        WM_UNDO = 0x0304,
        WM_RENDERFORMAT = 0x0305,
        WM_RENDERALLFORMATS = 0x0306,
        WM_DESTROYCLIPBOARD = 0x0307,
        WM_DRAWCLIPBOARD = 0x0308,
        WM_PAINTCLIPBOARD = 0x0309,
        WM_VSCROLLCLIPBOARD = 0x030a,
        WM_SIZECLIPBOARD = 0x030b,
        WM_ASKCBFORMATNAME = 0x030c,
        WM_CHANGECBCHAIN = 0x030d,
        WM_HSCROLLCLIPBOARD = 0x030e,
        WM_QUERYNEWPALETTE = 0x030f,
        WM_PALETTEISCHANGING = 0x0310,
        WM_PALETTECHANGED = 0x0311,
        WM_HOTKEY = 0x0312,
        WM_PRINT = 0x0317,
        WM_PRINTCLIENT = 0x0318,
        WM_APPCOMMAND = 0x0319,
        WM_HANDHELDFIRST = 0x0358,
        WM_HANDHELDLAST = 0x035f,
        WM_AFXFIRST = 0x0360,
        WM_AFXLAST = 0x037f,
        WM_PENWINFIRST = 0x0380,
        WM_RCRESULT = 0x0381,
        WM_HOOKRCRESULT = 0x0382,
        WM_GLOBALRCCHANGE = 0x0383,
        WM_PENMISCINFO = 0x0383,
        WM_SKB = 0x0384,
        WM_HEDITCTL = 0x0385,
        WM_PENCTL = 0x0385,
        WM_PENMISC = 0x0386,
        WM_CTLINIT = 0x0387,
        WM_PENEVENT = 0x0388,
        WM_PENWINLAST = 0x038f
    }
}
