/**********************************************
 * @file TimeDefinition.h
 * @brief None
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.03.24
 * @license GNU General Public License (GPL)
 **********************************************/

#ifndef TIMEDEFINITION_H
#define TIMEDEFINITION_H

#include "Definition.h"
#include "profileapi.h"
#include "winnt.h"

#define TIMER_START LARGE_INTEGER startTime, stopTime, timeParam; QueryPerformanceFrequency(&timeParam);QueryPerformanceCounter(&startTime);
#define TIMER_STOP QueryPerformanceCounter(&stopTime); const double time = sc_double(stopTime.QuadPart - startTime.QuadPart) / sc_double(timeParam.QuadPart);

#endif
