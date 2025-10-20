include(FetchContent)

FetchContent_Declare(
        SDL3_ttf
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
        GIT_TAG main
)

FetchContent_MakeAvailable(SDL3_ttf)