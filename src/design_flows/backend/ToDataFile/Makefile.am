#this file is src/design_flows/backend/ToDataFile/Makefile.am
noinst_LTLIBRARIES = lib_ToDataFile.la
METASOURCES = AUTO

lib_ToDataFile_la_CPPFLAGS = \
   -I$(top_srcdir)/src \
   -I$(top_srcdir)/src/constants \
   -I$(top_srcdir)/src/design_flows \
   -I$(top_srcdir)/src/graph\
   -I$(top_srcdir)/src/HLS/\
   -I$(top_srcdir)/src/HLS/evaluation \
   -I$(top_srcdir)/src/parser/polixml \
   -I$(top_srcdir)/src/polixml \
   -I$(top_srcdir)/src/utility \
   -I$(top_srcdir)/src/utility/probability_distribution \
   -I$(BOOST_DIR) $(all_includes)

if BUILD_LIB_HLS
lib_ToDataFile_la_CPPFLAGS += \
   -I$(top_srcdir)/src/HLS/evaluation
endif

if BUILD_LIB_RTL
lib_ToDataFile_la_CPPFLAGS += \
   -I$(top_srcdir)/src/parser/rtlgcc \
   -I$(top_srcdir)/src/rtl
endif

noinst_HEADERS = \
   to_data_file_step.hpp \
   to_data_file_step_factory.hpp \
   translator.hpp
lib_ToDataFile_la_SOURCES = \
   to_data_file_step.cpp \
   to_data_file_step_factory.cpp \
   translator.cpp

if BUILD_LIB_CIRCUIT
noinst_HEADERS += \
   generate_fu_list.hpp
lib_ToDataFile_la_SOURCES += \
   generate_fu_list.cpp
lib_ToDataFile_la_CPPFLAGS += \
   -I$(top_srcdir)/src/circuit \
   -I$(top_srcdir)/src/design_flows/technology \
   -I$(top_srcdir)/src/technology \
   -I$(top_srcdir)/src/technology/physical_library
endif

DOX_TAGFILES =
translator.cpp : latex_format_stat.data
BUILT_SOURCES = latex_format_stat.data

latex_format_stat.data: $(top_srcdir)/etc/latex_table_format/latex_format_stat.xml
	$(top_srcdir)/etc/scripts/add_escape.sh $(top_srcdir)/etc/latex_table_format/latex_format_stat.xml latex_format_stat.data

#do not touch the following line


