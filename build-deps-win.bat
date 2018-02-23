cd dependencies\binary\src\boost\boost_1_64_0\
call bootstrap.bat
b2 install --prefix=..\..\..\build_x86 --with-filesystem --with-system variant=debug,release link=shared threading=multi address-model=32 toolset=msvc -d0
b2 install --prefix=..\..\..\build_x64 --with-filesystem --with-system variant=debug,release link=shared threading=multi address-model=64 toolset=msvc -d0
cd ..\..\..\..\..
