
#include <wchar.h>
#include <windows.h>
#include <wbemidl.h>

#include "wmi.h"
#include "fingerprint.h"

struct wmitarget wmitargets[] = {
	{
		L"Operating System", L"Win32_OperatingSystem", {
			L"Caption", L"Description", L"InstallDate", L"Name", L"CountryCode",
			L"FreePhysicalMemory", L"FreeVirtualMemory", L"MaxProcessMemorySize",
			L"Version", L"LastBootUpTime", L"TotalSwapSpaceSize",
			L"TotalVirtualMemorySize", L"TotalVisibleMemorySize", L"BootDevice",
			L"BuildNumber", L"BuildType", L"CodeSet", L"CountryCode",
			L"DataExecutionPrevention_Available", L"DataExecutionPrevention_32BitApplications",
			L"DataExecutionPrevention_Drivers", L"DataExecutionPrevention_SupportPolicy",
			L"Debug", L"Locale", L"Manufacturer", L"Organization", L"OSLanguage",
			L"OSProductSuite", L"PlusProductID", L"PlusVersionNumber",
			L"RegisteredUser", L"SerialNumber", L"ServicePackMajorVersion",
			L"ServicePackMinorVersion", L"SystemDevice", L"SystemDirectory", L"SystemDrive",
			L"WindowsDirectory", L"PAEEnabled",
			NULL
		}
	},
	{
		L"Computer System", L"Win32_ComputerSystem", {
			L"Caption", L"Description", L"Name", L"PrimaryOwnerContact",
			L"PrimaryOwnerName", L"DNSHostName", L"Domain", L"Manufacturer", L"Model",
			L"SystemType", L"UserName", L"OEMStringArray", L"TotalPhysicalMemory",
			L"Workgroup",
			NULL
		}
	},
	{
		L"Page File", L"Win32_PageFileUsage", {
			L"Caption", L"Description", L"Name", L"AllocatedBaseSize", L"CurrentUsage",
			L"PeakUsage", L"TempPageFile",
			NULL
		}
	},
	{
		L"Processor", L"Win32_Processor", {
			L"Caption", L"Description", L"Name", L"DeviceID", L"PNPDeviceID", L"AddressWidth",
			L"DataWidth", L"Family", L"OtherFamilyDescription", L"MaxClockSpeed", L"Stepping",
			L"UniqueId", L"Version", L"Manufacturer", L"L2CacheSize", L"L2CacheSpeed",
			L"L3CacheSize", L"L3CacheSpeed", L"Architecture", L"Level", L"Revision",
			L"ProcessorId", L"ProcessorType", L"NumberOfLogicalProcessors", L"NumberOfCores",
			L"SocketDesignation",
			NULL
		}
	},
	{
		L"BIOS", L"Win32_BIOS", {
			L"Caption", L"Description", L"Name", L"Version", L"SoftwareElementState",
			L"SoftwareElementID", L"Manufacturer", L"BuildNumber", L"SerialNumber", L"CodeSet",
			L"IdentificationCode", L"LanguageEdition", L"ReleaseDate", L"SMBIOSPresent",
			L"SMBIOSMajorVersion", L"SMBIOSMinorVersion", L"SMBIOSBIOSVersion",
			NULL
		}
	},
	{
		L"Device", L"Win32_PnPEntity", {
			L"Caption", L"Description", L"Name", L"DeviceID", L"PNPDeviceID", L"HardwareID",
			L"CompatibleID", L"Manufacturer", L"Service", L"ClassGuid",
			NULL
		}
	},
	{
		L"CD-ROM", L"Win32_CDROMDrive", {
			L"Caption", L"Description", L"Name", L"DeviceID", L"PNPDeviceID", L"MaxMediaSize",
			L"DefaultBlockSize", L"MaxBlockSize", L"MinBlockSize", L"NumberOfMediaSupported",
			L"Manufacturer", L"MediaType", L"RevisionLevel", L"VolumeName", L"VolumeSerialNumber",
			L"MediaLoaded", L"Size", L"SerialNumber", L"Drive",
			NULL
		}
	},
	{
		L"Sound Device", L"Win32_SoundDevice", {
			L"Caption", L"Description", L"Name", L"DeviceID", L"PNPDeviceID", L"Manufacturer",
			L"ProductName",
			NULL
		}
	},
	{
		L"Video Controller", L"Win32_VideoController", {
			L"Caption", L"Description", L"Name", L"DeviceID", L"PNPDeviceID", L"TimeOfLastReset",
			L"VideoProcessor", L"VideoMemoryType", L"NumberOfVideoPages", L"MaxMemorySupported",
			L"CurrentBitsPerPixel", L"CurrentHorizontalResolution", L"CurrentVerticalResolution",
			L"MaxRefreshRate", L"MinRefreshRate", L"CurrentRefreshRate", L"CurrentNumberOfColors",
			L"VideoMode", L"AdapterCompatibility", L"AdapterDACType", L"AdapterRAM", L"InfSection",
			L"InstalledDisplayDrivers", L"DriverDate", L"DriverVersion", L"SpecificationVersion",
			L"VideoModeDescription",
			NULL
		}
	},
	{
		L"Keyboard", L"Win32_Keyboard", {
			L"Caption", L"Description", L"Name", L"DeviceID", L"PNPDeviceID", L"Layout",
			L"NumberOfFunctionKeys",
			NULL
		}
	},
	{
		L"Pointing Device", L"Win32_PointingDevice", {
			L"Caption", L"Description", L"Name", L"DeviceID", L"PNPDeviceID", L"Handedness",
			L"NumberOfButtons", L"PointingType", L"Resolution", L"HardwareType", L"InfSection",
			L"SampleRate", L"Manufacturer",
			NULL
		}
	},
	{
		L"Network Adapter", L"Win32_NetworkAdapter", {
			L"Caption", L"Description", L"Name", L"DeviceID", L"PNPDeviceID", L"PermanentAddress",
			L"NetworkAddresses", L"Speed", L"MaxSpeed", L"ProductName", L"AdapterType",
			L"MACAddress", L"ServiceName", L"Manufacturer", L"NetConnectionID", L"GUID",
			NULL
		}
	},
	{
		L"Network Adapter Configuration", L"Win32_NetworkAdapterConfiguration", {
			L"SettingID", L"Caption", L"Description",  L"DefaultIPGateway", L"DHCPLeaseExpires",
			L"DHCPLeaseObtained", L"DHCPServer", L"IPAddress", L"IPSubnet", L"MACAddress", L"ServiceName",
			L"DatabasePath", L"DNSHostName", L"DNSDomain", L"DNSServerSearchOrder",
			L"DNSDomainSuffixSearchOrder", L"WINSPrimaryServer", L"WINSSecondaryServer", L"WINSHostLookupFile",
			L"WINSScopeID",
			NULL
		}
	},
	{
		L"Serial Ports", L"Win32_SerialPort", {
			L"Caption", L"Description", L"Name", L"DeviceID", L"PNPDeviceID", L"CapabilityDescriptions",
			L"ProviderType",
			NULL
		}
	},
	{
		L"Parallel Ports", L"Win32_ParallelPort", {
			L"Caption", L"Description", L"Name", L"DeviceID", L"PNPDeviceID", L"CapabilityDescriptions",
			NULL
		}
	},
	{
		L"Logical Disks", L"Win32_LogicalDisk", {
			L"Caption", L"Description", L"Name", L"DeviceID", L"PNPDeviceID", L"ErrorMethodology",
			L"Purpose", L"Compressed", L"DriveType", L"FileSystem", L"ProviderName", L"VolumeName",
			L"VolumeSerialNumber", L"MediaType",
			NULL
		}
	},
	{
		L"Disk Partition", L"Win32_DiskPartition", {
			L"Caption", L"Description", L"Name", L"DeviceID", L"PNPDeviceID", L"ErrorMethodology",
			L"Purpose", L"BootPartition", L"Type",
			NULL
		}
	},
	{
		L"SCSI Controller", L"Win32_SCSIController", {
			L"Caption", L"Description", L"Name", L"DeviceID", L"PNPDeviceID", L"DriverName", L"DeviceMap",
			L"HardwareVersion", L"Manufacturer",
			NULL
		}
	},
	{
		L"IDE Controller", L"Win32_IDEController", {
			L"Caption", L"Description", L"Name", L"DeviceID", L"PNPDeviceID", L"Manufacturer",
			NULL
		}
	},
	{
		L"USB Controller", L"Win32_USBController", {
			L"Caption", L"Description", L"Name", L"DeviceID", L"PNPDeviceID", L"Manufacturer",
			NULL
		}
	},
	{
		L"Drivers", L"Win32_SystemDriver", {
			L"Caption", L"Description", L"Name", L"StartMode", L"Started", L"DisplayName", L"PathName",
			L"ServiceType", L"StartName", L"State",
			NULL
		}
	},
	{
		L"Services", L"Win32_Service", {
			L"Caption", L"Description", L"Name", L"StartMode", L"Started", L"DisplayName", L"PathName",
			L"ServiceType", L"StartName", L"State", L"ProcessId",
			NULL
		}
	},
	{
		L"Environment Variables", L"Win32_Environment", {
			L"Caption", L"Description", L"Name", L"SystemVariable", L"UserName", L"VariableValue",
			NULL
		}
	},
	{
		L"Network Connections", L"Win32_NetworkConnection", {
			L"Caption", L"Description", L"Name", L"Comment", L"ConnectionType", L"ConnectionState", L"DisplayType",
			L"LocalName", L"ProviderName", L"RemoteName", L"RemotePath", L"ResourceType", L"UserName",
			NULL
		}
	},
	{
		L"Processes", L"Win32_Process", {
			L"Caption", L"Description", L"Name", L"CreationDate", L"Handle", L"Priority", L"ExecutionState",
			L"TerminationDate", L"ExecutablePath", L"ProcessId", L"ParentProcessId", L"SessionId", L"CommandLine",
			NULL
		}
	},
	{
		L"Program Groups", L"Win32_LogicalProgramGroup", {
			L"Caption", L"Description", L"Name", L"GroupName", L"UserName",
			NULL
		}
	},
	{
		L"Startup Command", L"Win32_StartupCommand", {
			L"SettingID", L"Caption", L"Description", L"User", L"Name", L"Location", L"Command",
			NULL
		}
	},
	{
		L"Accounts", L"Win32_Account", {
			L"Caption", L"Description", L"Name", L"Domain", L"SID", L"SIDType", L"LocalAccount",
			NULL
		}
	},
	{
		L"Base Board", L"Win32_BaseBoard", {
			L"Caption", L"Description", L"Name", L"Manufacturer", L"Model", L"SKU", L"SerialNumber",
			L"Tag", L"Version", L"PartNumber", L"HostingBoard", L"SlotLayout", L"Product",
			L"ConfigOptions",
			NULL
		}
	},
	{
		L"Desktop Monitor", L"Win32_DesktopMonitor", {
			L"Caption", L"Description", L"Name", L"DeviceID", L"PNPDeviceID", L"MonitorManufacturer",
			L"ScreenHeight", L"ScreenWidth", L"MonitorType",
			NULL
		}
	},
	{
		L"Disk Drives", L"Win32_DiskDrive", {
			L"Caption", L"Description", L"Name", L"DeviceID", L"PNPDeviceID", L"Manufacturer",
			L"MediaLoaded", L"MediaType", L"Model", L"SerialNumber", L"FirmwareRevision", 
			NULL
		}
	},
	{
		L"Floppy Controller", L"Win32_FloppyController", {
			L"Caption", L"Description", L"Name", L"DeviceID", L"PNPDeviceID", L"Manufacturer",
			NULL
		}
	},
	{
		L"Mapped Logical Disks", L"Win32_MappedLogicalDisk", {
			L"Caption", L"Description", L"Name", L"DeviceID", L"PNPDeviceID", L"Purpose", L"SessionID",
			L"FileSystem", L"VolumeName", L"VolumeSerialNumber", L"ProviderName",
			NULL
		}
	},
	{
		L"Motherboard Device", L"Win32_MotherboardDevice", {
			L"Caption", L"Description", L"Name", L"DeviceID", L"PNPDeviceID", L"PrimaryBusType", L"RevisionNumber",
			L"SecondaryBusType",
			NULL
		}
	},
	{
		L"Software Features", L"Win32_SoftwareFeature", {
			L"Caption", L"Description", L"Name", L"IdentifyingNumber", L"ProductName", L"Vendor", L"Version",
			NULL
		}
	},
	{ NULL, NULL,  { NULL } }
};
