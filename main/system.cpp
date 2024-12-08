#include "system.hpp"

#define ModuleDelay() vTaskDelay(100 / portTICK_PERIOD_MS)
#define TRACK_INFO_LIST_SIZE 3

static const uint8_t CHOICE_1 = 0;
static const uint8_t CHOICE_2 = 1;
static const uint8_t SWITCH = 2;
static uint8_t folderNumber;
static uint8_t trackNumber;
static uint8_t volume;
static bool paused;
static PlayerSystem::Button buttons[3];
static TaskHandle_t runModuleHandle;
static bool changeTrackInfo = true;
static uint8_t trackInfoIndex = TRACK_INFO_LIST_SIZE - 1;
static const uint8_t TRACK_INFO_SEC = 5;
static const char trackInfo[TRACK_INFO_LIST_SIZE][5] = {"TIT2", "TPE1", "TALB"};
static const char trackInfoMissingStrings[TRACK_INFO_LIST_SIZE][17] = {"No title", "No artists", "No album"};

static void TrackInfoTick(TimerHandle_t timerHandle)
{
    changeTrackInfo = true;
}

static void RunModule(void* param)
{
    PlayerSystem::System* sys = (PlayerSystem::System*)param;
    while(!DF_begin(PlayerPins::SERIAL_TX, PlayerPins::SERIAL_RX, false, false, true))
	{
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
    ModuleDelay();
    DF_volume(volume);
    ModuleDelay();
    DF_playFolder(folderNumber, trackNumber);
    ModuleDelay();
    sys->DisplayFolder();
    while(true)
    {
        sys->TreatButtonEvents();
        if(changeTrackInfo)
        {
            trackInfoIndex++;
            if(trackInfoIndex >= TRACK_INFO_LIST_SIZE) trackInfoIndex = 0;
            char* rawInfo = sys->fileSystem->GetFolder(folderNumber - 1).GetFile(trackNumber - 1).GetFrameValue(trackInfo[trackInfoIndex]);
            if(!rawInfo) sys->lcd->Write(trackInfoMissingStrings[trackInfoIndex], 0);
            else
            {
                char info[17] = "";
                snprintf(info, 17, rawInfo);
                sys->lcd->Write(info, 0);
            }
            changeTrackInfo = false;
        }
        int val = 0;
        if(DF_isFinished(&val))
        {
            ModuleDelay();
            sys->DoAction(PlayerSystem::ACTION_INCREMENT_TRACK);
        }
        ModuleDelay();
    }
}

namespace PlayerSystem
{
    void Button::Start(uint8_t pin, uint8_t action)
    {
        this->pin = pin;
        this->action = action;
    }

    bool Button::IsActionQueued()
    {
        bool logicState = digitalRead(pin);
        if(logicState && !pressed)
        {
            pressed = true;
            return true;
        }
        pressed = logicState;
        return false;
    }

    System::System(PlayerSD::FileSystemManager* fileSystem, PlayerLCD::LCD* lcd, uint8_t choice1Pin, uint8_t choice2Pin, uint8_t switchPin)
    {
        this->fileSystem = fileSystem;
        this->lcd = lcd;
        lcd->SetIconMode(true);
        folderNumber = 1;
        trackNumber = 1;
        volume = 25;
        paused = false;
        buttons[CHOICE_1].Start(PlayerPins::BUTTON_CHOICE_1, ACTION_DECREMENT_FOLDER);
        buttons[CHOICE_2].Start(PlayerPins::BUTTON_CHOICE_2, ACTION_INCREMENT_FOLDER);
        buttons[SWITCH].Start(PlayerPins::BUTTON_SWITCH, ACTION_SWITCH);
        pinMode(PlayerPins::BUTTON_CHOICE_1, INPUT_PULLDOWN);
        pinMode(PlayerPins::BUTTON_CHOICE_2, INPUT_PULLDOWN);
        pinMode(PlayerPins::BUTTON_SWITCH, INPUT_PULLDOWN);
        infoTimerHandle = xTimerCreate("Track Info Timer", pdMS_TO_TICKS(TRACK_INFO_SEC * 1000), pdTRUE, (void*)0, TrackInfoTick);
        xTimerStart(infoTimerHandle, 0);
        xTaskCreate(RunModule, "System Run MP3 Module", 10000, this, 0, &runModuleHandle);
    }

    void System::TreatButtonEvents()
    {
        for(uint8_t i = 0; i < 3; i++)
        {
            if(buttons[i].IsActionQueued())
            {
                puts("ACTION NEEDED");
                DoAction(buttons[i].action);
                puts("ACTION PERFORMED");
            }
        }
    }

    void System::DoAction(uint8_t action)
    {
        switch(action)
        {
            case ACTION_DECREMENT_FOLDER:
                DecrementFolder();
                DisplayFolder();
                ResetTrackInfo();
                paused = false;
                break;
            case ACTION_INCREMENT_FOLDER:
                IncrementFolder();
                DisplayFolder();
                ResetTrackInfo();
                paused = false;
                break;
            case ACTION_DECREMENT_TRACK:
                DecrementTrack();
                DisplayTrack();
                ResetTrackInfo();
                paused = false;
                break;
            case ACTION_INCREMENT_TRACK:
                IncrementTrack();
                DisplayTrack();
                ResetTrackInfo();
                paused = false;
                break;
            case ACTION_PLAY_TRACK:
                playTrack();
                DisplayPlayStatus();
                break;
            case ACTION_PAUSE_TRACK:
                pauseTrack();
                DisplayPlayStatus();
                break;
            case ACTION_DECREMENT_VOLUME:
                DecrementVolume();
                DisplayVolume();
                break;
            case ACTION_INCREMENT_VOLUME:
                IncrementVolume();
                DisplayVolume();
                break;
            case ACTION_SWITCH:
                if(buttons[0].action == ACTION_DECREMENT_VOLUME)
                {
                    buttons[0].action = ACTION_DECREMENT_FOLDER;
                    buttons[1].action = ACTION_INCREMENT_FOLDER;
                }
                else
                {
                    buttons[0].action += 2;
                    buttons[1].action += 2;
                }
                if(buttons[0].action == ACTION_DECREMENT_FOLDER) DisplayFolder();
                if(buttons[0].action == ACTION_DECREMENT_TRACK) DisplayTrack();
                if(buttons[0].action == ACTION_DECREMENT_VOLUME) DisplayVolume();
                if(buttons[0].action == ACTION_PLAY_TRACK) DisplayPlayStatus();
                break;
        }
    }

    void System::IncrementFolder()
    {
        trackNumber = 1;
        folderNumber++;
        if((folderNumber > 99) || (fileSystem->GetFolder(folderNumber - 1).GetFileCount() == 0))
        {
            folderNumber = 1;
        }
        DF_playFolder(folderNumber, trackNumber);
        ModuleDelay();
    }

    void System::DecrementFolder()
    {
        trackNumber = 1;
        folderNumber--;
        if((folderNumber < 1) || (fileSystem->GetFolder(folderNumber - 1).GetFileCount() == 0))
        {
            folderNumber = 99;
            for(; fileSystem->GetFolder(folderNumber - 1).GetFileCount() == 0; folderNumber--);
        }
        DF_playFolder(folderNumber, trackNumber);
        ModuleDelay();
    }

    void System::IncrementTrack()
    {
        if(fileSystem->GetFolder(folderNumber - 1).GetFileCount() > trackNumber) trackNumber++;
        else IncrementFolder();
        DF_playFolder(folderNumber, trackNumber);
        ModuleDelay();
    }

    void System::DecrementTrack()
    {
        trackNumber--;
        if(trackNumber < 1) DecrementFolder();
        DF_playFolder(folderNumber, trackNumber);
        ModuleDelay();
    }

    void System::playTrack()
    {
        DF_start();
        paused = false;
        ModuleDelay();
    }
    
    void System::pauseTrack()
    {
        DF_pause();
        paused = true;
        ModuleDelay();
    }

    void System::IncrementVolume()
    {
        volume++;
        if(volume > 30) volume = 30;
        DF_volume(volume);
        ModuleDelay();
    }

    void System::DecrementVolume()
    {
        volume--;
        if(volume > 30) volume = 0; //Volume is unsigned (underflow!)
        DF_volume(volume);
        ModuleDelay();
    }

    void System::DisplayFolder()
    {
        char text[17] = "";
        sprintf(text, "FOLDER: %02d", folderNumber);
        lcd->Write(text, 1);
        lcd->SetIcons(PlayerLCD::CHAR_FOLDER_DEC, ' ', PlayerLCD::CHAR_FOLDER_INC, ' ');
    }

    void System::DisplayTrack()
    {
        char text[17] = "";
        sprintf(text, "TRACK: %03d", trackNumber);
        lcd->Write(text, 1);
        lcd->SetIcons(PlayerLCD::CHAR_TRACK_DEC, ' ', PlayerLCD::CHAR_TRACK_INC, ' ');
    }

    void System::DisplayVolume()
    {
        char text[17] = "";
        sprintf(text, "VOLUME: %02d", volume);
        lcd->Write(text, 1);
        lcd->SetIcons(PlayerLCD::CHAR_VOLUME_DEC, ' ', PlayerLCD::CHAR_VOLUME_INC, ' ');
    }

    void System::DisplayPlayStatus()
    {
        if(paused)
        {
            lcd->Write("PAUSED ", 1);
        }
        else
        {
            lcd->Write("PLAYING", 1);
        }
        lcd->SetIcons(PlayerLCD::CHAR_PLAY, ' ', PlayerLCD::CHAR_PAUSE, ' ');
    }

    void System::ResetTrackInfo()
    {
        trackInfoIndex = TRACK_INFO_LIST_SIZE - 1;
        changeTrackInfo = true;
        xTimerReset(infoTimerHandle, 0);
    }
}