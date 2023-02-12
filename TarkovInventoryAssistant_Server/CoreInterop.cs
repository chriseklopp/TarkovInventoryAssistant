using System.Runtime.InteropServices;



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
            IntPtr myCore = CreateCoreInterface();

           

            int y = getUsefulNumber(myCore);

            DetectionResultMarshal result = detectImageContent(myCore, 69);
            int x = getUsefulNumber(myCore);
        }


        [DllImport("TIA_Core", SetLastError = true)]
        static extern int getUsefulNumber(IntPtr core);

        // UnmanagedType is the cool thingy 
        [DllImport("TIA_Core", SetLastError = true)]
        static extern IntPtr CreateCoreInterface();

        [DllImport("TIA_Core", SetLastError = true)]
        static extern DetectionResultMarshal detectImageContent(IntPtr core, int image);
    };

}