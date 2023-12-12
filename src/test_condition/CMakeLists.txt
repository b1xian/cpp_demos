project(test_condition)



set(cond true11)

if (${cond} STREQUAL "true" OR ${cond} STREQUAL "false")
    message("yes!!!!!!!!!!!!!")
else()
    message("no!!!!!!!!!!!!!")
endif()


if ((NOT ${cond} STREQUAL "true") AND (NOT ${cond} STREQUAL "false"))
    message("not valid!!!!!!!!!!!!!")
endif()