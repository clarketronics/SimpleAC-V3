#pragma once
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include "Arduino.h"
#include "FlashBeep.h"
#include "Config.h"
#include "Helpers.h"
#include "Data.h"

void printDetail(uint8_t type, int value);
void mp3Begin(DFRobotDFPlayerMini &mp3, SoftwareSerial &softSerial, FlashBeep &feedback, Data &data);
void mp3Play(DFRobotDFPlayerMini &mp3, int track);