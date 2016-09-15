# COMPILER
IF (MSVC)
    ADD_DEFINITIONS(
        -D_CRT_NONSTDC_NO_WARNINGS 
        -D_CRT_SECURE_NO_WARNINGS 
        -D_SCL_SECURE_NO_WARNINGS
        -DWIN32_LEAN_AND_MEAN
        -D_VCRTIMP=
    )

    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}
        /wd4819
        /wd4577
        /GS-
        /Gy-
        /GR-
        /MD
    ")

    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}
        /ENTRY:\"main_pie_init\"
        /NODEFAULTLIB
        /Zc:threadSafeInit-
    ")
ENDIF()

# PIE LIBRARY
INCLUDE_DIRECTORIES(../lib/include)
FILE(GLOB PIE_LIB_SRC
    ../lib/src/pie_crt.cpp
)

# PIE EXECUTABLE
FILE(GLOB PIE_SRC
    process_manager.cpp process_manager.hpp
    
    main.cpp
)

ADD_EXECUTABLE(pie ${PIE_LIB_SRC} ${PIE_SRC})
