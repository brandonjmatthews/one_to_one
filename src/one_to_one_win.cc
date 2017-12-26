const int BUFFER_SIZE = 1024;

struct SP_DEVICE_INTERFACE_DATA
{
    int32_t cbSize;
    int32_t flags;
    uint8_t* reserved;
}

struct SP_DEVINFO_DATA
{
    int32_t cbSize;
    uint32_t devInst;
    int8_t* reserved;
}

struct SP_DEVICE_INTERFACE_DETAIL_DATA
{
    int32_t cbSize;
    std::string DevicePath;
}

    [StructLayout(LayoutKind.Sequential)] internal struct SP_DEVICE_INTERFACE_DATA
{
  public
    Int32 cbSize;
  public
    Guid interfaceClassGuid;
  public
    Int32 flags;
  private
    UIntPtr reserved;
}

    [StructLayout(LayoutKind.Sequential)] internal struct SP_DEVINFO_DATA
{
  public
    uint cbSize;
  public
    Guid classGuid;
  public
    uint devInst;
  public
    IntPtr reserved;
}

    [Flags()] public enum DisplayDeviceStateFlags : int {
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

                                                        [Flags] public enum DiGetClassFlags : uint {
                                                            DIGCF_DEFAULT = 0x00000001, // only valid with DIGCF_DEVICEINTERFACE
                                                            DIGCF_PRESENT = 0x00000002,
                                                            DIGCF_ALLCLASSES = 0x00000004,
                                                            DIGCF_PROFILE = 0x00000008,
                                                            DIGCF_DEVICEINTERFACE = 0x00000010,
                                                        }

                                                                                              public enum EnumFlags : uint {
                                                                                                  EDD_GET_DEVICE_INTERFACE_NAME = 0x00000001,
                                                                                              }

                                                                                                                          [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] public struct DISPLAY_DEVICE
{
    [MarshalAs(UnmanagedType.U4)] public int cb;
    [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 32)] public string DeviceName;
    [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)] public string DeviceString;
    [MarshalAs(UnmanagedType.U4)] public DisplayDeviceStateFlags StateFlags;
    [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)] public string DeviceID;
    [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)] public string DeviceKey;
}

    [DllImport("gdi32.dll")] static extern int
    GetDeviceCaps(IntPtr hdc, int nIndex);
[DllImport("setupapi.dll")] internal static extern IntPtr SetupDiGetClassDevsEx(IntPtr ClassGuid,
                                                                                [MarshalAs(UnmanagedType.LPStr)] String enumerator,
                                                                                IntPtr hwndParent, Int32 Flags, IntPtr DeviceInfoSet,
                                                                                [MarshalAs(UnmanagedType.LPStr)] String MachineName, IntPtr Reserved);

[DllImport("setupapi.dll", CharSet = CharSet.Auto)] internal static extern IntPtr SetupDiGetClassDevs( // 1st form using a ClassGUID only, with null Enumerator
    ref Guid ClassGuid,
    IntPtr Enumerator,
    IntPtr hwndParent,
    int Flags);

[DllImport("setupapi.dll")] internal static extern Int32 SetupDiDestroyDeviceInfoList(IntPtr DeviceInfoSet);

[DllImport(@"setupapi.dll", CharSet = CharSet.Auto, SetLastError = true)] internal static extern Boolean SetupDiEnumDeviceInterfaces(
    IntPtr hDevInfo,
    IntPtr devInfo,
    ref Guid interfaceClassGuid,
    UInt32 memberIndex,
    ref SP_DEVICE_INTERFACE_DATA deviceInterfaceData);

[DllImport("setupapi.dll", CharSet = CharSet.Auto)] internal static extern int CM_Get_Device_ID(
    UInt32 dnDevInst,
    IntPtr buffer,
    int bufferLen,
    int flags);

[DllImport(@"setupapi.dll", CharSet = CharSet.Auto, SetLastError = true)] internal static extern Boolean SetupDiGetDeviceInterfaceDetail(
    IntPtr hDevInfo,
    ref SP_DEVICE_INTERFACE_DATA deviceInterfaceData,
    ref SP_DEVICE_INTERFACE_DETAIL_DATA deviceInterfaceDetailData,
    UInt32 deviceInterfaceDetailDataSize,
    ref UInt32 requiredSize,
    ref SP_DEVINFO_DATA deviceInfoData);

[DllImport("setupapi.dll")] internal static extern Int32 SetupDiEnumDeviceInfo(IntPtr DeviceInfoSet,
                                                                               Int32 MemberIndex, ref SP_DEVINFO_DATA DeviceInterfaceData);

[DllImport("setupapi.dll")] internal static extern Int32 SetupDiClassNameFromGuid(ref Guid ClassGuid,
                                                                                  StringBuilder className, Int32 ClassNameSize, ref Int32 RequiredSize);

[DllImport("setupapi.dll")] internal static extern Int32 SetupDiGetClassDescription(ref Guid ClassGuid,
                                                                                    StringBuilder classDescription, Int32 ClassDescriptionSize, ref Int32 RequiredSize);

[DllImport("setupapi.dll")] internal static extern Int32 SetupDiGetDeviceInstanceId(IntPtr DeviceInfoSet,
                                                                                    ref SP_DEVINFO_DATA DeviceInfoData,
                                                                                    StringBuilder DeviceInstanceId, Int32 DeviceInstanceIdSize, ref Int32 RequiredSize);

[DllImport("user32.dll")] static extern bool EnumDisplayDevices(string lpDevice, uint iDevNum, ref DISPLAY_DEVICE lpDisplayDevice, uint dwFlags);

const Int64 INVALID_HANDLE_VALUE = -1;

public
enum DeviceCap
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

public
static PointF GetMonitorSizeUsingDeviceCaps()
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
            PNP = PNP.Substring(0, PNP.Length - 2); // remove _0

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

BOOL GetMonitorDevice(TCHAR *adapterName, DISPLAY_DEVICE &ddMon)
{
    DWORD devMon = 0;

    while (EnumDisplayDevices(adapterName, devMon, &ddMon, 0))
    {
        if (ddMon.StateFlags & DISPLAY_DEVICE_ACTIVE &&
            ddMon.StateFlags & DISPLAY_DEVICE_ATTACHED) // for ATI, Windows XP
            break;

        devMon++;
    }

    if (ddMon.DeviceString[0] == '\0')
    {
        EnumDisplayDevices(adapterName, 0, &ddMon, 0);
        if (ddMon.DeviceString[0] == '\0')
            _tcscpy_s(ddMon.DeviceString, _T("Default Monitor"));
    }
    return ddMon.DeviceID[0] != '\0';
}

BOOL GetMonitorSizeFromEDID(TCHAR *adapterName, DWORD &Width, DWORD &Height)
{
    DISPLAY_DEVICE ddMon;
    ZeroMemory(&ddMon, sizeof(ddMon));
    ddMon.cb = sizeof(ddMon);

    //read edid
    bool result = false;
    Width = 0;
    Height = 0;
    if (GetMonitorDevice(adapterName, ddMon))
    {
        TCHAR model[8];
        TCHAR *s = _tcschr(ddMon.DeviceID, '\\') + 1;
        size_t len = _tcschr(s, '\\') - s;
        if (len >= _countof(model))
            len = _countof(model) - 1;
        _tcsncpy_s(model, s, len);

        TCHAR *path = _tcschr(ddMon.DeviceID, '\\') + 1;
        TCHAR str[MAX_PATH] = _T("SYSTEM\\CurrentControlSet\\Enum\\DISPLAY\\");
        _tcsncat_s(str, path, _tcschr(path, '\\') - path);
        path = _tcschr(path, '\\') + 1;
        HKEY hKey;
        if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, str, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
        {
            DWORD i = 0;
            DWORD size = MAX_PATH;
            FILETIME ft;
            while (RegEnumKeyEx(hKey, i, str, &size, NULL, NULL, NULL, &ft) == ERROR_SUCCESS)
            {
                HKEY hKey2;
                if (RegOpenKeyEx(hKey, str, 0, KEY_READ, &hKey2) == ERROR_SUCCESS)
                {
                    size = MAX_PATH;
                    if (RegQueryValueEx(hKey2, _T("Driver"), NULL, NULL, (LPBYTE)&str, &size) == ERROR_SUCCESS)
                    {
                        if (_tcscmp(str, path) == 0)
                        {
                            HKEY hKey3;
                            if (RegOpenKeyEx(hKey2, _T("Device Parameters"), 0, KEY_READ, &hKey3) == ERROR_SUCCESS)
                            {
                                BYTE EDID[256];
                                size = 256;
                                if (RegQueryValueEx(hKey3, _T("EDID"), NULL, NULL, (LPBYTE)&EDID, &size) == ERROR_SUCCESS)
                                {
                                    DWORD p = 8;
                                    TCHAR model2[9];

                                    char byte1 = EDID[p];
                                    char byte2 = EDID[p + 1];
                                    model2[0] = ((byte1 & 0x7C) >> 2) + 64;
                                    model2[1] = ((byte1 & 3) << 3) + ((byte2 & 0xE0) >> 5) + 64;
                                    model2[2] = (byte2 & 0x1F) + 64;
                                    _stprintf(model2 + 3, _T("%X%X%X%X"), (EDID[p + 3] & 0xf0) >> 4, EDID[p + 3] & 0xf, (EDID[p + 2] & 0xf0) >> 4, EDID[p + 2] & 0x0f);
                                    if (_tcscmp(model, model2) == 0)
                                    {
                                        Width = EDID[22];
                                        Height = EDID[21];
                                        result = true;
                                    }
                                    else
                                    {
                                        // EDID incorrect
                                    }
                                }
                                RegCloseKey(hKey3);
                            }
                        }
                    }
                    RegCloseKey(hKey2);
                }
                i++;
            }
            RegCloseKey(hKey);
        }
    }

    return result;
}

static UInt32 Hash(Byte[] dataToHash)
{
    Int32 dataLength = dataToHash.Length;
    if (dataLength == 0)
        return 0;
    UInt32 hash = Convert.ToUInt32(dataLength);
    Int32 remainingBytes = dataLength & 3; // mod 4
    Int32 numberOfLoops = dataLength >> 2; // div 4
    Int32 currentIndex = 0;
    while (numberOfLoops > 0)
    {
        hash += BitConverter.ToUInt16(dataToHash, currentIndex);
        UInt32 tmp = (UInt32)(BitConverter.ToUInt16(dataToHash, currentIndex + 2) << 11) ^ hash;
        hash = (hash << 16) ^ tmp;
        hash += hash >> 11;
        currentIndex += 4;
        numberOfLoops--;
    }

    switch (remainingBytes)
    {
    case 3:
        hash += BitConverter.ToUInt16(dataToHash, currentIndex);
        hash ^= hash << 16;
        hash ^= ((UInt32)dataToHash[currentIndex + 2]) << 18;
        hash += hash >> 11;
        break;
    case 2:
        hash += BitConverter.ToUInt16(dataToHash, currentIndex);
        hash ^= hash << 11;
        hash += hash >> 17;
        break;
    case 1:
        hash += dataToHash[currentIndex];
        hash ^= hash << 10;
        hash += hash >> 1;
        break;
    default:
        break;
    }

    /* Force "avalanching" of final 127 bits */
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 4;
    hash += hash >> 17;
    hash ^= hash << 25;
    hash += hash >> 6;

    return hash;
}