#pragma once

#include <SDL3/SDL.h>

#define CHECK(x) do { if (!(x)) SDL_Log("%s failed: %s", #x, SDL_GetError()); } while (0)

static void SDLCALL log_prop_cb(void *label, SDL_PropertiesID props, const char *name) {
    switch (SDL_GetPropertyType(props, name)) {
    case SDL_PROPERTY_TYPE_STRING:
        SDL_Log("[%s] %s = \"%s\"", (const char *)label, name, SDL_GetStringProperty(props, name, ""));
        break;
    case SDL_PROPERTY_TYPE_NUMBER:
        SDL_Log("[%s] %s = %lld", (const char *)label, name, (long long)SDL_GetNumberProperty(props, name, 0));
        break;
    case SDL_PROPERTY_TYPE_FLOAT:
        SDL_Log("[%s] %s = %f", (const char *)label, name, SDL_GetFloatProperty(props, name, 0.0f));
        break;
    case SDL_PROPERTY_TYPE_BOOLEAN:
        SDL_Log("[%s] %s = %s", (const char *)label, name, SDL_GetBooleanProperty(props, name, false) ? "true" : "false");
        break;
    case SDL_PROPERTY_TYPE_POINTER:
        SDL_Log("[%s] %s = %p", (const char *)label, name, SDL_GetPointerProperty(props, name, NULL));
        break;
    default:
        SDL_Log("[%s] %s = (unknown type)", (const char *)label, name);
    }
}

static inline void log_properties(const char *label, SDL_PropertiesID props) {
    if (!props) {
        SDL_Log("[%s] no properties: %s", label, SDL_GetError());
        return;
    }
    SDL_EnumerateProperties(props, log_prop_cb, (void *)label);
}