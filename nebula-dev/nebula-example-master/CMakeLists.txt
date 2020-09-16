# Nebula Example

# 3.7.2 required for using FindVulkan
cmake_minimum_required(VERSION 3.7.2)

get_filename_component(FIPS_ROOT_DIR "../fips" ABSOLUTE)
include(${FIPS_ROOT_DIR}/cmake/fips.cmake)

fips_setup(PROJECT nackbula)
fips_include_directories(.)

nebula_begin_app(nebula-example windowed)
    
    fips_dir(code)
        fips_files(
            main.cc
            exampleapp.cc
            exampleapp.h
	    ECS.h
	    ECSComponents.cc
	    ECSComponents.h
	    ECSEntity.cc
	    ECSEntity.h
	    ECSManager.cc
	    ECSManager.h
	    ECSMessage.h
	    JSONONE.json
	    JSONTWO.json
	    JSONTHREE.json
	    neckPyScript.py
	    P2CP.cc
	    P2CP.h
	    readJSONone.py
	    readJSONtwo.py
	    readJSONthree.py
        )

    fips_deps(foundation render resource dynui scripting)
nebula_end_app()

fips_finish()
