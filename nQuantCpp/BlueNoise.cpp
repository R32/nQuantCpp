/**
 * Alan Wolfe, Nathan Morrical, Tomas Akenine-M�ller, Ravi Ramamoorthi:
 * Scalar Spatiotemporal Blue Noise Masks. CoRR abs/2112.09629 (2021)
 * Copyright (c) 2022 - 2023 Miller Cy Chan */

#include "stdafx.h"
#include "BlueNoise.h"

#include <memory>

namespace BlueNoise
{
	// Reference mask from: https://tellusim.com/download/noise/64x64_l64_s16.png
	// Made from: https://github.com/Tellusim/BlueNoise
	const char TELL_BLUE_NOISE[] = {
		18, 60, -13, 24, -93, -44, -107, 18, -34, 123, -120, 63, 115, 32, 91, -28, 118, -12, 97, -42, 77, -76, 30, -97, -39,
		87, -10, -57, 54, -3, -55, 118, 10, -35, 42, 124, -97, 57, -60, 1, -41, -103, -68, 97, -116, 44, -74, 60, -48, 87,
		-9, -117, 10, -73, 98, 40, -77, 55, -17, -86, 65, 36, 114, -48, -93, -39, -121, 98, -21, 119, 37, 84, -100, -2, 38,
		-26, -89, -10, -124, 13, -66, -115, 34, -89, -5, 121, -51, 98, 38, -118, 65, -88, 113, -124, 60, -18, -77, 101, -128, 2,
		-37, 95, -118, 86, 25, 58, -24, 69, -47, -9, 81, -127, 125, -77, 41, 111, -49, 53, -22, -108, 79, -61, 21, 126, -7,
		-128, -71, 71, 107, 36, 81, -56, 52, -80, -5, -55, 47, -77, 99, -57, 27, 76, -41, 106, 43, 78, -34, 107, 53, -123,
		21, -16, -70, 124, -29, 24, -48, 8, 96, -102, 78, -49, 25, 77, -70, 12, 47, -51, -80, 102, 13, -87, 36, 107, -85,
		27, -28, 10, -99, -24, 73, -101, 123, 15, -38, 97, -113, -51, 81, -34, 46, -20, 8, -82, -4, -100, 11, 74, -128, 112,
		-23, 81, 9, -112, 111, -79, 50, -103, -24, -81, 19, -99, -22, -65, 70, -101, 52, 5, -99, 84, 50, -78, -33, 39, -5,
		56, -93, -16, 111, -86, -27, 122, -2, -123, -53, 120, -106, -55, -16, 66, -59, 55, 105, -69, 29, -5, -57, 55, -94, 3,
		60, 31, -96, 17, 92, -109, -50, 124, 49, -30, 110, -58, 34, -81, 62, -105, -39, 48, -22, 14, -49, 87, 0, 126, -56,
		84, 31, 96, -8, 110, -47, 91, -67, -4, -116, 102, 68, -60, -117, 119, -64, 45, -117, 26, 69, -105, 45, 77, -15, 29,
		72, 8, 100, -98, 91, -113, -15, 82, -120, 108, -84, 90, -16, 117, -75, -20, 111, -57, -80, 65, 30, -120, -62, 69, -108,
		-9, 101, 11, -46, 30, 125, -85, 94, -119, 70, -83, 33, -126, 56, -4, -111, -42, -86, 20, -121, 35, -30, 116, 23, -21,
		-93, 11, 85, -40, 3, 75, -33, 100, -66, 8, -45, -84, 92, -76, -35, -122, 46, -65, -6, 31, -49, 49, 11, -27, 47,
		18, -125, -53, 67, -107, 51, -3, 102, -26, 86, 0, 97, 15, -72, 43, -35, -114, 70, -75, -9, 72, -47, 30, -7, 108,
		-42, 95, -25, -78, 115, 40, 68, -21, 80, -79, 57, -107, -53, 44, 127, -32, 30, -104, 107, -85, 20, -99, -13, 112, 67,
		24, -113, 1, 111, 22, -40, 121, 68, -79, 113, -97, -65, 69, -105, -42, 80, 28, -8, 87, -39, -124, 16, -93, 42, -38,
		-99, -23, 120, 66, -86, 93, -19, 103, -117, 10, -93, 62, -67, -103, 13, -89, 30, 76, 2, -64, -104, 123, -61, 14, 98,
		0, 71, -69, -125, 78, -79, 57, -14, 48, -45, 78, 36, -126, -73, -26, 105, 45, -61, 70, -83, 10, -116, -19, 57, 4,
		92, -35, 119, 1, -88, 110, -100, -60, 36, 122, 63, -58, 116, -73, 63, 24, -122, -51, 5, 38, -59, 27, -37, 50, 99,
		-38, 120, -21, 53, 86, -57, -118, -34, 101, 11, -32, 53, -103, -38, -85, 110, -6, 61, -50, 5, 100, -63, -120, 125, -90,
		-35, 57, 2, 73, -51, -90, -8, -109, 90, -31, 41, 95, -51, -126, 27, -81, 39, -59, 65, -34, 9, 101, -88, -24, -102,
		-13, -116, 4, 106, -62, 80, -11, 108, -109, 86, -89, 117, -65, -3, -123, 24, 77, -113, -35, 122, 7, 71, -89, 47, -128,
		103, -9, 64, 20, -45, -101, 17, 108, -112, -35, 15, 59, -25, 9, 97, -60, 116, -104, -3, 58, 126, 21, -54, 59, -98,
		-69, -2, 124, -24, 83, -111, 95, 20, -120, 54, -44, 25, 91, 13, 77, -39, 51, -102, -27, 39, -95, 56, -64, -15, 13,
		58, -101, 41, 91, -84, -58, -3, 31, -96, 41, -63, -5, 89, -45, 24, -90, 120, -118, 89, 52, -32, -67, 55, 82, -97,
		103, -80, 73, -111, 26, -81, 38, 92, -118, -32, -78, 99, -1, 115, 35, 74, -84, 61, -54, 3, -68, -18, 127, -78, 72,
		-114, -48, -83, 35, -64, 96, -6, 85, -72, 124, -37, 18, 70, -126, -29, 80, -47, -15, 12, 112, 56, -71, 75, -31, 108,
		-106, 33, -72, 84, -20, -52, 42, -23, -82, 115, 28, -90, -11, 35, -46, -3, 41, -50, -11, 82, -35, -62, 25, 82, 51,
		-106, -67, -22, -116, -38, 16, -104, 32, 109, 59, -86, 34, 4, -28, 104, 48, -15, 113, 18, -90, 37, -127, -12, 23, -112,
		91, -79, 110, 2, -87, 127, -111, 75, -34, -108, 102, -10, -121, 52, -40, 126, -10, -116, 57, 93, -70, 2, 70, -123, -4,
		97, -59, 119, -127, 87, -97, 122, 48, -123, 10, 117, -93, -2, -48, 18, 56, 86, 24, 110, -75, 78, -14, -123, -34, 84,
		-106, 91, -64, -3, -93, 84, -120, -17, 76, -43, 104, 60, -60, 49, -4, -42, 45, -61, 60, 21, -66, 50, -87, 7, -58,
		25, 94, -74, 18, -94, 67, 20, -80, 6, -111, 100, 26, -49, 49, -28, -108, 16, 68, -23, 14, -70, -26, 96, -72, 64,
		-27, 94, -128, 120, -33, -86, -58, -12, 40, -50, 121, -70, 48, -2, -52, 51, -126, 120, 29, -46, 61, 126, -65, 10, -94,
		-30, 87, -81, 113, -108, 13, 98, -118, -36, 106, -21, 31, 121, 71, -86, -26, 61, -11, 98, -62, -31, 118, 46, -59, -29,
		-88, 122, -100, 89, 62, -16, -90, -55, 95, 55, -102, 29, -7, -108, 33, -62, 58, 4, -104, 67, 104, -122, 86, -96, 28,
		5, 98, -96, 115, 6, -30, 60, -72, 8, -95, -27, -115, 49, 114, 21, -118, 8, -29, 69, -71, -17, 79, 7, -105, 81,
		-51, -127, -15, 45, -109, 115, -50, -125, 40, 74, -108, -14, 107, 9, 72, -14, 11, -78, -49, 30, 115, 42, -116, -5, 114,
		-61, 86, -40, 111, -5, -82, 80, -22, 31, -43, 52, -3, -33, 66, -115, -56, -21, 32, -78, 96, -102, -14, 83, 35, 96,
		68, -13, -75, -52, 63, 101, -62, 24, 122, -98, 31, -56, 53, -85, -5, 61, -61, 99, 16, -80, 34, 86, 4, -81, -42,
		81, -98, 44, -121, -64, 108, 43, 83, -104, -64, 4, 74, -42, -89, 64, 7, -80, 63, -115, 45, -58, 106, -82, 10, -103,
		118, -81, 89, -13, 44, 80, -111, 63, -50, 21, 106, -119, -55, -87, 14, -101, 82, 35, -12, -91, 49, -125, -23, 56, -74,
		109, -25, 91, 21, 114, -96, -2, -41, 64, -7, -103, -30, 114, 52, 16, -69, -37, 98, 34, -43, -115, -25, 14, 97, -29,
		-78, 103, 34, -32, -123, 100, 12, -43, 123, 13, -112, 55, -51, 73, 35, -45, 23, -94, 124, -77, 5, -27, 77, -84, -31,
		63, -1, 42, -45, 121, -28, -114, 110, -41, 88, -1, -59, 101, 1, -120, 36, -66, -112, -36, 49, 84, -120, 124, -61, 93,
		22, -55, -121, -11, 124, 60, -5, -92, 68, 0, 125, -87, 33, -125, 55, 7, -105, 127, 53, -17, -74, 36, -95, -35, 86,
		-21, 112, -75, -15, -127, 107, -27, 16, -44, 98, 45, -124, 122, 43, -67, 112, -108, 58, -71, 19, 66, -74, 27, -104, 72,
		26, -99, 75, -38, 121, -11, 73, 15, -90, -50, 38, -85, -18, 59, -87, 104, 72, -83, 27, -111, -55, 22, 105, -76, 58,
		-47, 68, -14, 91, -58, -9, -77, 15, -107, 62, 94, 1, 59, -66, 39, -117, 18, 84, 43, -61, 63, -113, 70, -93, -59,
		11, -44, 2, -99, -20, 88, -12, 102, -96, -8, 90, -21, 125, -69, -27, 51, -72, 22, -101, 44, -74, 95, -13, 109, 3,
		83, 32, -115, 5, -34, 35, -22, -63, 95, 71, -30, -128, -11, 27, -105, 100, -65, -99, 38, 68, 90, -52, 111, -30, -63,
		-125, 116, -90, -1, 97, -38, -87, -2, 93, -83, 38, -12, 116, 29, 85, -76, 101, 67, 24, -48, -128, 34, -41, 52, -121,
		-54, 11, 44, -116, 115, -18, 93, -46, 85, -23, -122, 55, -69, -110, -34, -75, 101, 69, -69, -106, 111, 50, -6, -95, 38,
		90, -60, 112, -31, 20, -6, 121, -39, -121, -23, 47, -84, 31, 83, -8, 29, -33, 76, -62, 55, 126, -111, -28, 10, 103,
		-74, -36, -110, -16, 37, -116, -33, -80, 127, -5, 78, -88, 118, 3, 65, -95, 102, -6, -83, 63, -128, 1, -87, 31, 126,
		-42, 19, 62, 119, 12, -52, -14, 40, 85, 7, -123, -47, 119, -71, 3, 69, -92, 51, -120, 73, -82, 2, 103, 23, -107,
		76, 5, -98, -46, 100, -110, 37, -103, 9, -71, 47, 78, -53, -123, 26, 88, 60, -86, 113, 6, 91, 42, -102, 47, -64,
		20, -25, -69, 93, -18, -61, 78, 18, -63, 33, 112, 66, -57, 3, -84, 92, -26, -94, 49, -127, 114, -93, -40, -78, 75,
		17, 56, -24, -113, 32, -15, -70, 104, -54, 40, 65, -93, -56, 113, -19, -54, 125, 60, -76, 4, 89, -18, 108, -36, 17,
		-91, 119, 57, -10, -62, 16, -46, 65, -59, -91, -23, 85, -44, 109, -117, 86, 38, -112, 21, 58, -105, -40, 99, -33, -94,
		-17, -114, 77, 36, -109, 4, -54, 100, -30, 17, 77, -3, 127, -25, -66, -101, 98, 73, -54, 124, 59, 11, -17, -113, -37,
		88, 7, 57, -81, 44, -115, 16, -27, 49, -53, -88, 43, -120, 69, -6, -40, -82, 99, -108, 123, -13, -124, -1, 74, 31,
		10, -90, -10, 66, -38, -86, 112, -73, -32, 124, 28, -113, 73, 0, 40, 104, -28, -61, 109, 67, 24, -79, 56, -102, -63,
		35, -112, 53, 106, 37, -4, -76, 4, -109, -39, -91, 75, 116, 25, -73, -25, -127, 100, -7, 80, -85, 96, -124, 121, -7,
		84, -44, 105, -103, 14, 40, -26, 48, -78, 79, 41, 108, -49, -112, -31, 94, 27, -74, 51, 14, -15, 47, 81, -92, -8,
		57, -70, 120, -52, -82, 10, 51, -90, -19, -118, 88, -3, 110, -19, 67, -45, 0, -90, -38, -126, 52, 102, 22, 89, 37,
		-67, -3, -106, 52, 121, 27, -34, -73, 34, -38, 0, 68, -77, 19, -100, 29, -62, 54, 96, -113, 77, -55, 17, -32, -95,
		-68, 22, 120, 46, -125, -54, 123, -109, 89, -53, -121, -2, -67, 107, -45, 22, -105, 54, 89, -124, 117, -46, 82, 44, -40,
		-68, 29, -123, 102, -74, 92, 23, 67, 121, -28, -64, -91, -13, -124, 62, -49, 95, -17, -91, -57, 59, 92, -111, 117, -60,
		26, -36, 62, -19, 116, -8, -87, -29, -68, -8, 94, -117, 105, 32, 71, -11, -85, 85, 3, 71, -25, 6, -76, 59, 118,
		24, 74, -20, -126, 69, -5, -40, 23, -68, -2, 32, -106, -7, 123, -98, 83, -31, 5, 44, -110, -16, -60, -94, -1, 79,
		48, 119, -37, 104, 20, -79, 74, 17, 107, -100, 8, -21, 49, -97, 106, -114, 91, -68, -128, 67, 28, 108, 63, 22, -84,
		54, 0, -50, -108, 102, -43, -17, -70, 40, -87, 108, 33, -26, -89, -42, -101, 37, 90, -75, 109, -99, 65, -27, 86, -85,
		99, -75, 39, -12, 54, -62, -93, 122, -39, 82, 39, 94, 26, -115, -49, 13, -80, -2, -110, 50, -44, -119, -24, 42, -51,
		79, -70, -14, 56, 5, -47, 37, 82, -39, -99, 4, -121, -40, 126, -26, -77, 115, 15, 53, -100, 34, 117, -105, 78, -50,
		-116, 87, 6, 101, 63, -57, -12, 28, -48, 5, 127, -112, 47, -44, 14, 68, -52, -114, 104, 12, 73, 25, -82, -6, -119,
		-67, -35, 111, 40, -104, 56, 86, -52, 127, -4, 92, 64, -77, 123, -123, 13, 99, -55, -92, 125, 9, -82, -5, 119, -53,
		89, 59, -105, 38, 84, -126, -21, -58, 93, -119, -37, 17, -20, 49, -4, -73, 46, -122, -1, -89, 100, -113, 72, -89, 32,
		-59, -4, 111, -126, -18, 114, 28, -76, -22, -121, -48, 87, 46, 117, 15, 64, -87, -11, 79, -68, -17, 33, -99, -67, 26,
		-101, -5, 31, -28, 59, -106, 40, 69, -19, -110, 96, 58, -69, 35, -14, -72, 7, -61, -6, 50, 75, -89, 7, 66, 88,
		-73, 105, -85, 71, 125, -24, -55, 110, 20, -34, 41, -7, 102, -20, 89, -97, 72, -67, 58, -92, -37, 81, 47, 106, 0,
		-100, -33, -60, -102, -20, 97, -56, 17, 119, -121, 99, 3, 78, -36, 106, -56, 88, -84, 113, -43, -1, -72, 88, -51, 21,
		-33, -118, 84, -92, 106, 47, 91, -90, -48, 12, 124, -31, -96, -53, 38, -113, 8, -46, -103, 23, 80, -108, 68, -79, 117,
		-70, -42, -117, 59, 6, -38, 34, -9, 92, 3, -108, -44, -86, 39, 112, 25, 65, 91, 36, -118, 58, -98, -30, 66, -39,
		-88, 43, -128, 16, 66, -113, 10, -62, 28, 102, -125, 3, 54, -95, 108, 50, 15, -45, -2, -122, -26, 31, 103, -115, -66,
		45, 20, -12, 95, -27, 113, 33, 65, -70, -9, 38, -40, 6, -122, 75, 49, 19, -65, 98, -83, 119, -116, -57, 41, 125,
		17, 72, -24, -64, -127, -3, -89, -54, 8, 110, -13, 41, -78, 26, 87, -63, 116, -19, -73, -12, 79, 54, -103, -18, 77,
		-66, 118, -20, -74, -11, -101, 118, 54, -59, 112, -98, -15, 76, 0, 101, -124, 70, -92, 54, -67, -127, -23, 103, -95, 121,
		-72, 92, 30, -27, -96, 122, -17, -109, 51, -25, 13, 67, -94, -15, -68, -116, 99, 13, 83, -32, 126, 57, -35, -81, 76,
		-60, 114, -114, 6, -32, 62, -95, 45, 122, -41, -77, 110, -54, 45, 11, -107, 32, 64, 88, -63, 24, -108, 80, 2, 48,
		-54, 34, -91, -44, 121, -57, 22, -38, 89, -3, 81, -54, 19, -28, 71, -98, -49, 105, 8, -56, 81, 27, -50, 75, -77,
		103, -36, 60, 108, 32, -39, -92, 42, -67, 16, -105, 94, 36, -125, 5, 53, -23, 100, -106, 20, 102, -58, 17, -107, 38,
		1, 64, -90, -27, 98, -43, -124, 12, -33, 97, -8, -42, -88, 72, -109, 113, -26, 50, -94, -8, 103, -108, 46, -83, 38,
		-103, 61, -120, 25, -10, 60, -79, 67, -128, 1, -86, 114, -103, -13, 32, -125, 9, -49, -99, 81, 1, 113, -112, 86, -9,
		-65, -19, 83, -47, -93, 73, -74, 50, -47, -6, -119, 83, -11, 99, -62, -120, 127, 29, -75, 77, -12, 122, -90, 57, -74,
		42, 126, -19, 12, -71, 73, 5, 83, 39, -71, -16, 126, 9, -42, 114, -1, 94, -53, 117, -112, 29, -18, 106, 43, -34,
		54, 15, 87, -69, 113, -82, 90, -7, 50, -60, -23, 66, -39, 45, 110, -100, 19, 120, 34, -54, 12, 125, -85, 89, 54,
		-36, -84, 54, -25, 84, -9, -53, 60, -101, 39, -69, 22, -110, 79, 6, -128, -52, 96, 37, -114, -52, -29, -119, 88, 26,
		-56, -117, 70, -84, -35, -96, 54, -75, 9, 91, -71, -44, -102, 80, -68, -119, -40, 42, -23, 19, 57, -66, -123, 103, -91,
		29, -72, -122, 6, 67, -79, -24, -109, 95, -20, -125, -33, 8, -66, 19, 108, -104, 28, -81, 69, -112, 10, 112, -41, 85,
		-19, 109, -33, -63, 88, 56, -96, -13, 108, 21, 118, 53, -41, -91, 55, 99, -17, 45, 79, 18, -12, 82, -29, -97, 63,
		15, 127, -23, 7, 61, 120, -98, 74, -114, -35, 124, 30, -17, 71, 1, 121, 58, -58, -32, 100, 5, 49, -84, 79, 33,
		64, 109, -113, 76, -70, 0, 116, -42, 19, 97, -48, -88, -1, -121, 52, -80, 11, 46, -90, 2, -42, 78, -60, -92, -7,
		-75, 4, 112, -29, -94, 28, -65, -126, 124, -63, -115, 43, 113, -2, -118, 32, -83, 101, -59, -16, 10, -55, 106, -1, -96,
		-46, 90, -109, -63, -28, -98, 90, 48, -118, -61, 112, -41, -1, -63, -95, -7, -43, 39, -17, 63, -127, 57, -94, -19, 35,
		65, 95, 25, -52, 74, -114, 116, -30, 99, 25, -119, 40, 70, -106, 91, 41, -123, 21, 85, -45, 107, 3, -39, 36, 96,
		-52, -89, -34, 93, -51, 74, -113, 48, -84, 81, 44, -78, 61, 83, 19, -80, 33, 62, 98, 12, -15, -82, 30, 74, 16,
		-114, 56, 118, 13, 88, -85, 117, -100, -46, 94, -10, -66, 123, -118, -64, -32, -98, 121, -7, 28, -58, 64, -107, -61, 123,
		2, -46, 22, -23, -58, 73, -76, -1, -109, 53, -82, 73, -101, 11, -14, 75, 30, -77, 45, -1, -31, 16, 105, -124, -27,
		21, -107, -18, -56, 117, -5, -41, -120, 43, -55, 103, -4, -97, -19, 85, -77, -36, -110, 52, -32, 8, 85, 31, -88, 21,
		79, 44, -5, 107, 14, 62, -73, -36, 90, -92, -12, 34, 76, -24, -80, 107, 77, -83, 120, -18, 49, 114, -25, 15, 92,
		-16, 53, -76, 118, -123, -8, 111, -109, 82, -80, 67, -58, -7, 123, -51, 46, 107, -127, 64, -94, 85, -67, 127, -103, 50,
		-43, 116, -56, 37, -11, 93, 31, -62, 72, -122, -65, -21, 119, -48, -104, -36, -84, 55, -108, -13, 103, -126, 60, 18, 106,
		-50, -96, 14, 52, -126, -2, 37, -102, 16, -115, -65, 74, -91, -48, -117, 103, -35, 34, -67, 70, -46, 17, -59, 120, -103,
		11, 60, -99, 71, -74, -6, 25, -38, 42, -15, 28, -33, 74, 14, -128, 59, -88, 101, -121, 7, -91, 126, -13, 20, 77,
		46, -115, 59, -2, 104, 33, -47, 80, -60, 41, 6, -76, -32, -117, 55, 115, -45, 92, -65, 61, -49, 79, -42, 97, 27,
		-37, 42, 123, 29, 1, -96, 88, -18, 26, -95, 95, -9, 35, -26, 104, -70, 29, -29, 94, -97, 69, -70, 102, -115, 89,
		-93, -7, -78, 95, -18, 23, -37, 70, -66, 51, -43, -106, 102, -87, -38, 14, 91, -65, 17, -123, 118, 4, -95, 86, -46,
		125, 66, 12, -73, -2, -112, 24, -22, -110, 104, -79, 53, -13, -94, 88, -111, -29, -75, 65, -54, 52, -110, 125, -27, 42,
		-114, -71, 53, -111, -14, 79, -122, 19, -50, 120, 6, -86, 13, -60, 59, 113, -47, 43, -67, 124, -102, 15, 111, -6, 86,
		27, -48, 4, 114, -76, -22, -97, 65, -16, -71, 62, -23, 25, -107, -4, -91, 100, -22, 75, 42, -81, 127, -68, -3, 27,
		-124, 121, -61, -9, 64, 7, 88, -125, 115, -31, 9, -71, 80, -50, 71, 112, -47, 94, 9, -57, 109, -17, 47, -117, -16,
		55, 115, -32, 19, -117, 7, 87, -111, -10, 55, -80, -31, -114, -72, 70, -94, 58, -126, 74, 39, 126, -44, 96, 43, -113,
		107, -72, 92, 52, -55, 34, -105, -63, 98, -40, 49, 80, -47, 63, -31, 11, 48, 106, -87, -56, 37, -22, 23, -95, 94,
		50, -128, 23, -87, -10, 20, -81, 56, -103, 33, -88, 100, -68, 86, -44, -110, 42, -75, 77, -86, -28, 35, 83, -45, 92,
		39, 105, 12, -34, 107, -19, 24, -51, -4, -111, 24, -91, -53, 11, -40, 69, -15, -122, 114, -30, 88, 15, -11, -100, 13,
		-117, 117, -93, 83, -72, -104, -35, 27, 93, -100, 107, -73, 72, -52, -22, 2, 101, -39, 61, -121, 84, -26, 123, -48, 75,
		-32, 10, 37, -83, 65, -3, 110, -21, 28, 115, -60, -93, 22, -122, 1, -62, 62, -110, 32, -81, 90, -101, 105, -65, 87,
		0, 71, 122, -102, 26, -82, 43, 5, -91, 62, -128, 116, 56, -27, 28, -10, -57, 21, 111, -4, 76, -122, -16, 66, -45,
		1, 33, -105, 109, -85, 36, -102, 118, -56, 35, -69, 0, 22, -116, 61, -98, 125, -31, 93, -59, -102, 53, -48, -126, 71,
		-8, 120, -53, 76, -96, -15, 123, -48, 67, -37, 4, 61, -32, 41, -125, -20, -76, 55, -34, 109, -61, 78, -44, 26, -20,
		-56, -88, 96, -78, 54, 75, -118, -45, 61, -68, 126, 19, -77, 48, -120, 122, -12, 44, -56, 69, -28, 25, -8, 105, -109,
		82, -79, 104, -8, -50, 3, -125, 23, -10, 84, -70, 99, 9, -41, 39, -81, 51, -36, 97, 41, -89, 16, -119, 114, -69,
		21, -89, 118, -63, 34, 82, 3, -119, 70, -1, -107, 124, -75, 93, 36, 72, -41, 111, -107, -30, 93, 16, -97, 44, -28,
		-107, 102, -23, 78, -43, -77, 76, 9, -118, 85, -72, -97, 58, -16, 32, -40, 47, -73, 79, 40, 106, -79, 47, -116, -17,
		35, -92, 77, -110, 104, -14, -107, 5, -57, 89, -29, 46, -11, 75, -113, 85, -18, 61, -39, -98, 105, -15, -71, 49, -30,
		15, 65, -98, 2, -118, 18, -60, 6, 43, -69, -17, 105, -54, 8, 58, -71, 25, -102, 12, 99, -108, -37, 124, 3, 50,
		96, -44, -81, 112, -122, 9, 117, -105, -64, -29, 76, -46, 114, 66, -56, 126, -28, 19, -58, 73, 36, 117, -124, 30, -72,
		81, -86, -43, 44, -73, 15, -106, 100, 20, -54, 37, 116, -92, 92, -119, -12, -50, 117, -25, 77, -16, 82, -105, 113, 54,
		-127, 78, -91, 116, -11, 93, -65, 51, -21, 29, 58, -92, -66, -30, -126, 22, 80, 0, 65, -85, -20, 27, 72, -4, -100,
		14, -82, 1, -111, 27, -78, 90, -6, -99, -65, -30, 64, -9, 112, -105, 14, 125, -9, 104, -47, 73, -10, -74, 76, -110,
		-20, 23, -55, 55, 106, 30, -78, 57, -97, 125, -75, 21, -82, -39, 24, -22, 37, -42, -121, 62, -31, 119, -86, -58, 109,
		-7, 81, 39, 116, -22, -100, -62, -29, 94, 48, -49, -115, 122, 36, 96, -26, 46, 94, -11, 57, -119, 46, 110, 1, 84,
		-82, -51, 25, -24, 66, -71, -128, 29, -89, 41, -121, 127, -33, 59, -69, 70, -8, -88, -36, -111, 93, -52, 14, 62, -36,
		47, -7, 76, -95, 111, -68, 89, 23, -83, 7, -112, 70, 2, -119, -46, 17, -106, -57, 72, 37, 127, 18, -104, -69, 103,
		19, -37, -64, -127, 69, -94, -45, -71, 111, -21, -52, -87, 29, -110, 44, 95, -116, 52, -53, 89, -28, 79, 0, 105, -24,
		48, -95, -5, 95, -127, 122, 36, 84, 8, -24, 45, -125, -57, 92, -116, 119, -59, 6, 45, -112, -1, 68, -52, 87, 41,
		-44, 100, 43, 90, -83, 102, 2, -89, -6, -119, 57, -37, 66, -13, -83, 87, 58, 7, -36, 120, 13, 74, -101, 23, 99,
		67, -24, 123, -43, 10, -68, 109, -95, 5, 46, -74, -41, -100, 20, -73, 79, 30, -53, 18, -30, -66, -100, 53, -74, 109,
		-6, -85, 9, 68, -22, -107, 87, -51, 105, -26, -97, 115, -14, -79, 17, -98, -64, -19, 59, -43, 51, 109, -46, 90, -73,
		4, 114, -122, 47, -9, -102, 103, -79, 40, -117, -33, 52, -81, 6, -126, -66, 4, -103, 69, -18, 31, -38, 98, -106, 122,
		40, 85, -55, 113, -29, -106, 110, -80, 83, 2, 110, -43, 78, -101, 33, 115, -45, -90, 28, 61, -4, -92, 15, 56, -59,
		23, -127, 68, 126, -6, 30, 76, -124, 23, -67, -109, 40, -26, 64, -99, 31, -66, 98, -53, 35, -25, 72, -49, 93, -3,
		118, -41, 75, -10, 56, 97, 32, -79, 116, -124, 62, -76, -21, 13, -123, -10, 61, -115, 12, 63, -12, 33, -112, 46, -19,
		-122, 17, -16, 66, -120, 45, 97, -37, -75, 117, -35, 80, -114, 97, -29, 51, -70, -31, -114, 107, -76, -25, 123, -9, 78,
		-86, 12, 101, -51, -20, 71, 9, -88, 125, -120, 16, -92, 33, -68, -106, 39, -63, 115, -96, -55, -22, 85, -47, 18, -10,
		81, 38, -50, 96, -77, 28, -41, 94, -84, -49, 74, -58, 101, -80, 68, 121, -72, -40, -18, 8, -104, 71, 2, -124, 41,
		-64, -2, 32, -91, 104, 11, 83, 42, -43, 7, 82, -96, 60, -36, 113, -59, -127, 44, 119, -111, -38, 64, -20, 82, -41,
		109, -21, 83, 14, 90, -115, -25, 46, 12, -119, 51, -92, 75, -64, -109, 108, -85, 67, -24, 119, -94, -14, 50, 120, -97,
		20, -37, 38, 0, -99, 51, 95, 81, -76, 127, -61, 50, 87, -14, 105, -84, 68, -47, -4, -104, -49, -83, 63, -104, 39,
		-63, 18, -114, 48, -13, 82, 3, -78, 26, 101, -101, 30, -83, 43, -110, 64, -88, -53, -14, 22, 104, -87, 73, 127, -8,
		26, -34, 118, 8, -25, 22, -104, 48, -57, 80, 26, -125, -33, 1, 108, -117, 90, -62, -31, 6, -107
	};
	
	ARGB diffuse(const Color& pixel, const Color& qPixel, const float weight, const float strength, const int x, const int y)
	{
		int r_pix = pixel.GetR();
		int g_pix = pixel.GetG();
		int b_pix = pixel.GetB();
		int a_pix = pixel.GetA();

		auto adj = (TELL_BLUE_NOISE[(x & 63) | (y & 63) << 6] + 0.5f) / 127.5f;
		adj += ((x + y & 1) - 0.5f) * strength / 8.0f;
		adj *= weight;
		r_pix = static_cast<BYTE>(min(BYTE_MAX, max(r_pix + (adj * (r_pix - qPixel.GetR())), 0)));
		g_pix = static_cast<BYTE>(min(BYTE_MAX, max(g_pix + (adj * (g_pix - qPixel.GetG())), 0)));
		b_pix = static_cast<BYTE>(min(BYTE_MAX, max(b_pix + (adj * (b_pix - qPixel.GetB())), 0)));
		a_pix = static_cast<BYTE>(min(BYTE_MAX, max(a_pix + (adj * (a_pix - qPixel.GetA())), 0)));

		return Color::MakeARGB(a_pix, r_pix, g_pix, b_pix);
	}
	
	void dither(const UINT width, const UINT height, const ARGB* pixels, const ARGB* pPalette, const unsigned short nMaxColors, DitherFn ditherFn, GetColorIndexFn getColorIndexFn, unsigned short* qPixels, const float weight)
	{
		const float strength = 1 / 3.0f;  	
		
		for (UINT y = 0; y < height; ++y) {
			for (UINT x = 0; x < width; ++x) {
				UINT bidx = x + y * width;
				Color pixel(pixels[bidx]);
				Color c1 = pPalette[qPixels[bidx]];

				c1 = diffuse(pixel, c1, weight, strength, x, y);
				qPixels[bidx] = ditherFn(pPalette, nMaxColors, c1.GetValue(), bidx);
			}
		}
    }
}
