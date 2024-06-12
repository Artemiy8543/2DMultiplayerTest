/*
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Main.hpp>
#include <SFML/Network.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <bits/stdc++.h>
#define ll long long
using namespace std;
using namespace sf;
string address;
int port, porto;
string conv(double x)
{
  ostringstream ss;
  ss << x;
  return ss.str();
}
int main()
{
    double speed;
    TcpSocket socket;
    TcpListener listener;
    cout << "Speed:";
    cin >> speed;
    cout << "Port:";
    cin >> porto;
    if (listener.listen(porto) != Socket::Done){
        cout << "Error to open port!\n";
    }
    cout << "IPAdress:";
    cin >> address;
    cout << "Port:";
    cin >> port;
    cout << "try connect to " << address << ":" << port << "\n";
    Socket::Status status = socket.connect(address, port);
    if(status!=Socket::Done){
        cout << "Connection error!\n";
    }
    ContextSettings settings;
    settings.sRgbCapable = true;
    RenderWindow window(VideoMode(), "Draw", Style::Default, settings);
    double x=150.0,y=150.0,xa=150.0,ya=150.0;
    CircleShape player(20);
    CircleShape player2(20);
    player.setFillColor(Color::Green);
    player2.setFillColor(Color::Red);
    TcpSocket client;
    if (listener.accept(client) != Socket::Done){
        cout << "Error get client!\n";
    }
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))if (event.type == Event::Closed)window.close();
        window.clear(Color::Black);
        if(Keyboard::isKeyPressed(Keyboard::W))y-=speed;
        if(Keyboard::isKeyPressed(Keyboard::S))y+=speed;
        if(Keyboard::isKeyPressed(Keyboard::A))x-=speed;
        if(Keyboard::isKeyPressed(Keyboard::D))x+=speed;
        string xstr = conv(x),ystr = conv(y),datastr;
        while(xstr.size()<5)xstr = "0" + xstr;
        while(ystr.size()<5)ystr = "0" + ystr;
        datastr = xstr + '|' + ystr;
        char datas[11];
        strcpy(datas, datastr.c_str());
        socket.send(datas, 11);
        char datar[11];
        size_t received;
        if (client.receive(datar, 11, received) == sf::Socket::Done){
            string str(datar);
            string xstr = str.substr(0,str.find("|")), ystr = str.substr(str.find("|")+1, 11);
            xa = stod(xstr);
            ya = stod(ystr);
        }else{
            cout << "Receiving error!\n";
        }
        player.setPosition(Vector2f(x-20,y-20));
        player2.setPosition(Vector2f(xa-20,ya-20));
        window.draw(player);
        window.draw(player2);
        window.display();
    }
    return 0;
}
*/

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Main.hpp>
#include <SFML/Network.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <bits/stdc++.h>
#define ll long long
using namespace std;
using namespace sf;
string address;
ll rc;
int port, ports;
bool is_pause=true,is_server=true,is_end=false,is_yes=false;
SocketSelector selector;
double x=150.1,y=150.1,rcd=1.0,fps=1,frames=1,speed=1;
vector<TcpSocket*> users;
auto e = std::chrono::system_clock::now();
time_t end_time = chrono::system_clock::to_time_t(e);
string name(ctime(&end_time)),datastr="|||||||||||";
string path = "logs/" + name.substr(4,7) + name.substr(11,2) + "." + name.substr(14,2) + "." + name.substr(17,2) + " " + name.substr(20,4) + ".txt";
ofstream out(path);
Clock second;
Color sprites[10]={Color(237,209,156),Color(102,0,102),Color(199,252,236),Color(255,133,151),Color(245,171,0),Color(182,110,121),Color(104,108,94),Color(92,53,48),Color(255,43,43),Color(148,255,255)};
CircleShape player(20);
string conv(double x)
{
  ostringstream ss;
  ss << x;
  return ss.str();
}
void getInfo()
{
    cout << "|------------------------------------------------\n";
	cout << "|Public Address : " << IpAddress::getPublicAddress() << "\n";
	cout << "|Local Address : " << IpAddress::getLocalAddress() << "\n";
	cout << "|Port : " << ports << "\n";
	cout << "|" << users.size() << "\n";
    out << "|------------------------------------------------\n";
	out << "|Public Address : " << IpAddress::getPublicAddress() << "\n";
	out << "|Local Address : " << IpAddress::getLocalAddress() << "\n";
	out << "|Port : " << ports << "\n";
	out << "|" << users.size() << "\n";
	for (auto& user : users)
	{
		if (user->getRemoteAddress() != TcpSocket().getRemoteAddress()){
			cout << "|" << user->getRemoteAddress() << "\n";
			out << "|" << user->getRemoteAddress() << "\n";
		}
	}
	cout << "|------------------------------------------------\n";
	out << "|------------------------------------------------\n";
}
void ServerMain()
{
	TcpListener listener;
	if (listener.listen(ports) != sf::Socket::Done){
		cout << "Error to open port\n";
	}
	while (!is_end){
		if(users.empty() || users[users.size() - 1]->getRemoteAddress() != TcpSocket().getRemoteAddress())
			users.push_back(new TcpSocket);
		if (listener.accept(*users[users.size() - 1]) != sf::Socket::Done){
			cout << "None\n";
		}else{
            selector.add(*users[users.size() - 1]);
            cout << "new user\n";
            getInfo();
		}
	}
}
void ServerReceiver(RenderWindow* window)
{
    Font font;
    font.loadFromFile("Intro.otf");
    window->setActive(true);
	while (window->isOpen()){
        window->clear(Color::Black);
        Text fpst;
        fpst.setFont(font);
        fpst.setCharacterSize(10);
        fpst.setColor(Color::White);
        fpst.setPosition(0,0);
        char buff[1024];
        string klooo="FPS:";
        sprintf(buff,"%.2f",fps);
        klooo+=buff;
        klooo+="\tX:";
        sprintf(buff,"%.2f",x);
        klooo+=buff;
        klooo+=" Y:";
        sprintf(buff,"%.2f",y);
        klooo+=buff;
        klooo+="\nspeed:";
        sprintf(buff,"%f",speed);
        klooo+=buff;
        fpst.setString(klooo);
        window->draw(fpst);
        player.setPosition(x,y);
        player.setFillColor(sprites[rc]);
        window->draw(player);
	    char datas[55];
	    string cstr=conv(rcd),xstr=conv(round(x*10.0)/10.0),ystr=conv(round(y*10.0)/10.0);
        while(xstr.size()<5)xstr = "0" + xstr;
        while(ystr.size()<5)ystr = "0" + ystr;
	    datastr = cstr+xstr+ystr;
		if(selector.wait()){
			for (auto& user : users){
                char datar[11];
                size_t received;
                user->receive(datar, 11, received);
                string str(datar);
                string info=str.substr(0,11);
                datastr=datastr+info;
                string playercolor = info.substr(0,1),playerx = info.substr(1,5),playery=info.substr(6,5);
                int  pcolor = stoi(playercolor);
                double px = stod(playerx), py = stod(playery);
                player.setPosition(px,py);
                player.setFillColor(sprites[pcolor]);
                window->draw(player);
			}
		}
		while(datastr.size()<55)datastr=datastr+"|";
		//cout << "Datas:" << datastr << endl;
		strcpy(datas, datastr.c_str());
		for (auto& user : users){
            if(TcpSocket().getRemoteAddress()!=user->getRemoteAddress())user->send(datas,55);
        }
        window->display();
	}
}
int main()
{
    TcpSocket socket;
    TcpListener listener;
    Font font;
    font.loadFromFile("Intro.otf");
    srand(time(0));

    rc = abs(((abs((abs(rand()%10)^rand()%255-rand())%10)+abs(rand()%10))*rand())%10);
    rcd = rc + 0.0;
    cout << "Color number:" << rcd << endl;
    out << "Color number:" << rcd << endl;
    RECT desktop;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);
    ll heigth = desktop.right;
    ll width = desktop.bottom;

    double xa=150.0,ya=150.0;
    RenderWindow window(VideoMode(heigth,width,1), "Draw", Style::Default, ContextSettings(0,0,0,1,1,0,true));
    while (window.isOpen()){
        Event event;
        while (window.pollEvent(event))if (event.type == Event::Closed)window.close();
        if(is_pause){
            window.clear(Color::Black);

            RectangleShape button(Vector2f(260,120));
            button.setPosition(Vector2f(20,20));
            button.setFillColor(Color(36,36,36));
            button.setOutlineColor(Color(130,130,130));
            button.setOutlineThickness(2);
            window.draw(button);

            button.setPosition(Vector2f(20,160));
            window.draw(button);

            Text Host;
            Host.setFont(font);
            Host.setCharacterSize(30);
            Host.setColor(Color::White);
            Host.setPosition(110,60);
            Host.setString("Host");
            window.draw(Host);

            Text Join;
            Join.setFont(font);
            Join.setCharacterSize(30);
            Join.setColor(Color::White);
            Join.setPosition(110,200);
            Join.setString("Join");
            window.draw(Join);
            window.display();
            if(Mouse::getPosition(window).x>=20 and Mouse::getPosition(window).x<=280 and Mouse::getPosition(window).y>=20 and Mouse::getPosition(window).y<=140 and Mouse::isButtonPressed(Mouse::Left)){
                    is_pause=false;
                    window.setActive(false);
                    cout << "Port:";
                    cin >> ports;
                    thread SMain(&ServerMain);
                    thread SReceiver(&ServerReceiver, &window);
                    SMain.detach();
                    SReceiver.detach();
                    getInfo();
                    this_thread::sleep_for(chrono::milliseconds(2000));
            }
            if(Mouse::getPosition(window).x>=20 and Mouse::getPosition(window).x<=280 and Mouse::getPosition(window).y>=160 and Mouse::getPosition(window).y<=280 and Mouse::isButtonPressed(Mouse::Left)){
                    is_pause=false;
                    is_server=false;
                    cout << "IPAdress:";
                    cin >> address;
                    cout << "Port:";
                    cin >> port;
                    for(int i=1;i<=5;i++){
                        cout << i << "/5 try to connect to " << address << ":" << port << "\n";
                        Socket::Status status = socket.connect(address, port, seconds(5));
                        if(status==Socket::Done){
                            cout << "Connected!\n";
                            break;
                        }
                    }
                    this_thread::sleep_for(chrono::milliseconds(2000));
            }
        }else{
            speed = 1/(fps/60.0);
            if(Keyboard::isKeyPressed(Keyboard::LShift))speed*=2;
            if(Keyboard::isKeyPressed(Keyboard::W))y-=speed;
            if(Keyboard::isKeyPressed(Keyboard::S))y+=speed;
            if(Keyboard::isKeyPressed(Keyboard::A))x-=speed;
            if(Keyboard::isKeyPressed(Keyboard::D))x+=speed;
            if(!is_server){
                window.clear(Color::Black);

                Text fpst;
                fpst.setFont(font);
                fpst.setCharacterSize(10);
                fpst.setColor(Color::White);
                fpst.setPosition(0,0);
                char buff[1024];
                string klooo="FPS:";
                sprintf(buff,"%.2f",fps);
                klooo+=buff;
                klooo+="\tX:";
                sprintf(buff,"%.2f",x);
                klooo+=buff;
                klooo+=" Y:";
                sprintf(buff,"%.2f",y);
                klooo+=buff;
                klooo+="\nspeed:";
                sprintf(buff,"%f",speed);
                klooo+=buff;
                fpst.setString(klooo);
                window.draw(fpst);

                string xstr = conv(round(x*10.0)/10.0),ystr = conv(round(y*10.0)/10.0),colorstr = conv(rcd),datastr;
                while(xstr.size()<5)xstr = "0" + xstr;
                while(ystr.size()<5)ystr = "0" + ystr;
                datastr = colorstr + xstr + ystr;
                char datas[11];
                strcpy(datas, datastr.c_str());
                socket.send(datas, 11);
                cout << "Datas:" << datastr << endl;
                out << "Datas:" << datastr << endl;
                char datar[55];
                size_t received;
                if (socket.receive(datar, 55, received) == sf::Socket::Done){
                    string str(datar);
                    cout << "Datar:" << str << endl;
                    out << "Datar:" << str << "\n";
                    while(str[0]!='|'){
                        string playerinfo = str.substr(0,11);
                        string playercolor = playerinfo.substr(0,1),playerx = playerinfo.substr(1,5),playery=playerinfo.substr(6,5);
                        int pcolor=stoi(playercolor);
                        double px = stod(playerx), py = stod(playery);
                        str = str.substr(11);
                        player.setPosition(px,py);
                        player.setFillColor(sprites[pcolor]);
                        window.draw(player);
                    }
                }else{
                    player.setPosition(x,y);
                    player.setFillColor(sprites[rc]);
                    window.draw(player);
                    cout << "Receiving error!\n";
                }
                window.display();
            }
            if(second.getElapsedTime().asSeconds()>=1){
                fps=frames;
                frames=0;
                second.restart();
            }
            frames++;
        }
    }
    is_end=true;
    return 0;
}

//PerlinNoise
/*
#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Network/TcpSocket.hpp>
using namespace std;
using namespace sf;
#define ll long long
long long SmallGen(ll x, ll y, ll seed){
    double res;
    res = (x*seed*seed)^y*seed;
    return round(res);
}
double PerlinNoise(ll x, ll y, ll seed){
    double res, n, genp;
    ll gen;
    gen = SmallGen(x, y, seed);
    if(x==0)x=1;
    if(y==0)y=1;
    n=gen^(x*(-1*y));
    n /= (x+y*y);
    if(n==1.0/0.0)return PerlinNoise(abs(x),-y,gen);
    while(n*10<1.0)n=abs(n*10)+1;
    while(n>1.0)n=abs(n/10)-abs(n);
    //cout << "n=" << n << "\tx=" << x << "\ty=" << y << endl;
    return n;
}
ll x_wind=300, y_wind=300, seed=52;
int main()
{
    RenderWindow window(VideoMode(x_wind, y_wind), "Perlin test");
    window.clear();
    ll x=0, y=0, step=6;
    double maxi=0,maxi_col=0;
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }
        double col = PerlinNoise(x, y, seed);
        maxi+=col;
        maxi_col+=1;
        col *= 255.0;
        CircleShape shape(step);
        shape.setFillColor(Color(col,col,col));
        shape.setPosition(Vector2f(x, y));
        if(x<x_wind){
            x+=step;
        }else if(y<y_wind){
            y+=step;
            x=0;
        }
        if(y<y_wind){
            window.draw(shape);
            window.display();
        }
    }
    cout<<"====\n"<<maxi/maxi_col;
    return 0;
}
*/
