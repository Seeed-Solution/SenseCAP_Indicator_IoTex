set(UI_DIR ./ui)
file(GLOB_RECURSE UI_SOURCES ${UI_DIR}/*.c)

set(MODEL_DIR ./model)
file(GLOB_RECURSE MODEL_SOURCES ${MODEL_DIR}/*.c)

set(VIEW_DIR ./view)
file(GLOB_RECURSE VIEW_SOURCES ${VIEW_DIR}/*.c)

set(CONTROLLER_DIR ./controller)
file(GLOB_RECURSE CONTROLLER_SOURCES ${CONTROLLER_DIR}/*.c)

set(UTIL_DIR ./util)
file(GLOB_RECURSE UTIL_SOURCES ${UTIL_DIR}/*.c)

set(LIB_DIR ./lib)
file(GLOB_RECURSE LIB_SOURCES ${LIB_DIR}/*.c)

idf_component_register(
    SRCS "main.c" "config.c" ${MODEL_SOURCES}  ${UTIL_SOURCES} "lv_port.c"  ${VIEW_SOURCES}  ${UI_SOURCES} #${CONTROLLER_SOURCES}
    INCLUDE_DIRS "."  ${MODEL_DIR} ${UTIL_DIR}  ${VIEW_DIR}  ${UI_DIR} #${CONTROLLER_DIR}
    EMBED_TXTFILES timeapi_cert.pem)
