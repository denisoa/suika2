// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil; -*-

/*
 * Suika 2
 * Copyright (C) 2001-2016, TABATA Keiichi. All rights reserved.
 */

/*
 * [Changed]
 *  - 2016/06/15 Created.
 */

#import <Cocoa/Cocoa.h>

#import "sys/time.h"

#import "suika.h"
#import "aunit.h"

#ifdef SSE_VERSIONING
#import "cpuid.h"
#endif

//
// キーコード
//
enum {
    KC_SPACE = 49,
    KC_RETURN = 36,
    KC_ESCAPE = 53,
    KC_UP = 126,
    KC_DOWN = 125,
    KC_PAGE_UP = 116,
    KC_PAGE_DOWN = 121
};

//
// 初期化済みであるか
//
static bool isInitialized;

//
// 背景イメージ
//
static struct image *backImage;

//
// 背景イメージのピクセル
//
static unsigned char *backImagePixels;

//
// ログファイル
//
FILE *logFp;

//
// ウィンドウ
//
NSWindow *theWindow;

//
// ビュー
//
@interface SuikaView : NSView <NSWindowDelegate, NSApplicationDelegate>
@end
SuikaView *theView;

//
// 前方参照
//
static BOOL openLog(void);
static void closeLog(void);

//
// ビューの実装
//
@implementation SuikaView

// コントロールキーの状態
BOOL isControlPressed;

// アプリケーションの初期化処理を行う
- (void)applicationDidFinishLaunching:(NSNotification *)notification {
    UNUSED_PARAMETER(notification);

#ifndef SSE_VERSIONING
    backImagePixels = malloc(conf_window_width * conf_window_height * 4);
    if (pixels == NULL) {
        [NSApp terminate:nil];
        return false;
    }
#else
    if (posix_memalign((void **)&backImagePixels, SSE_ALIGN,
                       (size_t)(conf_window_width * conf_window_height * 4))
        != 0) {
        [NSApp terminate:nil];
        return;
    }
#endif

    
    backImage = create_image_with_pixels(conf_window_width, conf_window_height,
                                         (pixel_t *)backImagePixels);
    if(!init_aunit()) {
        [NSApp terminate:nil];
        return;
    }
    if(!on_event_init()) {
        [NSApp terminate:nil];
        return;
    }
    isInitialized = true;
}

- (void)applicationWillTerminate:(NSNotification *)notification {
    UNUSED_PARAMETER(notification);
    
    on_event_cleanup();
    cleanup_aunit();
    destroy_image(backImage);
}

- (void)timerFired:(NSTimer *)timer {
    UNUSED_PARAMETER(timer);
    
    // 未初期化の場合
    if (!isInitialized)
        return;
    
#if !__has_feature(objc_arc)
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
#else
    @autoreleasepool {
#endif

    // フレーム描画イベントを実行する
    int x = 0, y = 0, w = 0, h = 0;
    if (!on_event_frame(&x, &y, &w, &h)) {
        [NSApp terminate:nil];
        return;
    }
    
    // 描画範囲があればウィンドウへの再描画を行う
    if (w != 0 && h != 0)
        [self setNeedsDisplay:YES];
    
#if !__has_feature(objc_arc)
    [pool release];
#else
    }
#endif

    // FIXME:
    // [self setNeedsDisplayInRect:NSMakeRect(x, y, w, h)];
}

- (void)drawRect:(NSRect)rect {
    // 未初期化の場合
    if (!isInitialized) {
        [super drawRect:rect];
        return;
    }
    
    // 描画範囲がない場合
    if (rect.size.width == 0 || rect.size.height == 0)
        return;
    
    // 描画オブジェクトの解放用にプールを作成する
#if !__has_feature(objc_arc)
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
#else
    @autoreleasepool {
#endif

        // NSBitmapImageRepを作成する
        unsigned char *array[1];
        array[0] = backImagePixels;
        NSBitmapImageRep *rep =
        [[NSBitmapImageRep alloc]
         initWithBitmapDataPlanes:array
                       pixelsWide:conf_window_width
                       pixelsHigh:conf_window_height
                    bitsPerSample:8
                  samplesPerPixel:4
                         hasAlpha:YES
                         isPlanar:NO
                   colorSpaceName:NSDeviceRGBColorSpace
                      bytesPerRow:conf_window_width * 4
                    bitsPerPixel:32];
        assert(rep != NULL);

        // NSImageに変換する
        NSImage *img = [[NSImage alloc] initWithSize:rep.size];
        assert(img != NULL);
        [img addRepresentation:rep];
        
        // 描画を行う
        [img drawAtPoint:NSMakePoint(0, 0)
                fromRect:NSMakeRect(0, 0,
                                    conf_window_width,
                                    conf_window_height)
               operation:NSCompositeCopy
                fraction:1.0];

#if !__has_feature(objc_arc)
        [img autorelease];
        [rep autorelease];
        [pool release];
#else
    }
#endif
}

- (void)mouseDown:(NSEvent *)theEvent {
    NSPoint pos = [theEvent locationInWindow];
    if(pos.x < 0 && pos.x >= conf_window_width)
        return;
    if(pos.y < 0 && pos.y >= conf_window_height)
        return;
        
	on_event_mouse_press(MOUSE_LEFT, (int)pos.x,
                         conf_window_height - (int)pos.y);
}

- (void)mouseUp:(NSEvent *)theEvent {
    NSPoint pos = [theEvent locationInWindow];
    if(pos.x < 0 && pos.x >= conf_window_width)
        return;
    if(pos.y < 0 && pos.y >= conf_window_height)
        return;

	on_event_mouse_release(MOUSE_LEFT, (int)pos.x,
                           conf_window_height - (int)pos.y);
}

- (void)rightMouseDown:(NSEvent *)theEvent {
    NSPoint pos = [theEvent locationInWindow];
    if(pos.x < 0 && pos.x >= conf_window_width)
        return;
    if(pos.y < 0 && pos.y >= conf_window_height)
        return;
        
	on_event_mouse_press(MOUSE_RIGHT, (int)pos.x,
                         conf_window_height - (int)pos.y);
}

- (void)rightMouseUp:(NSEvent *)theEvent {
    NSPoint pos = [theEvent locationInWindow];
    if(pos.x < 0 && pos.x >= conf_window_width)
        return;
    if(pos.y < 0 && pos.y >= conf_window_height)
        return;

	on_event_mouse_release(MOUSE_RIGHT, (int)pos.x,
                           conf_window_height - (int)pos.y);
}

- (void)mouseMoved:(NSEvent *)theEvent {
    NSPoint pos = [theEvent locationInWindow];
    if(pos.x < 0 && pos.x >= conf_window_width)
        return;
    if(pos.y < 0 && pos.y >= conf_window_height)
        return;

	on_event_mouse_move((int)pos.x, conf_window_height - (int)pos.y);
}

- (void)scrollWheel:(NSEvent *)theEvent {
    if([theEvent deltaY] > 0) {
        on_event_key_press(KEY_DOWN);
        on_event_key_release(KEY_DOWN);
    } else if([theEvent deltaY] < 0) {
        on_event_key_press(KEY_UP);
        on_event_key_release(KEY_UP);
    }
}

- (void)flagsChanged:(NSEvent *)theEvent {
    // Controlキーの状態を取得する
    BOOL bit = ([theEvent modifierFlags] & NSControlKeyMask) ==
    NSControlKeyMask;
    
    // Controlキーの状態が変化した場合は通知する
    if(!isControlPressed && bit) {
        isControlPressed = YES;
        on_event_key_press(KEY_CONTROL);
    } else if(isControlPressed && !bit) {
        isControlPressed = NO;
        on_event_key_release(KEY_CONTROL);
    }
}

- (void)keyDown:(NSEvent *)theEvent {
    if([theEvent isARepeat])
        return;

    int kc = [self convertKeyCode:[theEvent keyCode]];
    if(kc != -1)
        on_event_key_press(kc);
}

- (void)keyUp:(NSEvent *)theEvent {
    int kc = [self convertKeyCode:[theEvent keyCode]];
    if(kc != -1)
        on_event_key_release(kc);
}

- (int)convertKeyCode:(int)keyCode {
    switch(keyCode) {
        case KC_SPACE:
            return KEY_SPACE;
        case KC_RETURN:
            return KEY_RETURN;
        case KC_ESCAPE:
            return KEY_ESCAPE;
        case KC_UP:
            return KEY_UP;
        case KC_DOWN:
            return KEY_DOWN;
        case KC_PAGE_UP:
            return KEY_PAGE_UP;
        case KC_PAGE_DOWN:
            return KEY_PAGE_DOWN;
    }
    return -1;
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)app {
    UNUSED_PARAMETER(app);
    return YES;
}

- (BOOL)acceptsFirstResponder {
    return YES;
}

@end

//
// メイン
//
int main()
{
#ifdef SSE_VERSIONING
	// ベクトル命令の対応を確認する
    x86_check_cpuid_flags();
#endif

#if !__has_feature(objc_arc)
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
#else
    @autoreleasepool {
#endif

    // ログをオープンする
    if (!openLog())
        return 1;

    // コンフィグの初期化処理を行う
    if (!init_conf())
        return 1;

    // アプリケーションの初期化処理を行う
    [NSApplication sharedApplication];
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];

    // メニューバーを作成する
    id menuBar = [NSMenu new];
    id appMenuItem = [NSMenuItem new];
    [menuBar addItem:appMenuItem];
    [NSApp setMainMenu:menuBar];

    // アプリケーションのメニューを作成する
    id appMenu = [NSMenu new];
    id quitMenuItem = [[NSMenuItem alloc]
                          initWithTitle:[NSString stringWithFormat:@"%@ %s",
                                                  @"Quit", conf_window_title]
                                 action:@selector(terminate:)
                          keyEquivalent:@"q"];
    [appMenu addItem:quitMenuItem];
    [appMenuItem setSubmenu:appMenu];

    // ウィンドウを作成する
    theWindow = [[NSWindow alloc]
                     initWithContentRect:NSMakeRect(0, 0,
                                                    conf_window_width,
                                                    conf_window_height)
                               styleMask:NSTitledWindowMask |
                                         NSClosableWindowMask |
                                         NSMiniaturizableWindowMask
                                 backing:NSBackingStoreBuffered
                                   defer:NO];
    [theWindow cascadeTopLeftFromPoint:NSMakePoint(20,20)];
    [theWindow setTitle:[[NSString alloc]
     initWithUTF8String:conf_window_title]];
    [theWindow makeKeyAndOrderFront:nil];
    [theWindow setAcceptsMouseMovedEvents:YES];

    // ビューを作成する
    theView = [[SuikaView alloc] init];
    [theWindow setContentView:theView];
    [theWindow makeFirstResponder:theView];

    // デリゲートを設定する
    [NSApp setDelegate:theView];
    [theWindow setDelegate:theView];

    // タイマをセットする
    id timer = [NSTimer scheduledTimerWithTimeInterval:1.0/30.0
                                                target:theView
                                              selector:@selector(timerFired:)
                                              userInfo:nil
                                               repeats:YES];

    // Hack: コマンドラインから起動された際にメニューを有効にする
    ProcessSerialNumber psn = {0, kCurrentProcess};
    TransformProcessType(&psn, kProcessTransformToForegroundApplication);

    // メインループを実行する
    [NSApp activateIgnoringOtherApps:YES];
    [NSApp run];

    // コンフィグの終了処理を行う
    cleanup_conf();
        
    // ログをクローズする
    closeLog();

#if !__has_feature(objc_arc)
    [menuBar autorelease];
    [appMenuItem autorelease];
    [appMenu autorelease];
    [appMenuItem autorelease];
    [theWindow autorelease];
    [theView autorelease];
    [timer autorelease];
    [pool release];
#else
    }
#endif

	return 0;
}

// ログをオープンする
static BOOL openLog(void)
{
    // .appバンドルのあるパスを取得する
    NSString *base = [[[NSBundle mainBundle] bundlePath]
                         stringByDeletingLastPathComponent];

    // ログのパスを生成する
    const char *path = [[NSString stringWithFormat:@"%@/%s", base,
                                  LOG_FILE] UTF8String];

    // ログをオープンする
    logFp = fopen(path, "w");
    if(logFp == NULL) {
        NSAlert *alert = [[NSAlert alloc] init];
        [alert setMessageText:@"エラー"];
        [alert setInformativeText:@"Cannot open log file."];
        [alert runModal];
#if !__has_feature(objc_arc)
        [alert autorelease];
#endif
        return NO;
    }
    return YES;
}

// ログをクローズする
static void closeLog(void)
{
    // ログをクローズする
    if(logFp != NULL)
        fclose(logFp);
}

//
// platform.hの実装
//

//
// データファイルのディレクトリ名とファイル名を指定して有効なパスを取得する
//
char *make_valid_path(const char *dir, const char *fname)
{
    char *ret;

#if !__has_feature(objc_arc)
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
#else
    @autoreleasepool {
#endif

    NSString *base = [[[NSBundle mainBundle] bundlePath]
                         stringByDeletingLastPathComponent];
    NSString *path = [NSString stringWithFormat:@"%@/%s/%s", base, dir, fname];
    const char *cstr = [path UTF8String];
    ret = strdup(cstr);

#if !__has_feature(objc_arc)
    [pool release];
#else
    }
#endif

    if(ret == NULL) {
        log_memory();
        return NULL;
    }
    return ret;
}

//
// INFOログを出力する
//
bool log_info(const char *s, ...)
{
	char buf[1024];
    va_list ap;
    
    va_start(ap, s);
    vsnprintf(buf, sizeof(buf), s, ap);
    va_end(ap);

    // ログファイルに出力する
    if (logFp != NULL) {
        fprintf(stderr, "%s", buf);
        fprintf(logFp, "%s", buf);
        fflush(logFp);
        if (ferror(logFp))
            return false;
    }
    return true;
}

//
// WARNログを出力する
//
bool log_warn(const char *s, ...)
{
	char buf[1024];
    va_list ap;

    va_start(ap, s);
    vsnprintf(buf, sizeof(buf), s, ap);
    va_end(ap);

    // ログファイルに出力する
    if (logFp != NULL) {
        fprintf(stderr, "%s", buf);
        fprintf(logFp, "%s", buf);
        fflush(logFp);
        if (ferror(logFp))
            return false;
    }
    return true;
}

//
// Errorログを出力する
//
bool log_error(const char *s, ...)
{
	char buf[1024];
    va_list ap;

    va_start(ap, s);
    vsnprintf(buf, sizeof(buf), s, ap);
    va_end(ap);

    // アラートを表示する
    NSAlert *alert = [[NSAlert alloc] init];
    [alert setMessageText:@"エラー"];
    [alert setInformativeText:[[NSString alloc] initWithUTF8String:buf]];
    [alert runModal];

    // ログファイルに出力する
    if (logFp != NULL) {
        fprintf(stderr, "%s", buf);
        fprintf(logFp, "%s", buf);
        fflush(logFp);
        if (ferror(logFp))
            return false;
    }
    return true;
}

//
// バックイメージを取得する
//
struct image *get_back_image(void)
{
    return backImage;
}

//
// タイマをリセットする
//
void reset_stop_watch(stop_watch_t *t)
{
    struct timeval tv;
    
    gettimeofday(&tv, NULL);
    
    *t = (stop_watch_t)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

//
// タイマのラップをミリ秒単位で取得する
//
int get_stop_watch_lap(stop_watch_t *t)
{
    struct timeval tv;
    stop_watch_t end;
        
    gettimeofday(&tv, NULL);
        
    end = (stop_watch_t)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
        
    if (end < *t) {
        reset_stop_watch(t);
            return 0;
    }
        
    return (int)(end - *t);
}
