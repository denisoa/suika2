@echo off

call "C:\Program Files\Microsoft Visual Studio 10.0\VC\bin\vcvars32.bat"
PATH=%PATH%;C:\msys32\usr\bin

cd libs

rm -rf zlib
bsdtar xzfv ../../libsrc\zlib-1.2.8.tar.gz
mv zlib-1.2.8 zlib
cd zlib
sed -i 's/-MD /-MT /' win32\Makefile.msc
nmake -f win32/Makefile.msc
cp zlib.lib ../
cd ..

rm -rf libpng
bsdtar xzfv ../../libsrc\libpng-1.6.21.tar.gz
mv libpng-1.6.21 libpng
cd libpng
sed -i 's/-MD /-MT /' scripts\makefile.vcwin32
nmake -f scripts\makefile.vcwin32
cp libpng.lib ../
cd..

cd ..

echo zlib, libpngをビルドしました。
echo libogg, libvorbis, freetypeはVCプロジェクトでビルドしてください。
echo その際にコード生成オプションで/MTを指定してください。
