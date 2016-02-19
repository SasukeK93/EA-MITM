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

#include "DLL.h"

namespace indigo {
	HANDLE DLL::loader_thread_;
	Handle DLL::wait_handle_;
	std::function<void()> DLL::initialize_callback_;
	std::function<void()> DLL::shutdown_callback_;

	void DLL::loader_thread_worker() {
		initialize_callback_();
		wait_handle_.Wait();
		shutdown_callback_();
	}

	void DLL::Initialize(std::function<void()> initialize_callback, std::function<void()> shutdown_callback) {
		initialize_callback_ = initialize_callback;
		shutdown_callback_ = shutdown_callback;

		wait_handle_.Lock();
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)loader_thread_worker, NULL, NULL, NULL);
	}

	void DLL::Shutdown() {
		wait_handle_.Unlock();
	}
}