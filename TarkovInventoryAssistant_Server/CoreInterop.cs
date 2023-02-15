using System.Runtime.InteropServices;
using System.Text;

namespace Interop
{

    // Current Size = 102 (consider reducing this, somehow?)
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct DetectionResultMarshal {

        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 60)]
        public string name;

        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 4)]
        public string fleaUnit;

        [MarshalAs(UnmanagedType.I4)]
        public int fleaprice;

        [MarshalAs(UnmanagedType.I4)]
        public int fleaPricePerSlot;

        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 4)]
        public string traderUnit;

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



        public DetectionResultMarshal detectImageContent(int image)
        {
            return detectImageContent_INTEROP(m_core, image);
        }

        public void setDATA_DIR(string path)
        {
            setDATA_DIR_INTEROP(m_core, path);
        }

        public void setACTIVECATALOG(string path)
        {
            setACTIVECATALOG_INTEROP(m_core, path);
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



        // UnmanagedType is the cool thingy 
        [DllImport("TIA_CORE_SHARED", SetLastError = true)]
        private static extern IntPtr CreateCoreInterface_INTEROP();

        [DllImport("TIA_CORE_SHARED", SetLastError = true)]
        private static extern DetectionResultMarshal detectImageContent_INTEROP(IntPtr core, int image);

        [DllImport("TIA_CORE_SHARED", SetLastError = true)]
        private static extern void setDATA_DIR_INTEROP(IntPtr core, string path);

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


        private readonly IntPtr m_core;

    };

}