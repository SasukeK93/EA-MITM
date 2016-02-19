/*
*
*   Title: IndigoHAWX-MP
*   Authors: Eyaz Rehman [http://github.com/Imposter]
*   Date: 2/17/2016
*
*   Copyright (C) 2016 Eyaz Rehman. All Rights Reserved.
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

#ifndef INDIGO_UTILITY_DLL_H_
#define INDIGO_UTILITY_DLL_H_

#include "../Core/Handle.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <functional>

namespace indigo {
	class DLL {
		static HANDLE loader_thread_;
		static Handle wait_handle_;
		static std::function<void()> initialize_callback_;
		static std::function<void()> shutdown_callback_;

		static void loader_thread_worker();

	public:
		static void Initialize(std::function<void()> initialize_callback, std::function<void()> shutdown_callback);
		static void Shutdown();
	};
}

#endif // INDIGO_UTILITY_DLL_H_