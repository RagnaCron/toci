include(FetchContent)

FetchContent_Declare(
        SDL_mixer
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
        GIT_TAG main
)

FetchContent_MakeAvailable(SDL_mixer)