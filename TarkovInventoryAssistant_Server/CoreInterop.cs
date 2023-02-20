using Microsoft.AspNetCore.Razor.TagHelpers;
using SkiaSharp;
using System.Drawing;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;
using System.Runtime.Versioning;
using System.Text;
using System.Xml.Linq;
using static System.Net.Mime.MediaTypeNames;
using Image = System.Drawing.Image;

namespace Interop
{

    // Current Size = 102 (consider reducing this, somehow?)
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack =0)]
    public struct DetectionResultMarshal {

        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 60)]
        public string name;

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
        public byte[] fleaUnit;

        [MarshalAs(UnmanagedType.I4)]
        public int fleaprice;

        [MarshalAs(UnmanagedType.I4)]
        public int fleaPricePerSlot;

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
        public byte[] traderUnit;

        [MarshalAs(UnmanagedType.I4)]
        public int traderPrice;

        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 12)]
        public string trader;

        [MarshalAs(UnmanagedType.U1)]
        public byte dimHeight;

        [MarshalAs(UnmanagedType.U1)]
        public byte dimWidth;

        [MarshalAs(UnmanagedType.U2)]
        public ushort x1;

        [MarshalAs(UnmanagedType.U2)]
        public ushort y1;

        [MarshalAs(UnmanagedType.U2)]
        public ushort x2;

        [MarshalAs(UnmanagedType.U2)]
        public ushort y2;
    }



    public class CoreInterop
    {
        public CoreInterop()
        {
            m_core = CreateCoreInterface_INTEROP();
        }

        ~CoreInterop()
        {
            DeleteCoreInterface_INTEROP(m_core);
        }

        public DetectionResultMarshal[] detectImageContent(SKBitmap image)
        {

            if (m_isUpdating)
            {
                System.Threading.SpinWait.SpinUntil(() => !m_isUpdating); // Wait until we are no longer updating to allow access.
            }

            lock (m_refLock)
            {
                if (m_isUpdating) // By checking this again we prevent a race condition.
                {
                    System.Threading.SpinWait.SpinUntil(() => !m_isUpdating);
                }
                m_refCount++;
            }
            
            IntPtr res = new IntPtr();
            int size = detectImageContent_INTEROP(m_core, image.Bytes, image.Width, image.Height, image.BytesPerPixel, out res);

            DetectionResultMarshal[] results = new DetectionResultMarshal[size];
            IntPtr inc = res;
            for (int i = 0; i < size; i++)
            {
                results[i] = (DetectionResultMarshal)Marshal.PtrToStructure(inc, typeof(DetectionResultMarshal));
                inc += Marshal.SizeOf(typeof(DetectionResultMarshal)); // Pointer artihmetic in C#!
            }

            disposeDetectionResults_INTEROP(res);
            lock (m_refLock)
            {
                m_refCount--; // TODO: Might have to catch all exceptions in this method so this is guaranteed to decrement.
            }
            return results;
        }

        public void setDATA_DIR(string path)
        {
            setDATA_DIR_INTEROP(m_core, path);
        }

        public bool compileRawCatalog(string path, string name, bool makeRotations)
        {
            return compileRawCatalog_INTEROP(m_core, path, name, makeRotations);
        }

        public void setACTIVECATALOG(string path)
        {
            // By locking the refLock here we prevent a deadlock.
            lock (m_refLock)
            {
                m_isUpdating = true; // Blocks further threads from accessing our catalog.
            }
            System.Threading.SpinWait.SpinUntil(() => m_refCount == 0); // Wait until no threads are reading from our catalog.

            bool success = setACTIVECATALOG_INTEROP(m_core, path);
            m_isUpdating = false; // Unblock access to the catalog.

            if (success)
            {
                saveConfig();
            }

        }

        public void setRAW_CATALOGS_DIR(string path)
        {
            setRAW_CATALOGS_DIR_INTEROP(m_core, path);
        }

        public void setCATALOGS_DIR(string path)
        {
            setCATALOGS_DIR_INTEROP(m_core, path);
        }

        public void saveConfig()
        {
            saveConfig_INTEROP(m_core);
        }


        public string getDATA_DIR()
        {
            StringBuilder sb = new StringBuilder(260);
            getDATA_DIR_INTEROP(m_core, sb, sb.Capacity);
            return sb.ToString();
        }

        public string getACTIVE_CATALOG()
        {
            StringBuilder sb= new StringBuilder(260);
            getACTIVE_CATALOG_INTEROP(m_core, sb, sb.Capacity);
            return sb.ToString();
        }

        public string getRAW_CATALOGS_DIR()
        {
            StringBuilder sb = new StringBuilder(260);
            getRAW_CATALOGS_DIR_INTEROP(m_core, sb, sb.Capacity);
            return sb.ToString();
        }

        public string getCATALOGS_DIR()
        {
            StringBuilder sb = new StringBuilder(260);
            getCATALOGS_DIR_INTEROP(m_core, sb, sb.Capacity);
            return sb.ToString();
        }

        public string getROOT_DIR()
        {
            StringBuilder sb = new StringBuilder(260);
            getROOT_DIR_INTEROP(m_core, sb, sb.Capacity);
            return sb.ToString();
        }



        [DllImport("TIA_CORE_SHARED", SetLastError = true)]
        private static extern IntPtr CreateCoreInterface_INTEROP();

 
        [DllImport("TIA_CORE_SHARED", SetLastError = true)]
        private static extern void DeleteCoreInterface_INTEROP(IntPtr core);


        [DllImport("TIA_CORE_SHARED", SetLastError = true)]
        private static extern int detectImageContent_INTEROP(IntPtr core, byte[] image, int width, int height, int channels,  out IntPtr resultPtr);

        [DllImport("TIA_CORE_SHARED", SetLastError = true)]
        private static extern void setDATA_DIR_INTEROP(IntPtr core, string path);

        [DllImport("TIA_CORE_SHARED", SetLastError = true)]
        private static extern bool compileRawCatalog_INTEROP(IntPtr core, string path, string name, bool makeRotations);

        [DllImport("TIA_CORE_SHARED", SetLastError = true)]
        private static extern bool setACTIVECATALOG_INTEROP(IntPtr core, string path);

        [DllImport("TIA_CORE_SHARED", SetLastError = true)]
        private static extern void setRAW_CATALOGS_DIR_INTEROP(IntPtr core, string path);

        [DllImport("TIA_CORE_SHARED", SetLastError = true)]
        private static extern void setCATALOGS_DIR_INTEROP(IntPtr core, string path);

        [DllImport("TIA_CORE_SHARED", SetLastError = true)]
        private static extern void saveConfig_INTEROP(IntPtr core);


        [DllImport("TIA_CORE_SHARED", SetLastError = true)]
        private static extern void getDATA_DIR_INTEROP(IntPtr core, StringBuilder str, int size);

        [DllImport("TIA_CORE_SHARED", SetLastError = true)]
        private static extern void getACTIVE_CATALOG_INTEROP(IntPtr core, StringBuilder str, int size);

        [DllImport("TIA_CORE_SHARED", SetLastError = true)]
        private static extern void getRAW_CATALOGS_DIR_INTEROP(IntPtr core, StringBuilder str, int size);

        [DllImport("TIA_CORE_SHARED", SetLastError = true)]
        private static extern void getCATALOGS_DIR_INTEROP(IntPtr core, StringBuilder str, int size);

        [DllImport("TIA_CORE_SHARED", SetLastError = true)]
        private static extern void getROOT_DIR_INTEROP(IntPtr core, StringBuilder str, int size);

        [DllImport("TIA_CORE_SHARED", SetLastError = true)]
        private static extern void disposeDetectionResults_INTEROP(IntPtr resultPtr);

        private readonly object m_refLock = new object();
        private int m_refCount = 0;
        private bool m_isUpdating = false;
        private readonly IntPtr m_core;

    };

}