#include <SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include<SFML/System.hpp>
#include <iostream>
#include <vector>
#include <cmath> 
#include <cstdlib>
#include <ctime> 

using namespace std;

struct Player
{
	sf::RectangleShape shape;
	float speed = 0;
	bool isOriginalColor;
	sf::Color originalColor = sf::Color::White;
	sf::Color changeToColor = sf::Color::Red;

	Player() {
		shape.setSize(sf::Vector2f(100.f, 100.f));
		shape.setFillColor(originalColor);
		isOriginalColor = true;
	}

	void setPosition(float x, float y) {

		shape.setPosition(x, y);
	}

	void setColor(const sf::Color& color) {

		shape.setFillColor(color);
	}

	void setTextureee(sf::Texture& texture) {

		shape.setTexture(&texture);
	}

	void move(float x, float y) {

		shape.move(x, y);
	}

	sf::Vector2f getPosition() const {

		return shape.getPosition();
	}

	sf::Vector2f getSize() const {

		return shape.getSize();
	}

};
struct Bullet
{
	sf::RectangleShape shape;

	float bulletSpeed = 0;
	bool bulletFired = false;
	bool RightHandShot = true;


	Bullet() {
		shape.setSize(sf::Vector2f(11.f, 27.f));
	}

	void setPosition(float x, float y) {
		shape.setPosition(x, y);
	}

	void bulletMove(float x, float y) {
		shape.move(0.f, -bulletSpeed);
	}

	void setTextureee(sf::Texture& texture) {
		shape.setTexture(&texture);
	}

	sf::FloatRect getGlobalBounds() {
		return shape.getGlobalBounds();
	}

};
struct Chicken
{
	sf::RectangleShape shape;
	int chickenNumber = 0;

	Chicken() {
		shape.setSize(sf::Vector2f(70.f, 70.f));
		shape.setFillColor(sf::Color::White);
	}

	void setPosition(float x, float y) {
		shape.setPosition(x, y);
	}

	void setTextureee(sf::Texture& texture) {
		shape.setTexture(&texture);
	}

	void setOutline(sf::Color color) {
		shape.setOutlineColor(color);
	}
	
	void setTicknes(float x) {
		shape.setOutlineThickness(x);
	}

	void move(float x, float y) {
		shape.move(x, y);
	}

	sf::FloatRect getGlobalBounds() {
		return shape.getGlobalBounds();
	}

	sf::Vector2f getPosition() const {

		return shape.getPosition();
	}

	sf::Vector2f getSize() const {

		return shape.getSize();
	}
};

void UpdateMovment(Player&, sf::RenderWindow& window);
void LoopRightToLeft(Player& , sf::RenderWindow&, bool&, float);
void colorChange(Player&, sf::Event&);

void isBulletShot(Player&, sf::Event&,Bullet&,vector<Bullet>&);
void updateBullet(vector<Bullet>&,Bullet&);

void chickenSpawningPosition(Chicken&,sf::RenderWindow&);
void updateChicken(vector<Chicken>&,sf::Clock&, sf::RenderWindow&,sf::Texture&);
void chickenMovment(vector<Chicken>&, Chicken&);

void collision(vector<Bullet>&, vector<Chicken>&);

int main()
{
	//INIT ! ! !
	sf::RenderWindow window(sf::VideoMode(1000, 800), "SFML", sf::Style::Close | sf::Style::Titlebar);
	window.setFramerateLimit(144);

	//CLOCK FOR CHICKEN SPAWN
	sf::Clock spawnClock;
	std::srand(static_cast<unsigned int>(std::time(0)));

	//PLAYER 
	Player player;
	player.speed = 3.f;
	player.setPosition(600.f, 550.f);

	//BULLETS
	Bullet bullet;		//Left Bullet
	bullet.bulletSpeed = 3.f;

	//VEKTOR ZA SPREMANJE BULLETA
	vector<Bullet>bullets;
	bool goingRight = true;
	float speed = 3.f;

	//CHICKENS
	Chicken chicken;
	
	//VEKTOR ZA SPREMANJE CHICKEN
	vector<Chicken>chickens;
	
	//TEXTURE OF YOSEMITE
	sf::Texture texture;
	if (!texture.loadFromFile("Pictures/Yosemite.png"))
	{
		cout << "FILE DIDNT LOAD ! ! !";
		return 0;
	}
	player.setTextureee(texture);

	//TEXTURE OF BULLET
	sf::Texture texture2;
	if (!texture2.loadFromFile("Pictures/bullet.png"))
	{
		cout << "FILE DIDNT LOAD ! ! !";
		return 0;
	}
	bullet.setTextureee(texture2);


	//TEXTURE OF CHICKEN
	sf::Texture texture4;
	if (!texture4.loadFromFile("Pictures/chickenL.png"))
	{
		cout << "FILE DIDNT LOAD ! ! !";
		return 0;
	}
	chicken.setTextureee(texture4);

	//EVENT POLLING ! ! !
	//MAIN GAME LOOP ! ! !

	while (window.isOpen())
	{
		sf::Event ev;
		while (window.pollEvent(ev))
		{
			switch (ev.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (ev.key.code == sf::Keyboard::Escape)
				{
					window.close();
				}
				else if (ev.key.code==sf::Keyboard::Space)
				{
					isBulletShot(player, ev, bullet,bullets);
				}
			}
			//Change Color White to Red ! ! !
			colorChange(player, ev);
			
		}
		//UPDATE ! ! !
		//LoopRightToLeft(player, window,goingRight,speed);

		UpdateMovment(player, window);
		updateBullet(bullets,bullet);
		updateChicken(chickens,spawnClock,window,texture4);
		chickenMovment(chickens,chicken);

		collision(bullets,chickens);

		//DRAWWW
		window.clear(sf::Color(0, 40, 13));

		window.draw(player.shape);
		
		if (bullet.bulletFired)
		{
			for (auto& bullet : bullets) {
				window.draw(bullet.shape);
			}
		}

		for (auto& chicken : chickens) {
			window.draw(chicken.shape);
		}

		

		window.display();
	}
	return 0;
}
void UpdateMovment(Player& player, sf::RenderWindow& window) {
	
	float strafeX = 0.f;
	float strafeY = 0.f;


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && player.getPosition().y >0.f)
	{
		strafeY = strafeY - player.speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && player.getPosition().y + player.getSize().y < window.getSize().y)
	{
		strafeY = strafeY + player.speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && player.getPosition().x > 0)
	{
		strafeX = strafeX - player.speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && player.getPosition().x + player.getSize().x < window.getSize().x)
	{
		strafeX = strafeX + player.speed;
	}

	//SKALIRANJE X i Y
	
	if (strafeX != 0.f && strafeY != 0.f) {
		strafeX /= sqrt(2.f);
		strafeY /= sqrt(2.f);
	}

	player.move(strafeX, strafeY);
}
void LoopRightToLeft(Player& player, sf::RenderWindow& window, bool& goingRight, float speed) {
	if (goingRight == true)
	{
		player.move(speed, 0.f);
		if (player.getPosition().x + player.getSize().x >= window.getSize().x)
		{
			goingRight = false;
		}
	}
	else
	{
		player.move(-speed, 0.f);
		if (player.getPosition().x <= 0.f)
		{
			goingRight = true;
		}
	}
}

void colorChange(Player& player, sf::Event& ev) {
	if (ev.type == sf::Event::MouseButtonPressed) {
		if (ev.mouseButton.button == sf::Mouse::Left) {

			if (player.isOriginalColor) {
				player.setColor(player.changeToColor);
			}
			else {
				player.setColor(player.originalColor);
			}
			player.isOriginalColor = !player.isOriginalColor;
		}
	}
}

void isBulletShot(Player& player, sf::Event& ev, Bullet& bullet,vector<Bullet>& bullets) {
	
	bullet.bulletFired = true;

	if (bullet.RightHandShot==true)
	{
		bullet.setPosition(player.getPosition().x, player.getPosition().y);
		bullet.RightHandShot = false;
	}
	else 
	{
		bullet.setPosition(player.getPosition().x + player.getSize().x, player.getPosition().y);
		bullet.RightHandShot = true;
	}
	
	bullets.push_back(bullet);
}

void updateBullet(vector<Bullet>&bullets,Bullet& bullet) {
	
	for (auto& bullet :bullets)
	{	
		bullet.bulletMove(0, -bullet.bulletSpeed);
	}
}

void chickenSpawningPosition(Chicken& chicken, sf::RenderWindow& window) {		
	int rand_spawn = std::rand() % (window.getSize().x - static_cast<int>(chicken.getSize().x));
	cout << "rand_spawn je " << rand_spawn<<endl;

	chicken.setPosition(rand_spawn,0.f);
}

void updateChicken(vector<Chicken>& chickens,sf::Clock& clock,sf::RenderWindow& window, sf::Texture& texture) {
	if (clock.getElapsedTime().asSeconds() > 1.5f)
	{
		clock.restart();
		Chicken newChicken;
		sf::Color colorOfChicken = sf::Color::Red;
		newChicken.setTextureee(texture);

		chickenSpawningPosition(newChicken, window);
		
		chickens.push_back(newChicken);
	}
}

void chickenMovment(vector<Chicken>& chickens, Chicken& chicken) {

	for (auto& chicken : chickens) {
		chicken.move(0, 0.6f);
	}
}

void collision(vector<Bullet>& bullets, vector<Chicken>& chickens) {
	for (size_t i = 0; i < bullets.size(); ++i) {
		for (size_t j = 0; j < chickens.size(); ++j) {
			if (bullets[i].getGlobalBounds().intersects(chickens[j].getGlobalBounds())) {
				
				bullets.erase(bullets.begin() + i);
				chickens.erase(chickens.begin() + j);
				--i;
				break; 
			}
		}
	}

}

