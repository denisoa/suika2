dummy:

install:
	cp package-linux64 ../suika2/tool/
	cp package-mac ../suika2/tool/
	cp package-win.exe ../suika2/tool/

package-linux64: package.c
	gcc -O2 -o package-linux64 package.c

package-mac: package.c
	clang -O2 -arch i386 -arch x86_64 -o package-mac package.c

package-win.exe: package.c
	i686-w64-mingw32-gcc -O2 -o package-win.exe package.c
