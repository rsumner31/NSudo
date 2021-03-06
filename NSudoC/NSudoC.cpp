// NSudoC.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "NSudoVersion.h"

#include "NSudoResourceManagement.h"

void NSudoPrintMsg(
	_In_opt_ HINSTANCE hInstance,
	_In_opt_ HWND hWnd,
	_In_ LPCWSTR lpContent)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hWnd);

	std::wstring DialogContent =
		g_ResourceManagement.GetLogoText() +
		lpContent +
		g_ResourceManagement.GetUTF8WithBOMStringResources(
			IDR_String_Links);
	
	DWORD NumberOfCharsWritten = 0;
	WriteConsoleW(
		GetStdHandle(STD_OUTPUT_HANDLE),
		DialogContent.c_str(),
		(DWORD)DialogContent.size(),
		&NumberOfCharsWritten,
		nullptr);
}

HRESULT NSudoShowAboutDialog(
	_In_ HWND hwndParent)
{
	UNREFERENCED_PARAMETER(hwndParent);
	
	std::wstring DialogContent =
		g_ResourceManagement.GetLogoText() +
		g_ResourceManagement.GetUTF8WithBOMStringResources(IDR_String_CommandLineHelp) +
		g_ResourceManagement.GetUTF8WithBOMStringResources(IDR_String_Links);

	SetLastError(ERROR_SUCCESS);

	DWORD NumberOfCharsWritten = 0;
	WriteConsoleW(
		GetStdHandle(STD_OUTPUT_HANDLE),
		DialogContent.c_str(),
		(DWORD)DialogContent.size(),
		&NumberOfCharsWritten,
		nullptr);

	return NSudoGetLastCOMError();
}

int main()
{
	CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	std::vector<std::wstring> command_args =
		g_ResourceManagement.GetCommandParameters();

	if (command_args.size() == 1)
	{
		command_args.push_back(L"-?");
	}

	NSUDO_MESSAGE message = NSudoCommandLineParser(
		g_ResourceManagement.IsElevated,
		false,
		command_args);
	if (NSUDO_MESSAGE::NEED_TO_SHOW_COMMAND_LINE_HELP == message)
	{
		NSudoShowAboutDialog(nullptr);
	}
	else if (NSUDO_MESSAGE::SUCCESS != message)
	{
		std::wstring Buffer = g_ResourceManagement.GetMessageString(
			message);
		NSudoPrintMsg(
			g_ResourceManagement.Instance,
			nullptr,
			Buffer.c_str());
		return -1;
	}

	return 0;
}

