#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := sample_project
EXTRA_COMPONENT_DIRS += $(PROJECT_PATH)/components/esp-azure
include $(IDF_PATH)/make/project.mk
