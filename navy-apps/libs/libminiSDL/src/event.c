#include <NDL.h>
#include <SDL.h>
#include <string.h>

#define keyname(k) #k,

static const char *keyname[] = {"NONE", _KEYS(keyname)};

int SDL_PushEvent(SDL_Event *ev) { return 0; }

int SDL_PollEvent(SDL_Event *ev) { return 0; }

int SDL_WaitEvent(SDL_Event *event) {
    char buf[64];
    char state;
    char name[20];
    memset(name, 0, sizeof(name));
    NDL_PollEvent(buf, 64);
    sscanf(buf, "k%c %s\n", state, name);
    printf("%s\n", name);
    event->key.type = event->type = (state == 'd') ? SDL_KEYDOWN : SDL_KEYUP;
    for (int i = 0; i <83; i++) {
        if (!strcmp(keyname[i], name)) {
            event->key.keysym = (SDL_keysym){i};
            break;
        }
    }

    return 1;
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
    return 0;
}

uint8_t *SDL_GetKeyState(int *numkeys) { return NULL; }
