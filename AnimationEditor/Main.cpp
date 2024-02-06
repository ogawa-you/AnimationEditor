#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <Siv3D.hpp> // OpenSiv3D v0.6.6
#include <AnimeData.h> //todo:企業に提出するときには自作と分けるためにとりだず●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●
//●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●
#include "Siv3D0_6_5_1280x720_60fps_20221107.h"
#include<fstream>



void Main()
{
	// 背景の色を設定 | Set background color
	Scene::SetBackground(ColorF{ 0.8, 0.9, 1.0 });

	//ウィンドウサイズを手動で変換できる
	Window::SetStyle(WindowStyle::Sizable);

	//上の空白のサイズ
	const int TopSpace = 110;

	//ドラッグした画像の配列
	Array<Texture> textures;

	//データ管理用
	AnimeDataClass animeData;

	//上のテキストボックスの中身
	TextEditState widthBox;
	TextEditState heightBox;

	TextEditState onePatternTime;
	TextEditState totalPatternTime;

	TextEditState saveDataNameBox;

	//現在のタイルナンバー
	int nowTileNumber = 0;

	//現在アニメーションしてるタイルナンバー
	int animeTileNumber = 0;

	//枠の点線アニメーションの間隔
	double offset = 0.0;

	//始点ナンバー選択か
	bool isStartTileSelect = true;

	//一パターン選択か
	bool isOnePatternTimeSelect = true;

	//セーブされたかどうか
	bool isSave = false;

	//時間管理
	Stopwatch stopwatch;

	//フォント
	Font font20{ 20 };

	Font font40{ 40 };


	//todo:マウスホイールで画像サイズ変更
	//todo:マウスホイール押し込みでリセット
	//todo:二枚目の画像がドロップされたら一枚目排除
	//todo:セーブなどのUI追加
	//todo:始点と終点の四角の色付け

	while (System::Update())
	{
		//上の白色
		Rect{ 0,0,Scene::Width() ,TopSpace - 2 } .drawFrame(3,Palette::Black).draw(Palette::White);

		//テキストボックスのフォント
		font20(U"幅").draw(10, 14, Palette::Red);

		font20(U"高").draw(10, 64,Palette::Red);

		font20(U"1ptT").draw(187, 14, Palette::Red);

		font20(U"totalT").draw(180, 64, Palette::Red);

		font20(U"セーブ名").draw(680, 39, Palette::Red);

		//フォントの色
		std::map<bool, Color>tileNumberColor;

		//色管理
		tileNumberColor[true] = Palette::Red;
		tileNumberColor[false] = Palette::Gray;

		font20(U"=").draw(550, 12, tileNumberColor[isStartTileSelect]);

		font20(U"=").draw(550, 62, tileNumberColor[not isStartTileSelect]);

		font40(animeData.startPattern).draw(575, 0, tileNumberColor[isStartTileSelect]);

		font40(animeData.endPattern).draw(575, 50, tileNumberColor[not isStartTileSelect]);

		font40(nowTileNumber).draw(630, 25,Palette::Black);


		//テキストボックス
		SimpleGUI::TextBox(widthBox, Vec2{ 50, 10 },50, 4);

		SimpleGUI::TextBox(heightBox, Vec2{ 50, 60 },50, 4);

		SimpleGUI::TextBox(onePatternTime, Vec2{ 245, 10 }, 100, 6, isOnePatternTimeSelect);

		SimpleGUI::TextBox(totalPatternTime, Vec2{ 245, 60 }, 100, 3, not isOnePatternTimeSelect);

		SimpleGUI::TextBox(saveDataNameBox, Vec2{ 770, 35 }, 200, 10);

		//テキストボックス内容削除
		if (SimpleGUI::Button(U"×", Vec2{ 110, 10 }))
		{
			// テキストを消去
			widthBox.clear();
		}

		if (SimpleGUI::Button(U"×", Vec2{ 110, 60 }))
		{
			// テキストを消去
			heightBox.clear();
		}

		if (SimpleGUI::Button(U"×", Vec2{ 355, 10 }))
		{
			// テキストを消去
			onePatternTime.clear();
		}

		if (SimpleGUI::Button(U"×", Vec2{ 355, 60 }))
		{
			// テキストを消去
			totalPatternTime.clear();
		}
		
		
		//画像の当たり判定
		Rect _1ptTBox{ 245,10,100,37 };

		Rect totalTBox{ 245,60,100,37 };

		Rect startBox{ 440,10,102,37 };

		Rect endBox{ 442,60,100,37 };

		//クリックされたら切り替え
		if (_1ptTBox.mouseOver() && MouseL.down())
		{
			isOnePatternTimeSelect = true;
		}

		if (totalTBox.mouseOver() && MouseL.down())
		{
			isOnePatternTimeSelect = false;
		}

		if (startBox.mouseOver() && MouseL.down())
		{
			isStartTileSelect = true;
		}

		if (endBox.mouseOver() && MouseL.down())
		{
			isStartTileSelect = false;
		}

		//片方しか表示されない、描画ようなので処理はない
		bool dmy;
		dmy = SimpleGUI::Button(U"START", Vec2{ 440, 10 }, unspecified, isStartTileSelect);

		dmy = SimpleGUI::Button(U"  END  ", Vec2{ 442, 60 }, unspecified, not isStartTileSelect);


		//テキストボックス内の文字情報を数値に変換
		animeData.widthCount  = ParseOr<int>(widthBox.text, 10);
		animeData.heightCount = ParseOr<int>(heightBox.text, 5);
		if (isOnePatternTimeSelect)
		{
			animeData.oneAnimationTimeMS = ParseOr<double>(onePatternTime.text, 0);
		}
		else
		{
			animeData.oneAnimationTimeMS = (ParseOr<double>(totalPatternTime.text, 0.1) * 1000)/animeData.TotalPattern();
		}

		//現在の数値をテキストボックスに反映
		widthBox.text = Format(animeData.widthCount);
		heightBox.text = Format(animeData.heightCount);
		onePatternTime.text = Format(animeData.oneAnimationTimeMS);
		totalPatternTime.text = Format(animeData.TotalTimeS());



		//ドラッグしてきた画像を配列に保存
		if (DragDrop::HasNewFilePaths())
		{
			for (const auto& dropped : DragDrop::GetDroppedFilePaths())
			{
				textures << Texture(dropped.path);

				int i = 0;
				for (auto& a : dropped.path)
				{
					animeData.fileName[i++] = a;
				}
			}
		}

		//配列内画像
		for (auto& texture : textures)
		{
			//画像のy座標
			int yShift = TopSpace + 0;

			//描画
			texture.draw(0, yShift);

			//画像１枚のサイズ
			int width = texture.width() / animeData.widthCount;
			int height = texture.height() / animeData.heightCount;
			Point position = {0,0};

			//グリッド描画
			for (int i = 0; i < animeData.widthCount + 1; i++)
			{
				Line(0 + (width * i), TopSpace, 0 + (width * i), TopSpace + texture.height()).draw(2,Palette::Black);
			}
		
			for (int i = 0; i < animeData.heightCount + 1; i++)
			{
				Line(0, TopSpace + height * i, texture.width(), TopSpace + height * i).draw(2, Palette::Black);
			}

			//補正選択範囲座標
			position = { Cursor::Pos().x - Cursor::Pos().x % width,
						((Cursor::Pos().y - TopSpace) - (Cursor::Pos().y - TopSpace) % height) + TopSpace };

			//選択範囲
			Rect rect(position, width, height);

			//画像外判定
			bool widthRange = 0 <= Cursor::Pos().x && Cursor::Pos().x <= texture.width();
			bool heightRange = TopSpace <= Cursor::Pos().y && Cursor::Pos().y <= texture.height() + TopSpace;

			//時間加算(点線アニメーション)
			offset += (Scene::DeltaTime() * 10);

			//画像外に出てなければ選択範囲表示
			if (widthRange && heightRange)
			{
				//点線四角描画(点線アニメーション)
				rect.top().draw(LineStyle::SquareDot(offset), 3);
				rect.right().draw(LineStyle::SquareDot(offset), 3);
				rect.left().draw(LineStyle::SquareDot(offset), 3);
				rect.bottom().draw(LineStyle::SquareDot(offset), 3);

				//何番目か
				int patternX = (position.x / width);
				int patternY = ((position.y - TopSpace) / height);

				//現在の番号
				nowTileNumber = patternX + patternY * animeData.widthCount;

				//左クリックで処理
				if (MouseL.down())
				{
					//始点選択中か否か
					if (isStartTileSelect)
					{
						//現在の番号を代入
						animeData.startPattern = nowTileNumber;

						isStartTileSelect = false;
					}
					else
					{
						//現在の番号を代入
						animeData.endPattern = nowTileNumber;

						isStartTileSelect = true;
					}
				}
			}

			//ラムダ式関数(変数みたい)
			auto GetRect = [&](int tileNumber)
			{
				int xPosition = width * (tileNumber % animeData.widthCount);
				int yPosition = height * (tileNumber / animeData.widthCount);

				return Rect{ xPosition, yPosition, width, height };
			};

			//エンターキーが押されたらアニメーション
			if (KeyEnter.down())
			{
				//カウントスタート
				stopwatch.start();

				//最初の画像に
				animeTileNumber = animeData.startPattern;
			}

		
			//ストップウォッチが動いているなら
			if (stopwatch.isRunning())
			{
				//100ms毎に画像変更(デフォルト0.1s)
				if (stopwatch.ms() >= animeData.oneAnimationTimeMS)
				{
					//ストップウォッチを再スタート
					stopwatch.restart();

					//最後のタイルまで行ったら最初のタイルに戻る
					if (animeTileNumber == animeData.endPattern)
					{
						animeTileNumber = animeData.startPattern;
					}
					else
					{
						//加算
						animeTileNumber++;
					}
				}
			}

			//右上の画像の枠を描画
			Rect(Scene::Width() - width - 2, 0 ,width + 2, height + 2).draw(Palette::Black);

			//右上にアニメーションを描画
			texture(GetRect(animeTileNumber))
				.draw(Scene::Width() - width, 0);
		}

		//Sキーを押したらセーブ
		if (KeyControl.pressed() && KeyS.pressed() && not isSave)
		{
			saveDataNameBox.text += U".dat";

			char saveDataName[256] = { 0 };

			for (int i = 0; i < saveDataNameBox.text.size(); i++)
			{
				saveDataName[i] = saveDataNameBox.text[i];
			}

			animeData.Save(saveDataName);

			isSave = true;
		}

		//セーブされたら描画
		if (isSave == true)
		{
			font20(U"※セーブしました").draw(700,70,Palette::Black);
		}
	}
}

//
// - Debug ビルド: プログラムの最適化を減らす代わりに、エラーやクラッシュ時に詳細な情報を得られます。
//
// - Release ビルド: 最大限の最適化でビルドします。
//
// - [デバッグ] メニュー → [デバッグの開始] でプログラムを実行すると、[出力] ウィンドウに詳細なログが表示され、エラーの原因を探せます。
//
// - Visual Studio を更新した直後は、プログラムのリビルド（[ビルド]メニュー → [ソリューションのリビルド]）が必要です。
//
// チュートリアル
// https://siv3d.github.io/ja-jp/tutorial/tutorial/
//
// Tutorial
// https://siv3d.github.io/tutorial/tutorial/
//
// Siv3D コミュニティへの参加（Discord などで気軽に質問や交流, 最新情報の入手ができます）
// https://siv3d.github.io/ja-jp/community/community/
//
// Siv3D User Community
// https://siv3d.github.io/community/community/
//
// 新機能の提案やバグの報告 | Feedback
// https://siv3d.github.io/ja-jp/develop/report/
//
// Sponsoring Siv3D
// https://github.com/sponsors/Reputeless
//
