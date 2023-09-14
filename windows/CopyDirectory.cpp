//拷贝文件夹
bool CopyDirectory(const std::wstring& source_dir, const std::wstring& target_dir)
{
	CreateDirectory(target_dir.c_str(), nullptr);

	// 遍历源文件夹
	std::wstring search_path = source_dir + L"\\*.*";
	WIN32_FIND_DATAW file_data;
	HANDLE handle = FindFirstFile(search_path.c_str(), &file_data);

	if (handle != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (wcscmp(file_data.cFileName, L".") == 0 || wcscmp(file_data.cFileName, L"..") == 0)
			{
				continue;
			}

			// 构造源文件/文件夹和目标文件/文件夹路径
			std::wstring source_file = source_dir + L"\\" + file_data.cFileName;
			std::wstring target_file = target_dir + L"\\" + file_data.cFileName;

			// 如果是文件夹，递归拷贝
			if (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (CopyDirectory(source_file, target_file) == false)
					return false;
			}
			// 如果是文件，直接拷贝
			else
			{
				if (CopyFileW(source_file.c_str(), target_file.c_str(), FALSE) == false)
					return false;
			}
		} while (FindNextFileW(handle, &file_data) != 0);

		FindClose(handle);
	}
	return true;
}

void test(){
		std::wstring source_dir = L"E:\\workbj\\wcb06\\zhou\\WCBClient\\VRDClientOne\\redist32";
		std::wstring target_dir = L"C:\\Users\\70918\\AppData\\Roaming\\VRDClient";

		CopyDirectory(source_dir, target_dir);
}