using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Device;
using System.Runtime.InteropServices;
using System.Management;
using Microsoft.Win32;
using one_to_one_chromium_hack;

namespace one_to_one
{
    class Program
    {
        const int BUFFER_SIZE = 1024;


        [StructLayout(LayoutKind.Sequential)]
        internal struct SP_DEVICE_INTERFACE_DATA
        {
            public Int32 cbSize;
            public Guid interfaceClassGuid;
            public Int32 flags;
            private UIntPtr reserved;
        }

        [StructLayout(LayoutKind.Sequential)]
        internal struct SP_DEVINFO_DATA
        {
            public uint cbSize;
            public Guid classGuid;
            public uint devInst;
            public IntPtr reserved;
        }

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Auto)]
        internal struct SP_DEVICE_INTERFACE_DETAIL_DATA
        {
            public int cbSize;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = BUFFER_SIZE)]
            public string DevicePath;
        }

        [Flags()]
        public enum DisplayDeviceStateFlags : int
        {
            /// <summary>The device is part of the desktop.</summary>
            AttachedToDesktop = 0x1,
            MultiDriver = 0x2,
            /// <summary>The device is part of the desktop.</summary>
            PrimaryDevice = 0x4,
            /// <summary>Represents a pseudo device used to mirror application drawing for remoting or other purposes.</summary>
            MirroringDriver = 0x8,
            /// <summary>The device is VGA compatible.</summary>
            VGACompatible = 0x10,
            /// <summary>The device is removable; it cannot be the primary display.</summary>
            Removable = 0x20,
            /// <summary>The device has more display modes than its output devices support.</summary>
            ModesPruned = 0x8000000,
            Remote = 0x4000000,
            Disconnect = 0x2000000
        }

        [Flags]
        public enum DiGetClassFlags : uint
        {
            DIGCF_DEFAULT = 0x00000001,  // only valid with DIGCF_DEVICEINTERFACE
            DIGCF_PRESENT = 0x00000002,
            DIGCF_ALLCLASSES = 0x00000004,
            DIGCF_PROFILE = 0x00000008,
            DIGCF_DEVICEINTERFACE = 0x00000010,
        }

        public enum EnumFlags : uint
        {
            EDD_GET_DEVICE_INTERFACE_NAME = 0x00000001,
        }


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
        public struct DISPLAY_DEVICE
        {
            [MarshalAs(UnmanagedType.U4)]
            public int cb;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 32)]
            public string DeviceName;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)]
            public string DeviceString;
            [MarshalAs(UnmanagedType.U4)]
            public DisplayDeviceStateFlags StateFlags;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)]
            public string DeviceID;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)]
            public string DeviceKey;
        }


        [DllImport("gdi32.dll")]
        static extern int GetDeviceCaps(IntPtr hdc, int nIndex);
        [DllImport("setupapi.dll")]
        internal static extern IntPtr SetupDiGetClassDevsEx(IntPtr ClassGuid,
        [MarshalAs(UnmanagedType.LPStr)]String enumerator,
        IntPtr hwndParent, Int32 Flags, IntPtr DeviceInfoSet,
        [MarshalAs(UnmanagedType.LPStr)]String MachineName, IntPtr Reserved);

        [DllImport("setupapi.dll", CharSet = CharSet.Auto)]
        internal static extern IntPtr SetupDiGetClassDevs(           // 1st form using a ClassGUID only, with null Enumerator
           ref Guid ClassGuid,
           IntPtr Enumerator,
           IntPtr hwndParent,
           int Flags
        );

        [DllImport("setupapi.dll")]
        internal static extern Int32 SetupDiDestroyDeviceInfoList(IntPtr DeviceInfoSet);

        [DllImport(@"setupapi.dll", CharSet = CharSet.Auto, SetLastError = true)]
        internal static extern Boolean SetupDiEnumDeviceInterfaces(
           IntPtr hDevInfo,
           IntPtr devInfo,
           ref Guid interfaceClassGuid,
           UInt32 memberIndex,
           ref SP_DEVICE_INTERFACE_DATA deviceInterfaceData
        );

        [DllImport("setupapi.dll", CharSet = CharSet.Auto)]
        internal static extern int CM_Get_Device_ID(
           UInt32 dnDevInst,
           IntPtr buffer,
           int bufferLen,
           int flags
        );

        [DllImport(@"setupapi.dll", CharSet = CharSet.Auto, SetLastError = true)]
        internal static extern Boolean SetupDiGetDeviceInterfaceDetail(
           IntPtr hDevInfo,
           ref SP_DEVICE_INTERFACE_DATA deviceInterfaceData,
           ref SP_DEVICE_INTERFACE_DETAIL_DATA deviceInterfaceDetailData,
           UInt32 deviceInterfaceDetailDataSize,
           ref UInt32 requiredSize,
           ref SP_DEVINFO_DATA deviceInfoData
        );


        [DllImport("setupapi.dll")]
        internal static extern Int32 SetupDiEnumDeviceInfo(IntPtr DeviceInfoSet,
            Int32 MemberIndex, ref SP_DEVINFO_DATA DeviceInterfaceData);

        [DllImport("setupapi.dll")]
        internal static extern Int32 SetupDiClassNameFromGuid(ref Guid ClassGuid,
            StringBuilder className, Int32 ClassNameSize, ref Int32 RequiredSize);

        [DllImport("setupapi.dll")]
        internal static extern Int32 SetupDiGetClassDescription(ref Guid ClassGuid,
            StringBuilder classDescription, Int32 ClassDescriptionSize, ref Int32 RequiredSize);

        [DllImport("setupapi.dll")]
        internal static extern Int32 SetupDiGetDeviceInstanceId(IntPtr DeviceInfoSet,
            ref SP_DEVINFO_DATA DeviceInfoData,
            StringBuilder DeviceInstanceId, Int32 DeviceInstanceIdSize, ref Int32 RequiredSize);

        [DllImport("user32.dll")]
        static extern bool EnumDisplayDevices(string lpDevice, uint iDevNum, ref DISPLAY_DEVICE lpDisplayDevice, uint dwFlags);

        const Int64 INVALID_HANDLE_VALUE = -1;

        public enum DeviceCap
        {
            HORZSIZE = 4,
            VERTSIZE = 6,
            HORZRES = 8,
            VERTRES = 10
        }

        static void Main(string[] args)
        {
            if (args.Length != 1)
            {
                Console.WriteLine("one_to_one on win32 requires a screenID as it's first argument");
                Environment.Exit(-1);
            }

            string targetDeviceID = args[0];
            //string targetDeviceID = "2528732444";
            PointF res = GetScreenResolution(targetDeviceID);
            PointF size = GetPhysicalMonitorSize(targetDeviceID);
            PointF dpmm = new PointF(res.X / size.X, res.Y / size.Y);

            Console.WriteLine("{0},{1},{2},{3},{4},{5}", res.X, res.Y, size.X, size.Y, dpmm.X, dpmm.Y);
            //Console.ReadKey();
            Environment.Exit(0);
        }

        static PointF GetScreenResolution(string targetScreenID)
        {
            PointF resPt = new PointF(-1.0f, -1.0f);

            foreach (var screen in Screen.AllScreens)
            {
                string deviceHashID = SuperFastHashSimple.Hash(Encoding.ASCII.GetBytes(screen.DeviceName)).ToString();
                if (deviceHashID == targetScreenID)
                {
                    Rectangle resRect = screen.Bounds;
                    resPt.X = resRect.Width;
                    resPt.Y = resRect.Height;
                }
            }

            return resPt;
        }

        static PointF GetPhysicalMonitorSize(string targetDeviceID)
        {
            List<DISPLAY_DEVICE> displays = GetDisplayList();
            PointF size = new PointF(-1.0f, -1.0f);
            for (int i = 0; i < displays.Count; i++)
            {
                DISPLAY_DEVICE d = displays[i];

                string deviceHashID = SuperFastHashSimple.Hash(Encoding.ASCII.GetBytes(d.DeviceName)).ToString();

                // If the DeviceID is empty and matches our target ID, the following object is a connected monitor
                // if the following objects DeviceID starts with "MONTIOR" or "DISPLAY"

                if (d.DeviceID == "" && deviceHashID == targetDeviceID)
                {
                    DISPLAY_DEVICE m = displays[i + 1];
                    if (m.DeviceID.ToUpper().IndexOf("DISPLAY") != -1 || m.DeviceID.ToUpper().IndexOf("MONITOR") != -1)
                    {
                        string InstanceID = GetInstanceIDFromDevicePath(m.DeviceID);
                        size = GetMonitorSizeUsingEDID(InstanceID);
                        return size;
                    }
                }
                else if (deviceHashID == targetDeviceID)
                {
                    string InstanceID = GetInstanceIDFromDevicePath(d.DeviceID);
                    size = GetMonitorSizeUsingEDID(InstanceID);
                    return size;
                }
            }

            return size;
        }


        static List<DISPLAY_DEVICE> GetDisplayList()
        {
            List<DISPLAY_DEVICE> displays = new List<DISPLAY_DEVICE>();

            DISPLAY_DEVICE d = new DISPLAY_DEVICE();
            d.cb = Marshal.SizeOf(d);
            try
            {
                for (uint id = 0; EnumDisplayDevices(null, id, ref d, (uint)EnumFlags.EDD_GET_DEVICE_INTERFACE_NAME); id++)
                {
     
                    DISPLAY_DEVICE monitor = new DISPLAY_DEVICE();
                    monitor.cb = Marshal.SizeOf(monitor);
                    EnumDisplayDevices(d.DeviceName, 0, ref monitor, (uint)EnumFlags.EDD_GET_DEVICE_INTERFACE_NAME);
                    displays.Add(d);
                    displays.Add(monitor);
                    d.cb = Marshal.SizeOf(d);
                    monitor.cb = Marshal.SizeOf(monitor);

                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(String.Format("{0}", ex.ToString()));
            }

            return displays;

        }

        public static PointF GetMonitorSizeUsingDeviceCaps()
        {
            // Assumes 1 monitor
            long count = Screen.AllScreens.Length;

            var hDC = Graphics.FromHwnd(IntPtr.Zero).GetHdc();
            // Physical size
            int horizontalSizeInMilliMeters = GetDeviceCaps(hDC, (int)DeviceCap.HORZSIZE);
            int verticalSizeInMilliMeters = GetDeviceCaps(hDC, (int)DeviceCap.VERTSIZE);

            PointF pt = new PointF();
            pt.X = horizontalSizeInMilliMeters;
            pt.Y = verticalSizeInMilliMeters;

            return pt;
        }


        static PointF GetMonitorSizeUsingEDID(string targetInstanceName)
        {

            PointF pt = new PointF();
            try
            {
                ManagementObjectSearcher searcher = new ManagementObjectSearcher("root\\WMI", "SELECT * FROM WmiMonitorID");

                foreach (ManagementObject queryObj in searcher.Get())
                {
                    dynamic code = queryObj["ProductCodeID"];
                    string pcid = "";
                    for (int i = 0; i < code.Length; i++)
                    {
                        pcid = pcid + Char.ConvertFromUtf32(code[i]);
                    }

                    float xSize = 0;
                    float ySize = 0;

                    string PNP = queryObj["InstanceName"].ToString();
                    PNP = PNP.Substring(0, PNP.Length - 2);  // remove _0

                    if (PNP != null && PNP.Length > 0 && PNP.ToUpper() == targetInstanceName.ToUpper())
                    {
                        string displayKey = "SYSTEM\\CurrentControlSet\\Enum\\";
                        string strSubDevice = displayKey + PNP + "\\" + "Device Parameters\\";
    
                        Microsoft.Win32.RegistryKey regKey = Registry.LocalMachine.OpenSubKey(strSubDevice, false);
                        if (regKey != null)
                        {
                            if (regKey.GetValueKind("edid") == RegistryValueKind.Binary)
                            {
                                byte[] edid = (byte[])regKey.GetValue("edid");
                                const int edid_x_size_in_mm = 21;
                                const int edid_y_size_in_mm = 22;
                                xSize = ((float)edid[edid_x_size_in_mm] * 10f);
                                ySize = ((float)edid[edid_y_size_in_mm] * 10f);

                                pt.X = xSize;
                                pt.Y = ySize;
                                return pt;
                            }
                            regKey.Close();
                        }
                    }
  
                }
            }
            catch (ManagementException e)
            {
                Console.WriteLine("An error occurred while querying for WMI data: " + e.Message);
                return new PointF(-1.0f, -1.0f);
            }

            return new PointF(-1.0f, -1.0f);
        }

     

        static string GetInstanceIDFromDevicePath(string targetDevicePath)
        {
            Guid MonitorGUID = new Guid("E6F07B5F-EE97-4a90-B076-33F57BF4EAA7");

            // We start at the "root" of the device tree and look for all
            // devices that match the interface GUID of a monitor
            IntPtr h = SetupDiGetClassDevs(ref MonitorGUID, IntPtr.Zero, IntPtr.Zero, (int)(DiGetClassFlags.DIGCF_PRESENT | DiGetClassFlags.DIGCF_DEVICEINTERFACE));
            if (h.ToInt64() != INVALID_HANDLE_VALUE)
            {
                bool Success = true;
                uint i = 0;
                while (Success)
                {
                    // create a Device Interface Data structure
                    SP_DEVICE_INTERFACE_DATA dia = new SP_DEVICE_INTERFACE_DATA();
                    dia.cbSize = (int)Marshal.SizeOf(dia);

                    // start the enumeration 
                    Success = SetupDiEnumDeviceInterfaces(h, IntPtr.Zero, ref MonitorGUID, i, ref dia);
                    if (Success)
                    {
                        // build a DevInfo Data structure
                        SP_DEVINFO_DATA da = new SP_DEVINFO_DATA();
                        da.cbSize = (uint)Marshal.SizeOf(da);

                        // build a Device Interface Detail Data structure
                        SP_DEVICE_INTERFACE_DETAIL_DATA didd = new SP_DEVICE_INTERFACE_DETAIL_DATA();
                        didd.cbSize = (int)(4 + Marshal.SystemDefaultCharSize); // trust me :)

                        // now we can get some more detailed information
                        uint nRequiredSize = 0;
                        uint nBytes = BUFFER_SIZE;
                        if (SetupDiGetDeviceInterfaceDetail(h, ref dia, ref didd, nBytes, ref nRequiredSize, ref da))
                        {
                            // Now we get the InstanceID
                            IntPtr ptrInstanceBuf = Marshal.AllocHGlobal((int)nBytes);
                            CM_Get_Device_ID(da.devInst, ptrInstanceBuf, (int)nBytes, 0);
                            string InstanceID = Marshal.PtrToStringAuto(ptrInstanceBuf);
                            //Console.WriteLine("InstanceID: {0}", InstanceID);
                            Marshal.FreeHGlobal(ptrInstanceBuf);

                            //Console.WriteLine("DevicePath: {0}", didd.DevicePath);

                            
                            if (didd.DevicePath.ToUpper() == targetDevicePath.ToUpper())
                            {
                                return InstanceID;
                            }
                        }
                        i++;
                    }
                }
            }
            SetupDiDestroyDeviceInfoList(h);
            return null;
        }
    }
}