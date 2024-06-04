#pragma once
#include "globalFunctions.h"
//#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <chrono>
#include <format>
#include <string>


//typedefs
typedef std::chrono::milliseconds::rep miliseconds;//stores miliseconds
typedef std::chrono::microseconds::rep microseconds;//stores microseconds
typedef long double seconds;
typedef const miliseconds cmiliseconds;
typedef const microseconds cmicroseconds;
typedef const seconds cseconds;

//the time at which the application booted
extern std::chrono::steady_clock::time_point boottime;

inline uint GetSecond()
{
	return (uint)time(NULL);
}

//DONT CONVERT TO INT!
//HUGE NUMBERS
//inline seconds getSeconds()
//{
//	return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).c;
//}
inline microseconds getmicroseconds()
{
	return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}
inline miliseconds getMiliseconds()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}
inline seconds milisectosec(const miliseconds& ms)
{
	return (seconds)ms * 0.001;
}
inline seconds microsectosec(const microseconds& ms)
{
	return (seconds)ms * 0.000001;
}
inline miliseconds getMiliseconds(const seconds& s)
{
	return (miliseconds)(s * 1000);
}
inline miliseconds getMiliseconds(const microseconds& s)
{
	return (miliseconds)(s / 1000);
}

inline microseconds GetMicroSecondsSinceApplicationBoot()
{
	auto current_time = std::chrono::steady_clock::now();
	return std::chrono::duration_cast<std::chrono::microseconds>(current_time - boottime).count();
}
inline miliseconds GetMiliSecondsSinceApplicationBoot()
{
	auto current_time = std::chrono::steady_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(current_time - boottime).count();
}
inline seconds GetSecondsSinceApplicationBoot()
{
	return microsectosec(GetMicroSecondsSinceApplicationBoot());
}

// Converts UTC time string to a time_t value.

// Let's consider we are getting all the input in
// this format: '2014-07-25T20:17:22Z' (T denotes
// start of Time part, Z denotes UTC zone).
// A better approach would be to pass in the format as well.

inline std::time_t getEpochTime(const std::wstring& dateTime, const std::wstring dateTimeFormat = L"%Y-%m-%d %H:%M %p")
{

	// Create a stream which we will use to parse the string,
	// which we provide to constructor of stream to fill the buffer.
	std::wistringstream ss{ dateTime };

	// Create a tm object to store the parsed date and time.
	std::tm dt;

	// Now we read from buffer using get_time manipulator
	// and formatting the input appropriately.
	ss >> std::get_time(&dt, dateTimeFormat.c_str());

	// Convert the tm structure to time_t value and return.
	return std::mktime(&dt);
}
inline std::wstring timeToString(const std::wstring& format = L"%A, %B %d, %Y %I:%M:%S %p") {
	const auto& t = std::time(nullptr);
	const auto& tm = *std::localtime(&t);

	std::wostringstream oss;
	oss << std::put_time(&tm, format.c_str());
	return oss.str();
	//https://stackoverflow.com/questions/24750855/c-how-to-get-timestamp-string-both-on-linux-and-windows


	//time_t rawtime;
	//struct tm timeinfo = tm();
	//wchar_t buffer[80];

	//time(&rawtime);
	//localtime_r(&timeinfo, &rawtime);

	// Wednesday, March 19, 2014 01:06:18 PM
	//wcsftime(buffer, 80, format.c_str(), &timeinfo);
	

	//return std::wstring(buffer);
    //https://stackoverflow.com/questions/34963738/c11-get-current-date-and-time-as-string
    //return std::format(L"{:"+ format + L"}", std::chrono::system_clock::now());
}