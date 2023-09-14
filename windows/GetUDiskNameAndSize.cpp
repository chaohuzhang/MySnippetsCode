//扫描磁盘，获取U盘的盘符和容量大小
void ScanHD(string &uDiskAvaliable)             //uDiskAvaliable即可使用的U盘名称字符串
{
    uDiskAvaliable = "";
    char diskPath[3] = { 0 };                   //检测存在的U盘设备
    DISK_GEOMETRY pdg;
    HANDLE DiskHandle;
    UINT64 DiskSize;
    DWORD allDisk = GetLogicalDrives();         //获取电脑中的所有磁盘（即二进制对应的十进制数值）,假设电脑中可能有15个磁盘
    for(int i = 0; i < 15; i++){
        if((allDisk & 1) == 1){                 //判断最右边的一位的二进制数值是否为1
            sprintf(diskPath, "%c", 'A'+i);     
            //此时的diskPath的值为类似 "C:"，如果只需要所有磁盘，则增加 uDiskAvaliable += diskPath 即可结束当前的if语句
            
            //判断是否为U盘，且获取U盘的字节大小
            strcat(diskPath,":");               
            if (GetDriveTypeA(diskPath) ==  DRIVE_REMOVABLE){   //判断磁盘的类型
                char tmp[10] = "\\\\.\\";
                uDiskAvaliable += diskPath;
                uDiskAvaliable += "(";
                strcat(tmp,diskPath);           //形成\\.\F:
                DiskHandle = CreateFileA(tmp,   //获取磁盘句柄
                        GENERIC_WRITE | GENERIC_READ,
                        FILE_SHARE_WRITE | FILE_SHARE_READ | FILE_SHARE_DELETE,
                        NULL,
                        OPEN_EXISTING,
                        FILE_ATTRIBUTE_NORMAL,
                        NULL);
                DeviceIoControl(DiskHandle, IOCTL_DISK_GET_DRIVE_GEOMETRY, NULL, 0, &pdg, sizeof(pdg), NULL, (LPOVERLAPPED)NULL);
                CloseHandle(DiskHandle);        //关闭磁盘句柄
                DiskSize = (UINT64)pdg.Cylinders.QuadPart * pdg.TracksPerCylinder * pdg.SectorsPerTrack * pdg.BytesPerSector;
                sprintf(tmp, "%.2lf", 1.0*DiskSize/1024/1024/1024);
                uDiskAvaliable += tmp;
                uDiskAvaliable += "GB),";       //以GB作单位，保留两位小数
            }


        }
        allDisk = allDisk >> 1;                 //这一句非常重要！！！挪动判断的位置！！！！！！！
    }
    uDiskAvaliable += "please fill such as F:";
}