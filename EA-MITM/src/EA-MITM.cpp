/*
*
*   Title: EA-MITM
*   Authors: Baggie [http://bitbucket.com/Bag123],
*   Date: 2/17/2016
*
*   Copyright (C) 2016 Baggie. All Rights Reserved.
*
*   This program is free software; you can redistribute it and/or
*   modify it under the terms of the GNU General Public License
*   as published by the Free Software Foundation; either version 2
*   of the License, or any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program; if not, write to the Free Software
*   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
*   02110-1301, USA.
*
*/

#include "EA-MITM.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "EA/ProtoSSL.h"
#include "Indigo/IO/CommandLine.h"
#include "Indigo/IO/MessageBox.h"
#include "Indigo/Utility/Memory.h"
#include "Indigo/Utility/DLL.h"

namespace base {
	indigo::Config *EA_MITM::Config;
	indigo::Log *EA_MITM::Log;

	void EA_MITM::Initialize() {
#ifdef _DEBUG
		// Break and wait for the debugger to be attached
		__debugbreak();
#endif

		// Open the config file
		std::ifstream config_file;
		config_file.open(PRG_NAME ".ini");
		if (!config_file.is_open()) {
			indigo::MessageBox::Show(indigo::kMessageBoxType_Error, PRG_NAME " - Fatal Error", "Your installation of " PRG_NAME " is corrupt. Please reinstall and try again.");
			EA_MITM::Shutdown();
		}

		config_file.seekg(0, std::ios::end);
		size_t config_size = config_file.tellg();
		config_file.seekg(0, std::ios::beg);

		std::string config_buffer;
		config_buffer.resize(config_size);
		config_file.read(const_cast<char *>(config_buffer.c_str()), config_size);

		Config = new indigo::Config();
		if (!Config->Open(config_buffer)) {
			indigo::MessageBox::Show(indigo::kMessageBoxType_Error, PRG_NAME " - Fatal Error", "Your installation of " PRG_NAME " is corrupt. Please reinstall and try again.");
			EA_MITM::Shutdown();
		}

		// Open the log file
		Log = new indigo::Log(PRG_NAME ".log",
#ifdef _DEBUG
			indigo::kLogType_Debug,
			true
#else
			indigo::kLogType_Error,
			false
#endif
			);
		if (!Log->Open()) {
			indigo::MessageBox::Show(indigo::kMessageBoxType_Error, PRG_NAME " - Fatal Error", "Your installation of " PRG_NAME " is corrupt. Please reinstall and try again.");
			EA_MITM::Shutdown();
		}

		// Write configuration to log
		Log->Write(indigo::kLogType_Debug, "Global", "Copyright (C) 2016 Baggie. All Rights Reserved.");
		Log->Write(indigo::kLogType_Debug, "Global", PRG_NAME " starting up...");

		if (!ea::ProtoSSL::Initialize()) {
			indigo::MessageBox::Show(indigo::kMessageBoxType_Error, PRG_NAME " - Error", "Failed to initialize ProtoSSL!");
			ExitProcess(-1);
		}
	}

	void EA_MITM::Shutdown() {
		if (!ea::ProtoSSL::Shutdown()) {
			indigo::MessageBox::Show(indigo::kMessageBoxType_Error, PRG_NAME " - Error", "Failed to shutdown ProtoSSL!");
			ExitProcess(-1);
		}

		// Close the config
		if (Config != nullptr) {
			std::ofstream config_file;
			config_file.open(PRG_NAME ".ini");
			if (config_file.is_open()) {
				config_file << Config->GetBuffer();
			}

			Config->Close();
			delete Config;
		}

		// Close the log
		if (Log != nullptr) {
			Log->Close();
			delete Log;
		}

		// Force exit
		ExitProcess(0);
	}
}

// DirectInput exports (dinput8.dll)
#include <dinput.h>
#pragma comment(lib, "dinput8.lib")
extern "C" HRESULT __stdcall mDirectInput8Create(HINSTANCE program_instance, DWORD version, REFIID rguid, IDirectInput8A **direct_input, LPUNKNOWN unknown) {
	typedef HRESULT(__stdcall *tDirectInput8Create)(HINSTANCE, DWORD, REFIID, IDirectInput8A**, LPUNKNOWN);
	tDirectInput8Create pDirectInput8Create = (tDirectInput8Create)indigo::Memory::GetDllExport("DirectInput8Create", "dinput8.dll", true);
	if (pDirectInput8Create == nullptr) {
		return E_FAIL;
	}

	return pDirectInput8Create(program_instance, version, rguid, direct_input, unknown);
}

// PSAPI exports (psapi.dll)
#include <Psapi.h>
#pragma comment(lib, "psapi.lib")
extern "C" DWORD __stdcall mGetProcessImageFileNameW(HANDLE process, LPWSTR image_file_name, DWORD size) {
	typedef DWORD(__stdcall *tGetProcessImageFileNameW)(HANDLE, LPWSTR, DWORD);
	tGetProcessImageFileNameW pGetProcessImageFileNameW = (tGetProcessImageFileNameW)indigo::Memory::GetDllExport("GetProcessImageFileNameW", "psapi.dll", true);
	if (pGetProcessImageFileNameW == nullptr) {
		return E_FAIL;
	}

	return pGetProcessImageFileNameW(process, image_file_name, size);
}

extern "C" DWORD __stdcall mGetModuleFileNameExW(HANDLE process, HMODULE module, LPWSTR file_name, DWORD size) {
	typedef DWORD(__stdcall *tGetModuleFileNameExW)(HANDLE, HMODULE, LPWSTR, DWORD);
	tGetModuleFileNameExW pGetModuleFileNameExW = (tGetModuleFileNameExW)indigo::Memory::GetDllExport("GetModuleFileNameExW", "psapi.dll", true);
	if (pGetModuleFileNameExW == nullptr) {
		return E_FAIL;
	}

	return pGetModuleFileNameExW(process, module, file_name, size);
}

BOOL WINAPI DllMain(HINSTANCE program_instance, DWORD reason, LPVOID reserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		indigo::DLL::Initialize(base::EA_MITM::Initialize, base::EA_MITM::Shutdown);
	} else if (reason == DLL_PROCESS_DETACH) {
		indigo::DLL::Shutdown();
	}

	return TRUE;
}