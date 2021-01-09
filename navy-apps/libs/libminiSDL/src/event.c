#include <NDL.h>
#include <SDL.h>
#include <string.h>

#define keyname(k) #k,

static const char *keyname[] = {"NONE", _KEYS(keyname)};

int SDL_PushEvent(SDL_Event *ev) { return 0; }

int SDL_PollEvent(SDL_Event *ev) { return 0; }

int SDL_WaitEvent(SDL_Event *event) {
    char buf[64];
    char state[2];
    char name[20];
    NDL_PollEvent(buf, 64);
    if (buf[0] == 0) return 0;
    sscanf(buf, "%s %s\n", state, name);
    event->key.type = event->type = (state[1] == 'd') ? SDL_KEYDOWN : SDL_KEYUP;
    for (int i = 0; i <83; i++) {
        if (!strcmp(keyname[i], name)) {
            event->key.keysym.sym = i;
            break;
        }
    }

    return 1;
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
    return 0;
}

uint8_t *SDL_GetKeyState(int *numkeys) { return NULL; }
