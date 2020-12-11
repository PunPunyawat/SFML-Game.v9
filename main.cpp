#include <SFML/Graphics.hpp>
#include <SFML\Audio.hpp>
#include <Windows.h>
#include "Player.h"
#include "Bullet.h"
#include "background.h"
#include <vector>
#include <sstream>
#include <fstream>
#include "Menu.h"
#include <cstdlib>

using namespace sf;
using namespace std;

static const float VIEW_HEIGHT = 864.0f;
static const float VIEW_WIDHT = 564.0f;

float detalTime = 0.0f;       // ตั้งค่าความเร็วให้ตัวละครเดิน 
float totaltime = 0;		  // ตั้งค่าความเร็วให้ตัวละครเดิน 	
float speed_player = 150.0f;  // ตั้งค่าความเร็วให้ตัวละครเดิน
int   animationFrame = 0; // แฟรมของตัวละคร ที่ 0 


///////////////////////////////// VIEW //////////////////////////////////////////

//ฟังชั่น ในการเซ้ทค่า display ให้คงที่ตลอดหลังจากปรับขยายหรือหดหน้า
void ResizeView(const sf::RenderWindow& window, sf::View& view)
{
	// aspectRatio คือการเซ้ทค่าของหน้าจอให้คงที่ตลอดหลังจากมีการขยายหน้าจอ เป็นต้น
	float aspectRatio = float(window.getSize().x) / float(window.getSize().y);
	view.setSize(VIEW_HEIGHT * aspectRatio, VIEW_WIDHT * aspectRatio);  // การให้มันคงที่ค่าในแกน x ภาพหน้าจะเหมือนเดิม
}

///////////////////////////////// MAIN /////////////////////////////////////////
int main()
{	
	int  hit = 0, hit_again = 0;  // rand HP
	int  Scorecurrent = 0;
	int rand_item;
	int endgame_1 = 0;

	sf::Font word;
	if (!word.loadFromFile("font/Jelly Crazies.ttf")); {

	}
	
	sf::Text scoreGame;
	scoreGame.setFont(word);
	scoreGame.setCharacterSize(10);
	scoreGame.setStyle(sf::Text::Regular);
	scoreGame.setPosition(400, 50);
	scoreGame.setString("SCORE . ");

	sf::Text scoreCurrent;
	scoreCurrent.setFont(word);
	scoreCurrent.setCharacterSize(10);
	scoreCurrent.setStyle(sf::Text::Regular);
	scoreCurrent.setPosition(500, 50);
	
	///////////////////////////////////////////////////////////////////////////
	int screen = 0; // หน้าจอตอนให้ตัวละครเดินขึ้นมา
	sf::Clock clock;
	srand(time(NULL));

	//////////////////////// DISPLAY ,  ICON GANE ///////////////////////////////////////
	
	sf::RenderWindow window(sf::VideoMode(564.0f, 864.0f), "student N I N J A" );  //ขนาดหน้าจอ "ชื่อหัวเกม"
	window.setFramerateLimit(60);

	sf::Image icon;
		if (!icon.loadFromFile("./sprite/ninjaweaponRED.png"))
			{
				return -1;
			}
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());  // เซ้ท icon เกม

	////////////////////////////// Menu  /////////////////////////////////
	int game = 0;
	sf::Event evnt;

	sf::Texture bgmenuTexture;   // พื้นหลังหน้าเกม
	bgmenuTexture.loadFromFile("./sprite/backgrounds_starts.png");
	sf::Texture startTexture;  // ปุ่ม start
	startTexture.loadFromFile("./sprite/start.png");
	sf::Texture scoreTexture;   // ปุ่ม score
	scoreTexture.loadFromFile("./sprite/score.png");
	sf::Texture quitTexture;    // ปุ่ม quit
	quitTexture.loadFromFile("./sprite/quit.png");
	sf::Texture backTexture;    // ปุ่ม back
	backTexture.loadFromFile("./sprite/back.png");
	sf::Texture HowtoplayTexture;
	HowtoplayTexture.loadFromFile("./sprite/Howto.png");
	//Score
	sf::Texture bgscoreTexture;  // พื้นหลังห้นา score
	bgscoreTexture.loadFromFile("./sprite/background_score.png");
											// ขนาดรูป					ตำแหน่งรูป
	Menu bgmenu(&bgmenuTexture, sf::Vector2f(1.0f, 1.0f), sf::Vector2f(0.0f, 0.0f));
	Menu bgscore(&bgscoreTexture, sf::Vector2f(1.0f, 1.0f), sf::Vector2f(0.0f, 0.0f));
	Menu start(&startTexture, sf::Vector2f(0.5f, 0.5f), sf::Vector2f(395.0f, 458.0f));
	Menu score(&scoreTexture, sf::Vector2f(0.5f, 0.5f), sf::Vector2f(395.0f, 545.0f));
	Menu Howtoplay(&HowtoplayTexture, sf::Vector2f(2.0f, 2.0f), sf::Vector2f(290.0f, 620.0f));
	Menu quit(&quitTexture, sf::Vector2f(0.5f, 0.5f), sf::Vector2f(420.0f, 705.0f));

	Menu back(&backTexture, sf::Vector2f(2.0f, 2.0f), sf::Vector2f(380.0f, 820.0f));
	
	///////////////////////////////// VIEW //////////////////////////////////////////

	sf::View viewCharacter;
	viewCharacter.setSize(sf::Vector2f(564.0f, 864.0f));
	viewCharacter.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));   // ให้พอดีขนาดหน้าจอ
	//viewCharacter.zoom(1.2f);  // ขยายภาพพื้นหลัง
	//sf::View view(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(564.0f, 864.0f));   // ทำให้การปรับหน้าจอภาพให้มีขนาดตงที่

	/////////////////////////////// BACKGROUND /////////////////////////////////////

	// ลองใส่พื้นหลัง
	//sf::RectangleShape background(sf::Vector2f(564.0f, 864.0f)); // ให้รูปใหญ่เท่าไหร่ ก็เลือกขนาดเอา
	sf::Texture backgroundTexture[2];
	backgroundTexture[0].loadFromFile("./sprite/background_game.png");
	//backgroundTexture[1].loadFromFile("./sprite/background_game2.jpg");
	std::vector<Background>backgrounds;
	backgrounds.push_back(Background(&backgroundTexture[0], 110.0f));
	//backgrounds.push_back(Background(&backgroundTexture[1], 80.0f));


	///////////////////////////////// ITEM //////////////////////////////////////////
	// Item
	sf::RectangleShape Heart(sf::Vector2f(42.0f, 43.0f));
	sf::Texture Heartlove;
	Heartlove.loadFromFile("./sprite/Heart.png");
	Heart.setTexture(&Heartlove);
	Heart.setPosition({ 270,340 });
	std::vector<RectangleShape> Hearts;
	Hearts.push_back(RectangleShape(Heart));

	//////////////////////////////////////////////////////

	// v.2  item แบบ mon
	sf::Clock Itemclock;
	sf::RectangleShape item(sf::Vector2f(42.0f, 43.0f));  // ของ monepic
	sf::Texture allitem;
	allitem.loadFromFile("./sprite/Heart.png");
	item.setTexture(&allitem);
	item.setPosition({ 270,340 });

	std::vector<RectangleShape> items;
	items.push_back(RectangleShape(item));
	float ItemDelay = Itemclock.getElapsedTime().asSeconds();
	int itemSpawnTimer = 0;

	///////////////////////////////// HP //////////////////////////////////////////
	sf::RectangleShape HP(sf::Vector2f(230.0f, 70.0f));
	sf::Texture HPplayer;
	HPplayer.loadFromFile("sprite/HP/HP_1_100p.png");
	HP.setTexture(&HPplayer);
	HP.setPosition({ 50,30 });
	
	///////////////////////////////// game over //////////////////////////////////////////
	
	sf::RectangleShape endgame(sf::Vector2f(564.0f,864.0f));  
	sf::Texture over;
	over.loadFromFile("sprite/gamesover.png");
	endgame.setTexture(&over);
	endgame.setPosition({ 0,0 });  // ตำแหน่งวงกลม (x,y)

	/////////////////////////////// how to play //////////////////////////////////////////

	sf::RectangleShape howtoplay(sf::Vector2f(564.0f, 864.0f));
	sf::Texture howto;
	howto.loadFromFile("sprite/HOWtoplay.png");
	howtoplay.setTexture(&howto);
	howtoplay.setPosition({ 0,0 });  // ตำแหน่งวงกลม (x,y)
	
	
	///////////////////////////////// PLAYER main //////////////////////////////////////////

	sf::RectangleShape player(sf::Vector2f(90.0f, 90.0f)); //ขนาดของตัวละคร
	player.setPosition(240.0f, 1000.0f); //ตน. ของตัวละครตอนเริ่ม
	sf::Texture playerTexture;													 /////////  main ////////
	playerTexture.loadFromFile("sprite/playermainful.png"); // ดึงรูปมาจากในไฟล์
	player.setTexture(&playerTexture); // แอดรูปเข้าไปใน 4 เหลี่ยม
	//256   / 4
	int playerSizeX = playerTexture.getSize().x / 4; // ตัดส่วนของตัวละคร แกน x
	int playerSizeY = playerTexture.getSize().y / 6; // ตัดส่วนของตัวละคร แกน y
	player.setTextureRect(sf::IntRect(0, 0, playerSizeX, playerSizeY));

	///////////////////////////////// ENEMY //////////////////////////////////////////
	//v.1  white
	sf::Vector2f playerCenter;  // เอามาจาก code ด้านล่าง
	sf::RectangleShape enemy(sf::Vector2f(120.0f, 100.0f));  // ของ monepic
	sf::Texture enemies_photo;
	enemies_photo.loadFromFile("sprite/monepicWHITE.png");
	enemy.setTexture(&enemies_photo);
	enemy.setPosition({200,100});
	
	std::vector<RectangleShape> enemies;
	enemies.push_back(RectangleShape(enemy));

	// v.2  red
	sf::RectangleShape enemy2(sf::Vector2f(120.0f, 100.0f));  // ของ monepic
	sf::Texture enemies_photo2;
	enemies_photo2.loadFromFile("sprite/monepicRED.png");
	enemy2.setTexture(&enemies_photo2);
	enemy2.setPosition({ 200,100 });

	std::vector<RectangleShape> enemies2;
	enemies2.push_back(RectangleShape(enemy2));

	int enemySpawnTimer = 0;
	///////////////////////////////// SHOT //////////////////////////////////////////

	sf::Clock shotclock;  // เอาไว้หน่วงเวลา กระสุน
	float shotDelay = shotclock.getElapsedTime().asSeconds();
	sf::RectangleShape shoot(sf::Vector2f(30.0f, 30.0f));  // ขนาดกระสุน
	sf::Texture shootz_photo;
	shootz_photo.loadFromFile("sprite/ninjaweaponWHITE.png");
	shoot.setTexture(&shootz_photo);
	std::vector<RectangleShape> shoots;
	shoots.push_back(RectangleShape(shoot));

	///////////////////////////////// SOUND //////////////////////////////////////////
	
	sf::Music start_sound; // เพลงเริ่มเกม
	if (!start_sound.openFromFile("sound/songstart.ogg"))
	{
		printf("ERROR\n");
	}
	start_sound.play(); //เปิดเพลง

	///////////////////////////////// SHOT SOUND //////////////////////////////////////////

	sf::SoundBuffer soundeffect;
	soundeffect.loadFromFile("sound/Soundshot.wav");
	sf::Sound shooteffect;
	shooteffect.setBuffer(soundeffect);

	///////////////////////////////// pickup SOUND //////////////////////////////////////////

	sf::SoundBuffer soundpick;
	soundpick.loadFromFile("sound/pickup_item.wav");
	sf::Sound Pickupsound;
	Pickupsound.setBuffer(soundpick);

	///////////////////////////////// Heal SOUND //////////////////////////////////////////

	sf::SoundBuffer soundpickITEM;
	soundpickITEM.loadFromFile("sound/Heal.wav");
	sf::Sound PickITEMsound;
	PickITEMsound.setBuffer(soundpickITEM);

	///////////////////////////// hit player SOUND //////////////////////////////////////////

	sf::SoundBuffer soundhitplayer;
	soundhitplayer.loadFromFile("sound/hitDamage.wav");
	sf::Sound playerhitsound;
	playerhitsound.setBuffer(soundhitplayer);

	///////////////////////////// Boom  SOUND //////////////////////////////////////////

	sf::SoundBuffer soundboom;
	soundboom.loadFromFile("sound/Boom.wav");
	sf::Sound Boomsound;
	Boomsound.setBuffer(soundboom);

	///////////////////////////// Dead  SOUND //////////////////////////////////////////

	sf::SoundBuffer sound_dead;
	sound_dead.loadFromFile("sound/dead.wav");
	sf::Sound deadsound;
	deadsound.setBuffer(sound_dead);

	///////////////////////////// coin  SOUND //////////////////////////////////////////

	sf::SoundBuffer soundcoin;
	soundcoin.loadFromFile("sound/Coin.wav");
	sf::Sound coinsound;
	coinsound.setBuffer(soundcoin);

	//******************************************************************************

	while (window.isOpen())
	{
		
		while (game == 0)
		{
			while (window.pollEvent(evnt))
			{
				start_sound.play();
				switch (evnt.type)
				{
				
				case sf::Event::Closed:
					window.close();
					break;
				}
			}

			detalTime = clock.restart().asSeconds(); // คำสั่งหน่วงให้น้องเดินช้า
			if (detalTime > 1 / 20.0f)
				detalTime = 1 / 20.0f;

			// วาดเมนู
			bgmenu.Draw(window);
			start.Draw(window);
			score.Draw(window);
			quit.Draw(window);
			Howtoplay.Draw(window);
			window.display();
			if (start.getGlobalBounds(window)) {
				start.setScale(sf::Vector2f(0.43f, 0.43f));

			}
			else {
				start.setScale(sf::Vector2f(0.4f, 0.4f));
			}
			if (score.getGlobalBounds(window)) {
				score.setScale(sf::Vector2f(0.43f, 0.43f));
			}
			else {
				score.setScale(sf::Vector2f(0.4f, 0.4f));
			}
			if (quit.getGlobalBounds(window)) {
				quit.setScale(sf::Vector2f(0.43f, 0.43f));
			}
			else {
				quit.setScale(sf::Vector2f(0.4f, 0.4f));
			}
			if (Howtoplay.getGlobalBounds(window)) {
				Howtoplay.setScale(sf::Vector2f(0.42f, 0.42f));
			}
			else {
				Howtoplay.setScale(sf::Vector2f(0.4f, 0.4f));
			}


			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (start.getGlobalBounds(window)) {
					game = 1;

					Scorecurrent = 0;
					enemies.clear();
					enemies2.clear();
					shoots.clear();
					items.clear();
					//start_sound.play();
					
					hit = 0;
					hit_again = 0;
					HPplayer.loadFromFile("sprite/HP/HP_1_100p.png");
					HP.setTexture(&HPplayer);
					player.setPosition(240.0f, 1000.0f); 
					screen = 0;

					if (screen == 0) {
						if (player.getPosition().y >= 720) {   // ถ้าตำแหน่งน้อยกว่า 721 ให้ตัวละครเดินขึ้นมาเองจนไม่น้อยกว่า 721
							player.move(0.0f, -5.0f);
						}
						else {
							screen = 1;
						}
					}

					if (TRUE) {  // ให้หันหน้าอยู่เสมอ
						window.draw(player);
						player.setTextureRect(sf::IntRect(playerSizeX * animationFrame, playerSizeY * 4, playerSizeX, playerSizeY));

					}

				}
				else if (score.getGlobalBounds(window)) {
					game = 2;
				}
				else if (quit.getGlobalBounds(window)) {
					game = 3;
				}
				else if (Howtoplay.getGlobalBounds(window)) {
					game = 5;
				}
			
			}

		}

		//score  หน้าต่างสกอ
		while (game == 2)
		{
			bgscore.Draw(window);
			back.Draw(window);
			window.display();
			if (back.getGlobalBounds(window)) {
				back.setScale(sf::Vector2f(0.32f, 0.32f));
			}
			else {
				back.setScale(sf::Vector2f(0.28f, 0.28f));  // ให้ขนาดใหญ่ขึ้นเมื่อชี้
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && back.getGlobalBounds(window))
			{
				game = 0;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))   // หกแล้วไปหน้าเมนูเลย
			{
				game = 0;
			}
			
		}

		//howto play  ออกเกมเลย
		while (game == 5)
		{

			window.draw(howtoplay);
			back.Draw(window); // ปุ่มย้อนกลับ
			window.display();
			if (back.getGlobalBounds(window)) {
				back.setScale(sf::Vector2f(0.32f, 0.32f));
			}
			else {
				back.setScale(sf::Vector2f(0.28f, 0.28f));  // ให้ขนาดใหญ่ขึ้นเมื่อชี้
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && back.getGlobalBounds(window))
			{
				game = 0;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))   // หกแล้วไปหน้าเมนูเลย
			{
				game = 0;
			}

		}


		//quit  ออกเกมเลย
		while (game == 3)
		{
			window.close();
			break;
		}


		// หน้าจอเกม over 
		while (game == 4)
		{
			
			window.display();
			if (endgame_1 == 0) {
				start_sound.stop();
				window.draw(endgame);

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
					game = 0;
					//window.close();
				}
			}
		}


		/////////////////////////////  เลื่อน PLAYER  //////////////////////////////////////////
		while (game==1)
		{

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))   // หกแล้วไปหน้าเมนูเลย
			{
				//game = 0;
				//_mm_pause;
				window.close();
				break;

			}

		if (screen == 0) {
			if (player.getPosition().y >= 720) {   // ถ้าตำแหน่งน้อยกว่า 721 ให้ตัวละครเดินขึ้นมาเองจนไม่น้อยกว่า 721
				player.move(0.0f, -5.0f);
			}
			else {
				screen = 1;
			}
		}

		if (TRUE) {  // ให้หันหน้าอยู่เสมอ
			window.draw(player);
			player.setTextureRect(sf::IntRect(playerSizeX * animationFrame, playerSizeY * 4, playerSizeX, playerSizeY));
		}

		if (screen == 1) {   // เดี๋ยวเอาการรับค่าปุ่มการเดินมาใส่ เพราะตอนเริ่มเกมจะได้กดปุ่มไม่ได้


			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				if (player.getPosition().x < 400) {

					player.move(speed_player * detalTime, 0.f * speed_player);
					player.setTextureRect(sf::IntRect(playerSizeX * animationFrame, playerSizeY * 3, playerSizeX, playerSizeY));  //ปรับให้ตัวละครเดินตามทิศทาง

				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				if (player.getPosition().x > 90) {

					player.move(-speed_player * detalTime, 0.f * speed_player);
					player.setTextureRect(sf::IntRect(playerSizeX * animationFrame, playerSizeY * 2, playerSizeX, playerSizeY));

				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				if (player.getPosition().y > 130) {

					player.move(0.f * speed_player, -speed_player * detalTime);
					player.setTextureRect(sf::IntRect(playerSizeX * animationFrame, playerSizeY * 4, playerSizeX, playerSizeY));

				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				if (player.getPosition().y < 715) {

					player.move(0.f * speed_player, speed_player * detalTime);
					player.setTextureRect(sf::IntRect(playerSizeX * animationFrame, playerSizeY * 1, playerSizeX, playerSizeY));

				}

			}

		}
		ItemDelay = Itemclock.getElapsedTime().asSeconds();  // delay หัวใจออกมา
		shotDelay = shotclock.getElapsedTime().asSeconds();  // delay กระสุนออกมา
		

		
		

			
			///////////////////////////////// LOOP BG //////////////////////////////////////////	
		if (game == 1) {

			for (Background& background : backgrounds) {
				
					background.Update(detalTime);
				
			}

			for (Background& background : backgrounds) {   // ลูปให้รูปวน 
				background.Draw(window);

			}
		}
			/// ///////////////////////////////////////////////////////////////////////////////

			totaltime += detalTime; // ให้น้องวิ่งเป็นเอนิเมชั่น
			if (totaltime >= 0.2) {  // ทำให้น้องสิ่งเร็วหรือช้า แบบเอนิเมชั่น
				totaltime -= 0.2;
				animationFrame++;   // ให้น้องตัวละครไม่เดินเร็วเกิน เป็นการเซ้ทค่าความหน่วงให้น้อง
			}

			if (animationFrame > 3) {   // ทำเอนิเมชั่นให้มันเคลื่อนไหว ตลอด
				animationFrame = 0;
			}

			/////////////////////////////  20/11/20  Update shoot //////////////////////////////////////////
																	// -15 เป็นของ class
			playerCenter = sf::Vector2f(player.getPosition().x + 30 /*- 15*/, player.getPosition().y + 10); // ตำแหน่งกระสุนออกจากตัว

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				if (shotDelay > 0.25) {
					shoot.setPosition(playerCenter);
					shoots.push_back(RectangleShape(shoot));
					shotDelay = shotclock.restart().asSeconds();
					shooteffect.play();
				}

			}

			for (size_t i = 0; i < shoots.size(); i++)
			{
				shoots[i].move(0.0f, -10.0f);

				if (shoots[i].getPosition().y <= 0)
				{
					shoots.erase(shoots.begin() + i);
				}
			}


			/////////////////////////////  spawn monster  //////////////////////////////////////////
			//v.1  mon_white
			if (Scorecurrent <= 600) {  // คะแนนไม่เกิน 150 ให้มอนปกติออกมา 

				if (enemySpawnTimer < 30)   // การ spawn mon  (กำหนดลิมิตการสปอน)
				{
					enemySpawnTimer++;
				}


				int rand_x = (rand() % int(window.getSize().x - enemy.getSize().x));

				if (enemySpawnTimer >= 70)
				{
					enemy.setPosition(Vector2f(rand_x, -20.0f));
					enemies.push_back(RectangleShape(enemy));
					enemySpawnTimer = 0;
				}

				enemySpawnTimer++;
			}

			for (size_t i = 0; i < enemies.size(); i++)
			{
				enemies[i].move(0.0f, 1.5f);

				if (enemies[i].getPosition().y > window.getSize().y)  // สปอน มอนให้เกิด
				{
					enemies.erase(enemies.begin() + i);
					playerhitsound.play();
					if (hit == 0) {
						hit++;
						hit_again += hit;
						HPplayer.loadFromFile("sprite/HP/HP_2_70p.png");
					}

					if (hit_again == 2) {   // เซ้ทการชนแล้วเลือดค่อยๆลด
						HPplayer.loadFromFile("sprite/HP/HP_3_50p.png");
					}

					if (hit_again == 3) {
						HPplayer.loadFromFile("sprite/HP/HP_4_30p.png");
					}

					if (hit_again == 4) {
						HPplayer.loadFromFile("sprite/HP/HP_5_0p.png");
						game = 4;
						deadsound.play();

					}

					if (Scorecurrent >= 5)   // มอนหลุดหน้าจอแล้วลดคะแนน
					{
						Scorecurrent -= 5;
					}
				}
				// เช็คตัวละคร ชนกับตัวมอน ให้เลือดตัวละครลด
				if (player.getGlobalBounds().intersects(enemies[i].getGlobalBounds()))
				{
					enemies.erase(enemies.begin() + i);
					playerhitsound.play();
					if (hit == 0) {
						hit++;
						hit_again += hit;
						HPplayer.loadFromFile("sprite/HP/HP_2_70p.png");

					}

					if (hit_again == 2) {   // เซ้ทการชนแล้วเลือดค่อยๆลด
						HPplayer.loadFromFile("sprite/HP/HP_3_50p.png");

					}

					if (hit_again == 3) {
						HPplayer.loadFromFile("sprite/HP/HP_4_30p.png");

					}

					if (hit_again == 4) {
						HPplayer.loadFromFile("sprite/HP/HP_5_0p.png");

						game = 4;
						deadsound.play();
						break;
					}

					if (Scorecurrent >= 5)
					{
						Scorecurrent -= 5;
					}
					continue;
				}

				window.draw(enemies[i]);     // วาดศัตรุออกมา
			}


			//v.2   mon_red
			if (Scorecurrent >= 600) {  // คะแนนไม่เกิน 150 ให้มอนปกติออกมา 

				if (enemySpawnTimer < 40)   // การ spawn mon  (กำหนดลิมิตการสปอน)
				{
					enemySpawnTimer++;
				}


				int rand_x = (rand() % int(window.getSize().x - enemy2.getSize().x));

				if (enemySpawnTimer >= 70)
				{
					enemy2.setPosition(Vector2f(rand_x, -20.0f));
					enemies2.push_back(RectangleShape(enemy2));
					enemySpawnTimer = 0;
				}

				enemySpawnTimer++;
			}

			for (size_t i = 0; i < enemies2.size(); i++)
			{
				enemies2[i].move(0.0f, 1.7f);  //ตอนแรกวิ่งช้า
				if (Scorecurrent >= 1200) { enemies2[i].move(0.0f, 1.9f); } // ปรับมอนวิ่งให้เร็วขึ้น ตอนสอง

				if (enemies2[i].getPosition().y > window.getSize().y)  // สปอน มอนให้เกิด
				{
					enemies2.erase(enemies2.begin() + i);
					playerhitsound.play();

					if (hit == 0) {
						hit++;
						hit_again += hit;
						HPplayer.loadFromFile("sprite/HP/HP_2_70p.png");
					}

					if (hit_again == 2) {   // เซ้ทการชนแล้วเลือดค่อยๆลด
						HPplayer.loadFromFile("sprite/HP/HP_3_50p.png");
					}

					if (hit_again == 3) {
						HPplayer.loadFromFile("sprite/HP/HP_4_30p.png");
					}

					if (hit_again == 4) {
						HPplayer.loadFromFile("sprite/HP/HP_5_0p.png");

						game = 4;  // ไปหน้า game over
						deadsound.play();
					}

					if (Scorecurrent >= 5)
					{
						Scorecurrent -= 5;
					}

				}
				// เช็คตัวละคร ชนกับตัวมอน ให้เลือดตัวละครลด
				if (player.getGlobalBounds().intersects(enemies2[i].getGlobalBounds()))
				{
					enemies2.erase(enemies2.begin() + i);
					playerhitsound.play();
					if (hit == 0) {
						hit++;
						hit_again += hit;
						HPplayer.loadFromFile("sprite/HP/HP_2_70p.png");

					}

					if (hit_again == 2) {   // เซ้ทการชนแล้วเลือดค่อยๆลด
						HPplayer.loadFromFile("sprite/HP/HP_3_50p.png");

					}

					if (hit_again == 3) {
						HPplayer.loadFromFile("sprite/HP/HP_4_30p.png");

					}

					if (hit_again == 4) {
						HPplayer.loadFromFile("sprite/HP/HP_5_0p.png");

						game = 4;  //  ไปหน้า game over
						deadsound.play();
					}

					if (Scorecurrent >= 10)
					{
						Scorecurrent -= 10;
					}
					continue;
				}


				window.draw(enemies2[i]);     // วาดศัตรุออกมา
			}


			for (size_t i = 0; i < shoots.size(); i++)
			{
				window.draw(shoots[i]);   // วาดกระสุนออกมาให้ยิง
			}


			//////////////////// collision mon - shoot //////////////////////////////////////////

			//v.1   mon_white
			for (size_t i = 0; i < shoots.size(); i++)
			{
				for (size_t k = 0; k < enemies.size(); k++)
				{

					if (shoots[i].getGlobalBounds().intersects(enemies[k].getGlobalBounds()))
					{
						Pickupsound.play();
						shoots.erase(shoots.begin() + i);
						enemies.erase(enemies.begin() + k);
						Scorecurrent += 10;
						
						break;
					}

				}
			}


			//v.2  mon_red
			for (size_t i = 0; i < shoots.size(); i++)
			{
				for (size_t k = 0; k < enemies2.size(); k++)
				{

					if (shoots[i].getGlobalBounds().intersects(enemies2[k].getGlobalBounds()))
					{
						Pickupsound.play();
						shoots.erase(shoots.begin() + i);
						enemies2.erase(enemies2.begin() + k);
						Scorecurrent += 20;
						
						break;
					}

				}
			}			
			hit = 0;  // เซ้ทค่าตอนอมนชนจอด้านล่างแล้วให้ครั้งที่ชนกลับเป็น 0 เพื่อเข้า ไปเช็คเงื่อนไขใหม่

			//////////////////// ITEM //////////////////////////////////////////

			if(ItemDelay>10)
			{
				rand_item = rand() % 5; // สุ่มให้ไอเท็มเกิด

				if (rand_item == 0) { allitem.loadFromFile("./sprite/Heart.png"); }
				if (rand_item == 1) { allitem.loadFromFile("./sprite/gem.png"  ); }
				if (rand_item == 2) { allitem.loadFromFile("./sprite/coin.png" ); }
				if (rand_item == 3) { allitem.loadFromFile("./sprite/Boom.png" ); }
				if (rand_item == 4) { allitem.loadFromFile("./sprite/Dead.png" ); }
				
			}

			int rand_x= (rand() % 320) + 110;
				
			if (itemSpawnTimer >= 20 && ItemDelay > 10 )
			{
				item.setPosition(Vector2f(rand_x, -20.0f));
				items.push_back(RectangleShape(item));
				itemSpawnTimer = 0;
				ItemDelay = Itemclock.restart().asSeconds();
				
			}

			itemSpawnTimer++;
			printf("%f\n", ItemDelay);

			
			for (size_t d = 0; d < items.size(); d++)
			{
				items[d].move(0.0f, 2.0f);

				if (items[d].getPosition().y > window.getSize().y)  // สปอน มอนให้เกิด
				{
					items.erase(items.begin() + d);
					break;
					}

					if (player.getGlobalBounds().intersects(items[d].getGlobalBounds()))
					{
						items.erase(items.begin() + d);
						
						if (rand_item == 0) // เก็บได้หัวใจ
						{   
							PickITEMsound.play();
							if (hit_again == 1) {
								HPplayer.loadFromFile("sprite/HP/HP_1_100p.png");
								hit_again--;
							}

							if (hit_again == 2) {   // เซ้ทการชนแล้วเลือดค่อยๆลด
								HPplayer.loadFromFile("sprite/HP/HP_2_70p.png");
								hit_again--;
							}

							if (hit_again == 3) {
								HPplayer.loadFromFile("sprite/HP/HP_3_50p.png");
								hit_again--;
							}

							if (hit_again == 4) {
								HPplayer.loadFromFile("sprite/HP/HP_4_30p.png");

								hit_again--;
							}						
						}


						if (rand_item == 1) {
							coinsound.play();
							Scorecurrent += 200;  // เก็บได้ gem
						}

						if (rand_item == 2) {
							coinsound.play();
							Scorecurrent += 100;  // เก็บได้ coin
						}

						if (rand_item == 3) {    // เก็บได้ Boom  ลดเลือด
							Boomsound.play();
							if (hit == 0) {
								hit++;
								hit_again += hit;
								HPplayer.loadFromFile("sprite/HP/HP_2_70p.png");

							}

							if (hit_again == 2) {   // เซ้ทการชนแล้วเลือดค่อยๆลด
								HPplayer.loadFromFile("sprite/HP/HP_3_50p.png");

							}

							if (hit_again == 3) {
								HPplayer.loadFromFile("sprite/HP/HP_4_30p.png");

							}

							if (hit_again == 4) {
								HPplayer.loadFromFile("sprite/HP/HP_5_0p.png");
								game = 4;  // ไปหน้า game over
								deadsound.play();

							}

						}

						if (rand_item == 4) {     // เก็บได้ dead  ตายเลย
							deadsound.play();
							HPplayer.loadFromFile("sprite/HP/HP_5_0p.png");
							game = 4;  // ไปหน้า game over
							deadsound.play();

						}


					   ItemDelay = Itemclock.restart().asSeconds();
					   break;
					}

				window.draw(items[d]);     // วาดศัตรุออกมา
			}
			
			/////////////////////////////  DRAW //////////////////////////////////////////

			stringstream scoreShow;
			scoreShow << Scorecurrent;
			scoreCurrent.setString(scoreShow.str().c_str());    // วาด คะแนนตอนยิงได้

			window.draw(scoreGame);
			window.draw(scoreCurrent);
			window.draw(HP);		
			window.draw(player);
			window.display();
			window.clear(sf::Color::Transparent);
			
		}
	}
	return 0;

}

static void SavePlayerData(const std::map<int, std::string, std::greater<int>>& data) {

	std::ofstream file;
	file.open("player_score.dat");

	if (!file.is_open()) return;

	for (auto itr = data.begin(); itr != data.end(); itr++)
	{
		file.write((char*)&itr->first,sizeof(int));

		size_t strSize = itr->second.size();
		file.write((char*)&strSize, sizeof(size_t));
		file.write(itr->second.data(), strSize);
	}

	file.close();

}

static std::map<int, std::string, std::greater<int>> loadPlayerData() {

	std::map<int, std::string, std::greater<int>> res;

	std::ifstream file;
	file.open("player_score.dat");

	if (!file.is_open()) return res;

	while (!file.eof()) {
		std::pair<int, std::string> pair;

		int score;
		file.read((char*)&score, sizeof(int));

		size_t strSize;
		std::string userName;
		file.read((char*)&strSize, sizeof(size_t));
		userName.resize(strSize);
		//file.read(userName.data(), strSize);

		pair.first = score;
		pair.second = userName;
		res.insert(pair);

	}

	return res;

}




