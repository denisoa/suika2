/* -*- coding: utf-8; tab-width: 4; indent-tabs-mode: t; -*- */

/*
 * Suika 2
 * Copyright (C) 2001-2016, TABATA Keiichi. All rights reserved.
 */

package jp.luxion.suika;

import android.app.Activity;
import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.Rect;
import android.os.Handler;
import android.os.Message;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;

import java.io.IOException;
import java.io.InputStream;

public class MainActivity extends Activity {
	static {
		System.loadLibrary("suika");
	}

	/** 仮想ビューポートの幅です。 */
	private static final int VIEWPORT_WIDTH = 1280;

	/** 仮想ビューポートの高さです。 */
	private static final int VIEWPORT_HEIGHT = 720;

	/** 60fpsを実現するための待ち時間です。 */
	private static final int DELAY = 0;

	/** Viewです。 */
	private MainView view;

	/** フレームを処理するためのHandlerです。 */
	private Handler handler;

	/** 背景ビットマップです。 */
	private Bitmap backBitmap;

	/** Paintです。 */
	private Paint paint;

	/** 転送元のRectです。 */
	private Rect srcRect;

	/** 転送先のRectです。 */
	private Rect dstRect;

	/** invalidateされたかを表します。 */
	private boolean isInvalidated;

	/**
	 * アクティビティが作成されるときに呼ばれます。
	 */
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		// フルスクリーンにする
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
		requestWindowFeature(Window.FEATURE_NO_TITLE);

		// JNIコードで初期化処理を実行する
		backBitmap = init();
		if(backBitmap == null)
			throw new RuntimeException("onCreate() returned false");

		// ビューを作成してセットする
		view = new MainView(this);
		setContentView(view);
	}

	/**
	 * ビューです。
	 */
	private class MainView extends View {
		/**
		 * コンストラクタです。
		 */
		public MainView(Context context) {
			super(context);
			setFocusable(true);
		}

		/**
		 * ビューのサイズが決定した際に呼ばれます。
		 */
		@Override
		protected void onSizeChanged(int w, int h, int oldw, int oldh) {
			// ビューポートの拡大率を求める
			float scaleX = (float)w / VIEWPORT_WIDTH;
			float scaleY = (float)h / VIEWPORT_HEIGHT;
			float scale = scaleX > scaleY ? scaleY : scaleX;

			// 実際に描画する領域のサイズを求める
			int width = (int)(VIEWPORT_WIDTH * scale);
			int height = (int)(VIEWPORT_HEIGHT * scale);

			// 実際に描画する領域のオフセットを求める
			int offsetX = (w - width) / 2;
			int offsetY = (h - height) / 2;

			// 描画用のオブジェクトを作成する
			paint = new Paint();
			srcRect = new Rect(0, 0, VIEWPORT_WIDTH - 1, VIEWPORT_HEIGHT - 1);
			dstRect = new Rect(offsetX, offsetY, offsetX + width - 1,
							   offsetY + height - 1);
		}

		/**
		 * 表示される際に呼ばれます。
		 */
		@Override
		protected void onAttachedToWindow() {
			handler = new Handler() {
				public void handleMessage(Message msg) {
					// ビューがデタッチされた場合は処理しない
					if (handler == null)
						return;

					// JNIコードでフレームを処理する
					if (!frame()) {
						// 終了する
						finish();
					}

					// 更新領域がない場合、タイマをセットする
					if (!isInvalidated)
						sendEmptyMessageDelayed(0, DELAY);
				}
			};
			handler.sendEmptyMessage(0);
			super.onAttachedToWindow();
		}

		/**
		 * 表示されなくなる際に呼ばれます。
		 */
		@Override
		protected void onDetachedFromWindow() {
			handler = null;
			super.onDetachedFromWindow();
			cleanup();
		}

		/**
		 * フレーム処理の後半で、実際の描画を行う際に呼ばれます。
		 */
		@Override
		protected void onDraw(Canvas canvas) {
			isInvalidated = false;

			// 描画を行う
			canvas.drawBitmap(backBitmap, srcRect, dstRect, paint);

			// フレーム処理時刻を更新する
			handler.sendEmptyMessageDelayed(0, DELAY);
		}

		/**
		 * タッチされた際に呼ばれます。
		 */
		@Override
		public boolean onTouchEvent(MotionEvent event) {
			float x = event.getX();
			float y = event.getY();
			return true;
		}
	}

	/*
	 * ネイティブメソッド
	 */

	/** 初期化処理を行います。 */
	private native Bitmap init();

	/** 終了処理を行います。 */
	private native void cleanup();

	/** フレーム処理を行います。 */
	private native boolean frame();

	/*
	 * ndkmain.cのためのユーティリティ
	 */

	/** 再描画を行います。 */
	private void invalidateView() {
		isInvalidated = true;
		view.invalidate();
	}

	/*
	 * ndkimage.cのためのユーティリティ
	 */

	/** ビットマップを作成します。 */
	private Bitmap createBitmap(int w, int h) {
		return Bitmap.createBitmap(w, h, Bitmap.Config.ARGB_8888);
	}

	/** assetからビットマップを作成します。 */
	private Bitmap loadBitmap(String fileName) {
		try {
			InputStream is = getResources().getAssets().open(fileName);
			return BitmapFactory.decodeStream(is);
		} catch (IOException e) {
			Log.e("Suika", "Failed to load image " + fileName);
			return null;
		}
	}

	/** Bitmapに矩形を描画します。 */
	private void drawRect(Bitmap bm, int x, int y, int w, int h, int color) {
		Canvas canvas = new Canvas(bm);
		Paint paint = new Paint();
		Rect rect = new Rect(x, y, x+w-1, y+h-1);
		paint.setColor(color);
		paint.setStyle(Paint.Style.FILL);
		canvas.drawRect(rect, paint);
	}

	/** BitmapにBitmapを描画します。 */
	private void drawBitmap(Bitmap dst, int dx, int dy, Bitmap src, int w, int h, int sx, int sy, int alpha) {
		Canvas canvas = new Canvas(dst);
		Rect dstRect = new Rect(dx, dy, dx+w-1, dy+h-1);
		Rect srcRect = new Rect(sx, sy, sx+w-1, sy+h-1);
		Paint paint = new Paint();
		paint.setAlpha(alpha);
		canvas.drawBitmap(src, srcRect, dstRect, paint);
	}

	/*
	 * ndkfile.cのためのユーティリティ
	 */

	/** Assetのファイルの内容を取得します。 */
	private byte[] getFileContent(String fileName) {
		byte[] buf = null;
		try {
			InputStream is = getResources().getAssets().open(fileName);
			buf = new byte[is.available()];
			is.read(buf);
			is.close();
		} catch(IOException e) {
			Log.e("Suika", "Failed to read file " + fileName);
			return null;
		}
		return buf;
	}
}
