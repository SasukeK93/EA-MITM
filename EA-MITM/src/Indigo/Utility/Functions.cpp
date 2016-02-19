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

#include "Functions.h"
#include "../Core/String.h"
#include <ctime>
#include <WS2tcpip.h>
#include <SensAPI.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "SensAPI.lib")

namespace indigo {
	std::string Functions::ToHexString(uint8_t *obj, uint32_t size) {
		std::string output;
		output.resize(size * 2);
		for (uint32_t i = 0; i < size; i++) {
			uint8_t byte = obj[i];
			_itoa_s(byte, const_cast<char*>(output.c_str()) + (i * 2), output.size() - i * 2, 16);
		}
		return output;
	}

	std::string Functions::DumpArray(uint8_t *obj, uint32_t size) {
		std::string output;

		output.append("----- Array Dump -----\n");

		output.append(String::Format("Length: %i\n", size));
		output.append("Data: ");
		for (uint32_t i = 0; i < size; i++) {
			char character = obj[i];
			if (character == '\0' || character == '\n' || character == '\r') {
				character = ' ';
			}
			output.push_back(character);
		}
		output.append("\nHex Dump:");

		for (uint32_t i = 0; i < size; i++) {
			if (i % 8 == 0) {
				output.append("\n");
			}

			char character = obj[i];
			if (character == '\0' || character == '\n' || character == '\r') {
				character = ' ';
			}

			output.append(String::Format("0x%02X %c ", obj[i], character));
		}

		output.append("\n----------------------\n");

		return output.c_str();
	}

	std::string Functions::CompareArrays(uint8_t *obj_1, uint32_t size_1, uint8_t *obj_2, uint32_t size_2) {
		std::string output;
		output.append("----- Array Comparison -----\n");

		uint8_t *smaller_obj = obj_1;
		uint32_t smaller_size = size_1;
		uint8_t *larger_obj = obj_2;
		uint32_t larger_size = size_2;
		if (size_1 > size_2) {
			smaller_obj = obj_2;
			smaller_size = size_2;
			larger_obj = obj_1;
			larger_size = size_1;
			output.append("A) Array 2\n");
			output.append("B) Array 1\n");
		}
		else {
			output.append("A) Array 1\n");
			output.append("B) Array 2\n");
		}

		int32_t differences = 0;
		for (uint32_t i = 0; i < smaller_size; i++) {
			uint8_t b_1 = smaller_obj[i];
			uint8_t b_2 = larger_obj[i];
			if (b_1 != b_2) {
				output.append(String::Format("Difference at %i: (A) 0x%02X vs. (B) 0x%02X\n", i, b_1, b_2));
				differences++;
			}
		}

		for (uint32_t i = smaller_size; i < larger_size; i++) {
			uint8_t b = larger_obj[i];
			output.append(String::Format("Extra byte at %i: (%c) 0x%02X\n", i, larger_obj == obj_1 ? 'A' : 'B', b));
			differences++;
		}

		output.append("----------------------\n");

		if (differences == 0) {
			output = "No differences";
		}

		return output;
	}

	void Functions::FlipEndian(uint8_t *buffer, uint32_t size) {
		for (uint32_t i = 0; i < size / 2; i++) {
			uint8_t a = buffer[i];
			buffer[i] = buffer[size - 1 - i];
			buffer[size - 1 - i] = a;
		}
	}

	int32_t Functions::Random(int32_t min, int32_t max) {
		srand(static_cast<unsigned int>(time(nullptr)));
		return rand() % (max - min + 1) + min;
	}

	bool Functions::EndpointsEqual(sockaddr_in ip_endpoint_1, sockaddr_in ip_endpoint_2) {
		bool result = true;

		if (ip_endpoint_1.sin_family != ip_endpoint_2.sin_family)
			result = false;

		if (ip_endpoint_1.sin_port != ip_endpoint_2.sin_port)
			result = false;

		if (ip_endpoint_1.sin_addr.S_un.S_addr != ip_endpoint_2.sin_addr.S_un.S_addr)
			result = false;

		return result;
	}

	sockaddr_in Functions::GetEndpointInfo(char *ip_address, uint16_t port)
	{
		sockaddr_in endpoint;

		endpoint.sin_family = AF_INET;
		endpoint.sin_port = htons(port);
		ZeroMemory(&endpoint.sin_zero, sizeof(endpoint.sin_zero));
		inet_pton(AF_INET, ip_address, &endpoint.sin_addr);

		return endpoint;
	}

	bool Functions::IsWANConnected() {
		DWORD flags = NETWORK_ALIVE_WAN;
		if (IsNetworkAlive(&flags) && GetLastError() == NO_ERROR) {
			return true;
		}
		return false;
	}
}