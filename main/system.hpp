#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <Arduino.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "sd_card.hpp"
#include "id3.hpp"
#include "pins.hpp"
#include "DFRobotDFPlayerMini.h"

namespace PlayerSystem
{
    const uint8_t ACTION_DECREMENT_FOLDER = 0;
    const uint8_t ACTION_INCREMENT_FOLDER = 1;
    const uint8_t ACTION_DECREMENT_TRACK = 2;
    const uint8_t ACTION_INCREMENT_TRACK = 3;
    const uint8_t ACTION_PLAY_TRACK = 4;
    const uint8_t ACTION_PAUSE_TRACK = 5;
    const uint8_t ACTION_DECREMENT_VOLUME = 6;
    const uint8_t ACTION_INCREMENT_VOLUME = 7;
    const uint8_t ACTION_SWITCH = 8;
    struct Button
    {
        uint8_t pin;
        uint8_t action;
        bool pressed;
        void Start(uint8_t pin, uint8_t action);
        bool IsActionQueued();
    };
    class System
    {
        private:
            TimerHandle_t infoTimerHandle;
        public:
            PlayerSD::FileSystemManager* fileSystem;
            PlayerLCD::LCD* lcd;
            System(PlayerSD::FileSystemManager* fileSystem, PlayerLCD::LCD* lcd, uint8_t hoice1Pin = PlayerPins::BUTTON_CHOICE_1,
                   uint8_t choice2Pin = PlayerPins::BUTTON_CHOICE_2, uint8_t switchPin = PlayerPins::BUTTON_SWITCH);
            void TreatButtonEvents();
            void DoAction(uint8_t action);
            void IncrementFolder();
            void DecrementFolder();
            void IncrementTrack();
            void DecrementTrack();
            void playTrack();
            void pauseTrack();
            void IncrementVolume();
            void DecrementVolume();
            void DisplayFolder();
            void DisplayTrack();
            void DisplayVolume();
            void DisplayPlayStatus();
            void ResetTrackInfo();
    };
}

#endif