cmake_minimum_required(VERSION 3.18)
project(IngameIME)

# ##############################################################################
# Targets
# ##############################################################################

# Library Target
add_library(IngameIME INTERFACE)
target_include_directories(IngameIME INTERFACE include)
