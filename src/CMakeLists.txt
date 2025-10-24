add_executable(${PROJECT_NAME})
target_sources(
        ${PROJECT_NAME} PRIVATE
        main.c
        common.h
        todo.c todo.h
)

#target_link_libraries(
#        ${PROJECT_NAME}
#        PRIVATE
#        SDL3::SDL3
#        SDL3_mixer::SDL3_mixer
#        SDL3_image::SDL3_image
#        glm::glm
#        tinyxml2::tinyxml2
#)
