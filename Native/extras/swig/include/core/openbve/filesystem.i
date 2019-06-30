%rename(core_filesystem_home_directory) bve::openbve::home_directory;
%rename(core_filesystem_appdata_directory) bve::openbve::appdata_directory;
%rename(core_filesystem_data_directory) bve::openbve::data_directory;
%rename(core_filesystem_cwd) bve::openbve::cwd;
%rename(core_filesystem_absolute) bve::openbve::absolute;

%{
  #include <core/openbve/filesystem.hpp>
%}

%include <core/openbve/filesystem.hpp>
