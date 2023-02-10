/*
 * Control panel for IC Simulation
 *
 * OpenGarages
 *
 * craig@theialabs.com
 *
 * Forked version :
 *  - removed joypad support, all commands are sent through keyboard
 *  - no more randomize ID, for teaching purpose
 *  - added new commands : day/night mode, warning
 *  - add a challenges / points system for more fun
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <signal.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "data.h"


#ifndef DATA_DIR
#define DATA_DIR "./data/"
#endif
#define DEFAULT_CAN_TRAFFIC DATA_DIR "sample-can.log"
#define DEFAULT_DIFFICULTY 1


#define SCREEN_WIDTH 835
#define SCREEN_HEIGHT 402

#define ACCEL_RATE 8.0 // 0-MAX_SPEED in seconds


int s; // socket
struct canfd_frame cf;
char *trafficLog = DEFAULT_CAN_TRAFFIC;
struct ifreq ifr;
int doorPos = DEFAULT_DOOR_POS;
int signalPos = DEFAULT_SIGNAL_POS;
int speedPos = DEFAULT_SPEED_POS;
int warningPos = DEFAULT_WARNING_POS;
int luminosityPos = DEFAULT_LUMINOSITY_POS;
int doorLen = DEFAULT_DOOR_POS + 1;
int signalLen = DEFAULT_SIGNAL_POS + 1;
int speedLen = DEFAULT_SPEED_POS + 2;
int warningLen = DEFAULT_WARNING_POS +3;
int luminosityLen = DEFAULT_LUMINOSITY_POS + 2;


char *model = NULL;

char doorState = 0xf;
char signalState = 0;
char warningState = 0;
int throttle = 0;
float currentSpeed = 0;
int luminosity = 0;

int doorId, signalId, speedId, warningId, luminosityId, ecuId, controlId;
int currentTime;
int lastAccel = 0;
int lastTurnSignal = 0;
int lastWarningSignal = 0;
int lastSharedData = 0;
int lastLuminositySignal = 0;
int lastTesterSignal = 0;

int doorStatus[4] = {DOOR_LOCKED, DOOR_LOCKED, DOOR_LOCKED, DOOR_LOCKED};
char doorButtonPressed[4] = {0,0,0,0};

int turning = 0;
char turnValue = 0;
char turnButtonPressed = 0;
char lightOn = 0;
char lightButtonPressed = 0;
char isNight = 0;
char nightButtonPressed = 0;
char warningActive = 0;
char diagOn = 0;
char diagButtonPressed = 0;
char diagActive = 0;
char routineButtonPressed = 0;
int lastDiagTesterPresent = 0;

int difficulty  = 1;

int shareSeed = 0;

int playId;
char dataFile[256];
SDL_Renderer *renderer = NULL;
SDL_Texture *baseTexture = NULL;

FILE *fptr;

// Adds data dir to file name
// Uses a single pointer so not to have a memory leak
// returns point to dataFiles or NULL if append is too large
char *get_data(char *fname) {
  if(strlen(DATA_DIR) + strlen(fname) > 255) return NULL;
  strncpy(dataFile, DATA_DIR, 255);
  strncat(dataFile, fname, 255-strlen(dataFile));
  return dataFile;
}

void sendPkt(int mtu) {
  if(write(s, &cf, mtu) != mtu) {
	  perror("write");
  }
}

// Randomizes bytes in CAN packet if difficulty is hard enough
void randomizePkt(int start, int stop) {
  if (difficulty < 2) return;
	int i = start;
	for(;i < stop;i++) {
		if(rand() % 3 < 1) cf.data[i] = rand() % 255;
	}
}

void updateSharedData() {
  shareSeed = (shareSeed+1) % 65536;

  memset(&cf, 0, sizeof(cf));
  cf.can_id = controlId;
  cf.len = 7;

  cf.data[0] += (lightOn & 0x1) << 6;
  cf.data[0] += (isNight & 0x1) << 5;
  cf.data[0] += (warningActive & 0x1) << 4;
  cf.data[0] += (diagOn & 0x1) << 3;
  cf.data[0] += (diagActive & 0x1) << 2;
  cf.data[0] += (turnValue & 0x3);

  cf.data[1] = luminosity;
  cf.data[2] = ((char)currentSpeed) %256;
  cf.data[3] = (cf.data[0] + cf.data[1] + cf.data[2])%256; // CRC
  cf.data[4] = ((shareSeed&0xFF00) >> 8) ^ cf.data[3];     // Counter check
  cf.data[5] = (shareSeed & 0xFF00) >> 8;                  // Counter byte 1
  cf.data[6] = shareSeed & 0xFF;                           // Counter byte 2

  for (char i=0; i <= 4; i++) {
    cf.data[i] ^= (shareSeed & 0xFF);
  }

  sendPkt(CAN_MTU);
}

void sendLock(char door) {
	doorState |= door;
  updateSharedData();

	memset(&cf, 0, sizeof(cf));
	cf.can_id = doorId;
	cf.len = doorLen;
	cf.data[doorPos] = doorState;
  if (doorPos) randomizePkt(0, doorPos);
	if (doorLen != doorPos + 1) randomizePkt(doorPos + 1, doorLen);
	sendPkt(CAN_MTU);
}

void sendUnlock(char door) {
	doorState &= ~door;
  updateSharedData();

	memset(&cf, 0, sizeof(cf));
	cf.can_id = doorId;
	cf.len = doorLen;
	cf.data[doorPos] = doorState;
  if (doorPos) randomizePkt(0, doorPos);
	if (doorLen != doorPos + 1) randomizePkt(doorPos + 1, doorLen);
	sendPkt(CAN_MTU);
}

void updateDoorStatus(char doorId, char doorValue) {
  if (doorStatus[doorId] == DOOR_LOCKED) {
    doorStatus[doorId] = DOOR_UNLOCKED;
    sendUnlock(doorValue);
  } else {
    doorStatus[doorId] = DOOR_LOCKED;
    sendLock(doorValue);
  }
}

void sendSpeed() {
	int kph = (currentSpeed / 0.6213751) * 100;
  updateSharedData();

  memset(&cf, 0, sizeof(cf));
	cf.can_id = speedId;
	cf.len = speedLen;
	cf.data[speedPos+1] = (char)kph & 0xff;
	cf.data[speedPos] = (char)(kph >> 8) & 0xff;
	if(kph == 0) { // IDLE
		cf.data[speedPos] = 1;
		cf.data[speedPos+1] = rand() % 255+100;
	}
  if (speedPos) randomizePkt(0, speedPos);
	if (speedLen != speedPos + 2) randomizePkt(speedPos+2, speedLen);
	sendPkt(CAN_MTU);
}

void sendTurnSignal() {
  updateSharedData();

	memset(&cf, 0, sizeof(cf));
	cf.can_id = signalId;
	cf.len = signalLen;
	cf.data[signalPos] = signalState;
  if(signalPos) randomizePkt(0, signalPos);
	if(signalLen != signalPos + 1) randomizePkt(signalPos+1, signalLen);
	sendPkt(CAN_MTU);
}

void sendWarningSignal() {
  updateSharedData();

	memset(&cf, 0, sizeof(cf));
	cf.can_id = warningId;
	cf.len = warningLen;
	cf.data[warningPos] = warningState;
  if(warningPos) randomizePkt(0, warningPos);
	if(warningLen != warningPos + 1) randomizePkt(warningPos+1, warningLen);
	sendPkt(CAN_MTU);
}

void sendLuminositySignal() {
  if (isNight == 1) {
    luminosity = 30 + rand()%10;
  }
  else {
    luminosity  = 194 + rand()%10;
  }

  updateSharedData();

	memset(&cf, 0, sizeof(cf));
	cf.can_id = luminosityId;
	cf.len = luminosityLen;
  cf.data[luminosityPos] = luminosity;
  if (luminosityPos) randomizePkt(0, luminosityPos);
	if (luminosityLen != luminosityPos + 1) randomizePkt(luminosityPos+1, luminosityLen);
	sendPkt(CAN_MTU);
}

// Checks throttle to see if we should accelerate or decelerate the vehicle
void checkAccel() {
	float rate = MAX_SPEED / (ACCEL_RATE * 100);
	// Updated every 10 ms
	if(currentTime > lastAccel + 10) {
		if(throttle < 0) {
			currentSpeed -= rate;
			if(currentSpeed < 1) currentSpeed = 0;
		} else if(throttle > 0) {
			currentSpeed += rate;
			if(currentSpeed > MAX_SPEED) { // Limiter
				currentSpeed = MAX_SPEED;
			}
		}
		sendSpeed();
		lastAccel = currentTime;
	}
}

// Checks if turning and activates the turn signal
void checkTurn() {
	if(currentTime > lastTurnSignal + 500) {
    if (warningState == 1) {
      signalState ^= CAN_WARNING_SIGNAL;
    }
		else if(turning < 0) {
			signalState ^= CAN_LEFT_SIGNAL;
		} else if(turning > 0) {
			signalState ^= CAN_RIGHT_SIGNAL;
		} else {
			signalState = 0;
		}

    turnValue = signalState;

		sendTurnSignal();
		lastTurnSignal = currentTime;
	}
  if (currentTime > lastWarningSignal + 250) {
    sendWarningSignal();
    lastWarningSignal = currentTime;
  }
}

void checkLuminosity() {
  if (currentTime > lastLuminositySignal + 300) {
    sendLuminositySignal();
    lastLuminositySignal = currentTime;
  }
}


// Plays background can traffic
void playCanTraffic() {
	char can2can[50];
	snprintf(can2can, 49, "%s=can0", ifr.ifr_name);
	if (execlp("canplayer", "canplayer", "-I", trafficLog, "-l", "i", can2can, NULL) == -1) printf("WARNING: Could not execute canplayer. No bg data\n");
}

void killChild() {
	kill(playId, SIGINT);
}

void redrawScreen() {
  SDL_RenderCopy(renderer, baseTexture, NULL, NULL);
  SDL_RenderPresent(renderer);
}

void usage(char *msg) {
  if(msg) printf("%s\n", msg);
  printf("Usage: controls [options] <can>\n");
  printf("\t-t\ttraffic file to use for bg CAN traffic\n");
  printf("\t-l\tdifficulty level. 1-2 (default: %d)\n", DEFAULT_DIFFICULTY);
  printf("\t-X\tDisable background CAN traffic.  Cheating if doing RE but needed if playing on a real CANbus\n");
  exit(1);
}


int main(int argc, char *argv[]) {
  int opt;
  struct sockaddr_can addr;
  struct canfd_frame frame;
  int running = 1;
  int seed = 0;
  int enable_canfd = 1;
  int play_traffic = 1;
  struct stat st;
  SDL_Event event;

  while ((opt = getopt(argc, argv, "Xl:t:h?")) != -1) {
    switch(opt) {
	case 't':
		trafficLog = optarg;
		break;
  case 'l':
		difficulty = atoi(optarg);
		break;
	case 'X':
		play_traffic = 0;
		break;
	case 'h':
	case '?':
	default:
		usage(NULL);
		break;
    }
  }

  if (optind >= argc) usage("You must specify at least one can device");

  if(stat(trafficLog, &st) == -1) {
    char msg[256];
    snprintf(msg, 255, "CAN Traffic file not found: %s\n", trafficLog);
    usage(msg);
  }

  /* open socket */
  if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
    perror("socket");
    return 1;
  }

  addr.can_family = AF_CAN;

  strcpy(ifr.ifr_name, argv[optind]);
  if (ioctl(s, SIOCGIFINDEX, &ifr) < 0) {
    perror("SIOCGIFINDEX");
    return 1;
  }
  addr.can_ifindex = ifr.ifr_ifindex;

  if (setsockopt(s, SOL_CAN_RAW, CAN_RAW_FD_FRAMES, &enable_canfd, sizeof(enable_canfd))){
    printf("error when enabling CAN FD support\n");
    return 1;
  }

  if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("bind");
    return 1;
  }

  doorId = DEFAULT_DOOR_ID;
  signalId = DEFAULT_SIGNAL_ID;
  speedId = DEFAULT_SPEED_ID;
  warningId = DEFAULT_WARNING_ID;
  luminosityId = DEFAULT_LUMINOSITY_ID;
  controlId = DEFAULT_CONTROL_ID;

  if (play_traffic) {
	  playId = fork();
	  if ((int)playId == -1) {
		  printf("Error: Couldn't fork bg player\n");
		  exit(-1);
	  } else if (playId == 0) {
		  playCanTraffic();
		  // Shouldn't return
		  exit(0);
	  }
	  atexit(killChild);
  }

  seed = time(NULL);
  srand(seed);
  shareSeed = rand()% 65536;

  // GUI Setup
  SDL_Window *window = NULL;
  SDL_Surface *screenSurface = NULL;
  if(SDL_Init ( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK ) < 0 ) {
    printf("SDL Could not initializes\n");
    exit(40);
  }

  window = SDL_CreateWindow("CANBus Control Panel", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
  if(window == NULL) {
    printf("Window could not be shown\n");
  }
  renderer = SDL_CreateRenderer(window, -1, 0);
  SDL_Surface *image = IMG_Load(get_data("joypad.png"));
  baseTexture = SDL_CreateTextureFromSurface(renderer, image);
  SDL_RenderCopy(renderer, baseTexture, NULL, NULL);
  SDL_RenderPresent(renderer);

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
			        redrawScreen();
			        break;
		      }
	      case SDL_KEYDOWN:
          switch(event.key.keysym.sym) {
		        case SDLK_UP:
                throttle = 1;
			        break;
		        case SDLK_LEFT:
              if (turnButtonPressed == 0 && warningState == 0) {
                if (turning == -1) {
                  turning = 0;
                }
                else {
                  signalState = 0;
                  turning = -1;
                }
                turnButtonPressed = 1;
              }
              break;
		        case SDLK_RIGHT:
              if (turnButtonPressed == 0 && warningState == 0) {
                if (turning == 1) {
                  turning = 0;
                }
                else {
                  signalState = 0;
                  turning = 1;
                }
                turnButtonPressed = 1;
              }
			        break;
            case SDLK_w:
              if (warningState == 1 && warningActive == 0) {
                warningState = 0;
              } else if (warningState == 0 && warningActive == 0) {
                warningState = 1;
                turning = 0;
              }
              warningActive = 1;
              break;
		        case SDLK_u:
              if (doorButtonPressed[0] == 0) {
                updateDoorStatus(0, CAN_DOOR1_LOCK);
                doorButtonPressed[0] = 1;
              }
			        break;
              case SDLK_i:
                if (doorButtonPressed[1] == 0) {
                  updateDoorStatus(1, CAN_DOOR2_LOCK);
                  doorButtonPressed[1] = 1;
                }
  			        break;
		        case SDLK_j:
              if (doorButtonPressed[2] == 0) {
                updateDoorStatus(2, CAN_DOOR3_LOCK);
                doorButtonPressed[2] = 1;
              }
              break;
		        case SDLK_k:
              if (doorButtonPressed[3] == 0) {
                updateDoorStatus(3, CAN_DOOR4_LOCK);
                doorButtonPressed[3] = 1;
              }
              break;
            case SDLK_s:
              if (nightButtonPressed == 0) {
                isNight ^= 1;
                nightButtonPressed = 1;
              }
              break;
            case SDLK_l:
              if (lightButtonPressed == 0) {
                lightOn ^= 1;
                lightButtonPressed = 1;
              }
              break;
            case SDLK_d:
              if (diagButtonPressed == 0) {
                diagOn ^= 1;
                if (diagOn == 0) {
                  diagActive = 0;
                  system("cansend vcan0 7E0#0210010000000000");
                } else {
                  system("cansend vcan0 7E0#0210020000000000");
                }
                diagButtonPressed = 1;
              }
              break;
            case SDLK_x:
              if (diagOn == 1 && routineButtonPressed == 0) {
                diagActive ^= 1;
                if (diagActive == 1)
                  system("cansend vcan0 7E0#0431411001000000");
                else
                  system("cansend vcan0 7E0#0431411000000000");
                routineButtonPressed = 1;
              }
              break;
		      }
          break;
	      case SDL_KEYUP:
		      switch(event.key.keysym.sym) {
		        case SDLK_UP:
			        throttle = -1;
			        break;
		        case SDLK_LEFT:
		        case SDLK_RIGHT:
              turnButtonPressed = 0;
			        break;
            case SDLK_u:
              doorButtonPressed[0] = 0;
              break;
            case SDLK_i:
              doorButtonPressed[1] = 0;
              break;
            case SDLK_j:
              doorButtonPressed[2] = 0;
              break;
		        case SDLK_k:
			        doorButtonPressed[3] = 0;
			        break;
            case SDLK_w:
              warningActive = 0;
              break;
            case SDLK_s:
              nightButtonPressed = 0;
              break;
            case SDLK_l:
              lightButtonPressed = 0;
              break;
            case SDLK_d:
              diagButtonPressed = 0;
              break;
            case SDLK_x:
              routineButtonPressed = 0;
              break;
		      }
		    break;
      }
    }
    currentTime = SDL_GetTicks();
    checkAccel();
    checkTurn();
    checkLuminosity();
    if (diagOn == 1 && (lastDiagTesterPresent + 1000) < currentTime) {
      lastDiagTesterPresent = currentTime;
      system("cansend vcan0 7E0#023E000000000000");
    }

    SDL_Delay(5);
  }

  close(s);
  SDL_DestroyTexture(baseTexture);
  SDL_FreeSurface(image);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
