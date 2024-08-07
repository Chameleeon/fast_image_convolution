# Alternative GNU Make workspace makefile autogenerated by Premake

ifndef config
  config=debug
endif

ifndef verbose
  SILENT = @
endif

ifeq ($(config),debug)
  conv_lib_config = debug
  main_config = debug

else ifeq ($(config),release)
  conv_lib_config = release
  main_config = release

else ifeq ($(config),releasesimd)
  conv_lib_config = releasesimd
  main_config = releasesimd

else ifeq ($(config),releasecsimd)
  conv_lib_config = releasecsimd
  main_config = releasecsimd

else
  $(error "invalid configuration $(config)")
endif

PROJECTS := conv_lib main

.PHONY: all clean help $(PROJECTS) 

all: $(PROJECTS)

conv_lib:
ifneq (,$(conv_lib_config))
	@echo "==== Building conv_lib ($(conv_lib_config)) ===="
	@${MAKE} --no-print-directory -C conv_lib -f Makefile config=$(conv_lib_config)
endif

main: conv_lib
ifneq (,$(main_config))
	@echo "==== Building main ($(main_config)) ===="
	@${MAKE} --no-print-directory -C main -f Makefile config=$(main_config)
endif

clean:
	@${MAKE} --no-print-directory -C conv_lib -f Makefile clean
	@${MAKE} --no-print-directory -C main -f Makefile clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "  debug"
	@echo "  release"
	@echo "  releasesimd"
	@echo "  releasecsimd"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   conv_lib"
	@echo "   main"
	@echo ""
	@echo "For more information, see https://github.com/premake/premake-core/wiki"