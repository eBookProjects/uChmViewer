#
# License: MIT
# Copyright (c) 2022 Nick Egorrov
########################################

#[==============================[
Attempts to set the cache variable to ON when building for a specific OS.

Synopsis
------------

    use_in(
        flag
        hint
        system
    )

For the given system the function tries to set the cache variable to ON,
for other systems it sets OFF. If the specified variable already exists
and has a non-empty value, the function does nothing. So by default the
function turns on the option for the specified system.

Options
------------

flag
    Variable name.
hint
    A brief description of the purpose of the variable.
system
    Name of the system for which the variable is set.
    
Example
------------

    use_in(USE_DBUS "Use D-Bus integration" "Linux")
    ...
    ...
    if (USE_DBUS)
        find_package(Qt6 REQUIRED DBus)
    endif ()
#]==============================]
function (use_in flag hint system)
    if (CMAKE_SYSTEM_NAME MATCHES ${system})
        set(_use ON)
    else ()
        set(_use OFF)
    endif ()

    set(${flag} ${_use} CACHE BOOL ${hint})
endfunction ()
