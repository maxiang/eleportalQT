# teleportalQT
1: Install gstreamer-1.0-mingw-x86_64-1.16.0.msi
gstreamer-1.0-devel-mingw-x86_64-1.16.0.msi
2: Set the windows environment variable
GSTREAMER_1_0_ROOT_X86_64=C:\gstreamer\1.0\x86_64\
PKG_CONFIG_PATH=C:\gstreamer\1.0\x86_64\lib\pkgconfig
C:\gstreamer\1.0\x86_64\bin added to %path%

3: gstqmlgl.dll is a plugin I compiled. It needs to be added to C:\gstreamer\1.0\x86_64\lib\gstreamer-1.0
4:boost_1.73
5:qt5.14.1 mingw64
ok!
open teleportalQT.pro start working





