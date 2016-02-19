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

#ifndef INDIGO_UTILITY_FUNCTIONS_H_
#define INDIGO_UTILITY_FUNCTIONS_H_

#include <string>
#include <stdint.h>
#include <Winsock2.h>

namespace indigo {
	class Functions {
	public:
		static std::string ToHexString(uint8_t *obj, uint32_t size);
		static std::string DumpArray(uint8_t *obj, uint32_t size);
		static std::string CompareArrays(uint8_t *obj_1, uint32_t size_1, uint8_t *obj_2, uint32_t size_2);
		static void FlipEndian(uint8_t *buffer, uint32_t size);

		template<typename T>
		static T FlipEndian(T obj) {
			uint32_t size = sizeof(T);
			uint8_t *buffer = new uint8_t[size];
			memcpy(buffer, &obj, size);
			FlipEndian(buffer, size);
			T result = *reinterpret_cast<T*>(&buffer[0]);
			delete[] buffer;
			return result;
		}

		static int32_t Random(int32_t min, int32_t max);

		static bool EndpointsEqual(sockaddr_in ip_endpoint_1, sockaddr_in ip_endpoint_2);
		static sockaddr_in GetEndpointInfo(char *ip_address, uint16_t port);
		static bool IsWANConnected();
	};
}

#endif // INDIGO_UTILITY_FUNCTIONS_H_