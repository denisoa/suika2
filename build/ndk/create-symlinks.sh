#!/bin/sh

ln -sf ../../../../../../src/conf.h app/src/main/jni/
ln -sf ../../../../../../src/event.h app/src/main/jni/
ln -sf ../../../../../../src/file.h app/src/main/jni/
ln -sf ../../../../../../src/glyph.h app/src/main/jni/
ln -sf ../../../../../../src/history.h app/src/main/jni/
ln -sf ../../../../../../src/image.h app/src/main/jni/
ln -sf ../../../../../../src/log.h app/src/main/jni/
ln -sf ../../../../../../src/main.h app/src/main/jni/
ln -sf ../../../../../../src/mixer.h app/src/main/jni/
ln -sf ../../../../../../src/platform.h app/src/main/jni/
ln -sf ../../../../../../src/script.h app/src/main/jni/
ln -sf ../../../../../../src/save.h app/src/main/jni/
ln -sf ../../../../../../src/stage.h app/src/main/jni/
ln -sf ../../../../../../src/suika.h app/src/main/jni/
ln -sf ../../../../../../src/types.h app/src/main/jni/
ln -sf ../../../../../../src/vars.h app/src/main/jni/
ln -sf ../../../../../../src/wave.h app/src/main/jni/

ln -sf ../../../../../../src/cmd_bg.c app/src/main/jni/
ln -sf ../../../../../../src/cmd_bgm.c app/src/main/jni/
ln -sf ../../../../../../src/cmd_ch.c app/src/main/jni/
ln -sf ../../../../../../src/cmd_click.c app/src/main/jni/
ln -sf ../../../../../../src/cmd_goto.c app/src/main/jni/
ln -sf ../../../../../../src/cmd_if.c app/src/main/jni/
ln -sf ../../../../../../src/cmd_load.c app/src/main/jni/
ln -sf ../../../../../../src/cmd_menu.c app/src/main/jni/
ln -sf ../../../../../../src/cmd_message.c app/src/main/jni/
ln -sf ../../../../../../src/cmd_retrospect.c app/src/main/jni/
ln -sf ../../../../../../src/cmd_se.c app/src/main/jni/
ln -sf ../../../../../../src/cmd_select.c app/src/main/jni/
ln -sf ../../../../../../src/cmd_set.c app/src/main/jni/
ln -sf ../../../../../../src/cmd_vol.c app/src/main/jni/
ln -sf ../../../../../../src/cmd_wait.c app/src/main/jni/

ln -sf ../../../../../../src/conf.c app/src/main/jni/
ln -sf ../../../../../../src/event.c app/src/main/jni/
ln -sf ../../../../../../src/history.c app/src/main/jni/
ln -sf ../../../../../../src/log.c app/src/main/jni/
ln -sf ../../../../../../src/main.c app/src/main/jni/
ln -sf ../../../../../../src/mixer.c app/src/main/jni/
ln -sf ../../../../../../src/save.c app/src/main/jni/
ln -sf ../../../../../../src/script.c app/src/main/jni/
ln -sf ../../../../../../src/stage.c app/src/main/jni/
ln -sf ../../../../../../src/vars.c app/src/main/jni/

ln -sf ../../../../../../src/ndkfile.c app/src/main/jni/
ln -sf ../../../../../../src/ndkglyph.c app/src/main/jni/
ln -sf ../../../../../../src/ndkimage.c app/src/main/jni/
ln -sf ../../../../../../src/ndkmain.h app/src/main/jni/
ln -sf ../../../../../../src/ndkmain.c app/src/main/jni/
ln -sf ../../../../../../src/ndkwave.c app/src/main/jni/

rm -r app/src/main/assets/bg
ln -sf ../../../../../../suika2/bg app/src/main/assets/
rm -r app/src/main/assets/bgm
ln -sf ../../../../../../suika2/bgm app/src/main/assets/
rm -r app/src/main/assets/cg
ln -sf ../../../../../../suika2/cg app/src/main/assets/
rm -r app/src/main/assets/ch
ln -sf ../../../../../../suika2/ch app/src/main/assets/
rm -r app/src/main/assets/conf
ln -sf ../../../../../../suika2/conf app/src/main/assets/
rm -r app/src/main/assets/cv
ln -sf ../../../../../../suika2/cv app/src/main/assets/
rm -r app/src/main/assets/se
ln -sf ../../../../../../suika2/se app/src/main/assets/
rm -r app/src/main/assets/txt
ln -sf ../../../../../../suika2/txt app/src/main/assets/
