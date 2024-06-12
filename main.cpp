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
double x=150.1,y=150.1,fps=120,frames=1,speed=1;
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
    font.loadFromFile("utils/Intro.otf");
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
	    char datas[45];
	    double xtoconv = round(x*10.0)/10.0,ytoconv = round(y*10.0)/10.0;
	    int xd=floor(xtoconv),yd=floor(ytoconv),xp=(xtoconv-xd)*10.0,yp=(ytoconv-yd)*10.0;
        string cstr=to_string(rc), xstr=to_string(xd)+to_string(xp),ystr=to_string(yd)+to_string(yp);
        while(xstr.size()<4)xstr="0"+xstr;
        while(ystr.size()<4)ystr="0"+ystr;
	    datastr = cstr + xstr + ystr;
		if(selector.wait()){
			for (auto& user : users){
                char datar[9];
                size_t received;
                user->receive(datar, 9, received);
                if (user->getRemoteAddress() != TcpSocket().getRemoteAddress()){
                    string str(datar);
                    str = str.substr(0,9);
                    datastr = datastr + str;
                    string playerinfo = str.substr(0,9);
                    string playercolor = playerinfo.substr(0,1),xd = playerinfo.substr(1,3),xp = playerinfo.substr(4,1),yd=playerinfo.substr(5,3),yp=playerinfo.substr(8,1);
                    int pcolor=stoi(playercolor),pxd=stoi(xd),pxp=stoi(xp),pyd=stoi(yd),pyp=stoi(yp);
                    double px=pxd+(pxp/10.0),py=pyd+(pyp/10.0);
                    player.setPosition(px,py);
                    player.setFillColor(sprites[pcolor]);
                    window->draw(player);
                }
			}
		}
		while(datastr.size()<45)datastr=datastr+"|";
		//cout << "Datas:" << datastr << endl;
		strcpy(datas, datastr.c_str());
		for (auto& user : users){
            if(TcpSocket().getRemoteAddress()!=user->getRemoteAddress())user->send(datas,45);
        }
        window->display();
	}
}
int main()
{
    TcpSocket socket;
    TcpListener listener;
    Font font;
    font.loadFromFile("utils/Intro.otf");
    srand(time(0));

    rc = abs(((abs((abs(rand()%10)^rand()%255-rand())%10)+abs(rand()%10))*rand())%10);
    cout << "Color number:" << rc << endl;
    out << "Color number:" << rc << endl;
    RECT desktop;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);
    ll heigth = desktop.right;
    ll width = desktop.bottom;

    double xa=150.0,ya=150.0;
    RenderWindow window(VideoMode(300,300,1), "Draw", Style::Default, ContextSettings(0,0,0,1,1,0,true));
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

                double xtoconv = round(x*10.0)/10.0,ytoconv = round(y*10.0)/10.0;
                int xd=floor(xtoconv),yd=floor(ytoconv),xp=(xtoconv-xd)*10.0,yp=(ytoconv-yd)*10.0;
                string cstr=to_string(rc), xstr=to_string(xd)+to_string(xp),ystr=to_string(yd)+to_string(yp);
                while(xstr.size()<4)xstr="0"+xstr;
                while(ystr.size()<4)ystr="0"+ystr;
                datastr = cstr + xstr + ystr;
                char datas[9];
                strcpy(datas, datastr.c_str());
                socket.send(datas, 9);
                char datar[45];
                size_t received;
                if (socket.receive(datar, 45, received) == sf::Socket::Done){
                    string str(datar);
                    while(str[0]!='|' and str.size()>=9){
                        string playerinfo = str.substr(0,9);
                        string playercolor = playerinfo.substr(0,1),xd = playerinfo.substr(1,3),xp = playerinfo.substr(4,1),yd=playerinfo.substr(5,3),yp=playerinfo.substr(8,1);
                        int pcolor=stoi(playercolor),pxd=stoi(xd),pxp=stoi(xp),pyd=stoi(yd),pyp=stoi(yp);
                        double px=pxd+(pxp/10.0),py=pyd+(pyp/10.0);
                        str = str.substr(9);
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
