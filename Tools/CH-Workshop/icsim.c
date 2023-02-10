/*
 * Instrument cluster simulator
 *
 * (c) 2014 Open Garages - Craig Smith <craig@theialabs.com>
 *
 *
 * Forked version :
 *  - new GUI
 *  -
 *
 */

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <getopt.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "lib.h"
#include "data.h"

#ifndef DATA_DIR
#define DATA_DIR "./data/"  // Needs trailing slash
#endif

// ICSim constants
#define SCREEN_WIDTH 862
#define SCREEN_HEIGHT 669


// ICSim globals
int can; // socket
struct canfd_frame cf;
const int canfd_on = 1;
int debug = 0;
int randomize = 0;
int seed = 0;
int currentTime;

int doorPos = DEFAULT_DOOR_POS;
int signalPos = DEFAULT_SIGNAL_POS;
int speedPos = DEFAULT_SPEED_POS;
int warningPos = DEFAULT_WARNING_POS;
int luminosityPos = DEFAULT_LUMINOSITY_POS;
int lightPos = DEFAULT_LIGHT_POS;

long currentSpeed = 0;
int doorStatus[4];
int turnStatus[2];
int luminosityLevel = 0;
char lightStatus = 0;
char *model = NULL;
char dataFile[256];

char warningState = 0;
char diagActive = 0;

char diagSession = 1;
int lastDiagTesterPresent = 0;
int diagSeed[2];
int sessionKey[2] = {0x35, 0x30}; // Based on the 2 last character of the VIN, ie 50
char seedGenerated = 0;
char secretSessionFound = 0;

int shareSeed = -1;
char controlLightOn = 0;
char controlIsNight = 0;
char controlWarningActive = 0;
char controlDiagOn = 0;
char controlDiagActive = 0;
char controlTurnValue = 0;
char controlLuminosity = 0;
char controlCurrentSpeed = 0;

char pristine = 1;
int lastUpdate = 0;
char isoTpRequest = 0;
int isoTpRemainingBytes = 0;
int isoTpFirstFlowTime = 0;

char score = 0;

SDL_Renderer *renderer = NULL;
SDL_Texture *baseTexture = NULL;
SDL_Texture *needleTex = NULL;
SDL_Texture *spriteTex = NULL;
SDL_Texture *spriteAltTex = NULL;
SDL_Texture *scoreboardTex = NULL;

int running = 0;

FILE *fptr;

/* Prototypes */
void print_pkt(struct canfd_frame);
void print_bin(unsigned char *, int);

// Simple map function
long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Adds data dir to file name
// Uses a single pointer so not to have a memory leak
// returns point to dataFiles or NULL if append is too large
char *getData(char *fname) {
  if(strlen(DATA_DIR) + strlen(fname) > 255) return NULL;
  strncpy(dataFile, DATA_DIR, 255);
  strncat(dataFile, fname, 255-strlen(dataFile));
  return dataFile;
}

void sendPkt(int mtu) {
  if(write(can, &cf, mtu) != mtu) {
	  perror("write");
  }
}
/* Default vehicle state */
void initCarState() {
  doorStatus[0] = DOOR_LOCKED;
  doorStatus[1] = DOOR_LOCKED;
  doorStatus[2] = DOOR_LOCKED;
  doorStatus[3] = DOOR_LOCKED;
  turnStatus[0] = OFF;
  turnStatus[1] = OFF;
  diagActive = 0;
  diagSession = 1;
  seedGenerated = 0;
  secretSessionFound = 0;
  warningState = 0;
}

/* Empty IC */
void blankIC() {
  SDL_RenderCopy(renderer, baseTexture, NULL, NULL);
}

void validateChallenge(int challenge) {
  if (challenges[challenge] == 0) {
    challenges[challenge] = 1;
    score += challengeValue[challenge];
  }
}

void updateScore(int chall) {
  if (chall >= sizeof(challenges) || chall < 0) {
    printf("Error : challenge ID is out of range !");
    exit(42);
  }

  if (challenges[chall] == 0) {
    score += challengeValue[chall];
    challenges[chall] = 1;
    if (score > 100) score = 100;
  }
}

/* Draw functions */
void drawScore() {
  SDL_Rect scoreRect, scoreSrc;
  scoreSrc.x = 0;
  scoreSrc.y = (score/5) * 40;
  scoreSrc.h = 40;
  scoreSrc.w = 68;

  scoreRect.x = 50;
  scoreRect.y = 389;
  scoreRect.h = 40;
  scoreRect.w = 68;

  SDL_RenderCopy(renderer, scoreboardTex, &scoreSrc, &scoreRect);
}

void drawSpeed() {
  SDL_Rect dialRect;
  SDL_Point center;
  double angle = 0;
  dialRect.x = 281;
  dialRect.y = 405;
  dialRect.h = 250;
  dialRect.w = 250;

  if (diagActive == 2 || secretSessionFound == 1) {
    if (currentTime % 1000 > 500)
      SDL_RenderCopy(renderer, baseTexture, &dialRect, &dialRect);
    else
      SDL_RenderCopy(renderer, spriteAltTex, &dialRect, &dialRect);
  } else
   SDL_RenderCopy(renderer, baseTexture, &dialRect, &dialRect);

  center.x = 125;
  center.y = 125;
  angle = map(currentSpeed, 0, 230, -50, 200);
  if(angle > 200) angle = 200;
  SDL_RenderCopyEx(renderer, needleTex, NULL, &dialRect, angle, &center, SDL_FLIP_NONE);
}

void drawDiag() {
    SDL_Rect diagScreen, diagStatus, diagFeedback;
    diagScreen.x = 610;
    diagScreen.y = 551;
    diagScreen.w = 162;
    diagScreen.h = 102;

    diagStatus.x = 51;
    diagStatus.y = 461;
    diagStatus.w = 161;
    diagStatus.h = 19;

    diagFeedback.x = 33;
    diagFeedback.y = 483;
    diagFeedback.w = 204;
    diagFeedback.h = 152;

    if (controlDiagOn == 0) {
      SDL_RenderCopy(renderer, baseTexture, &diagScreen, &diagScreen);
    } else {
      if (controlDiagActive == 0) {
        SDL_RenderCopy(renderer, spriteTex, &diagScreen, &diagScreen);
      } else {
        SDL_RenderCopy(renderer, spriteAltTex, &diagScreen, &diagScreen);
      }
    }

    if (diagSession == 2 || diagSession == 3)
      SDL_RenderCopy(renderer, spriteTex, &diagStatus, &diagStatus);
    else
      SDL_RenderCopy(renderer, baseTexture, &diagStatus, &diagStatus);

    if (diagActive == 2 || secretSessionFound == 1) {
      SDL_RenderCopy(renderer, spriteAltTex, &diagFeedback, &diagFeedback);

    } else if (diagActive == 1)
      SDL_RenderCopy(renderer, spriteTex, &diagFeedback, &diagFeedback);
    else
      SDL_RenderCopy(renderer, baseTexture, &diagFeedback, &diagFeedback);
}

void drawRoadAndLights() {
  SDL_Rect sky, road, light, autoIndicator, lightDebug;
  sky.x = 0;
  sky.y = 0;
  sky.w = 862;
  sky.h = 162;

  road.x = 0;
  road.y = 162;
  road.w = 862;
  road.h = 160;

  light.x = 171;
  light.y = 382;
  light.w = 40;
  light.h = 24;

  autoIndicator.x = 160;
  autoIndicator.y = 410;
  autoIndicator.w = 57;
  autoIndicator.h = 17;

  lightDebug.x = 685;
  lightDebug.y = 396;
  lightDebug.w = 61;
  lightDebug.h = 35;

  if (controlIsNight == 0) {
    SDL_RenderCopy(renderer, baseTexture, &sky, &sky);
    SDL_RenderCopy(renderer, baseTexture, &road, &road);
  } else {
    SDL_RenderCopy(renderer, spriteTex, &sky, &sky);
    if (controlLightOn == 1 || luminosityLevel < LIGHT_LEVEL) {
      SDL_RenderCopy(renderer, spriteAltTex, &road, &road);
    } else {
      SDL_RenderCopy(renderer, spriteTex, &road, &road);
    }
  }

  if (controlLightOn == 1 ||  luminosityLevel < LIGHT_LEVEL) {
    SDL_RenderCopy(renderer, spriteTex, &light, &light);
    SDL_RenderCopy(renderer, spriteAltTex, &lightDebug, &lightDebug);
    if (luminosityLevel < LIGHT_LEVEL)
      SDL_RenderCopy(renderer, spriteTex, &autoIndicator, &autoIndicator);
    else
      SDL_RenderCopy(renderer, baseTexture, &autoIndicator, &autoIndicator);
  } else {
    SDL_RenderCopy(renderer, baseTexture, &lightDebug, &lightDebug);
    SDL_RenderCopy(renderer, baseTexture, &light, &light);
    SDL_RenderCopy(renderer, baseTexture, &autoIndicator, &autoIndicator);
  }
}

/* Updates door unlocks simulated by door open icons */
void drawDoors() {
  SDL_Rect door_area, update, pos;
  door_area.x = 674;
  door_area.y = 432;
  door_area.w = 81;
  door_area.h = 78;
  SDL_RenderCopy(renderer, baseTexture, &door_area, &door_area);
  // No update if all doors are locked
  if(doorStatus[0] == DOOR_LOCKED && doorStatus[1] == DOOR_LOCKED &&
     doorStatus[2] == DOOR_LOCKED && doorStatus[3] == DOOR_LOCKED) return;
  // Make the base body red if even one door is unlocked
  update.x = 693;
  update.y = 432;
  update.w = 43;
  update.h = 78;
  SDL_RenderCopy(renderer, spriteTex, &update, &update);
  if(doorStatus[0] == DOOR_UNLOCKED) {
    update.x = 678;
    update.y = 456;
    update.w = 18;
    update.h = 17;
    SDL_RenderCopy(renderer, spriteTex, &update, &update);
  }
  if(doorStatus[1] == DOOR_UNLOCKED) {
    update.x = 738;
    update.y = 456;
    update.w = 18;
    update.h = 18;
    SDL_RenderCopy(renderer, spriteTex, &update, &update);
  }
  if(doorStatus[2] == DOOR_UNLOCKED) {
    update.x = 678;
    update.y = 481;
    update.w = 18;
    update.h = 18;
    SDL_RenderCopy(renderer, spriteTex, &update, &update);
  }
  if(doorStatus[3] == DOOR_UNLOCKED) {
    update.x = 738;
    update.y = 481;
    update.w = 18;
    update.h = 18;
    SDL_RenderCopy(renderer, spriteTex, &update, &update);
  }
  SDL_RenderPresent(renderer);
}

/* Updates turn signals */
void drawTurnSignals() {
  SDL_Rect left, right, leftTU, leftTD, rightTU, rightTD, warning;
  left.x = 242;
  left.y = 378;
  left.w = 50;
  left.h = 31;

  right.x = 528;
  right.y = 378;
  right.w = 50;
  right.h = 31;

  leftTU.x = 678;
  leftTU.y = 422;
  leftTU.w = 16;
  leftTU.h = 16;

  leftTD.x = 678;
  leftTD.y = 506;
  leftTD.w = 16;
  leftTD.h = 16;

  rightTU.x = 734;
  rightTU.y = 422;
  rightTU.w = 16;
  rightTU.h = 16;

  rightTD.x = 739;
  rightTD.y = 506;
  rightTD.w = 16;
  rightTD.h = 16;

  warning.x = 545;
  warning.y = 418;
  warning.h = 39;
  warning.w = 52;

  if (diagActive == 2) {
    if (currentTime % 1000 >= 500) {
      turnStatus[0] = OFF;
      turnStatus[1] = OFF;
      controlTurnValue == 0;
    } else {
      turnStatus[0] = ON;
      turnStatus[1] = ON;
      controlTurnValue == 3;
    }
  }

  if (turnStatus[0] == OFF) {
	  SDL_RenderCopy(renderer, baseTexture, &left, &left);
  } else {
	  SDL_RenderCopy(renderer, spriteTex, &left, &left);
  }

  if(turnStatus[1] == OFF) {
	  SDL_RenderCopy(renderer, baseTexture, &right, &right);
  } else {
	  SDL_RenderCopy(renderer, spriteTex, &right, &right);
  }

  if (controlTurnValue == 1 || controlTurnValue == 3) {
    SDL_RenderCopy(renderer, spriteTex, &leftTU, &leftTU);
    SDL_RenderCopy(renderer, spriteTex, &leftTD, &leftTD);
  }
  else {
    SDL_RenderCopy(renderer, baseTexture, &leftTU, &leftTU);
    SDL_RenderCopy(renderer, baseTexture, &leftTD, &leftTD);
  }

  if (controlTurnValue == 2 || controlTurnValue == 3) {
    SDL_RenderCopy(renderer, spriteTex, &rightTU, &rightTU);
    SDL_RenderCopy(renderer, spriteTex, &rightTD, &rightTD);
  }
  else {
    SDL_RenderCopy(renderer, baseTexture, &rightTU, &rightTU);
    SDL_RenderCopy(renderer, baseTexture, &rightTD, &rightTD);
  }

  if (warningState == 1)
    SDL_RenderCopy(renderer, spriteTex, &warning, &warning);
  else
    SDL_RenderCopy(renderer, baseTexture, &warning, &warning);
}

/* Redraws the IC updating everything
 * Slowest way to go.  Should only use on init
 */
void updateIC() {
  drawDoors();
  drawRoadAndLights();
  drawTurnSignals();
  drawDiag();
  drawSpeed();
  drawScore();
  SDL_RenderPresent(renderer);
}

void redrawIC() {
  blankIC();
  updateIC();
}

/* Parse can frames and update variables */
void updateSharedData(struct canfd_frame *cf, int maxdlen) {
  int seed = (cf->data[5] << 8) + cf->data[6];

  if (shareSeed == -1 || seed == ((shareSeed +1)%65536)) {
    char key1 = cf->data[5];
    char key2 = cf->data[6];

    for (char i =0; i <= 4; i++) {
      cf->data[i] ^= key2;
    }
    int crc = cf->data[3];
    int check = cf->data[4];

    if (check == crc ^ key1 ) {

      if (((cf->data[0] + cf->data[1] + cf->data[2])%256) == crc) {
        shareSeed = seed;

        controlLightOn = cf->data[0] >> 6;
        controlIsNight = (cf->data[0] >> 5) & 0x1;
        controlWarningActive = (cf->data[0] >> 4) & 0x1;
        controlDiagOn = (cf->data[0] >> 3) & 0x1;
        controlDiagActive = (cf->data[0] >> 2) & 0x1;
        controlTurnValue = cf->data[0] & 0x3;
        controlLuminosity = cf-> data[1];
        controlCurrentSpeed = cf-> data[2];
      }
    }
  }
}

void updateLuminosityStatus(struct canfd_frame *cf, int maxdlen) {
  int len = (cf->len > maxdlen) ? maxdlen : cf->len;
  if (len < luminosityPos + 1) return;
  pristine = 0;
  luminosityLevel = cf->data[luminosityPos];
  // CHALLENGE CHECK : cut light by night
  if (controlIsNight == 1 && luminosityLevel > LIGHT_LEVEL)
    validateChallenge(CHALLENGE_SPOOF_LIGHT);
}

void drawSpeedStatus(struct canfd_frame *cf, int maxdlen) {
  int len = (cf->len > maxdlen) ? maxdlen : cf->len;
  if(len < speedPos + 1) return;
  pristine = 0;

  int speed = cf->data[speedPos] << 8;
	speed += cf->data[speedPos + 1];
	speed = speed / 100; // speed in kilometers
  currentSpeed = speed;

  // CHALLENGE CHECK :  spoof speed on IC
  if (currentSpeed >= MAX_SPEED)
    validateChallenge(CHALLENGE_SPOOF_SPEED);
}

void updateWarningStatus(struct canfd_frame *cf, int maxdlen) {
  int len = (cf->len > maxdlen) ? maxdlen : cf->len;
  if (len < warningPos + 1) return;
  pristine = 0;
  warningState = cf->data[warningPos] & 0x01;
}

void updateLightStatus(struct canfd_frame *cf, int maxdlen) {
  int len = (cf->len > maxdlen) ? maxdlen : cf->len;
  if (len < lightPos + 1) return;
  pristine = 0;
  lightStatus = cf->data[lightPos] & 0x01;
}

/* Parses CAN frame and updates turn signal status */
void updateSignalStatus(struct canfd_frame *cf, int maxdlen) {
  int len = (cf->len > maxdlen) ? maxdlen : cf->len;
  if(len < signalPos) return;
  pristine = 0;
  if (cf->data[signalPos] & CAN_LEFT_SIGNAL) {
    turnStatus[0] = ON;
  } else {
    turnStatus[0] = OFF;
  }
  if(cf->data[signalPos] & CAN_RIGHT_SIGNAL) {
    turnStatus[1] = ON;
  } else {
    turnStatus[1] = OFF;
  }
  // CHALLENGE CHECK :  spoof turn signals on IC
  if ((turnStatus[1] == ON || turnStatus[0] == ON) && (controlWarningActive ==0 && controlTurnValue == 0))
    validateChallenge(CHALLENGE_TURN_SIGNALS);
}

/* Parses CAN frame and updates door status */
void updateDoorStatus(struct canfd_frame *cf, int maxdlen) {
  int len = (cf->len > maxdlen) ? maxdlen : cf->len;
  if(len < doorPos) return;
  pristine = 0;
  if(cf->data[doorPos] & CAN_DOOR1_LOCK) {
	  doorStatus[0] = DOOR_LOCKED;
  } else {
	  doorStatus[0] = DOOR_UNLOCKED;
  }
  if(cf->data[doorPos] & CAN_DOOR2_LOCK) {
	  doorStatus[1] = DOOR_LOCKED;
  } else {
	  doorStatus[1] = DOOR_UNLOCKED;
  }
  if(cf->data[doorPos] & CAN_DOOR3_LOCK) {
	  doorStatus[2] = DOOR_LOCKED;
  } else {
	  doorStatus[2] = DOOR_UNLOCKED;
  }
  if(cf->data[doorPos] & CAN_DOOR4_LOCK) {
	  doorStatus[3] = DOOR_LOCKED;
  } else {
	  doorStatus[3] = DOOR_UNLOCKED;
  }
}

void sendFrameError(int func, int errorCode) {
  memset(&cf, 0, sizeof(cf));
  cf.can_id = 0x7E8;
  cf.len = 4;
  cf.data[0] = 0x03;
  cf.data[1] = 0x7F;
  cf.data[2] = func;
  cf.data[3] = errorCode;
  sendPkt(CAN_MTU);
}

void sendFrameFeedback(int *data, int frameLen) {
  memset(&cf, 0, sizeof(cf));
  cf.can_id = 0x7E8;
  cf.len = frameLen;
  for (char i = 0; i < frameLen; i++) {
    cf.data[i] = data[i];
  }
  sendPkt(CAN_MTU);
}

void sendIsoTpData() {
  int frameFeedback[8];
  char i = 1;
  frameFeedback[0] = 0x20 + ((isoTpRequest - 1)%16);

  for (i; i < 8 && isoTpRemainingBytes > 0; i++) {
    frameFeedback[i] = VIN[sizeof(VIN) - isoTpRemainingBytes];
    isoTpRemainingBytes --;
  }

  if (isoTpRemainingBytes <= 0) {
    // CHALLENGE CHECK :  VIN request
    validateChallenge(CHALLENGE_REQUEST_VIN);
    isoTpRequest = 0;
  } else
    isoTpRequest += 1;
  sendFrameFeedback(frameFeedback, i);
}

void analyseDiagRequest(struct canfd_frame *frame, int maxdlen) {
  int frameFeedback[8];
  int len = (frame->len > maxdlen) ? maxdlen : frame->len;

  if (frame->data[0] < 0x09 && frame->data[0] > 0) {
    int subf = frame->data[2];

    switch (frame->data[1]) {
      case PID_INFO:
        if (frame->data[0] != 2) {
          sendFrameError(PID_INFO, UDS_ERROR_INCORRECT_LENGTH);
          return;
        }
        if (frame->data[2] == PID_INFO_VEHICLE_SPEED) {
          frameFeedback[0] = 0x03;
          frameFeedback[1] = PID_INFO + 0x40;
          frameFeedback[2] = PID_INFO_VEHICLE_SPEED;
          frameFeedback[3] = controlCurrentSpeed;
          sendFrameFeedback(frameFeedback, 4);
        }
        else
          sendFrameError(PID_INFO, UDS_ERROR_SUBFUNC_NOT_SUPPORTED);
        break;
      case PID_VEHICLE_INFO:
        if (frame->data[0] != 2) {
          sendFrameError(PID_INFO, UDS_ERROR_INCORRECT_LENGTH);
          return;
        }
        switch (frame->data[2]) {
          case PID_VEHICLE_ECU_NAME:
            frameFeedback[0] = 0x03;
            frameFeedback[1] = PID_VEHICLE_INFO + 0x40;
            frameFeedback[2] = PID_VEHICLE_ECU_NAME;
            frameFeedback[3] = 69;
            frameFeedback[4] = 67;
            frameFeedback[5] = 85;
            sendFrameFeedback(frameFeedback, 6);
            break;

          case PID_VEHICLE_VIN:
            isoTpRemainingBytes = sizeof(VIN);

            if (isoTpRemainingBytes > 6) {
              frameFeedback[0] = 0x10 + ((isoTpRemainingBytes & 0x0F00) >> 8);
              frameFeedback[1] = isoTpRemainingBytes & 0xFF;
              isoTpFirstFlowTime = currentTime;
            }
            else {
              frameFeedback[0] = isoTpRemainingBytes + 1;
              frameFeedback[1] = PID_VEHICLE_INFO + 0x40;
            }
            char i = 2;
            for (i; i < 8 && isoTpRemainingBytes > 0; i++) {
              frameFeedback[i] = VIN[sizeof(VIN) - isoTpRemainingBytes];
              isoTpRemainingBytes --;
            }

          sendFrameFeedback(frameFeedback, i);

            break;
          default:
            sendFrameError(PID_INFO, UDS_ERROR_SUBFUNC_NOT_SUPPORTED);
            break;
        }
        break;

      case UDS_SID_TESTER_PRESENT:
        if (frame->data[0] == 0x02) {
          lastDiagTesterPresent = currentTime;
          frameFeedback[0] = 0x02;
          frameFeedback[1] = UDS_SID_TESTER_PRESENT + 0x40;
          frameFeedback[2] = subf;
          sendFrameFeedback(frameFeedback, 3);
        } else {
          sendFrameError(UDS_SID_TESTER_PRESENT, UDS_ERROR_INCORRECT_LENGTH);
        }
        break;

        case UDS_SID_DIAGNOSTIC_CONTROL:
          if (frame->data[0] == 0x02) {
            switch (frame->data[2]) {
              case 0x01:
                diagSession = 1;
                diagActive = 0;
                secretSessionFound = 0;
                lastDiagTesterPresent = currentTime;
                frameFeedback[0] = 0x02;
                frameFeedback[1] = UDS_SID_DIAGNOSTIC_CONTROL + 0x40;
                frameFeedback[2] = subf;
                sendFrameFeedback(frameFeedback, 3);
                break;
              case 0x02:
                diagSession = 2;
                lastDiagTesterPresent = currentTime;
                secretSessionFound = 0;
                frameFeedback[0] = 0x02;
                frameFeedback[1] = UDS_SID_DIAGNOSTIC_CONTROL + 0x40;
                frameFeedback[2] = subf;
                sendFrameFeedback(frameFeedback, 3);
                break;
              case 0x03:
                diagSession = 3;
                lastDiagTesterPresent = currentTime;
                secretSessionFound = 0;
                frameFeedback[0] = 0x02;
                frameFeedback[1] = UDS_SID_DIAGNOSTIC_CONTROL + 0x40;
                frameFeedback[2] = subf;
                sendFrameFeedback(frameFeedback, 3);
                break;
              default:
                sendFrameError(UDS_SID_DIAGNOSTIC_CONTROL, UDS_ERROR_SUBFUNC_NOT_SUPPORTED);
                return;
                break;

              frameFeedback[0] = 0x02;
              frameFeedback[1] = UDS_SID_DIAGNOSTIC_CONTROL + 0x40;
              frameFeedback[2] = subf;
              sendFrameFeedback(frameFeedback, 3);
            }
          } else {
            sendFrameError(UDS_SID_DIAGNOSTIC_CONTROL, UDS_ERROR_INCORRECT_LENGTH);
          }
          break;

        case UDS_SID_ECU_RESET:
          if (frame->data[0] == 2) {
            if (frame->data[2] > 0 && frame->data[2] <= 3) {
              diagSession = 1;
              seedGenerated = 0;
              diagActive = 0;
              secretSessionFound = 0;
              frameFeedback[0] = 0x02;
              frameFeedback[1] = UDS_SID_ECU_RESET + 0x40;
              frameFeedback[2] = subf;
              sendFrameFeedback(frameFeedback, 3);
            }
            else
              sendFrameError(UDS_SID_ECU_RESET, UDS_ERROR_SUBFUNC_NOT_SUPPORTED);
          } else
            sendFrameError(UDS_SID_ECU_RESET, UDS_ERROR_INCORRECT_LENGTH);
          break;

        case UDS_SID_ROUTINE_CONTROL:
          if (frame->data[0] == 4) {
            if (diagSession == 2) {
              if (frame->data[2] == 0x41) {
                if (frame->data[3] == 0x10 || frame->data[3] == 0x22) {
                  if (frame->data[4] == 0x00 || frame->data[4] == 0x01) {
                    switch (frame->data[3]) {
                      case 0x10:
                        diagActive = frame->data[4];
                        break;
                      case 0x22:
                        diagActive = frame->data[4] * 2;
                        validateChallenge(CHALLENGE_FIND_ROUTINE_CONTROL);
                        break;
                    }
                    frameFeedback[0] = 0x04;
                    frameFeedback[1] = UDS_SID_ROUTINE_CONTROL + 0x40;
                    frameFeedback[2] = subf;
                    frameFeedback[3] = frame->data[3];
                    frameFeedback[4] = frame->data[4];
                    sendFrameFeedback(frameFeedback, 5);
                  } else
                      sendFrameError(UDS_SID_ROUTINE_CONTROL, UDS_ERROR_REQUEST_OUT_RANGE);
                } else
                    sendFrameError(UDS_SID_ROUTINE_CONTROL, UDS_ERROR_REQUEST_OUT_RANGE);
              } else
                  sendFrameError(UDS_SID_ROUTINE_CONTROL, UDS_ERROR_SUBFUNC_NOT_SUPPORTED);
            } else
                sendFrameError(UDS_SID_ROUTINE_CONTROL, UDS_ERROR_FUNC_INCORRECT_SESSION);
          } else
              sendFrameError(UDS_SID_ROUTINE_CONTROL, UDS_ERROR_INCORRECT_LENGTH);
          break;

        case UDS_SID_SECURITY_ACCESS:
          if (diagSession != 0x03) {
            sendFrameError(UDS_SID_SECURITY_ACCESS, UDS_ERROR_FUNC_INCORRECT_SESSION);
            return;
          }

          switch (frame->data[2]) {
            case 0x01:
              if (frame->data[0] != 2) {
                sendFrameError(UDS_SID_SECURITY_ACCESS, UDS_ERROR_INCORRECT_LENGTH);
                return;
              }
              frameFeedback[0] = 0x04;
              frameFeedback[1] = UDS_SID_SECURITY_ACCESS + 0x40;
              frameFeedback[2] = subf;
              diagSeed[0] = rand()%255;
              diagSeed[1] = rand()%255;
              frameFeedback[3] = diagSeed[0];
              frameFeedback[4] = diagSeed[1];
              seedGenerated = 1;
              sendFrameFeedback(frameFeedback, 5);
              break;
            case 0x02:
              if (frame->data[0] != 4) {
                sendFrameError(UDS_SID_SECURITY_ACCESS, UDS_ERROR_INCORRECT_LENGTH);
                return;
              }
              if (seedGenerated == 0)
                sendFrameError(UDS_SID_SECURITY_ACCESS, UDS_ERROR_SEQUENCE_ERROR);

              else {
                if (frame->data[3] == (diagSeed[0] ^ sessionKey[0]) && frame->data[4] == (diagSeed[1] ^ sessionKey[1])) {
                  diagSession = 0x02;
                  frameFeedback[0] = 0x02;
                  frameFeedback[1] = UDS_SID_SECURITY_ACCESS + 0x40;
                  frameFeedback[2] = subf;
                  seedGenerated = 0;
                  secretSessionFound = 1;
                  validateChallenge(CHALLENGE_SECURITY_ACCESS);
                  sendFrameFeedback(frameFeedback, 3);
                } else {
                  sendFrameError(UDS_SID_SECURITY_ACCESS, UDS_ERROR_INVALID_KEY);
                }
              }
              break;
            default:
              sendFrameError(UDS_SID_SECURITY_ACCESS, UDS_ERROR_SUBFUNC_NOT_SUPPORTED);
              return;
              break;
          }
          break;
        default:
          sendFrameError(frame->data[1], UDS_ERROR_SERVICE_NOT_SUPPORTED);
          break;
    }
  }
  else if ((frame->data[0] & 0xF0) == 0x30 && isoTpRemainingBytes > 0) {
    if ((isoTpFirstFlowTime + ISOTP_TIMEOUT) >= currentTime)
      isoTpRequest = 1;
    else
      isoTpRemainingBytes;
  }
}

void Usage(char *msg) {
  if(msg) printf("%s\n", msg);
  printf("Usage: icsim [options] <can>\n");
  printf("\t-r\trandomize IDs\n");
  printf("\t-d\tdebug mode\n");
  exit(1);
}

int main(int argc, char *argv[]) {
  int opt;
  struct ifreq ifr;
  struct sockaddr_can addr;
  struct canfd_frame frame;
  struct iovec iov;
  struct msghdr msg;
  struct cmsghdr *cmsg;
  struct timeval tv, timeoutConfig = { 0, 0 };
  struct stat dirstat;
  fd_set rdfs;
  char ctrlmsg[CMSG_SPACE(sizeof(struct timeval)) + CMSG_SPACE(sizeof(__u32))];
  int running = 1;
  int nbytes, maxdlen;
  int ret;
  int seed = 0;
  int doorId, signalId, speedId, warningId, diagId, lightId, luminosityId, controlId;
  SDL_Event event;

  while ((opt = getopt(argc, argv, "r:d:h?")) != -1) {
    switch(opt) {
	case 'r':
		randomize = 1;
		break;
	case 'd':
		debug = 1;
		break;
	case 'h':
	case '?':
	default:
		Usage(NULL);
		break;
    }
  }

  if (optind >= argc) Usage("You must specify at least one can device");

  // Verify data directory exists
  if(stat(DATA_DIR, &dirstat) == -1) {
  	printf("ERROR: DATA_DIR not found.  Define in make file or run in src dir\n");
	exit(34);
  }

  // Create a new raw CAN socket
  can = socket(PF_CAN, SOCK_RAW, CAN_RAW);
  if (can < 0) Usage("Couldn't create raw socket");

  addr.can_family = AF_CAN;
  memset(&ifr.ifr_name, 0, sizeof(ifr.ifr_name));
  strncpy(ifr.ifr_name, argv[optind], strlen(argv[optind]));
  printf("Using CAN interface %s\n", ifr.ifr_name);
  if (ioctl(can, SIOCGIFINDEX, &ifr) < 0) {
    perror("SIOCGIFINDEX");
    exit(1);
  }
  addr.can_ifindex = ifr.ifr_ifindex;

  // CAN FD Mode
  setsockopt(can, SOL_CAN_RAW, CAN_RAW_FD_FRAMES, &canfd_on, sizeof(canfd_on));

  iov.iov_base = &frame;
  iov.iov_len = sizeof(frame);
  msg.msg_name = &addr;
  msg.msg_namelen = sizeof(addr);
  msg.msg_iov = &iov;
  msg.msg_iovlen = 1;
  msg.msg_control = &ctrlmsg;
  msg.msg_controllen = sizeof(ctrlmsg);
  msg.msg_flags = 0;

  if (bind(can, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
	  perror("bind");
	  return 1;
  }

  initCarState();

  doorId = DEFAULT_DOOR_ID;
  signalId = DEFAULT_SIGNAL_ID;
  speedId = DEFAULT_SPEED_ID;
  warningId = DEFAULT_WARNING_ID;
  diagId = DEFAULT_ECU_ID;
  lightId = DEFAULT_LIGHT_ID;
  luminosityId = DEFAULT_LUMINOSITY_ID;
  controlId = DEFAULT_CONTROL_ID;

  if (randomize) {
  	if(randomize) seed = time(NULL);
  	srand(seed);
  	doorId = (rand() % 2046) + 1;
  	signalId = (rand() % 2046) + 1;
  	speedId = (rand() % 2046) + 1;
  	doorPos = rand() % 9;
  	signalPos = rand() % 9;
  	speedPos = rand() % 8;
  	printf("Seed: %d\n", seed);
  	FILE *fdseed = fopen("/tmp/icsim_seed.txt", "w");
  	fprintf(fdseed, "%d\n", seed);
  	fclose(fdseed);
  }

  SDL_Window *window = NULL;
  SDL_Surface *screenSurface = NULL;
  if(SDL_Init ( SDL_INIT_VIDEO ) < 0 ) {
	  printf("SDL Could not initializes\n");
	  exit(40);
  }

  window = SDL_CreateWindow("IC Simulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN); // | SDL_WINDOW_RESIZABLE);
  if(window == NULL) {
	  printf("Window could not be shown\n");
  }

  renderer = SDL_CreateRenderer(window, -1, 0);

  SDL_Surface *image = IMG_Load(getData("dashboard.png"));
  SDL_Surface *needle = IMG_Load(getData("needle.png"));
  SDL_Surface *sprites = IMG_Load(getData("spritesheet.png"));
  SDL_Surface *spritesAlt = IMG_Load(getData("spritesheet-alt.png"));
  SDL_Surface *scoreboard = IMG_Load(getData("scoreboard.png"));
  baseTexture = SDL_CreateTextureFromSurface(renderer, image);
  needleTex = SDL_CreateTextureFromSurface(renderer, needle);
  spriteTex = SDL_CreateTextureFromSurface(renderer, sprites);
  spriteAltTex = SDL_CreateTextureFromSurface(renderer, spritesAlt);
  scoreboardTex = SDL_CreateTextureFromSurface(renderer, scoreboard);

  // Draw the IC
  redrawIC();

  /* For now we will just operate on one CAN interface */
  while(running) {
    while( SDL_PollEvent(&event) != 0 ) {
	    switch(event.type) {
	      case SDL_QUIT:
		      running = 0;
		      break;
	      case SDL_WINDOWEVENT:
	        switch(event.window.event) {
		        case SDL_WINDOWEVENT_ENTER:
            case SDL_WINDOWEVENT_RESIZED:
			        redrawIC();
		          break;
	        }
   	   }
       SDL_Delay(3);
      }

      nbytes = recvmsg(can, &msg, 0);
      if (nbytes < 0) {
        perror("read");
        return 1;
      }
      if ((size_t)nbytes == CAN_MTU)
        maxdlen = CAN_MAX_DLEN;
      else if ((size_t)nbytes == CANFD_MTU)
        maxdlen = CANFD_MAX_DLEN;
      else {
        fprintf(stderr, "read: incomplete CAN frame\n");
        return 1;
      }

      for (cmsg = CMSG_FIRSTHDR(&msg);
           cmsg && (cmsg->cmsg_level == SOL_SOCKET);
           cmsg = CMSG_NXTHDR(&msg,cmsg)) {
        if (cmsg->cmsg_type == SO_TIMESTAMP)
            tv = *(struct timeval *)CMSG_DATA(cmsg);
        else if (cmsg->cmsg_type == SO_RXQ_OVFL)
               //dropcnt[i] = *(__u32 *)CMSG_DATA(cmsg);
  	           fprintf(stderr, "Dropped packet\n");
      }

      currentTime = SDL_GetTicks();
      pristine = 1;
      if (frame.can_id == controlId) updateSharedData(&frame, maxdlen);
      if (frame.can_id == doorId) updateDoorStatus(&frame, maxdlen);
      if (frame.can_id == signalId) updateSignalStatus(&frame, maxdlen);
      if (frame.can_id == speedId) drawSpeedStatus(&frame, maxdlen);
      if (frame.can_id == warningId) updateWarningStatus(&frame, maxdlen);
      if (frame.can_id == lightId) updateLightStatus(&frame, maxdlen);
      if (frame.can_id == luminosityId) updateLuminosityStatus(&frame, maxdlen);
      if (frame.can_id == diagId  || frame.can_id == diagId - 1) analyseDiagRequest(&frame, maxdlen);
      if (diagSession > 1 && ((lastDiagTesterPresent + 3500) < currentTime)) {
        diagSession = 1;
        seedGenerated = 0;
        diagActive = 0;
        secretSessionFound = 0;
      }
      if (isoTpRequest > 0 && isoTpRemainingBytes > 0) {
        sendIsoTpData();
      }

      if (pristine == 0) updateIC();
  }

  SDL_DestroyTexture(baseTexture);
  SDL_DestroyTexture(needleTex);
  SDL_DestroyTexture(spriteTex);
  SDL_FreeSurface(image);
  SDL_FreeSurface(needle);
  SDL_FreeSurface(sprites);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  IMG_Quit();
  SDL_Quit();

  return 0;
}
