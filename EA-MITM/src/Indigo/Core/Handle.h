/*
*
*   Title: IndigoHAWX-MP
*   Authors: Eyaz Rehman [http://github.com/Imposter]
*   Date: 1/24/2016
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

#ifndef INDIGO_CORE_HANDLE_H_
#define INDIGO_CORE_HANDLE_H_

#include <stdint.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace indigo {
	class Handle {
		HANDLE handle_;
	public:
		Handle(bool locked = true);
		~Handle();

		void Lock();
		void Unlock();
		bool Wait(int32_t timeout = -1);
	};
}

#endif // INDIGO_CORE_HANDLE_H_